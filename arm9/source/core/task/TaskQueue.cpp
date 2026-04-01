#include "common.h"
#include "TaskQueue.h"

void TaskQueueBase::ThreadMain(TaskBase** queue, u32 queueLength)
{
    while (true)
    {
        _idle = false;
        u32 readPtr = _queueReadPtr;
        while (readPtr != _queueWritePtr)
        {
            TaskBase* task = queue[readPtr];
            if (readPtr == queueLength - 1)
                readPtr = 0;
            else
                readPtr++;
            _queueReadPtr = readPtr;
            if (!task)
                continue;
            task->Execute();
            if (task->GetDestroyWhenComplete())
            {
                // this will destroy the task
                ReturnOwnership(task);
            }
        }
        if (_endThreadWhenDone)
            return;
        _idle = true;
        rtos_waitEvent(&_event, false, true);
    }
}

void QueueTaskBase::Dispose()
{
    if (_task)
    {
        TaskBase* task = _task;
        _task = nullptr;
        _taskQueue->ReturnOwnership(task);
        _taskQueue = nullptr;
    }
}