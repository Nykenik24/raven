/**
 * @file diag/metadata.h
 * @brief Diagnostic metadata and type definitions.
 * @details Defines diagnostic severity levels, metadata structures,
 * and diagnostic type registration for metadata-driven error reporting.
 */

#ifndef CORE_DIAG_METADATA_H
#define CORE_DIAG_METADATA_H

#include <stdint.h>

/// Unique identifier for diagnostic types (metadata-driven)
typedef uint32_t DiagID;

/**
 * @brief Diagnostic severity levels
 */
typedef enum {
  DIAG_LEVEL_NOTE,    ///< Informational message
  DIAG_LEVEL_WARNING, ///< Warning condition
  DIAG_LEVEL_ERROR,   ///< Error condition
  DIAG_LEVEL_FATAL    ///< Fatal error condition
} DiagLevel;

/**
 * @brief Metadata describing a diagnostic type
 * @details Defines properties of a diagnostic including its unique ID,
 * human-readable name, default message template, severity level, and help text.
 * This allows for easy localization and consistent error reporting across the compiler.
 */
typedef struct {
  DiagID id;              ///< Unique diagnostic identifier
  const char *code;       ///< Error code (e.g., "E0001")
  const char *name;       ///< Diagnostic name (e.g., "invalid_character")
  DiagLevel level;        ///< Severity level
  const char *message;    ///< Default message template with format specifiers
  const char *help_text;  ///< Help text explaining the diagnostic
} DiagMetadata;

#endif
