#pragma once
#include "core/math/Rgb.h"
#include "MaterialColorScheme.h"

class MaterialColorSchemeFactory
{
public:
    static void FromPrimaryColor(const Rgb<8, 8, 8>& primaryColor,
        bool darkTheme, MaterialColorScheme& materialColorScheme);
};