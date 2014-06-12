# Platform Abstraction Layer
[![Build Status](https://travis-ci.org/cvra/platform-abstraction.png)](https://travis-ci.org/cvra/platform-abstraction)

This modules provides abstractions over common RTOS operations, such as
semaphore creation, threads, etc.
This abstraction layers allows for better portability across various RTOS
platforms and allows to mock operating system calls for unit/integration
testing.

Currently we aim to support uc/OS-II and the mock platform for testing.

# Semaphores

Create a semaphore with an initial count of 10:

```cpp
semaphore_t *mysem;
mysem = os_semaphore_create(10);
```

Delete a semaphore:
```cpp
os_semaphore_delete(mysem);
```

Signal (release) a semaphore:
```cpp
os_semaphore_release(mysem);
```

Take a semaphore (blocking):
```cpp
os_semaphore_take(mysem);
```

## Use in testing
When using the mock implementation `semaphore_t` is defined as a structure with two public fields:

1. `count` which indicates the value of the simulated semaphore counter.
2. `acquired_count` which contains the number of time this semaphore was taken.

This allows the developper to test that, for example, a function correctly releases any acquired semaphore.

# Mutex

Create a mutex:
```c
mutex_t *my_mutex;
my_mutex = os_mutex_create();
```

Delete a mutex:
```c
os_mutex_delete(my_mutex);
```

Acquire a mutex:
```c
os_mutex_take(my_mutex);
```

Release a mutex:
```c
os_mutex_release(my_mutex);
```

## Use in testing
When using the mock implementation, `mutex_t` is defined as a structure with two public fields:

1. `acquired` which is `true` when the mutex is acquired.
2. `acquired_count` which contains the number of time this mutex was taken.
