#pragma once

class TickCounter
{
public:
    void Start();
    void Stop();
    u64 GetValue();

    static u32 TicksToMilliSeconds(u32 ticks)
    {
        return ((ticks * 8201887ULL) + (1ULL << 31)) >> 32;
    }

    static u32 TicksToMicroSeconds(u32 ticks)
    {
        return (ticks * 4100943703ULL + (1 << 30)) >> 31;
    }

private:
    u64 _msb = 0;

    void TimerOverflowIrq();
};

extern TickCounter gTickCounter;