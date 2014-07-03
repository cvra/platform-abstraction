#ifndef PANIC_H_
#define PANIC_H_

/** Panics with a given error message and provides correct file and line
 * informations to panic(). */
#define PANIC(m) panic(__FILE__, __LINE__, m)

extern void (*panic)(const char *file, int line, const char *msg);

#endif
