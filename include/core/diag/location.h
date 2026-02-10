/**
 * @file diag/location.h
 * @brief Source location tracking for diagnostics.
 * @details Manages source file locations with efficient index-based lookup
 * and string interning for memory optimization.
 */

#ifndef CORE_DIAG_LOCATION_H
#define CORE_DIAG_LOCATION_H

#include <stdint.h>

/// Opaque location manager handle
typedef struct LocationManager LocationManager;

/// Unique identifier for source locations (index into location table)
typedef uint32_t LocationID;

/**
 * @brief Source location information
 * @details Stores file path, line, column, and affected token length
 */
typedef struct {
  const char *path;   ///< Source file path
  uint32_t line;      ///< Line number (1-based)
  uint32_t column;    ///< Column number (1-based)
  uint32_t length;    ///< Length of affected token/code region
} DiagLocation;

/**
 * @brief Creates a new location manager
 * @details Initializes an empty location manager for tracking source locations.
 * @return Pointer to newly allocated LocationManager, or NULL on memory error.
 */
LocationManager *diag_location_manager_create(void);

/**
 * @brief Frees a location manager and all stored locations
 * @details Releases all memory associated with the location manager.
 * @param manager The location manager to free. Safe to call with NULL.
 */
void diag_location_manager_free(LocationManager *manager);

/**
 * @brief Adds a source location to the location manager
 * @details Stores location information and returns a LocationID for indexing.
 * Strings are interned to avoid duplication.
 * @param manager The location manager instance.
 * @param path Source file path. String is duplicated internally.
 * @param line Line number in source file.
 * @param column Column number in source file.
 * @param length Length of affected code region.
 * @return LocationID that uniquely identifies this location, or 0 on error.
 */
LocationID diag_location_add(LocationManager *manager, const char *path,
                             uint32_t line, uint32_t column, uint32_t length);

/**
 * @brief Retrieves location information from a LocationID
 * @details Looks up stored location data by its ID.
 * @param manager The location manager instance.
 * @param loc_id The location ID to retrieve.
 * @return DiagLocation structure with location data.
 */
DiagLocation diag_location_get(LocationManager *manager, LocationID loc_id);

#endif
