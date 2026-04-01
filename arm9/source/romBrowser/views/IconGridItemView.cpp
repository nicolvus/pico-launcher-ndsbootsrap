#include "common.h"
#include "gui/IVramManager.h"
#include "gui/VramContext.h"
#include "IconGridItemView.h"

void IconGridItemView::InitVram(const VramContext& vramContext)
{
    const auto objVramManager = vramContext.GetObjVramManager();
    if (objVramManager)
    {
        _iconVramOffset = objVramManager->Alloc(FILE_ICON_VRAM_SIZE);
        _iconVram = objVramManager->GetVramAddress(_iconVramOffset);
    }
}

void IconGridItemView::Update()
{
    if (_icon)
    {
        _icon->Update();
    }
}
