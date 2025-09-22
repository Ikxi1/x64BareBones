#ifndef NAIVE_TIMER_H
#define NAIVE_TIMER_H

#include <lib.h>
#include <stdint.h>

void pit_irq();

void timer_init();

uint64 timer_get();

#endif
