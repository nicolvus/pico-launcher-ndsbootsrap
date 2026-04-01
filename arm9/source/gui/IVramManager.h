#pragma once

/// @brief Interface for managing and allocating vram.
class IVramManager
{
public:
    virtual ~IVramManager() = 0;

    /// @brief Allocates a block of vram of the given length.
    /// @param length The size to allocate.
    /// @return The vram offset of the allocated block.
    virtual u32 Alloc(u32 length) = 0;

    /// @brief Converts the vram offset to a vram pointer.
    /// @param offset The offset to convert.
    /// @return The pointer corresponding to the vram offset.
    virtual vu16* GetVramAddress(u32 offset) const = 0;
};

inline IVramManager::~IVramManager() { }
