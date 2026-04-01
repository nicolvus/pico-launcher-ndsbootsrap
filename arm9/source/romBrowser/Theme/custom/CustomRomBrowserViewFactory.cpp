#include "common.h"
#include <string.h>
#include <libtwl/mem/memVram.h>
#include "themes/ITheme.h"
#include "gui/VramContext.h"
#include "CustomRomBrowserViewFactory.h"

void CustomRomBrowserViewFactory::LoadResources(const ITheme& theme, const VramContext& mainVramContext)
{
    auto tmpBuf = std::unique_ptr<u8[]>(new(cache_align) u8[13 * 1024]);
    const auto file = std::make_unique<File>();
    if (theme.OpenThemeFile(*file, "gridcell.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 4096, bytesRead);
        _gridCellTexVramOffset = mainVramContext.GetTexVramManager()->Alloc(4096);
        mem_setVramDMapping(MEM_VRAM_D_LCDC);
        memcpy((void*)mainVramContext.GetTexVramManager()->GetVramAddress(_gridCellTexVramOffset), tmpBuf.get(), 4096);
        mem_setVramDMapping(MEM_VRAM_D_TEX_SLOT_0);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "gridcellPltt.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 64, bytesRead);
        _gridCellPlttVramOffset = mainVramContext.GetTexPlttVramManager()->Alloc(64);
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        memcpy((void*)mainVramContext.GetTexPlttVramManager()->GetVramAddress(_gridCellPlttVramOffset), tmpBuf.get(), 64);
        mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "gridcellSelected.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 4096, bytesRead);
        mem_setVramAMapping(MEM_VRAM_AB_LCDC);
        memcpy((void*)0x681E200, tmpBuf.get(), 4096);
        mem_setVramAMapping(MEM_VRAM_AB_TEX_SLOT_1);
        file->Close();
        _gridCellSelectedTexVramOffset = 0x3E200;
    }
    if (theme.OpenThemeFile(*file, "gridcellSelectedPltt.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 64, bytesRead);
        _gridCellSelectedPlttVramOffset = mainVramContext.GetTexPlttVramManager()->Alloc(64);
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        memcpy((void*)mainVramContext.GetTexPlttVramManager()->GetVramAddress(_gridCellSelectedPlttVramOffset), tmpBuf.get(), 64);
        mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "scrim.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 336, bytesRead);
        _scrimTexVramOffset = mainVramContext.GetTexVramManager()->Alloc(336);
        mem_setVramDMapping(MEM_VRAM_D_LCDC);
        memcpy((void*)mainVramContext.GetTexVramManager()->GetVramAddress(_scrimTexVramOffset), tmpBuf.get(), 336);
        mem_setVramDMapping(MEM_VRAM_D_TEX_SLOT_0);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "scrimPltt.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 16, bytesRead);
        _scrimPlttVramOffset = mainVramContext.GetTexPlttVramManager()->Alloc(16);
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        memcpy((void*)mainVramContext.GetTexPlttVramManager()->GetVramAddress(_scrimPlttVramOffset), tmpBuf.get(), 16);
        mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "bannerListCell.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 0x3100, bytesRead);
        mem_setVramAMapping(MEM_VRAM_AB_LCDC);
        memcpy((void*)0x6818000, tmpBuf.get(), 12544);
        mem_setVramAMapping(MEM_VRAM_AB_TEX_SLOT_1);
        file->Close();
        _bannerListCellTexVramOffset = 0x38000;
    }
    if (theme.OpenThemeFile(*file, "bannerListCellPltt.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 64, bytesRead);
        _bannerListCellPlttVramOffset = mainVramContext.GetTexPlttVramManager()->Alloc(64);
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        memcpy((void*)mainVramContext.GetTexPlttVramManager()->GetVramAddress(_bannerListCellPlttVramOffset), tmpBuf.get(), 64);
        mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
        file->Close();
    }
    if (theme.OpenThemeFile(*file, "bannerListCellSelected.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 0x3100, bytesRead);
        mem_setVramAMapping(MEM_VRAM_AB_LCDC);
        memcpy((void*)0x681B100, tmpBuf.get(), 0x3100);
        mem_setVramAMapping(MEM_VRAM_AB_TEX_SLOT_1);
        file->Close();
        _bannerListCellSelectedTexVramOffset = 0x3B100;
    }
    if (theme.OpenThemeFile(*file, "bannerListCellSelectedPltt.bin"))
    {
        u32 bytesRead = 0;
        file->Read(tmpBuf.get(), 64, bytesRead);
        _bannerListCellSelectedPlttVramOffset = mainVramContext.GetTexPlttVramManager()->Alloc(64);
        mem_setVramEMapping(MEM_VRAM_E_LCDC);
        memcpy((void*)mainVramContext.GetTexPlttVramManager()->GetVramAddress(_bannerListCellSelectedPlttVramOffset), tmpBuf.get(), 64);
        mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
        file->Close();
    }
}
