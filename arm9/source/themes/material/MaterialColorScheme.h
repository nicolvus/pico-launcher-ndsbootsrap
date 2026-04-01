#pragma once
#include "gui/materialDesign.h"
#include "core/math/Rgb.h"

struct MaterialColorScheme
{
    using Rgb888 = Rgb<8, 8, 8>;
    
    Rgb888 primary;
    Rgb888 onPrimary;
    Rgb888 secondaryContainer;
    Rgb888 onSecondaryContainer;
    Rgb888 tertiary;
    Rgb888 onTertiary;
    Rgb888 tertiaryContainer;
    Rgb888 onTertiaryContainer;
    Rgb888 surfaceBright;
    Rgb888 inverseOnSurface;
    Rgb888 onSurface;
    Rgb888 onSurfaceVariant;
    Rgb888 mainIconBg;
    // Rgb888 surfaceContainerLow;
    Rgb888 surfaceContainerHighest;
    Rgb888 scrim;
    Rgb888 outline;

    constexpr const Rgb888& GetColor(md::sys::color color) const
    {
        switch (color)
        {
            case md::sys::color::primary:
                return primary;
            case md::sys::color::onPrimary:
                return onPrimary;
            case md::sys::color::inverseOnSurface:
                return inverseOnSurface;
            case md::sys::color::onSurface:
                return onSurface;
            case md::sys::color::onSurfaceVariant:
                return onSurfaceVariant;
            case md::sys::color::outline:
                return outline;
            case md::sys::color::secondaryContainer:
                return secondaryContainer;
            case md::sys::color::onSecondaryContainer:
                return onSecondaryContainer;
            case md::sys::color::surfaceBright:
                return surfaceBright;
            case md::sys::color::surfaceContainerHighest:
                return surfaceContainerHighest;

            default:
                return inverseOnSurface;
        }
    }
};