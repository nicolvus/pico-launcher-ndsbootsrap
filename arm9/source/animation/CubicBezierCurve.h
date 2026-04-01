#pragma once
#include "Curve.h"

class CubicBezierCurve : public Curve
{
    using fx = fix16<13>;

    const fx _cx;
    const fx _bx;
    const fx _ax;

    const fx _cy;
    const fx _by;
    const fx _ay;

    [[gnu::noinline]]
    static constexpr fix32<26> SampleCurve(fx ax, fx bx, fx cx, fx t)
    {
        return ((ax * t + bx) * t + cx).LongMul(t);
    }

public:
    constexpr CubicBezierCurve(fx p1x, fx p1y, fx p2x, fx p2y)
        : _cx(3 * p1x)
        , _bx(3 * (p2x - p1x) - _cx)
        , _ax(1 - _cx - _bx)
        , _cy(3 * p1y)
        , _by(3 * (p2y - p1y) - _cy)
        , _ay(1 - _cy - _by) { }

    [[gnu::noinline]]
    fix32<26> Compute(fix32<26> t) const override
    {
        fx start = 0.0;
        fx end = 1.0;
        while (true)
        {
            const fx midpoint = (start + end) >> 1;
            const fix32<26> estimate = SampleCurve(_ax, _bx, _cx, midpoint);
            if ((t - estimate).Abs() < 0.001)
                return SampleCurve(_ay, _by, _cy, midpoint);
            if (estimate < t)
                start = midpoint;
            else
                end = midpoint;
        }
    }
};