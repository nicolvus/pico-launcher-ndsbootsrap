#pragma once
#include "TaskState.h"

template <class T>
class TaskResult
{
    const TaskState _finalState;
    const T _result;

    TaskResult(TaskState finalState)
        : _finalState(finalState) { }

    TaskResult(const T& result)
        : _finalState(TaskState::Completed), _result(result) { }

public:
    static TaskResult<T> Failed() { return TaskResult<T>(TaskState::Failed); }
    static TaskResult<T> Canceled() { return TaskResult<T>(TaskState::Canceled); }
    static TaskResult<T> Completed(const T& result) { return TaskResult<T>(result); }

    TaskState GetFinalState() const { return _finalState; }
    const T& GetResult() const { return _result; }
};

template <>
class TaskResult<void>
{
    const TaskState _finalState;

    TaskResult(TaskState finalState)
        : _finalState(finalState) { }

public:
    static TaskResult<void> Failed() { return TaskResult<void>(TaskState::Failed); }
    static TaskResult<void> Canceled() { return TaskResult<void>(TaskState::Canceled); }
    static TaskResult<void> Completed() { return TaskResult<void>(TaskState::Completed); }

    TaskState GetFinalState() const { return _finalState; }
};
