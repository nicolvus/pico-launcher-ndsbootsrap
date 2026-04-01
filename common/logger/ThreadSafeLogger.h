#pragma once
#include <memory>
#include <libtwl/rtos/rtosMutex.h>
#include "ILogger.h"

class ThreadSafeLogger : public ILogger
{
    std::unique_ptr<ILogger> _logger;
    rtos_mutex_t _mutex;

public:
    explicit ThreadSafeLogger(std::unique_ptr<ILogger> logger)
        : _logger(std::move(logger))
    {
        rtos_createMutex(&_mutex);
    }

    void LogV(LogLevel level, const char* fmt, va_list vlist) override
    {
        rtos_lockMutex(&_mutex);
        {
            _logger->LogV(level, fmt, vlist);
        }
        rtos_unlockMutex(&_mutex);
    }
};