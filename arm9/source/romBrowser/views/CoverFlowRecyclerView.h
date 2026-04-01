#pragma once
#include <array>
#include "CoverFlowRecyclerViewBase.h"
#include "animation/Animator.h"

class CoverFlowRecyclerView : public CoverFlowRecyclerViewBase
{
public:
    void Update() override;
    void Draw(GraphicsContext& graphicsContext) override;

private:
    struct ViewPoolEntryEx
    {
        Animator<int> yAngleAnimator;
        Animator<fix32<12>> xPositionAnimator;
        Animator<fix32<12>> zPositionAnimator;
    };

    std::array<ViewPoolEntryEx, 10> _viewPoolEx;

    void UpdateItemPosition(int viewPoolIndex, bool initial) override;

    void SwapViewPoolEntry(int indexA, int indexB) override
    {
        CoverFlowRecyclerViewBase::SwapViewPoolEntry(indexA, indexB);
        std::swap(_viewPoolEx[indexA], _viewPoolEx[indexB]);
    }
};
