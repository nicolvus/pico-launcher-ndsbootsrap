#pragma once
#include "RandomGenerator.h"

class LinearCongruentialGenerator : public RandomGenerator
{
public:
    explicit LinearCongruentialGenerator(u64 seed)
        : _state(seed) { }

    void Seed(u64 seed) override
    {
        _state = seed;
    }

protected:
    u32 Next() override;

private:
    u64 _state;
};
