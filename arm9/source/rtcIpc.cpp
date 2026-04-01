#include "common.h"
#include <string.h>
#include <nds/arm9/cache.h>
#include <libtwl/rtos/rtosEvent.h>
#include <libtwl/rtos/rtosMutex.h>
#include <libtwl/ipc/ipcFifoSystem.h>
#include "ipcChannels.h"
#include "rtcIpc.h"

static rtos_mutex_t sMutex;
static rtos_event_t sEvent;

alignas(32) static union
{
    rtc_datetime_t dateTime;
    u8 sizeAlign[32]; // ensure the size is also cache aligned to not corrupt nearby memory when invalidating
} sDateTime;

static void ipcMessageHandler(u32 channel, u32 data, void* arg)
{
    rtos_signalEvent(&sEvent);
}

void rtc_init()
{
    rtos_createMutex(&sMutex);
    rtos_createEvent(&sEvent);
    ipc_setChannelHandler(IPC_CHANNEL_RTC, ipcMessageHandler, nullptr);
}

void rtc_readDateTime(rtc_datetime_t* dateTime)
{
    rtos_lockMutex(&sMutex);
    {
        rtos_clearEvent(&sEvent);
        DC_InvalidateRange(&sDateTime, sizeof(sDateTime));
        ipc_sendFifoMessage(IPC_CHANNEL_RTC, (u32)&sDateTime >> 2);
        rtos_waitEvent(&sEvent, false, true);
        memcpy(dateTime, &sDateTime.dateTime, sizeof(rtc_datetime_t));
    }
    rtos_unlockMutex(&sMutex);
}
