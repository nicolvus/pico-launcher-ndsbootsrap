#pragma once
#include "Rgb.h"
#include "fixed.h"
#include "ColorConverter.h"

class RgbMixer
{
public:
    template <u32 RBits, u32 GBits, u32 BBits>
    static Rgb<RBits, GBits, BBits> Lerp(const Rgb<RBits, GBits, BBits>& from, const Rgb<RBits, GBits, BBits>& to, int t, int tMax)
    {
        return Rgb<RBits, GBits, BBits>(
            (u32)((from.r * (tMax - t) + to.r * t) + (tMax >> 1)) / tMax,
            (u32)((from.g * (tMax - t) + to.g * t) + (tMax >> 1)) / tMax,
            (u32)((from.b * (tMax - t) + to.b * t) + (tMax >> 1)) / tMax);
    }

    static void MakeGradientPalette(u16* palette, const Rgb<8, 8, 8>& from, const Rgb<8, 8, 8>& to)
    {
        for (int i = 0; i < 16; i++)
        {
            auto newColor = Lerp(from, to, i, 15);
            palette[i] = ColorConverter::ToGBGR565(newColor);
        }
    }
};