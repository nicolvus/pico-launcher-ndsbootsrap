#pragma once
#include "animation/Animator.h"
#include "gui/views/DialogView.h"

class BottomSheetView : public DialogView
{
public:
    Rectangle GetBounds() const override
    {
        return Rectangle(_position.x, _position.y, 256 - _position.x, 192 - _position.y);
    }

    Rectangle GetFullyCoveredArea() const override
    {
        return Rectangle(_position.x, _position.y + 12, 256 - _position.x, 148);
    }

    constexpr DialogType GetDialogType() const override { return DialogType::BottomSheet; }
};
