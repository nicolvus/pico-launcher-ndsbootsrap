#pragma once
#include <memory>
#include "IProcess.h"

class ProcessFactory
{
    ProcessFactory() { }
public:
    template <class T>
    static std::unique_ptr<IProcess> Construct();
};