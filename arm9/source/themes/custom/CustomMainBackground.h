#pragma once
#include "../background/IThemeBackground.h"

class CustomMainBackground : public IThemeBackground
{
public:
    void Draw(GraphicsContext& graphicsContext) override;
    void LoadResources(const ITheme& theme, const VramContext& vramContext) override;
};