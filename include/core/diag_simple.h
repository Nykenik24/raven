/**
 * @file diag_simple.h
 * @brief Simple immediate error reporting for diagnostics
 * @details Provides basic error reporting functions that print directly to stderr
 * for cases where full metadata-driven diagnostics are not available yet.
 */

#ifndef CORE_DIAG_SIMPLE_H
#define CORE_DIAG_SIMPLE_H

/// Reports an undefined variable error
void diag_error_undefined_variable(const char *filename, unsigned int line,
                                   unsigned int column, const char *varname);

/// Reports an undefined function error
void diag_error_undefined_function(const char *filename, unsigned int line,
                                   unsigned int column, const char *funcname);

/// Reports a type mismatch error
void diag_error_type_mismatch(const char *filename, unsigned int line,
                              unsigned int column, const char *expected,
                              const char *actual);

#endif
