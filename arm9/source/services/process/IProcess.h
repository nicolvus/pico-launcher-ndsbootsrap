#pragma once

class IProcess
{
public:
    virtual ~IProcess() { }

    virtual void Run() = 0;
    virtual void Exit() = 0;
};