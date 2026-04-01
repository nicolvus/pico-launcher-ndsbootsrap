#pragma once
#include "core/math/Rgb.h"

class VramContext;

class Rgb6Palette
{
public:
    void UploadGraphics(const VramContext& vramContext);
    void ApplyColor(const Rgb<6, 6, 6>& color) const;

private:
    u32 _vramOffset = 0;
};
