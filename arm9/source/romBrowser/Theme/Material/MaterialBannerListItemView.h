#pragma once
#include "gui/views/LabelView.h"
#include "../../views/BannerListItemView.h"

class MaterialColorScheme;
class IFontRepository;

class MaterialBannerListItemView : public BannerListItemView
{
public:
    MaterialBannerListItemView(const MaterialColorScheme* materialColorScheme,
        const IFontRepository* fontRepository);

    void Draw(GraphicsContext& graphicsContext) override;

    Rectangle GetBounds() const override
    {
        return Rectangle(_position, 203, 44);
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
