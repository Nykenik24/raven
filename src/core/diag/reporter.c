/**
 * @file diag/reporter.c
 * @brief Diagnostic reporter implementation
 */

#include <core/diag/reporter.h>
#include <core/diag/location.h>
#include <core/ansi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_INITIAL_SIZE 65536
#define MAX_METADATA 512
#define MAX_FORMATTED_MESSAGE 4096

/// Internal arena allocator node
typedef struct ArenaChunk {
  struct ArenaChunk *next;
  char *data;
  size_t size;
  size_t used;
} ArenaChunk;

/// Arena allocator for efficient string memory management
typedef struct {
  ArenaChunk *head;
} Arena;

/// Single diagnostic entry
typedef struct Diagnostic {
  LocationID location;
  DiagID diag_id;
  const DiagMetadata *metadata;
  char *formatted_message;
} Diagnostic;

/// Diagnostic linked list node
typedef struct DiagnosticNode {
  Diagnostic diag;
  struct DiagnosticNode *next;
} DiagnosticNode;

/// Reporter stores diagnostics and metadata
struct DiagReporter {
  LocationManager *location_manager;
  DiagMetadata *metadata;
  uint32_t metadata_count;
  uint32_t metadata_capacity;
  DiagnosticNode *head;
  DiagnosticNode *tail;
  size_t count;
  size_t error_count;
  size_t warning_count;
  size_t note_count;
  Arena message_arena;
};

static void arena_init(Arena *arena) {
  arena->head = NULL;
}

static void arena_free(Arena *arena) {
  ArenaChunk *chunk = arena->head;
  while (chunk) {
    ArenaChunk *next = chunk->next;
    free(chunk->data);
    free(chunk);
    chunk = next;
  }
  arena->head = NULL;
}

static void *arena_alloc(Arena *arena, size_t size) {
  if (size == 0)
    return NULL;

  ArenaChunk *chunk = arena->head;
  if (!chunk || chunk->used + size > chunk->size) {
    size_t chunk_size = ARENA_INITIAL_SIZE;
    if (size > chunk_size)
      chunk_size = size * 2;

    ArenaChunk *new_chunk = malloc(sizeof(ArenaChunk));
    if (!new_chunk)
      return NULL;

    new_chunk->data = malloc(chunk_size);
    if (!new_chunk->data) {
      free(new_chunk);
      return NULL;
    }

    new_chunk->size = chunk_size;
    new_chunk->used = 0;
    new_chunk->next = arena->head;
    arena->head = new_chunk;
    chunk = new_chunk;
  }

  void *result = chunk->data + chunk->used;
  chunk->used += size;
  return result;
}

static const char *arena_strdup(Arena *arena, const char *str) {
  if (!str)
    return NULL;

  size_t len = strlen(str) + 1;
  char *result = arena_alloc(arena, len);
  if (result)
    memcpy(result, str, len);
  return result;
}

DiagReporter *diag_reporter_create(void) {
  DiagReporter *reporter = malloc(sizeof(DiagReporter));
  if (!reporter)
    return NULL;

  reporter->location_manager = diag_location_manager_create();
  if (!reporter->location_manager) {
    free(reporter);
    return NULL;
  }

  reporter->metadata = malloc(sizeof(DiagMetadata) * MAX_METADATA);
  if (!reporter->metadata) {
    diag_location_manager_free(reporter->location_manager);
    free(reporter);
    return NULL;
  }

  reporter->metadata_count = 0;
  reporter->metadata_capacity = MAX_METADATA;
  reporter->head = NULL;
  reporter->tail = NULL;
  reporter->count = 0;
  reporter->error_count = 0;
  reporter->warning_count = 0;
  reporter->note_count = 0;

  arena_init(&reporter->message_arena);

  return reporter;
}

void diag_reporter_free(DiagReporter *reporter) {
  if (!reporter)
    return;

  DiagnosticNode *current = reporter->head;
  while (current) {
    DiagnosticNode *next = current->next;
    free(current);
    current = next;
  }

  arena_free(&reporter->message_arena);
  diag_location_manager_free(reporter->location_manager);
  free(reporter->metadata);
  free(reporter);
}

DiagID diag_register_metadata(DiagReporter *reporter, DiagMetadata metadata) {
  if (!reporter || reporter->metadata_count >= reporter->metadata_capacity)
    return 0;

  metadata.id = reporter->metadata_count + 1;
  reporter->metadata[reporter->metadata_count] = metadata;

  return (DiagID)(++reporter->metadata_count);
}

static const DiagMetadata *find_metadata(DiagReporter *reporter,
                                         DiagID diag_id) {
  if (!reporter || diag_id == 0 || diag_id > reporter->metadata_count)
    return NULL;

  return &reporter->metadata[diag_id - 1];
}

void diag_report_vfmt(DiagReporter *reporter, DiagID diag_id, LocationID loc_id,
                      const char *fmt, va_list args) {
  if (!reporter || diag_id == 0 || loc_id == 0)
    return;

  const DiagMetadata *metadata = find_metadata(reporter, diag_id);
  if (!metadata)
    return;

  DiagnosticNode *node = malloc(sizeof(DiagnosticNode));
  if (!node)
    return;

  char buffer[MAX_FORMATTED_MESSAGE];
  if (fmt) {
    vsnprintf(buffer, sizeof(buffer), fmt, args);
  } else {
    buffer[0] = '\0';
  }

  node->diag.location = loc_id;
  node->diag.diag_id = diag_id;
  node->diag.metadata = metadata;
  node->diag.formatted_message = (char *)arena_strdup(&reporter->message_arena, buffer);
  node->next = NULL;

  if (reporter->tail) {
    reporter->tail->next = node;
  } else {
    reporter->head = node;
  }
  reporter->tail = node;
  reporter->count++;

  switch (metadata->level) {
  case DIAG_LEVEL_ERROR:
    reporter->error_count++;
    break;
  case DIAG_LEVEL_WARNING:
    reporter->warning_count++;
    break;
  case DIAG_LEVEL_NOTE:
    reporter->note_count++;
    break;
  case DIAG_LEVEL_FATAL:
    reporter->error_count++;
    break;
  }
}

void diag_report_fmt(DiagReporter *reporter, DiagID diag_id, LocationID loc_id,
                     const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  diag_report_vfmt(reporter, diag_id, loc_id, fmt, args);
  va_end(args);
}

static const char *find_line_start(const char *buffer, uint32_t line) {
  const char *p = buffer;
  uint32_t current_line = 1;

  while (*p && current_line < line) {
    if (*p == '\n')
      current_line++;
    p++;
  }
  return p;
}

static size_t line_length(const char *start) {
  const char *p = start;
  while (*p && *p != '\n' && *p != '\r')
    p++;
  return (size_t)(p - start);
}

static const char *level_to_color(DiagLevel level) {
  switch (level) {
  case DIAG_LEVEL_ERROR:
    return ANSI_RED ANSI_BOLD;
  case DIAG_LEVEL_FATAL:
    return ANSI_RED ANSI_BOLD;
  case DIAG_LEVEL_WARNING:
    return ANSI_YELLOW ANSI_BOLD;
  case DIAG_LEVEL_NOTE:
    return ANSI_CYAN;
  default:
    return ANSI_RESET;
  }
}

static const char *level_to_string(DiagLevel level) {
  switch (level) {
  case DIAG_LEVEL_ERROR:
    return "error";
  case DIAG_LEVEL_FATAL:
    return "fatal error";
  case DIAG_LEVEL_WARNING:
    return "warning";
  case DIAG_LEVEL_NOTE:
    return "note";
  default:
    return "diagnostic";
  }
}

static void print_diagnostic_internal(const Diagnostic *diag,
                                      DiagReporter *reporter,
                                      const char *source_buffer) {
  if (!diag || !diag->metadata)
    return;

  DiagLocation location = diag_location_get(reporter->location_manager, diag->location);

  printf("%s%s%s: ", level_to_color(diag->metadata->level),
         level_to_string(diag->metadata->level), ANSI_RESET);

  const char *display_path = location.path ? location.path : "<unknown>";

  if (diag->metadata->code) {
    printf(ANSI_BOLD "[%s]" ANSI_RESET " ", diag->metadata->code);
  }

  printf(ANSI_BOLD "%s" ANSI_RESET "\n", diag->metadata->message);

  if (diag->formatted_message && diag->formatted_message[0]) {
    printf("%s\n", diag->formatted_message);
  }

  printf(ANSI_DIM "  --> " ANSI_RESET "%s" ANSI_DIM ":%u:%u" ANSI_RESET "\n",
         display_path, location.line, location.column);

  if (source_buffer) {
    const char *line_start = find_line_start(source_buffer, location.line);
    size_t len = line_length(line_start);

    if (len > 0) {
      printf(ANSI_DIM "%4u | " ANSI_RESET, location.line);

      char line_buffer[1024];
      size_t copy_len = len > 1023 ? 1023 : len;
      memcpy(line_buffer, line_start, copy_len);
      line_buffer[copy_len] = '\0';

      printf("%s\n", line_buffer);

      printf(ANSI_DIM "     | " ANSI_RESET);

      uint32_t caret_pos = location.column - 1;
      if (caret_pos > copy_len)
        caret_pos = copy_len;

      for (uint32_t i = 0; i < caret_pos; i++) {
        printf(" ");
      }

      printf("%s", level_to_color(diag->metadata->level));
      uint32_t highlight_len = location.length;
      if (highlight_len == 0)
        highlight_len = 1;
      if (caret_pos + highlight_len > copy_len) {
        highlight_len = copy_len - caret_pos;
      }
      for (uint32_t i = 0; i < highlight_len; i++) {
        printf("^");
      }
      printf(ANSI_RESET "\n");
    }
  }

  if (diag->metadata->help_text) {
    printf(ANSI_DIM "  = " ANSI_RESET "help: %s\n", diag->metadata->help_text);
  }
}

void diag_print_all(const DiagReporter *reporter, const char *source_buffer) {
  if (!reporter)
    return;

  DiagnosticNode *current = reporter->head;
  while (current) {
    print_diagnostic_internal(&current->diag, (DiagReporter *)reporter,
                              source_buffer);
    if (current->next)
      printf("\n");
    current = current->next;
  }
}

size_t diag_count_by_level(const DiagReporter *reporter, DiagLevel level) {
  if (!reporter)
    return 0;

  switch (level) {
  case DIAG_LEVEL_ERROR:
    return reporter->error_count;
  case DIAG_LEVEL_WARNING:
    return reporter->warning_count;
  case DIAG_LEVEL_NOTE:
    return reporter->note_count;
  case DIAG_LEVEL_FATAL:
    return 0;
  default:
    return 0;
  }
}

bool diag_has_errors(const DiagReporter *reporter) {
  return reporter && reporter->error_count > 0;
}

LocationManager *diag_get_location_manager(DiagReporter *reporter) {
  return reporter ? reporter->location_manager : NULL;
}
