#include "common.h"
#include <libtwl/rtos/rtosIrq.h>
#include "ProcessManager.h"

static void vblankIrq(u32 irqMask)
{
    VBlank::NotifyIrq();
}

void ProcessManager::MainLoop()
{
    SetupDefaultVBlankHandler();
    while (_nextProcConstructFunc)
    {
        _curProcess = _nextProcConstructFunc();
        _curProcess->Run();
        SetupDefaultVBlankHandler(); // reset because the currently registered one may use _curProcess
        _curProcess.reset();
    }
}

void ProcessManager::SetupDefaultVBlankHandler()
{
    rtos_setIrqFunc(RTOS_IRQ_VBLANK, vblankIrq);
    rtos_enableIrqMask(RTOS_IRQ_VBLANK);
}