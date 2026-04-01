#pragma once
#include "gui/font/nitroFont2.h"
#include "FontType.h"

class IFontRepository
{
public:
    virtual ~IFontRepository() = 0;

    virtual const nft2_header_t* GetFont(FontType fontType) const = 0;
};

inline IFontRepository::~IFontRepository() { }
