#pragma once
#include "IconButtonView.h"
#include "core/math/Rgb.h"
#include "gui/Rgb6Palette.h"

class IconButton3DView : public IconButtonView
{
public:
    IconButton3DView() : IconButtonView() { }

    IconButton3DView(Type type, State state,
        md::sys::color backgroundColor, const MaterialColorScheme* materialColorScheme)
        : IconButtonView(type, state, backgroundColor, materialColorScheme) { }

    void Draw(GraphicsContext& graphicsContext) override;

    static void UploadGraphics(const VramContext& vramContext);

private:
    static u32 sSelectorTextureVramOffset;

    void DrawSelector(GraphicsContext& graphicsContext, const Rgb<8, 8, 8>& color);
};
