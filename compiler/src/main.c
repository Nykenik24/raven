#include "raven/colors.h"
#include "raven/error.h"
#include "raven/lexer/lexer.h"
#include "raven/parser/parser.h"
#include "raven/parser/tree.h"
#include "raven/raven.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *read_file_into_string(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Error opening file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *buffer = malloc(file_size + 1);
  if (buffer) {
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
  }

  fclose(file);
  return buffer;
}

void print_debug_info(token_list_t *tokens, tree_t *cst, const char *src,
                      double elapsed) {
  int line_count = 0;
  for (const char *p = src; *p; p++)
    if (*p == '\n')
      line_count++;

  printf("\nLines: " BLU "%d\n" CRESET, line_count);
  printf("Lexing time: " BLU "%.6f seconds\n" CRESET, elapsed);
  printf(RED "<Source>\n" CRESET "%s" RED "\n<Source end>\n\n" CRESET, src);

  _display_token_list(tokens);
  display_tree(cst, 0);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    THROW("Too few arguments, expected file name.");
  }

  char *src = read_file_into_string(argv[1]);
  if (!src)
    return EXIT_FAILURE;

  // TODO: make this a flag
  printf("Version " MAG RAVEN_VERSION CRESET ", built on " MAG __DATE__
         " " __TIME__ "\n" CRESET);

  clock_t start = clock();
  token_list_t *tokens = lex(src);
  double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;

  tree_t *tree = parse(tokens);

#ifdef RAVEN_SHOW_DEBUG_OUTPUT
  print_debug_info(tokens, tree, src, elapsed);
#endif

  free_token_list(tokens);
  free_tree(tree);
  free(src);

  return EXIT_SUCCESS;
}
