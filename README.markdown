# Platform Abstraction Layer
[![Build Status](https://travis-ci.org/cvra/platform-abstraction.png)](https://travis-ci.org/cvra/platform-abstraction)

This modules provides abstractions over common RTOS operations, such as
semaphore creation, threads, etc.
This abstraction layers allows for better portability across various RTOS
platforms and allows to mock operating system calls for unit/integration
testing.

Currently we aim to support uc/OS-II and the mock platform for testing.

# Memory Allocation
Our custom wrapper `xmalloc` should be used whenever the program would die when `malloc` fails.
It has the same prototype as standard `malloc` and always returns a valid value or hard crashes.

# Fatal error handling
Fatal error handling is done through the use of the `PANIC` macro.

## Use in testing
`panic` is defined as a function pointer which allows one to replace it at runtime with a custom version to capture panics in tests.
See `tests/panic_mock_test.cpp` for an example.

# Semaphores

## Example
```cpp
/* Creates a semaphore with a counter value of 10. */
semaphore_t mysem;
os_semaphore_init(&mysem, 10);

/* Take (decrease) semaphore, blocks until available.*/
os_semaphore_wait(&mysem);

/* Only take semaphore when immediately available. */
if (os_semaphore_try(&mysem)) {
    // Decreased semaphore
}

/* Block until available or timeout. */
if (os_semaphore_try_timeout(&mysem, 42000)) {
    // Decreased semaphore
} else {
    // Semaphore not greater than zero after 42 ms of waiting
}

/* Release (increase) semaphore. */
os_semaphore_signal(&mysem);
```

## Use in testing
When using the mock implementation `semaphore_t` is defined as a structure with two public fields:

1. `count` which indicates the value of the simulated semaphore counter.
2. `acquired_count` which contains the number of time this semaphore was taken.

This allows the developper to test that, for example, a function correctly releases any acquired semaphore :

```cpp
semaphore_t *sem = os_semaphore_create(5);
do_concurrent_stuff(sem);
CHECK_EQUAL(5, sem->count);
```


# Mutex
Mutex are different than just a semaphore with a value of 0 or 1.
They provide a way to reduce priority inversion of two tasks by promoting the priority of task A if another task B of higher priority than A is waiting on a mutex owned by A.

## Example

```c
/* Mutex creation */
mutex_t my_mutex;
os_mutex_init(&my_mutex);

/* Acquire mutex */
os_mutex_take(&my_mutex);

/* Acquire mutex non-blocking */
if (os_mutex_try(&my_mutex)) {
    // The resource is mine.
}

/* Acquire mutex blocking with timeout */
if (os_mutex_try_timeout(&my_mutex, 69000)) {
    // The resource is mine.
} else {
    // Waited 69 ms for the resource. It wasn't enough.
}

/* Release mutex */
os_mutex_release(&my_mutex);
```

## Use in testing
When using the mock implementation, `mutex_t` is defined as a structure with two public fields:

1. `acquired` which is `true` when the mutex is acquired.
2. `acquired_count` which contains the number of time this mutex was taken.

This allows test to check that a mutex is correctly released :
```c
mutex_t *my_mutex = os_mutex_create();
do_atomic_stuff(my_mutex);
CHECK_FALSE(my_mutex->acquired);
```

# Critical Section
A critical section disables temporarily all interrupts at the CPU level, ensuring that all variables and data structures are edited atomically.


## Direct Example 

```c
void myFunction(void* mySharedResource) {

    /* initialize variables */
    int myVariable = 0; 

    /* after ALL initialization, call this */
    CRITICAL_SECTION_ALLOC();

    /* ... do whatever stuff you need ... */

    /* enter critical section */
    CRITICAL_SECTION_ENTER();

    // The resource is mine.

    /* exit critical section */
    CRITICAL_SECTION_EXIT();
}
```

## Block Example 

```c
void myFunction(void* mySharedResource) {

    /* initialize variables */
    int myVariable = 0; 

    /* after ALL initialization, call this */
    CRITICAL_SECTION_ALLOC();

    /* ... do whatever stuff you need ... */

    /* enter critical section */
    CRITICAL_SECTION() {
        // The resource is mine.
    }
}
```

## Use in testing
When using the mock implementation, critical section is represented through a single variable.
`__mock_critical_depth` contains the number of nested critical sections currently active.
To access this value, two function-like macros are available.

1. `mock_critsec_is_critical()` returns a bool, indicating whether the calling code is currently in a critical section
2. `mock_critsec_get_depth()` returns an `int16_t` indicating the nesting depth of the critical sections

This allows tests to check that a critical section finishes correctly :
```c
CRITICAL_SECTION_ALLOC();
CRIICAL_SECTION() { 
    do_atomic_stuff();
}
CHECK_FALSE(mock_critsec_is_critical());
```

# Threading
## Usage
```cpp
/* Creates a 2048 bytes stack. */
THREAD_STACK mystack[2048];
os_thread_t mythread;

const int myprio = 10;

void mymain(void *context)
{
    while (1) {
        /* ... */

        do_something();

        /* wait 1000000 microseconds */
        os_thread_sleep_us(1000000);

        /* ... */
    }
}

void main(void) {
    /* Inits all the operating system structures. */
    os_init();

    /* Run thread. Last parameter will be passed as context to the thread */
    os_thread_create(&mythread, mymain, mystack, sizeof(mystack), "my thread", myprio, NULL);

    /* Same thread but with a dynamically allocated stack. */
    os_thread_create(&mythread, mymain, NULL, 2048, "my thread", myprio, NULL);

    /* Starts multi-tasking. */
    thread_run();
}
```
