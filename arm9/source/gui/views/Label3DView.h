#pragma once
#include "LabelView.h"
#include "gui/VBlankTextureLoader.h"

class alignas(32) Label3DView : public LabelView
{
public:
    Label3DView(u32 width, u32 height, u32 maxStringLength, const nft2_header_t* font,
        VBlankTextureLoader* vblankTextureLoader);

    void InitVram(const VramContext& vramContext) override;
    void Draw(GraphicsContext& graphicsContext) override;

private:
    void UpdateTileBuffer() override;

    u32 _texVramOffset = 0;
    VBlankTextureLoader* _vblankTextureLoader;
    VBlankTextureLoadRequest _textureLoadRequest;
};