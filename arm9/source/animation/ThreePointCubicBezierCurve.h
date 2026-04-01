#pragma once
#include "Curve.h"
#include "CubicBezierCurve.h"

class ThreePointCubicBezierCurve : public Curve
{
    using fx = fix16<13>;

    fix32<26> _midPointX;
    fix32<26> _midPointY;
    fix32<26> _invMidPointX;
    fix32<26> _invOneMinusMidPointX;

    const CubicBezierCurve _firstCurve;
    const CubicBezierCurve _secondCurve;

public:
    constexpr ThreePointCubicBezierCurve(
        fx a1x, fx a1y, fx b1x, fx b1y,
        fx midpointx, fx midpointy,
        fx a2x, fx a2y, fx b2x, fx b2y)
        : _midPointX(midpointx)
        , _midPointY(midpointy)
        , _invMidPointX(fix32<26>(1) / midpointx)
        , _invOneMinusMidPointX(fix32<26>(1) / (1 - midpointx))
        , _firstCurve(
            a1x / midpointx,
            a1y / midpointy,
            b1x / midpointx,
            b1y / midpointy)
        , _secondCurve(
            (a2x - midpointx) / (1 - midpointx),
            (a2y - midpointy) / (1 - midpointy),
            (b2x - midpointx) / (1 - midpointx),
            (b2y - midpointy) / (1 - midpointy)) { }

    [[gnu::noinline]]
    fix32<26> Compute(fix32<26> t) const override
    {
        if (t < _midPointX)
            return _firstCurve.Compute(t * _invMidPointX) * _midPointY;
        else
        {
            fix32<26> scaledT = (t - _midPointX) * _invOneMinusMidPointX;
            return _secondCurve.Compute(scaledT) * (1 - _midPointY) + _midPointY;
        }
    }
};