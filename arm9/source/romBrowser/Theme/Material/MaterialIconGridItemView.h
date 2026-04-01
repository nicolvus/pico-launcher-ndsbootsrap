#pragma once
#include "../../views/IconGridItemView.h"

class MaterialColorScheme;

class MaterialIconGridItemView : public IconGridItemView
{
public:
    explicit MaterialIconGridItemView(const MaterialColorScheme* materialColorScheme)
        : _materialColorScheme(materialColorScheme) { }

    void Draw(GraphicsContext& graphicsContext) override;

    Rectangle GetBounds() const override
    {
        return Rectangle(_position, 44, 44);
    }

    void SetGraphics(const VramToken& vramToken) override
    {
        _bgVramOffset = vramToken.GetVramOffset();
    }

    static VramToken UploadGraphics(const VramContext& vramContext);

private:
    const MaterialColorScheme* _materialColorScheme;
    u32 _bgVramOffset;
};