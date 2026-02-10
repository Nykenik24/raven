/**
 * @file diag/reporter.h
 * @brief Main diagnostic reporter interface.
 * @details Handles collection, storage, and reporting of compilation diagnostics
 * with variadic formatting support.
 */

#ifndef CORE_DIAG_REPORTER_H
#define CORE_DIAG_REPORTER_H

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#include <core/diag/location.h>
#include <core/diag/metadata.h>

/// Opaque diagnostic reporter handle
typedef struct DiagReporter DiagReporter;

/**
 * @brief Creates a new diagnostic reporter instance
 * @details Allocates and initializes an empty diagnostic reporter with
 * its own location manager and memory arena.
 * @return Pointer to newly allocated DiagReporter, or NULL on memory error.
 */
DiagReporter *diag_reporter_create(void);

/**
 * @brief Frees a diagnostic reporter and all collected diagnostics
 * @details Releases all memory associated with the reporter, including
 * its location manager and memory arena.
 * @param reporter The reporter to free. Safe to call with NULL.
 */
void diag_reporter_free(DiagReporter *reporter);

/**
 * @brief Registers a diagnostic metadata
 * @details Adds a diagnostic type definition to the reporter.
 * Must be called before reporting diagnostics of that type.
 * @param reporter The reporter instance.
 * @param metadata The diagnostic metadata to register.
 * @return The registered DiagID, or 0 on error.
 */
DiagID diag_register_metadata(DiagReporter *reporter, DiagMetadata metadata);

/**
 * @brief Reports a diagnostic with formatted message using variadic arguments
 * @details Records a diagnostic with location ID and formatted message.
 * The message format string is combined with diagnostic metadata if available.
 * @param reporter The reporter instance.
 * @param diag_id The diagnostic type ID.
 * @param loc_id The source location ID (from diag_location_add).
 * @param fmt Printf-style format string for variable parts of the message.
 * @param ... Variable arguments for the format string.
 */
void diag_report_fmt(DiagReporter *reporter, DiagID diag_id, LocationID loc_id,
                     const char *fmt, ...);

/**
 * @brief Reports a diagnostic with variadic arguments (va_list)
 * @details Records a diagnostic using a va_list instead of variadic arguments.
 * @param reporter The reporter instance.
 * @param diag_id The diagnostic type ID.
 * @param loc_id The source location ID.
 * @param fmt Printf-style format string.
 * @param args Variable argument list.
 */
void diag_report_vfmt(DiagReporter *reporter, DiagID diag_id, LocationID loc_id,
                      const char *fmt, va_list args);

/**
 * @brief Prints all collected diagnostics to standard output
 * @details Formats and displays all diagnostics with ANSI colors and
 * source file context (if source buffer is available).
 * @param reporter The reporter containing diagnostics to display.
 * @param source_buffer Optional source code buffer for context display.
 * Can be NULL if source context is not needed.
 */
void diag_print_all(const DiagReporter *reporter, const char *source_buffer);

/**
 * @brief Retrieves the number of reported diagnostics at a given level
 * @details Counts diagnostics filtered by severity level.
 * @param reporter The reporter to query.
 * @param level The diagnostic level to count.
 * @return Number of diagnostics at the specified level.
 */
size_t diag_count_by_level(const DiagReporter *reporter, DiagLevel level);

/**
 * @brief Checks if any errors were reported
 * @details Determines whether the reporter contains error-level or higher diagnostics.
 * @param reporter The reporter to check.
 * @return True if error-level diagnostics exist, false otherwise.
 */
bool diag_has_errors(const DiagReporter *reporter);

/**
 * @brief Gets the location manager from a reporter
 * @details Returns the embedded location manager for adding new locations.
 * @param reporter The reporter instance.
 * @return Pointer to the embedded LocationManager.
 */
LocationManager *diag_get_location_manager(DiagReporter *reporter);

#endif
