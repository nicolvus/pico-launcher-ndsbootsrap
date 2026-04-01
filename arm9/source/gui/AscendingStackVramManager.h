#pragma once
#include "StackVramManager.h"

/// @brief Stack vram manager that increases the stack pointer after an allocation.
class AscendingStackVramManager : public StackVramManager
{
public:
    /// @brief Creates a new AscendingStackVramManager.
    AscendingStackVramManager() { }

    /// @brief Creates a new AscendingStackVramManager.
    /// @param vramAddress Pointer to the start of the vram block to manage.
    explicit AscendingStackVramManager(vu16* vramAddress)
        : StackVramManager(vramAddress) { }

    u32 Alloc(u32 length) override
    {
        u32 result = _offset;
        _offset += length;
        return result;
    }
};