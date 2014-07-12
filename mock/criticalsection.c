#include "../criticalsection.h"

void critical_entered_impl(void)
{
}

void critical_exited_impl(void)
{
}

void (*critical_entered)(void) = critical_entered_impl;
void (*critical_exited)(void) = critical_exited_impl;
