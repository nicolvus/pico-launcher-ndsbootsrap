#include "common.h"
#include <libtwl/sound/soundChannel.h>
#include "soundIpcCommand.h"
#include "SoundIpcService.h"

void SoundIpcService::OnMessageReceived(u32 data)
{
    const u32* commandList = reinterpret_cast<const u32*>(data);
    u32 cmdCount = *commandList++;
    for (u32 i = 0; i < cmdCount; i++)
    {
        u32 cmdValue = *commandList++;
        u32 cmd = cmdValue & 0xFF;
        u32 cmdArg = cmdValue >> 8;
        switch (cmd)
        {
            case SND_IPC_CMD_START_CHANNELS:
            {
                u32 channelsMask = cmdArg;
                for (u32 j = 0; j < 16; j++)
                {
                    if (channelsMask & (1u << j))
                        snd_startChannel(j);
                }
                break;
            }
            case SND_IPC_CMD_STOP_CHANNELS:
            {
                u32 channelsMask = cmdArg;
                for (u32 j = 0; j < 16; j++)
                {
                    if (channelsMask & (1u << j))
                        snd_stopChannel(j);
                }
                break;
            }
            case SND_IPC_CMD_SETUP_CHANNEL:
            {
                u32 channel = cmdArg;
                REG_SOUNDxSAD(channel) = *commandList++;
                REG_SOUNDxTMR(channel) = *commandList++;
                REG_SOUNDxPNT(channel) = *commandList++;
                REG_SOUNDxLEN(channel) = *commandList++;
                REG_SOUNDxCNT(channel) = *commandList++;
                break;
            }
        }
    }
}
