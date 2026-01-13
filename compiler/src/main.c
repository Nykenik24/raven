#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "colors.h"
#include "lexer.h"
#include "macros.h"
#include "raven.h"

int main(void) {
  FILE *f = fopen("source.rvn", "r");
  if (!f) {
    perror("Failed to open source.rvn");
    return EXIT_FAILURE;
  }

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *src = malloc(filesize + 1);
  fread(src, 1, filesize, f);
  src[filesize] = '\0';
  fclose(f);

  clock_t t0 = clock();
  token_list_t *tokens = lex(src);
  clock_t t1 = clock();
  double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;

  printf("Version " MAG RAVEN_VERSION CRESET ", built on " MAG __DATE__
         " " __TIME__ "\n" CRESET);

/* compile with -DRAVEN_SHOW_DEBUG_OUTPUT to show */
#ifdef RAVEN_SHOW_DEBUG_OUTPUT

  int line_count = 0;
  for (long i = 0; i < filesize; i++) {
    if (src[i] == '\n')
      line_count++;
  }

  printf("\n");
  printf("Lines:" BLU "%d\n" CRESET, line_count);
  printf("Used " BLU "%f%%" CRESET " (%d) of token limit (" RED "%d" CRESET
         ")\n",
         ((double)tokens->token_num / TOKEN_MAX * 100), tokens->token_num,
         TOKEN_MAX);
  printf("Lexing time: " BLU "%.6f seconds\n" CRESET, elapsed);
  printf(GRN "=================" RED " OUTPUT " GRN
             "=================\n\n" CRESET);

#else
  UNUSED(t0);
  UNUSED(t1);
  UNUSED(elapsed);
#endif

  //_display_token_list(tokens);
  free_token_list(tokens);
  free(src);

  return EXIT_SUCCESS;
}
