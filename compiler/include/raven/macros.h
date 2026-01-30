#ifndef INCLUDE_INCLUDE_MACROS_H_
#define INCLUDE_INCLUDE_MACROS_H_

#define TODO(SECTION)                                                          \
  printf("TODO: %s\n", SECTION);                                               \
  exit(0);

#define UNUSED(x) (void)(x);

#define SAFECPY(STR, TARGET)                                                   \
  if (STR) {                                                                   \
    size_t len = strlen(STR) + 1;                                              \
    TARGET = malloc(len);                                                      \
    memcpy(TARGET, STR, len);                                                  \
  } else {                                                                     \
    TARGET = NULL;                                                             \
  }

#endif // INCLUDE_INCLUDE_MACROS_H_
