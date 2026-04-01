#pragma once
#include "../background/IThemeBackground.h"

class MaterialColorScheme;

class MaterialMainBackground : public IThemeBackground
{
public:
    explicit MaterialMainBackground(const MaterialColorScheme* materialColorScheme)
        : _materialColorScheme(materialColorScheme) { }

    void Draw(GraphicsContext& graphicsContext) override;
    void LoadResources(const ITheme& theme, const VramContext& vramContext) override;

private:
    const MaterialColorScheme* _materialColorScheme;
};