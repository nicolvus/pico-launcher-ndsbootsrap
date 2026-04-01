#include "common.h"
#include <string.h>
#include <libtwl/mem/memVram.h>
#include "gui/Gx.h"
#include "gui/GraphicsContext.h"
#include "../ITheme.h"
#include "MaterialMainBackground.h"

#define BLOCK_VTX_PACK(x, y, z) (((x)&0x3FF) | ((((y) >> 3) & 0x3FF) << 10) | ((z) << 20))

void MaterialMainBackground::Draw(GraphicsContext& graphicsContext)
{
    Gx::MtxIdentity();
    Gx::PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGON_MODE_MODULATE, GX_DISPLAY_MODE_FRONT,
        false, false, false, GX_DEPTH_FUNC_LESS, false, 31, 0);
    Gx::TexImageParam((128 * 1024) >> 3, false, false, false, false, GX_TEXSIZE_8,
        GX_TEXSIZE_8, GX_TEXFMT_PLTT16, false, GX_TEXGEN_NONE);
    graphicsContext.GetRgb6Palette()->ApplyColor(Rgb<6, 6, 6>(_materialColorScheme->inverseOnSurface));
    Gx::Begin(GX_PRIMITIVE_QUAD);
    Gx::TexCoord(0, 0);
    REG_GX_VTX_10 = BLOCK_VTX_PACK(0, 0, 500);
    REG_GX_VTX_10 = BLOCK_VTX_PACK(0, 192, 500);
    REG_GX_VTX_10 = BLOCK_VTX_PACK(256, 192, 500);
    REG_GX_VTX_10 = BLOCK_VTX_PACK(256, 0, 500);
    Gx::End();
}

void MaterialMainBackground::LoadResources(const ITheme& theme, const VramContext& vramContext)
{
    mem_setVramAMapping(MEM_VRAM_AB_LCDC);
    *(vu32*)0x06800000 = 0;
    mem_setVramAMapping(MEM_VRAM_AB_TEX_SLOT_1);
}
