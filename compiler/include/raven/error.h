#ifndef INCLUDE_INCLUDE_RVNERROR_H_
#define INCLUDE_INCLUDE_RVNERROR_H_

#define THROW(MSG, ...)                                                        \
  do {                                                                         \
    printf(MSG "\n", ##__VA_ARGS__);                                           \
    exit(1);                                                                   \
  } while (0)

#endif // INCLUDE_INCLUDE_RVNERROR_H_
