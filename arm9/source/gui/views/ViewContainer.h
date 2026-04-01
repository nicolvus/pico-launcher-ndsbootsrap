#pragma once
#include "View.h"
#include "core/LinkedList.h"

/// @brief Base class for views that contain other views.
class ViewContainer : public View
{
public:
    void InitVram(const VramContext& vramContext) override
    {
        for (auto& view : _children)
        {
            view.InitVram(vramContext);
        }
    }

    void Update() override
    {
        for (auto& view : _children)
        {
            view.Update();
        }
    }

    void Draw(GraphicsContext& graphicsContext) override
    {
        for (auto& view : _children)
        {
            view.Draw(graphicsContext);
        }
    }

    void VBlank() override
    {
        for (auto& view : _children)
        {
            view.VBlank();
        }
    }

protected:
    /// @brief Adds a child to the head of the list.
    /// @param view The child to add.
    void AddChildHead(View* view)
    {
        _children.InsertHead(view);
        view->SetParent(this);
    }

    /// @brief Adds a child to the tail of the list.
    /// @param view The child to add.
    void AddChildTail(View* view)
    {
        _children.InsertTail(view);
        view->SetParent(this);
    }

private:
    LinkedList<View, &View::listLink> _children;
};
