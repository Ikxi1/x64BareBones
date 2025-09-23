#include <naiveTimer.h>

#include <naiveConsole.h>

#define TIMER_TPS 1000

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36

#define PIT_HZ 1193181
#define DIV_OF_FREQ(_f) (PIT_HZ / (_f))
#define FREQ_OF_DIV(_d) (PIT_HZ / (_d))
#define REAL_FREQ_OF_FREQ(_f) (FREQ_OF_DIV(DIV_OF_FREQ((_f))))

static struct {
    uint64 frequency;
    uint64 divisor;
    uint64 ticks;
} state;

static void timer_set(int32 hz) {
    outportb(PIT_CONTROL, PIT_SET);

    uint16 d = (uint16) (1193131.666 / hz);
    outportb(PIT_A, d & PIT_MASK);
    outportb(PIT_A, (d >> 8) & PIT_MASK);
}

uint64 timer_get() {
    return state.ticks;
}

void pit_irq() {
    state.ticks++;
    // ncPrintBase(state.ticks, 10, 1);
}

void timer_init() {
    const uint64 freq = REAL_FREQ_OF_FREQ(TIMER_TPS);
    state.frequency = freq;
    state.divisor = DIV_OF_FREQ(freq);
    state.ticks = 0;
    timer_set(TIMER_TPS);
}