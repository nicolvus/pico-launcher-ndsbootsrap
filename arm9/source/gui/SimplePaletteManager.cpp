#include "common.h"
#include <nds/arm9/cache.h>
#include <libtwl/dma/dmaNitro.h>
#include "SimplePaletteManager.h"

void SimplePaletteManager::Apply(vu16* dst)
{
    if (_idxOffset > 0 && _curRow == _idxOffset)
        return;

    u32 rows = _idxOffset == 0 ? 16 : _curRow - _idxOffset;
    dst += _idxOffset * 16;
    DC_FlushRange(_palette[_idxOffset], rows * 16 * 2);
    dma_ntrCopy32(3, _palette[_idxOffset], dst, rows * 16 * 2);
}