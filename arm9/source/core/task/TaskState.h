#pragma once

enum class TaskState : u8
{
    NotStarted,
    Running,
    Completed,
    Canceled,
    Failed
};
