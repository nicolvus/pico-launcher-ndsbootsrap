#pragma once
#include "ipc/ThreadIpcService.h"
#include "ipcChannels.h"

class RtcIpcService : public ThreadIpcService
{
    u32 _threadStack[128];

public:
    RtcIpcService()
        : ThreadIpcService(IPC_CHANNEL_RTC, 10, _threadStack, sizeof(_threadStack)) { }

    void HandleMessage(u32 data) override;
};
