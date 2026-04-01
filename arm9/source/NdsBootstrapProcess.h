#pragma once
#include "services/process/IProcess.h"

class NdsBootstrapProcess : public IProcess
{
public:
    void Run() override;

    void Exit() override
    {
        // This process never exits on success; may return if launch preparation fails.
    }
};
