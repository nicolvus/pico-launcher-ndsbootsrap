#include "common.h"
#include <libtwl/mem/memVram.h>
#include "romBrowser/Theme/Material/CarouselRecyclerView.h"
#include "MaterialTheme.h"

void MaterialTheme::LoadRomBrowserResources(const VramContext& mainVramContext, const VramContext& subVramContext)
{
    mem_setVramDMapping(MEM_VRAM_D_LCDC);
    mem_setVramEMapping(MEM_VRAM_E_LCDC);
    CarouselRecyclerView::UploadGraphics(mainVramContext);
    mem_setVramDMapping(MEM_VRAM_D_TEX_SLOT_0);
    mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
}