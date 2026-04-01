#include "common.h"
#include <string.h>
#include <nds/arm9/cache.h>
#include "libtwl/rtos/rtosEvent.h"
#include "libtwl/rtos/rtosMutex.h"
#include "libtwl/ipc/ipcFifoSystem.h"
#include "ipcChannels.h"
#include "dsiSdIpcCommand.h"

static rtos_mutex_t sMutex;
static rtos_event_t sEvent;

alignas(32) static dsisd_ipc_cmd_t sIpcCommand;

alignas(32) static u8 sTempBuffers[2][512];

static void ipcMessageHandler(u32 channel, u32 data, void* arg)
{
    rtos_signalEvent(&sEvent);
}

void dsisd_init()
{
    rtos_createMutex(&sMutex);
    rtos_createEvent(&sEvent);
    ipc_setChannelHandler(IPC_CHANNEL_DSI_SD, ipcMessageHandler, nullptr);
}

static void readSectorsCacheAligned(void* buffer, u32 sector, u32 count)
{
    sIpcCommand.cmd = DSI_SD_IPC_CMD_READ_SECTORS;
    sIpcCommand.buffer = buffer;
    sIpcCommand.sector = sector;
    sIpcCommand.count = count;
    DC_InvalidateRange(buffer, 512 * count);
    rtos_clearEvent(&sEvent);
    DC_FlushRange(&sIpcCommand, sizeof(sIpcCommand));
    ipc_sendFifoMessage(IPC_CHANNEL_DSI_SD, (u32)&sIpcCommand >> 2);
    rtos_waitEvent(&sEvent, false, true);
}

static void readSectorsNotCacheAligned(void* buffer, u32 sector, u32 count)
{
    rtos_clearEvent(&sEvent);
    sIpcCommand.cmd = DSI_SD_IPC_CMD_READ_SECTORS;
    sIpcCommand.count = 1;
    // not cache aligned, use a temp buffer
    DC_InvalidateRange(sTempBuffers[0], 512);
    for (u32 i = 0; i < count; i++)
    {
        sIpcCommand.buffer = sTempBuffers[i & 1];
        sIpcCommand.sector = sector + i;
        DC_FlushRange(&sIpcCommand, sizeof(sIpcCommand));
        ipc_sendFifoMessage(IPC_CHANNEL_DSI_SD, (u32)&sIpcCommand >> 2);
        if (i != 0)
        {
            memcpy(static_cast<u8*>(buffer) + 512 * (i - 1), sTempBuffers[(i - 1) & 1], 512);
        }
        if (i != count - 1)
        {
            DC_InvalidateRange(sTempBuffers[(i + 1) & 1], 512);
        }
        rtos_waitEvent(&sEvent, false, true);
    }
    memcpy(static_cast<u8*>(buffer) + 512 * (count - 1), sTempBuffers[(count - 1) & 1], 512);
}

extern "C" void dsisd_readSectors(void* buffer, u32 sector, u32 count)
{
    // LOG_TRACE("dsisd_readSectors %p, %d, %d\n", buffer, sector, count);
    if (count == 0)
        return;
    rtos_lockMutex(&sMutex);
    {
        if ((u32)buffer & 0x1F)
        {
            LOG_DEBUG("unaligned\n");
            readSectorsNotCacheAligned(buffer, sector, count);
        }
        else
        {
            readSectorsCacheAligned(buffer, sector, count);
        }
    }
    rtos_unlockMutex(&sMutex);
}

static void writeSectorsCacheAligned(const void* buffer, u32 sector, u32 count)
{
    sIpcCommand.cmd = DSI_SD_IPC_CMD_WRITE_SECTORS;
    sIpcCommand.buffer = (void*)buffer;
    sIpcCommand.sector = sector;
    sIpcCommand.count = count;
    DC_FlushRange(buffer, 512 * count);
    rtos_clearEvent(&sEvent);
    DC_FlushRange(&sIpcCommand, sizeof(sIpcCommand));
    ipc_sendFifoMessage(IPC_CHANNEL_DSI_SD, (u32)&sIpcCommand >> 2);
    rtos_waitEvent(&sEvent, false, true);
}

static void writeSectorsNotCacheAligned(const void* buffer, u32 sector, u32 count)
{
    rtos_clearEvent(&sEvent);
    sIpcCommand.cmd = DSI_SD_IPC_CMD_WRITE_SECTORS;
    sIpcCommand.count = 1;
    // not cache aligned, use a temp buffer
    memcpy(sTempBuffers[0], static_cast<const u8*>(buffer), 512);
    DC_FlushRange(sTempBuffers[0], 512);
    for (u32 i = 0; i < count; i++)
    {
        sIpcCommand.buffer = sTempBuffers[i & 1];
        sIpcCommand.sector = sector + i;
        DC_FlushRange(&sIpcCommand, sizeof(sIpcCommand));
        ipc_sendFifoMessage(IPC_CHANNEL_DSI_SD, (u32)&sIpcCommand >> 2);
        if (i != count - 1)
        {
            memcpy(sTempBuffers[(i + 1) & 1], static_cast<const u8*>(buffer) + 512 * (i + 1), 512);
            DC_FlushRange(sTempBuffers[(i + 1) & 1], 512);
        }
        rtos_waitEvent(&sEvent, false, true);
    }
}

extern "C" void dsisd_writeSectors(const void* buffer, u32 sector, u32 count)
{
    // LOG_TRACE("dsisd_writeSectors %p, %d, %d\n", buffer, sector, count);
    if (count == 0)
        return;
    rtos_lockMutex(&sMutex);
    {
        if ((u32)buffer & 0x1F)
        {
            LOG_DEBUG("unaligned\n");
            writeSectorsNotCacheAligned(buffer, sector, count);
        }
        else
        {
            writeSectorsCacheAligned(buffer, sector, count);
        }
    }
    rtos_unlockMutex(&sMutex);
}