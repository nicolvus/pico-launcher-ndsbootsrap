#pragma once
#include "palette/IPalette.h"

#define PALETTE_MANAGER_ROW_IDX_INVALID     0xFF

/// @brief Base class for palette management.
class PaletteManager
{
public:
    virtual ~PaletteManager() { }

    /// @brief Allocates a palette row for the given palette, which is used
    ///        in the display line range from yStart to yEnd (exclusive).
    /// @param palette The palette to allocate a row for.
    /// @param yStart The first display line the palette will be used on.
    /// @param yEnd The display line from which the palette will not be used anymore.
    /// @return The allocated palette row number.
    virtual u32 AllocRow(const IPalette& palette, int yStart, int yEnd) = 0;

    /// @brief Allocates a palette row for the given palette, assuming it
    ///        will be used on all display lines.
    /// @param palette The palette to allocate a row for.
    /// @return The allocated palette row number.
    /// @note It is recommended to specify the start and end display line
    ///       for more efficient palette management.
    u32 AllocRow(const IPalette& palette)
    {
        return AllocRow(palette, 0, 192);
    }
};