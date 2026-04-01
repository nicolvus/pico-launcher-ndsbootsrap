#pragma once
#include "ipc/ThreadIpcService.h"
#include "dsiSdIpcCommand.h"
#include "ipcChannels.h"

class DsiSdIpcService : public ThreadIpcService
{
    u32 _threadStack[128];

    void ReadSectors(const dsisd_ipc_cmd_t* cmd) const;
    void WriteSectors(const dsisd_ipc_cmd_t* cmd) const;

public:
    DsiSdIpcService()
        : ThreadIpcService(IPC_CHANNEL_DSI_SD, 5, _threadStack, sizeof(_threadStack)) { }

    void Start() override;
    void HandleMessage(u32 data) override;
};
