#pragma once
#include "IVramManager.h"

/// @brief Base class for vram managers that work like a stack.
class StackVramManager : public IVramManager
{
public:
    /// @brief Gets the current state of the stack.
    /// @return The current state of the stack.
    u32 GetState() const { return _offset; }

    /// @brief Restores the stack to a previously stored state.
    /// @param state The state to restore.
    void SetState(u32 state) { _offset = state; }

    vu16* GetVramAddress(u32 offset) const override
    {
        return (vu16*)((vu8*)_vramAddress + offset);
    }

protected:
    u32 _offset;
    vu16* _vramAddress;

    StackVramManager()
        : _offset(0), _vramAddress(nullptr) { }

    explicit StackVramManager(vu16* vramAddress)
        : _offset(0), _vramAddress(vramAddress) { }
};
