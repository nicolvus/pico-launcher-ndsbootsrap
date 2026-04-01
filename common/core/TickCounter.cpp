#include "common.h"
#include <libtwl/timer/timer.h>
#include <libtwl/rtos/rtosIrq.h>
#include "TickCounter.h"

TickCounter gTickCounter;

void TickCounter::TimerOverflowIrq()
{
    _msb += 1 << 16;
}

void TickCounter::Start()
{
    _msb = 0;
    tmr_configure(0, TMCNT_H_CLK_SYS_DIV_64, 0, true);
    rtos_ackIrqMask(RTOS_IRQ_TIMER0);
    rtos_setIrqFunc(RTOS_IRQ_TIMER0, [] (u32 mask) { gTickCounter.TimerOverflowIrq(); });
    rtos_enableIrqMask(RTOS_IRQ_TIMER0);
    tmr_start(0);
}

void TickCounter::Stop()
{
    rtos_disableIrqMask(RTOS_IRQ_TIMER0);
    rtos_setIrqFunc(RTOS_IRQ_TIMER0, nullptr);
    tmr_stop(0);
}

u64 TickCounter::GetValue()
{
    u32 irqs = rtos_disableIrqs();
    u32 value0 = tmr_getCounter(0);
    u64 msb = _msb;
    u32 value1 = tmr_getCounter(0);
    rtos_restoreIrqs(irqs);
    if (value1 < value0)
        value1 += 1 << 16;
    return msb + value1;
}