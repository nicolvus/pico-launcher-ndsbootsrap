#pragma once
#include "StackVramManager.h"

/// @brief Stack vram manager that decreases the stack pointer before an allocation.
class DescendingStackVramManager : public StackVramManager
{
public:
    /// @brief Creates a new DescendingStackVramManager.
    /// @param vramSize The size of the vram block to manage.
    ///                 This will be used as the initial offset.
    explicit DescendingStackVramManager(u32 vramSize)
        : _size(vramSize)
    {
        _offset = vramSize;
    }

    /// @brief Creates a new DescendingStackVramManager.
    /// @param vramAddress Pointer to the start of the vram block to manage.
    /// @param vramSize The size of the vram block to manage.
    ///                 This will be used as the initial offset.
    DescendingStackVramManager(vu16* vramAddress, u32 vramSize)
        : StackVramManager(vramAddress), _size(vramSize)
    {
        _offset = vramSize;
    }

    u32 Alloc(u32 length) override
    {
        _offset -= length;
        return _offset;
    }

private:
    u32 _size;
};