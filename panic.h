#ifndef PANIC_H_
#define PANIC_H_

#define PANIC(m) panic(__FILE__, __LINE__, m)

/** Panics with a given error message. */
extern void (*panic)(const char *file, int line, const char *msg);

#endif
