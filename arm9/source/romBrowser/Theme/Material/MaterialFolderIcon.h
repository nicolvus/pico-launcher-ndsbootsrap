#pragma once
#include "MaterialFileIcon.h"
#include "largeFolderIcon.h"
#include "themes/material/MaterialColorScheme.h"

class MaterialFolderIcon : public MaterialFileIcon
{
public:
    MaterialFolderIcon(const TCHAR* name, const MaterialColorScheme* materialColorScheme,
        const IFontRepository* fontRepository)
        : MaterialFileIcon(name, materialColorScheme, fontRepository) { }

protected:
    const void* GetIconTiles() const override
    {
        return largeFolderIconTiles;
    }

    Rgb<8, 8, 8> GetIconColor() const override
    {
        return _materialColorScheme->tertiaryContainer;
    }

    Rgb<8, 8, 8> GetTextColor() const override
    {
        return _materialColorScheme->onTertiaryContainer;
    }

    int GetTextYOffset() const override { return 10; }
};
