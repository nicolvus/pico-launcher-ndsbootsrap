#pragma once
#include <string.h>
#include "IPalette.h"

/// @brief Palette of 16 specified colors.
class alignas(4) DirectPalette : public IPalette
{
public:
    explicit DirectPalette(const u16* colors)
    {
        memcpy(_colors, colors, sizeof(_colors));
    }

    void GetColors(u16* dst) const override
    {
        memcpy(dst, _colors, sizeof(_colors));
    }

    u32 GetHashCode() const override;

private:
    u16 _colors[16];
};
