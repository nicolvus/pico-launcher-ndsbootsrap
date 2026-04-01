#pragma once

typedef enum
{
    DSI_SD_IPC_CMD_READ_SECTORS,
    DSI_SD_IPC_CMD_WRITE_SECTORS
} DsiSdIpcCommand;

typedef struct alignas(32)
{
    u32 cmd;
    void* buffer;
    u32 sector;
    u32 count;
    u8 sizeAlign[16]; // ensure the size is also cache aligned
} dsisd_ipc_cmd_t;