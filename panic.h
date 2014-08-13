#ifndef PANIC_H_
#define PANIC_H_

/** Panics with a given error message and provides correct file and line
 * informations to panic().
 * It supports printf-like variable arguments list :
 * PANIC("error %d", 1234); is a valid invocation. */
#define PANIC(m, ...) panic(__FILE__, __LINE__, m, ##__VA_ARGS__)

extern void (*panic)(const char *file, int line, const char *msg, ...);

#endif
