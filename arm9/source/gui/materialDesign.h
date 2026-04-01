#pragma once
#include "animation/LinearCurve.h"
#include "animation/CubicBezierCurve.h"
#include "animation/ThreePointCubicBezierCurve.h"

namespace md::sys
{
    enum class color
    {
        primary,
        onPrimary,
        secondaryContainer,
        onSecondaryContainer,
        onSurfaceVariant,
        outline,
        onSurface,
        inverseOnSurface,
        surfaceContainerLow = inverseOnSurface, // basically the same color
        surfaceContainerHighest,
        surfaceBright,

        Max = surfaceBright
    };
}

namespace md::sys::motion::duration
{
#define MILLISECONDS_TO_FRAMES(x)   ((x) * 60 / 1000)

    constexpr int short1 = MILLISECONDS_TO_FRAMES(50);
    constexpr int short2 = MILLISECONDS_TO_FRAMES(100);
    constexpr int short3 = MILLISECONDS_TO_FRAMES(150);
    constexpr int short4 = MILLISECONDS_TO_FRAMES(200);
    constexpr int medium1 = MILLISECONDS_TO_FRAMES(250);
    constexpr int medium2 = MILLISECONDS_TO_FRAMES(300);
    constexpr int medium3 = MILLISECONDS_TO_FRAMES(350);
    constexpr int medium4 = MILLISECONDS_TO_FRAMES(400);
    constexpr int long1 = MILLISECONDS_TO_FRAMES(450);
    constexpr int long2 = MILLISECONDS_TO_FRAMES(500);
    constexpr int long3 = MILLISECONDS_TO_FRAMES(550);
    constexpr int long4 = MILLISECONDS_TO_FRAMES(600);
    constexpr int extraLong1 = MILLISECONDS_TO_FRAMES(700);
    constexpr int extraLong2 = MILLISECONDS_TO_FRAMES(800);
    constexpr int extraLong3 = MILLISECONDS_TO_FRAMES(900);
    constexpr int extraLong4 = MILLISECONDS_TO_FRAMES(1000);

#undef MILLISECONDS_TO_FRAMES
}

namespace md::sys::motion::easing
{
    constexpr auto linear = LinearCurve();
    constexpr auto standard = CubicBezierCurve(0.2, 0, 0, 1);
    constexpr auto standardAccelerate = CubicBezierCurve(0.3, 0, 1, 1);
    constexpr auto standardDecelerate = CubicBezierCurve(0, 0, 0, 1);
    constexpr auto emphasized = ThreePointCubicBezierCurve(
        0.05, 0, 0.133333, 0.06,
        0.166666, 0.4,
        0.208333, 0.82, 0.25, 1);
    constexpr auto emphasizedDecelerate = CubicBezierCurve(0.05, 0.7, 0.1, 1);
    constexpr auto emphasizedAccelerate = CubicBezierCurve(0.3, 0, 0.8, 0.15);
}