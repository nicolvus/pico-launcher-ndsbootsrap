#include "common.h"
#include <nds/arm9/cache.h>
#include "OamManager.h"

void OamManager::Apply(vu16* dst)
{
    DC_FlushRange(&_oamTable, sizeof(gfx_oam_table_t));
    dma_ntrCopy32(3, &_oamTable, dst, sizeof(gfx_oam_table_t));
}

void OamManager::Clear()
{
    memset(&_oamTable, 0x2, sizeof(gfx_oam_table_t));
    _oamPtr = &_oamTable.objs[128];
    _mtxIdx = 0;
}