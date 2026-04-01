#pragma once
#include "Task.h"

class TaskFactory
{
    template <typename T>
    static T TaskResultToResultType(TaskResult<T> arg);

public:
    template <typename FuncType>
    static auto Create(const FuncType& function)
    {
        return FuncTask<decltype(TaskResultToResultType(function())), FuncType>(function);
    }

    template <typename FuncType>
    static auto CreateOnHeap(const FuncType& function) -> Task<decltype(TaskResultToResultType(function()))>*
    {
        return new FuncTask<decltype(TaskResultToResultType(function())), FuncType>(function);
    }

    template <typename FuncType>
    static auto CreateOnHeapUnique(const FuncType& function) -> std::unique_ptr<Task<decltype(TaskResultToResultType(function()))>>
    {
        return std::make_unique<FuncTask<decltype(TaskResultToResultType(function())), FuncType>>(function);
    }
};
