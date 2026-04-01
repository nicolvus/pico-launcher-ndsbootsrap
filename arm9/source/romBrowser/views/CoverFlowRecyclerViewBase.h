#pragma once
#include <array>
#include "gui/views/RecyclerViewBase.h"

class CoverFlowRecyclerViewBase : public RecyclerViewBase
{
public:
    void InitVram(const VramContext& vramContext) override;
    void SetAdapter(const RecyclerAdapter* adapter, int initialSelectedIndex = 0) override;
    View* MoveFocus(View* currentFocus, FocusMoveDirection direction, View* source) override;

    Rectangle GetBounds() const override
    {
        return Rectangle(_position, 256, 160);
    }

    void Focus(FocusManager& focusManager) override
    {
        focusManager.Focus(_selectedItem ? _selectedItem->view : this);
    }

    int GetSelectedItem() const override
    {
        return _selectedItem ? _selectedItem->itemIdx : -1;
    }

protected:
    struct ViewPoolEntry
    {
        View* view;
        int itemIdx = -1;
    };

    std::array<ViewPoolEntry, 10> _viewPool;
    u32 _viewPoolFreeCount;
    u32 _itemCount;
    ViewPoolEntry* _selectedItem = nullptr;
    int _curRangeStart = 0;
    int _curRangeLength = 0;

    ViewPoolEntry* GetViewPoolEntryByItemIndex(int itemIdx);
    ViewPoolEntry* BindViewPoolEntry(int itemIdx);
    void BindRange(int start, int end);
    void ReleaseViewPoolEntry(int itemIdx);
    void ReleaseRange(int start, int end);
    void SetSelectedItem(int itemIdx, bool initial);
    virtual void UpdateItemPosition(int viewPoolIndex, bool initial) = 0;

    virtual void SwapViewPoolEntry(int indexA, int indexB)
    {
        std::swap(_viewPool[indexA], _viewPool[indexB]);
    }
};
