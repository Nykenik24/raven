#include "raven/logger.h"
#include "raven/colors.h"

char *_log_type_to_string(logtype_t type) {
  switch (type) {
  case LOG_INFO:
    return GRN "INFO" CRESET;
    break;
  case LOG_DEBUG:
    return BLU "DEBUG" CRESET;
    break;
  case LOG_WARN:
    return YEL "WARN" CRESET;
    break;
  case LOG_ERROR:
    return RED "ERROR" CRESET;
    break;
  case LOG_FATAL:
    return MAG "FATAL" CRESET;
  }
  return "UNKNOWN";
}
