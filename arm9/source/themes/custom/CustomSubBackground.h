#pragma once
#include "../background/IThemeBackground.h"

class CustomSubBackground : public IThemeBackground
{
public:
    void VBlank() override;
    void LoadResources(const ITheme& theme, const VramContext& vramContext) override;
};
