#pragma once
#include <memory>
#include "IProcess.h"
#include "ProcessFactory.h"

class ProcessManager
{
    std::unique_ptr<IProcess> (*_nextProcConstructFunc)();
    std::unique_ptr<IProcess> _curProcess;

public:
    constexpr ProcessManager()
        : _nextProcConstructFunc(nullptr) { }

    void MainLoop();
    void SetupDefaultVBlankHandler();

    template <class T>
    [[gnu::noinline]]
    void Goto()
    {
        _nextProcConstructFunc = ProcessFactory::Construct<T>;
        if (_curProcess)
            _curProcess->Exit();
    }

    IProcess* GetRunningProcess() { return _curProcess.get(); }
};