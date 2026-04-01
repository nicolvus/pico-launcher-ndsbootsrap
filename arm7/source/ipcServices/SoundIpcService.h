#pragma once
#include "ipc/IpcService.h"
#include "ipcChannels.h"

class SoundIpcService : public IpcService
{
public:
    SoundIpcService()
        : IpcService(IPC_CHANNEL_SOUND) { }

    void OnMessageReceived(u32 data) override;
};
