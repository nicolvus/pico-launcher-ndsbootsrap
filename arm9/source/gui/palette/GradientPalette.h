#pragma once
#include "core/math/Rgb.h"
#include "core/math/RgbMixer.h"
#include "IPalette.h"

/// @brief Gradient palette between two colors.
class GradientPalette : public IPalette
{
public:
    GradientPalette(const Rgb<8, 8, 8>& from, const Rgb<8, 8, 8>& to)
        : _fromColor(from), _toColor(to) { }

    void GetColors(u16* dst) const override
    {
        RgbMixer::MakeGradientPalette(dst, _fromColor, _toColor);
    }

    u32 GetHashCode() const override;

private:
    Rgb<8, 8, 8> _fromColor;
    Rgb<8, 8, 8> _toColor;
};
