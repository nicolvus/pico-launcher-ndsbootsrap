#pragma once
#include <memory>
#include <libtwl/rtos/rtosMutex.h>
#include "RandomGenerator.h"

template <class TGenerator>
class ThreadSafeRandomGenerator : public RandomGenerator
{
public:
    template <typename ...Args>
    explicit ThreadSafeRandomGenerator(Args && ...args)
        : _generator(std::forward<Args>(args)...)
    {
        rtos_createMutex(&_mutex);
    }

    void Seed(u64 seed) override
    {
        rtos_lockMutex(&_mutex);
        {
            _generator.Seed(seed);
        }
        rtos_unlockMutex(&_mutex);
    }

protected:
    u32 Next() override
    {
        u32 result;
        rtos_lockMutex(&_mutex);
        {
            result = ((RandomGenerator&)_generator).Next();
        }
        rtos_unlockMutex(&_mutex);
        return result;
    }

private:
    rtos_mutex_t _mutex;
    TGenerator _generator;
};
