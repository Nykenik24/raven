/**
 * @file diag/location.c
 * @brief Source location manager implementation
 */

#include <core/diag/location.h>

#include <stdlib.h>
#include <string.h>

#define ARENA_INITIAL_SIZE 65536
#define MAX_LOCATIONS 16384

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

/// Location table entry
typedef struct {
  DiagLocation location;
} LocationEntry;

/// Location manager stores source locations indexed by ID
struct LocationManager {
  LocationEntry *locations;
  uint32_t count;
  uint32_t capacity;
  Arena string_arena;
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

LocationManager *diag_location_manager_create(void) {
  LocationManager *manager = malloc(sizeof(LocationManager));
  if (!manager)
    return NULL;

  manager->locations =
      malloc(sizeof(LocationEntry) * MAX_LOCATIONS);
  if (!manager->locations) {
    free(manager);
    return NULL;
  }

  manager->count = 0;
  manager->capacity = MAX_LOCATIONS;
  arena_init(&manager->string_arena);

  return manager;
}

void diag_location_manager_free(LocationManager *manager) {
  if (!manager)
    return;

  free(manager->locations);
  arena_free(&manager->string_arena);
  free(manager);
}

LocationID diag_location_add(LocationManager *manager, const char *path,
                             uint32_t line, uint32_t column,
                             uint32_t length) {
  if (!manager || manager->count >= manager->capacity)
    return 0;

  LocationEntry *entry = &manager->locations[manager->count];
  entry->location.path = arena_strdup(&manager->string_arena, path);
  entry->location.line = line;
  entry->location.column = column;
  entry->location.length = length;

  return (LocationID)(++manager->count);
}

DiagLocation diag_location_get(LocationManager *manager, LocationID loc_id) {
  static const DiagLocation empty = {NULL, 0, 0, 0};

  if (!manager || loc_id == 0 || loc_id > manager->count)
    return empty;

  return manager->locations[loc_id - 1].location;
}
