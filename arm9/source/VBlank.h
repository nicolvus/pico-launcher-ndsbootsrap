#pragma once
#include <libtwl/rtos/rtosEvent.h>

/// @brief Helper class for waiting for vblank.
class VBlank
{
public:
    static void Init()
    {
        rtos_createEvent(&sEvent);
    }

    static void Wait(bool waitNew = true, bool clearAfter = true)
    {
        rtos_waitEvent(&sEvent, waitNew, clearAfter);
    }

    static void NotifyIrq()
    {
        rtos_signalEvent(&sEvent);
    }

private:
    static rtos_event_t sEvent;
};