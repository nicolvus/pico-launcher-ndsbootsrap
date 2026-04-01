#pragma once
#include "services/process/IProcess.h"

class PicoLoaderProcess : public IProcess
{
public:
    void Run() override;

    void Exit() override
    {
        // This process never exits
    }
};
