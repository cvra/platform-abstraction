module_name=platform-abstraction
module_version=`git rev-parse --short=8 HEAD`

depends_on=()

includes=()

linkpaths=()

configs=(
    tests
    embedded
    x86
)

embedded=(
    xmalloc.c
    panic.c
)

embedded_linklibs=()

x86=(
    xmalloc.c
    panic.c
    mock/semaphores.c
    mock/mutex.c
)

x86_linklibs=()

tests=(
    ${x86[@]}
    tests/main.cpp
    tests/semaphore_mock_test.cpp
    tests/mutex_mock_test.cpp
    tests/criticalsection_mock_test.cpp
    tests/panic_mock_test.cpp
    tests/malloc_test.cpp
)

tests_linklibs=(
    CppUTest
    CppUTestExt
)
