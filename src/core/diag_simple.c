/**
 * @file diag_simple.c
 * @brief Simple immediate error reporting implementation
 */

#include <core/diag_simple.h>
#include <core/ansi.h>
#include <stdio.h>

void diag_error_undefined_variable(const char *filename, unsigned int line,
                                   unsigned int column, const char *varname) {
  fprintf(stderr, "%s%s:%u:%u:%s error: ", ANSI_RED, filename, line, column,
          ANSI_RESET);
  fprintf(stderr, ANSI_BOLD "undefined variable '%s'" ANSI_RESET "\n", varname);
  fprintf(stderr, ANSI_DIM "help: declare '%s' before using it" ANSI_RESET "\n",
          varname);
}

void diag_error_undefined_function(const char *filename, unsigned int line,
                                   unsigned int column, const char *funcname) {
  fprintf(stderr, "%s%s:%u:%u:%s error: ", ANSI_RED, filename, line, column,
          ANSI_RESET);
  fprintf(stderr, ANSI_BOLD "undefined function '%s'" ANSI_RESET "\n", funcname);
  fprintf(stderr,
          ANSI_DIM "help: declare function '%s' before calling it" ANSI_RESET
          "\n",
          funcname);
}

void diag_error_type_mismatch(const char *filename, unsigned int line,
                              unsigned int column, const char *expected,
                              const char *actual) {
  fprintf(stderr, "%s%s:%u:%u:%s error: ", ANSI_RED, filename, line, column,
          ANSI_RESET);
  fprintf(stderr, ANSI_BOLD "type mismatch" ANSI_RESET "\n");
  fprintf(stderr, ANSI_DIM "  expected: %s" ANSI_RESET "\n", expected);
  fprintf(stderr, ANSI_DIM "  got:      %s" ANSI_RESET "\n", actual);
}
