#pragma once
#include "common.h"

class Interpolator
{
public:
    template <typename T>
    static constexpr T InterpolateLinear(T a, T b, T t)
    {
        return a + (b - a) * t;
    }

    template <typename T, u32 FractionBits>
    static constexpr fixed<T, FractionBits> InterpolateCubic(
        fixed<T, FractionBits> cf0, fixed<T, FractionBits> cf1,
        fixed<T, FractionBits> cf2, fixed<T, FractionBits> cf3, fixed<T, FractionBits> t)
    {
        return ((((cf0 * t + cf1) * t) + cf2) * t) + cf3;
    }
};