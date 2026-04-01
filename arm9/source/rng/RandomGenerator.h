#pragma once
#include "common.h"

class RandomGenerator
{
    template<class> friend class ThreadSafeRandomGenerator;

public:
    virtual void Seed(u64 seed) = 0;

    u32 NextU32()
    {
        return Next();
    }

    u32 NextU32(u32 maxPlusOne)
    {
        return ((u64)Next() * maxPlusOne) >> 32;
    }

    u32 NextU32(u32 min, u32 maxPlusOne)
    {
        return NextU32(maxPlusOne - min) + min;
    }

    s32 NextS32()
    {
        return Next();
    }

    u32 NextS32(s32 min, s32 maxPlusOne)
    {
        return NextU32(maxPlusOne - min) + min;
    }

protected:
    virtual u32 Next() = 0;
};
