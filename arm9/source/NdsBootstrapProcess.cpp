#include "common.h"
#include <cstdio>
#include <cstring>
#include <nds/arm9/console.h>
#include <nds/arm9/video.h>
#include <nds/system.h>
#include "NdsBootstrapLaunch.h"
#include "NdsBootstrapProcess.h"
#include "fat/File.h"

namespace
{
int gBootstrapDebugLine = 0;
File gLogFile;

void InitTopDebugConsole()
{
    REG_MASTER_BRIGHT = 0;
    REG_MASTER_BRIGHT_SUB = 0;

    consoleDemoInit();
    
    std::printf("\x1b[2J");
    std::printf("nds-bootstrap launch log\n");
    std::printf("----------------------\n");
    gBootstrapDebugLine = 2;

    if (gLogFile.Open("sd:/_pico/launch_log.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        gLogFile.Open("fat:/_pico/launch_log.txt", FA_WRITE | FA_CREATE_ALWAYS);
    }
}

void TopScreenLaunchLog(const char* msg)
{
    if (!msg || !*msg)
        return;

    if (gBootstrapDebugLine >= 23)
    {
        std::printf("\x1b[2J");
        std::printf("nds-bootstrap launch log\n");
        std::printf("----------------------\n");
        gBootstrapDebugLine = 2;
    }

    std::printf("%s\n", msg);
    gBootstrapDebugLine++;

    u32 written;
    gLogFile.Write(msg, std::strlen(msg), written);
    gLogFile.Write("\n", 1, written);
    gLogFile.Sync();

    for (volatile int i = 0; i < 4000000; i++) {}
}
} // namespace

void NdsBootstrapProcess::Run()
{
    InitTopDebugConsole();
    TopScreenLaunchLog("Process started");

    const bool launched = TryLaunchNdsBootstrap(TopScreenLaunchLog);
    if (!launched)
    {
        TopScreenLaunchLog("Launch failed, waiting...");
        gLogFile.Close();
        while (true)
        {
            // spin
        }
    }
}
