#include "common.h"
#include <string.h>
#include <algorithm>
#include <libtwl/mem/memVram.h>
#include "gui/Gx.h"
#include "gui/GraphicsContext.h"
#include "gui/materialDesign.h"
#include "core/math/SinTable.h"
#include "romBrowser/FileType/FileCover.h"
#include "CoverFlowRecyclerView.h"

#define COVER_SPACING       6

void CoverFlowRecyclerView::Update()
{
    if (_itemCount == 0)
    {
        return;
    }

    int rangeStartIndex = GetSelectedItem() - 4;
    int rangeEndIndex = GetSelectedItem() + 1 + 4;
    rangeStartIndex = std::clamp(rangeStartIndex, 0, (int)_itemCount - 1);
    rangeEndIndex = std::clamp(rangeEndIndex, 0, (int)_itemCount);

    if (_curRangeStart != rangeStartIndex || _curRangeLength != rangeEndIndex - rangeStartIndex)
    {
        LOG_DEBUG("range: %d - %d\n", rangeStartIndex, rangeEndIndex - 1);
        if (_curRangeLength != 0)
        {
            if (_curRangeStart < rangeStartIndex)
                ReleaseRange(_curRangeStart, rangeStartIndex);

            if (rangeEndIndex < _curRangeStart + _curRangeLength)
                ReleaseRange(rangeEndIndex, _curRangeStart + _curRangeLength);
        }

        BindRange(rangeStartIndex, rangeEndIndex);

        _curRangeStart = rangeStartIndex;
        _curRangeLength = rangeEndIndex - rangeStartIndex;
    }

    for (u32 i = _viewPoolFreeCount; i < _viewPool.size(); i++)
    {
        _viewPoolEx[i].yAngleAnimator.Update();
        _viewPoolEx[i].xPositionAnimator.Update();
        _viewPoolEx[i].zPositionAnimator.Update();
        _viewPool[i].view->SetPosition(_viewPoolEx[i].xPositionAnimator.GetValue().Int(), 32 + 80);
        _viewPool[i].view->Update();
    }
}

void CoverFlowRecyclerView::Draw(GraphicsContext& graphicsContext)
{
    Gx::PolygonAttr(GX_LIGHTMASK_0, GX_POLYGON_MODE_MODULATE, GX_DISPLAY_MODE_FRONT,
        false, false, false, GX_DEPTH_FUNC_LESS, false, 31, 0);

    Gx::MtxMode(GX_MTX_MODE_PROJECTION);
    Gx::MtxPush();
    fix32<12> near = 2;
    fix32<12> far = 100;
    fix32<12> left = -1;
    fix32<12> right = 1;
    fix32<12> top = -1;
    fix32<12> bottom = 1;
    mtx44_t perspectiveMtx =
    {
        ((2 * near) / (right - left)).GetRawValue(), 0, 0, 0,
        0, ((2 * near) / (top - bottom)).GetRawValue(), 0, 0,
        ((right + left) / (right - left)).GetRawValue(), ((top + bottom) / (top - bottom)).GetRawValue(), ((far + near) / (near - far)).GetRawValue(), -FX32_ONE * 64,
        0, 0, ((2 * far * near) / (near - far)).GetRawValue(), 0
    };
    Gx::MtxLoad44(&perspectiveMtx);
    Gx::MtxMode(GX_MTX_MODE_POSITION_VECTOR);
    Gx::MtxIdentity();
    Gx::LightVector(0, 0, 0, -0x200);
    Gx::LightColor(0, 0x7FFF);
    Gx::DiffuseAmbient(Rgb<5, 5, 5>(16, 16, 16), false, Rgb<5, 5, 5>(20, 20, 20));
    Gx::SpecularEmission(0, false, 0);
    // Gx::MtxScale(2.0 / (128.0 / 64.0), 2.0 / (96.0 / 64.0), 1);
    Gx::MtxTranslate(-128, /*-96*/-128, fix32<12>(-4));

    for (u32 i = _viewPoolFreeCount; i < _viewPool.size(); i++)
    {
        graphicsContext.SetPolygonId(i);
        Gx::MtxPush();
        {
            fix32<12> x = _viewPoolEx[i].xPositionAnimator.GetValue();
            u32 angle = _viewPoolEx[i].yAngleAnimator.GetValue();
            fix32<12> z = _viewPoolEx[i].zPositionAnimator.GetValue();
            auto sinCos = gSinTable.SinCos(angle);
            mtx43_t rotMtx =
            {
                fix32<18>(sinCos.cos).GetRawValue(), 0, -fix32<12>(sinCos.sin).GetRawValue(),
                0, FX32_ONE * 64, 0,
                fix32<18>(sinCos.sin).GetRawValue(), 0, fix32<12>(sinCos.cos).GetRawValue(),
                x.GetRawValue() + 2048, 148 * FX32_ONE + 2048, 0
            };
            Gx::MtxMult43(&rotMtx);
            Gx::MtxTranslate(0, 0, z / 64);
            Gx::MtxScale(1.0 / 8, /*1.0 / 8*/1.0/*/6*/, 1);
            _viewPool[i].view->Draw(graphicsContext);
        }
        Gx::MtxPop(1);
    }

    Gx::MtxMode(GX_MTX_MODE_PROJECTION);
    Gx::MtxPop(1);
    Gx::MtxMode(GX_MTX_MODE_POSITION_VECTOR);
}

void CoverFlowRecyclerView::UpdateItemPosition(int viewPoolIndex, bool initial)
{
    ViewPoolEntry* item = &_viewPool[viewPoolIndex];
    ViewPoolEntryEx* itemEx = &_viewPoolEx[viewPoolIndex];
    int selectedIndex = GetSelectedItem();
    if (selectedIndex == -1)
    {
        selectedIndex = 0;
    }
    int itemIndex = item->itemIdx;
    fix32<12> x;
    fix32<12> z = 0;
    u32 angle;
    if (itemIndex < selectedIndex)
    {
        x = 256 / 2 - (COVER_HEIGHT / 4) + COVER_SPACING * (itemIndex - selectedIndex);
        angle = std::clamp((selectedIndex - itemIndex) * 10 + 45, -90, 90) * (1 << 16) / 360;
        z = -(selectedIndex - itemIndex) * 30 - 20;
    }
    else if (itemIndex > selectedIndex)
    {
        x = 256 / 2 + (COVER_HEIGHT / 4) + COVER_SPACING * (itemIndex - selectedIndex);
        angle = std::clamp((selectedIndex - itemIndex) * 10 - 45, -90, 90) * (1 << 16) / 360;
        z = (selectedIndex - itemIndex) * 30 - 20;
    }
    else
    {
        x = 256 / 2;
        angle = 0;
    }

    if (initial)
    {
        itemEx->yAngleAnimator = Animator<int>(angle);
        itemEx->xPositionAnimator = Animator(x);
        itemEx->zPositionAnimator = Animator(z);
    }
    else
    {
        itemEx->yAngleAnimator.Goto(angle, md::sys::motion::duration::medium4, &md::sys::motion::easing::standard);
        itemEx->xPositionAnimator.Goto(x, md::sys::motion::duration::medium4, &md::sys::motion::easing::standard);
        itemEx->zPositionAnimator.Goto(z, md::sys::motion::duration::medium4, &md::sys::motion::easing::standard);
    }
}
