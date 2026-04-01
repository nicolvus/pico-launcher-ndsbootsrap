#include <nds/ndstypes.h>
#include "../mmc/sdmmc.h"
#include "DsiSdIpcService.h"

void DsiSdIpcService::Start()
{
    pico_SDMMC_init(SDMMC_DEV_CARD);
    ThreadIpcService::Start();
}

void DsiSdIpcService::HandleMessage(u32 data)
{
    auto cmd = reinterpret_cast<const dsisd_ipc_cmd_t*>(data << 2);
    switch (cmd->cmd)
    {
        case DSI_SD_IPC_CMD_READ_SECTORS:
            ReadSectors(cmd);
            break;

        case DSI_SD_IPC_CMD_WRITE_SECTORS:
            WriteSectors(cmd);
            break;
    }
}

void DsiSdIpcService::ReadSectors(const dsisd_ipc_cmd_t* cmd) const
{
    pico_SDMMC_readSectors(SDMMC_DEV_CARD, cmd->sector, cmd->buffer, cmd->count);
    SendResponseMessage(0);
}

void DsiSdIpcService::WriteSectors(const dsisd_ipc_cmd_t* cmd) const
{
    pico_SDMMC_writeSectors(SDMMC_DEV_CARD, cmd->sector, cmd->buffer, cmd->count);
    SendResponseMessage(0);
}
