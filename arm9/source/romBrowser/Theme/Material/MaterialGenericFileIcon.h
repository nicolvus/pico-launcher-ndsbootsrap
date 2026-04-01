#pragma once
#include "MaterialFileIcon.h"
#include "largeFileIcon.h"
#include "themes/material/MaterialColorScheme.h"

class MaterialGenericFileIcon : public MaterialFileIcon
{
public:
    MaterialGenericFileIcon(const TCHAR* name, const MaterialColorScheme* materialColorScheme,
        const IFontRepository* fontRepository)
        : MaterialFileIcon(name, materialColorScheme, fontRepository) { }

protected:
    const void* GetIconTiles() const override
    {
        return largeFileIconTiles;
    }

    Rgb<8, 8, 8> GetIconColor() const override
    {
        return _materialColorScheme->tertiary;
    }

    Rgb<8, 8, 8> GetTextColor() const override
    {
        return _materialColorScheme->onTertiary;
    }

    int GetTextYOffset() const override { return 8; }
};
