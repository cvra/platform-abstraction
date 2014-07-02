#ifndef PANIC_H_
#define PANIC_H_

/** Panics with a given error message. */
extern void (*panic)(const char *msg);

#endif
