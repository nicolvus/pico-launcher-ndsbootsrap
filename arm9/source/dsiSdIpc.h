#pragma once

void dsisd_init();

#ifdef __cplusplus
extern "C" {
#endif

void dsisd_readSectors(void* buffer, u32 sector, u32 count);
void dsisd_writeSectors(const void* buffer, u32 sector, u32 count);

#ifdef __cplusplus
}
#endif
