#pragma once
#include "PaletteManager.h"

/// @brief Simple palette manager that manages 16 static palette rows.
class alignas(32) SimplePaletteManager : public PaletteManager
{
public:
    using PaletteManager::AllocRow;

    u32 GetState() const { return _curRow; }
    void SetState(u32 state) { _curRow = state; }

    void Reset(u32 idxOffset = 0)
    {
        _curRow = idxOffset;
        _idxOffset = idxOffset;
    }

    u32 AllocRow(const IPalette& palette, int yStart, int yEnd) override
    {
        u32 rowIdx = _curRow++;
        palette.GetColors(_palette[rowIdx]);
        return rowIdx;
    }

    u16* GetRow(u32 rowIdx)
    {
        return _palette[rowIdx];
    }

    void SetColor0(u16 color)
    {
        _palette[0][0] = color;
    }

    void Apply(vu16* dst);

private:
    u16 _palette[16][16] alignas(32);
    u8 _curRow = 0;
    u8 _idxOffset = 0;
};