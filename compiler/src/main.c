#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raven/colors.h"
#include "raven/lexer/lexer.h"
#include "raven/macros.h"
#include "raven/parser/parser.h"
#include "raven/parser/tree.h"
#include "raven/raven.h"

char *read_file_into_string(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    perror("Error opening file");
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    perror("Error seeking to end of file");
    fclose(file);
    return NULL;
  }

  long file_size = ftell(file);
  if (file_size == -1) {
    perror("Error getting file size");
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    perror("Error seeking to start of file");
    fclose(file);
    return NULL;
  }

  char *buffer = malloc(file_size + 1);
  if (buffer == NULL) {
    perror("Error allocating memory");
    fclose(file);
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, file_size, file);
  if (bytes_read != (unsigned long)file_size) {
    perror("Error reading file");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[file_size] = '\0';

  fclose(file);
  return buffer;
}

int find_str_len(char *s) {
  int c = 0;

  while (*s++)
    c++;

  return c;
}

int main(void) {

  char *src = read_file_into_string("source.rvn");
  int src_len = find_str_len(src);

  clock_t t0 = clock();
  token_list_t *tokens = lex(src);
  clock_t t1 = clock();
  double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

  tree_t *cst = parse(tokens);

  printf("Version " MAG RAVEN_VERSION CRESET ", built on " MAG __DATE__
         " " __TIME__ "\n" CRESET);

/* compile with -DRAVEN_SHOW_DEBUG_OUTPUT to show */
#ifdef RAVEN_SHOW_DEBUG_OUTPUT

  int line_count = 0;
  for (long i = 0; i < src_len; i++) {
    if (src[i] == '\n')
      line_count++;
  }

  printf("\n");
  printf("Lines:" BLU "%d\n" CRESET, line_count);
  printf("Used" BLU " %d/%d " CRESET "(" GRN "%f%%" CRESET ") tokens\n",
         tokens->token_num, TOKEN_MAX,
         ((double)tokens->token_num / TOKEN_MAX * 100));
  printf("Lexing time: " BLU "%.6f seconds\n" CRESET, elapsed);
  printf(GRN "=================" RED " OUTPUT " GRN
             "=================\n\n" CRESET);

  printf(RED "<Source>" CRESET "\n%s" RED "<Source end>\n\n" CRESET, src);
  _display_token_list(tokens);
  _display_tree(cst, 0);
#else
  UNUSED(t0);
  UNUSED(t1);
  UNUSED(elapsed);
  UNUSED(src_len);
#endif

  //_display_token_list(tokens);
  free_token_list(tokens);
  free_tree(cst);

  return EXIT_SUCCESS;
}
