// clang-format off

/*-----------------------------------------------------------------
 Copyright (C) 2005 - 2010
	Michael "Chishm" Chisholm
	Dave "WinterMute" Murphy

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

------------------------------------------------------------------*/
/* Pico Launcher: use external load.bin from SD (/_pico/load.bin), no embedded load.bin. */
#define _NO_BOOTSTUB_

#include <string.h>
#include <nds.h>
#include <nds/arm9/dldi.h>
#include <nds/memory.h>

#include "nds_loader_arm9.h"
/* Must match nds-bootstrap / TWiLight Chishm loader stub (extended parameter block). */
#define LCDC_BANK_C (u16*)0x06848000
#define STORED_FILE_CLUSTER (*(((u32*)LCDC_BANK_C) + 1))
#define INIT_DISC (*(((u32*)LCDC_BANK_C) + 2))
#define WANT_TO_PATCH_DLDI (*(((u32*)LCDC_BANK_C) + 3))


/*
	b	startUp
	
storedFileCluster:
	.word	0x0FFFFFFF		@ default BOOT.NDS
initDisc:
	.word	0x00000001		@ init the disc by default
wantToPatchDLDI:
	.word	0x00000001		@ by default patch the DLDI section of the loaded NDS
@ Used for passing arguments to the loaded app
argStart:
	.word	_end - _start
argSize:
	.word	0x00000000
dldiOffset:
	.word	_dldi_start - _start
dsiSD:
	.word	0
dsiMode:
	.word	0
*/

#define STORED_FILE_CLUSTER_OFFSET 4 
#define INIT_DISC_OFFSET 8
#define WANT_TO_PATCH_DLDI_OFFSET 12
#define ARG_START_OFFSET 16
#define ARG_SIZE_OFFSET 20
#define HAVE_DSISD_OFFSET 28
#define DSIMODE_OFFSET 32
#define CLEAR_MASTER_BRIGHT_OFFSET 36
#define DSMODE_SWITCH_OFFSET 40
#define LOADFROMRAM_OFFSET 44
#define LANGUAGE_OFFSET 48
#define TSC_TGDS_OFFSET 52


typedef signed int addr_t;
typedef unsigned char data_t;

#define FIX_ALL	0x01
#define FIX_GLUE	0x02
#define FIX_GOT	0x04
#define FIX_BSS	0x08

enum DldiOffsets {
	DO_magicString = 0x00,			// "\xED\xA5\x8D\xBF Chishm"
	DO_magicToken = 0x00,			// 0xBF8DA5ED
	DO_magicShortString = 0x04,		// " Chishm"
	DO_version = 0x0C,
	DO_driverSize = 0x0D,
	DO_fixSections = 0x0E,
	DO_allocatedSpace = 0x0F,

	DO_friendlyName = 0x10,

	DO_text_start = 0x40,			// Data start
	DO_data_end = 0x44,				// Data end
	DO_glue_start = 0x48,			// Interworking glue start	-- Needs address fixing
	DO_glue_end = 0x4C,				// Interworking glue end
	DO_got_start = 0x50,			// GOT start					-- Needs address fixing
	DO_got_end = 0x54,				// GOT end
	DO_bss_start = 0x58,			// bss start					-- Needs setting to zero
	DO_bss_end = 0x5C,				// bss end

	// IO_INTERFACE data
	DO_ioType = 0x60,
	DO_features = 0x64,
	DO_startup = 0x68,	
	DO_isInserted = 0x6C,	
	DO_readSectors = 0x70,	
	DO_writeSectors = 0x74,
	DO_clearStatus = 0x78,
	DO_shutdown = 0x7C,
	DO_code = 0x80
};

static addr_t readAddr (data_t *mem, addr_t offset) {
	return ((addr_t*)mem)[offset/sizeof(addr_t)];
}

static void writeAddr (data_t *mem, addr_t offset, addr_t value) {
	((addr_t*)mem)[offset/sizeof(addr_t)] = value;
}

static void vramcpy (void* dst, const void* src, int len)
{
	u16* dst16 = (u16*)dst;
	u16* src16 = (u16*)src;
	
	//dmaCopy(src, dst, len);

	for ( ; len > 0; len -= 2) {
		*dst16++ = *src16++;
	}
}	

static addr_t quickFind (const data_t* data, const data_t* search, size_t dataLen, size_t searchLen) {
	const int* dataChunk = (const int*) data;
	int searchChunk = ((const int*)search)[0];
	addr_t i;
	addr_t dataChunkEnd = (addr_t)(dataLen / sizeof(int));

	for ( i = 0; i < dataChunkEnd; i++) {
		if (dataChunk[i] == searchChunk) {
			if ((i*sizeof(int) + searchLen) > dataLen) {
				return -1;
			}
			if (memcmp (&data[i*sizeof(int)], search, searchLen) == 0) {
				return i*sizeof(int);
			}
		}
	}

	return -1;
}

// Normal DLDI uses "\xED\xA5\x8D\xBF Chishm"
// Bootloader string is different to avoid being patched
static const data_t dldiMagicLoaderString[] = "\xEE\xA5\x8D\xBF Chishm";	// Different to a normal DLDI file

#define DEVICE_TYPE_DLDI 0x49444C44

static bool dldiPatchLoader (data_t *binData, u32 binSize, bool clearBSS)
{
	addr_t memOffset;			// Offset of DLDI after the file is loaded into memory
	addr_t patchOffset;			// Position of patch destination in the file
	addr_t relocationOffset;	// Value added to all offsets within the patch to fix it properly
	addr_t ddmemOffset;			// Original offset used in the DLDI file
	addr_t ddmemStart;			// Start of range that offsets can be in the DLDI file
	addr_t ddmemEnd;			// End of range that offsets can be in the DLDI file
	addr_t ddmemSize;			// Size of range that offsets can be in the DLDI file

	addr_t addrIter;

	data_t *pDH;
	data_t *pAH;

	size_t dldiFileSize = 0;
	
	// Find the DLDI reserved space in the file
	patchOffset = quickFind (binData, dldiMagicLoaderString, binSize, sizeof(dldiMagicLoaderString));

	if (patchOffset < 0) {
		// does not have a DLDI section
		return false;
	}

	pDH = (data_t*)(io_dldi_data);
	
	pAH = &(binData[patchOffset]);

	if (*((u32*)(pDH + DO_ioType)) == DEVICE_TYPE_DLDI) {
		// No DLDI patch
		return false;
	}

	if (pDH[DO_driverSize] > pAH[DO_allocatedSpace]) {
		// Not enough space for patch
		return false;
	}
	
	dldiFileSize = 1 << pDH[DO_driverSize];

	memOffset = readAddr (pAH, DO_text_start);
	if (memOffset == 0) {
			memOffset = readAddr (pAH, DO_startup) - DO_code;
	}
	ddmemOffset = readAddr (pDH, DO_text_start);
	relocationOffset = memOffset - ddmemOffset;

	ddmemStart = readAddr (pDH, DO_text_start);
	ddmemSize = (1 << pDH[DO_driverSize]);
	ddmemEnd = ddmemStart + ddmemSize;

	// Remember how much space is actually reserved
	pDH[DO_allocatedSpace] = pAH[DO_allocatedSpace];
	// Copy the DLDI patch into the application
	vramcpy (pAH, pDH, dldiFileSize);

	// Fix the section pointers in the header
	writeAddr (pAH, DO_text_start, readAddr (pAH, DO_text_start) + relocationOffset);
	writeAddr (pAH, DO_data_end, readAddr (pAH, DO_data_end) + relocationOffset);
	writeAddr (pAH, DO_glue_start, readAddr (pAH, DO_glue_start) + relocationOffset);
	writeAddr (pAH, DO_glue_end, readAddr (pAH, DO_glue_end) + relocationOffset);
	writeAddr (pAH, DO_got_start, readAddr (pAH, DO_got_start) + relocationOffset);
	writeAddr (pAH, DO_got_end, readAddr (pAH, DO_got_end) + relocationOffset);
	writeAddr (pAH, DO_bss_start, readAddr (pAH, DO_bss_start) + relocationOffset);
	writeAddr (pAH, DO_bss_end, readAddr (pAH, DO_bss_end) + relocationOffset);
	// Fix the function pointers in the header
	writeAddr (pAH, DO_startup, readAddr (pAH, DO_startup) + relocationOffset);
	writeAddr (pAH, DO_isInserted, readAddr (pAH, DO_isInserted) + relocationOffset);
	writeAddr (pAH, DO_readSectors, readAddr (pAH, DO_readSectors) + relocationOffset);
	writeAddr (pAH, DO_writeSectors, readAddr (pAH, DO_writeSectors) + relocationOffset);
	writeAddr (pAH, DO_clearStatus, readAddr (pAH, DO_clearStatus) + relocationOffset);
	writeAddr (pAH, DO_shutdown, readAddr (pAH, DO_shutdown) + relocationOffset);

	if (pDH[DO_fixSections] & FIX_ALL) { 
		// Search through and fix pointers within the data section of the file
		for (addrIter = (readAddr(pDH, DO_text_start) - ddmemStart); addrIter < (readAddr(pDH, DO_data_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	if (pDH[DO_fixSections] & FIX_GLUE) { 
		// Search through and fix pointers within the glue section of the file
		for (addrIter = (readAddr(pDH, DO_glue_start) - ddmemStart); addrIter < (readAddr(pDH, DO_glue_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	if (pDH[DO_fixSections] & FIX_GOT) { 
		// Search through and fix pointers within the Global Offset Table section of the file
		for (addrIter = (readAddr(pDH, DO_got_start) - ddmemStart); addrIter < (readAddr(pDH, DO_got_end) - ddmemStart); addrIter++) {
			if ((ddmemStart <= readAddr(pAH, addrIter)) && (readAddr(pAH, addrIter) < ddmemEnd)) {
				writeAddr (pAH, addrIter, readAddr(pAH, addrIter) + relocationOffset);
			}
		}
	}

	if (clearBSS && (pDH[DO_fixSections] & FIX_BSS)) { 
		// Initialise the BSS to 0, only if the disc is being re-inited
		memset (&pAH[readAddr(pDH, DO_bss_start) - ddmemStart] , 0, readAddr(pDH, DO_bss_end) - readAddr(pDH, DO_bss_start));
	}

	return true;
}

static eRunNdsRetCode runNdsFull(const void* loader, u32 loaderSize, u32 cluster, bool initDisc, bool dldiPatchNds,
                                 bool loadFromRam, const char* filename, int argc, const char** argv, bool clearMasterBright,
                                 bool dsModeSwitch, bool lockScfg, bool boostCpu, bool boostVram, bool tscTgds, int language)
{
	char* argStart;
	u16* argData;
	u16 argTempVal = 0;
	int argSize;
	const char* argChar;

	irqDisable(IRQ_ALL);

	VRAM_C_CR = VRAM_ENABLE | VRAM_C_LCD;
	vramcpy(LCDC_BANK_C, loader, loaderSize);
	if (loaderSize < 0x18000)
		memset((u8*)LCDC_BANK_C + loaderSize, 0, 0x18000 - loaderSize);

	writeAddr((data_t*)LCDC_BANK_C, STORED_FILE_CLUSTER_OFFSET, cluster);
	writeAddr((data_t*)LCDC_BANK_C, INIT_DISC_OFFSET, initDisc);

	writeAddr((data_t*)LCDC_BANK_C, DSIMODE_OFFSET, isDSiMode());
	if (filename && filename[0] == 's' && filename[1] == 'd')
		writeAddr((data_t*)LCDC_BANK_C, HAVE_DSISD_OFFSET, 1);

	writeAddr((data_t*)LCDC_BANK_C, CLEAR_MASTER_BRIGHT_OFFSET, clearMasterBright);
	if (isDSiMode())
		writeAddr((data_t*)LCDC_BANK_C, DSMODE_SWITCH_OFFSET, dsModeSwitch);
	writeAddr((data_t*)LCDC_BANK_C, LOADFROMRAM_OFFSET, loadFromRam);
	writeAddr((data_t*)LCDC_BANK_C, LANGUAGE_OFFSET, language);
	writeAddr((data_t*)LCDC_BANK_C, TSC_TGDS_OFFSET, tscTgds);

	writeAddr((data_t*)LCDC_BANK_C, WANT_TO_PATCH_DLDI_OFFSET, dldiPatchNds);

	argStart = (char*)LCDC_BANK_C + readAddr((data_t*)LCDC_BANK_C, ARG_START_OFFSET);
	argStart = (char*)(((int)argStart + 3) & ~3);
	argData = (u16*)argStart;
	argSize = 0;

	for (; argc > 0 && *argv; ++argv, --argc) {
		for (argChar = *argv; *argChar != 0; ++argChar, ++argSize) {
			if (argSize & 1) {
				argTempVal |= (*argChar) << 8;
				*argData = argTempVal;
				++argData;
			} else {
				argTempVal = *argChar;
			}
		}
		if (argSize & 1) {
			*argData = argTempVal;
			++argData;
		}
		argTempVal = 0;
		++argSize;
	}
	*argData = argTempVal;

	writeAddr((data_t*)LCDC_BANK_C, ARG_START_OFFSET, (addr_t)argStart - (addr_t)LCDC_BANK_C);
	writeAddr((data_t*)LCDC_BANK_C, ARG_SIZE_OFFSET, argSize);

	if (dldiPatchNds) {
		if (!dldiPatchLoader((data_t*)LCDC_BANK_C, loaderSize, initDisc))
			return RUN_NDS_PATCH_DLDI_FAILED;
	}

	irqDisable(IRQ_ALL);

	if (REG_SCFG_EXT != 0 && dsModeSwitch) {
		memcpy((u32*)0x023F4000, (u32*)0x02FF4000, 0x8000);

		if (!boostCpu)
			REG_SCFG_CLK = 0x80;
		if (lockScfg)
			REG_SCFG_EXT = (boostVram ? 0x03002000 : 0x03000000);
		else
			REG_SCFG_EXT = (boostVram ? 0x83002000 : 0x83000000);
	}

	VRAM_C_CR = VRAM_ENABLE | VRAM_C_ARM7_0x06000000;
	REG_EXMEMCNT |= ARM7_OWNS_ROM | ARM7_OWNS_CARD;
	*((vu32*)0x02FFFFFC) = 0;
	*((vu32*)0x02FFFE04) = (u32)0xE59FF018;
	*((vu32*)0x02FFFE24) = (u32)0x02FFFE04;

	resetARM7(0x06008000);

	swiSoftReset();
	return RUN_NDS_OK;
}

eRunNdsRetCode runNdsFile (const char* filename, int argc, const char** argv)  {
	(void)filename;
	(void)argc;
	(void)argv;
	return RUN_NDS_STAT_FAILED;
}

eRunNdsRetCode runNdsWithExternalLoader(const void* loader, u32 loaderSize, u32 cluster, const char* bootstrapNdsPath,
                                        int argc, const char** argv, bool isRunFromSD, bool dsModeSwitch, bool boostCpu,
                                        bool boostVram, int language)
{
	const bool havedsiSD = (bootstrapNdsPath && bootstrapNdsPath[0] == 's' && bootstrapNdsPath[1] == 'd');
	installBootStub(havedsiSD, isRunFromSD, dsModeSwitch);

	bool patchLoader = true;
	if (havedsiSD)
		patchLoader = false;
	if (io_dldi_data && memcmp(io_dldi_data->friendlyName, "Default", 7) == 0)
		patchLoader = false;

	const bool lockScfg = (io_dldi_data && memcmp(io_dldi_data->friendlyName, "Default", 7) != 0);

	return runNdsFull(loader, loaderSize, cluster, true, patchLoader, false, bootstrapNdsPath, argc, argv, true,
	                  dsModeSwitch, lockScfg, boostCpu, boostVram, false, language);
}

/*
	b	startUp
	
storedFileCluster:
	.word	0x0FFFFFFF		@ default BOOT.NDS
initDisc:
	.word	0x00000001		@ init the disc by default
wantToPatchDLDI:
	.word	0x00000001		@ by default patch the DLDI section of the loaded NDS
@ Used for passing arguments to the loaded app
argStart:
	.word	_end - _start
argSize:
	.word	0x00000000
dldiOffset:
	.word	_dldi_start - _start
dsiSD:
	.word	0
*/

void(*exceptionstub)(void) = (void(*)(void))0x2ffa000;

bool installBootStub(bool havedsiSD, bool isRunFromSD, bool dsModeSwitch) {
#ifndef _NO_BOOTSTUB_
	extern char *fake_heap_end;
	struct __bootstub *bootstub = (struct __bootstub *)fake_heap_end;
	u32 *bootloader = (u32*)(fake_heap_end+bootstub_bin_size);

	memcpy(bootstub,bootstub_bin,bootstub_bin_size);
	memcpy(bootloader,load_bin,load_bin_size);
	bool ret = false;

	bootloader[8] = dsModeSwitch ? 0 : isDSiMode();
	if (havedsiSD && !dsModeSwitch) {
		if (io_dldi_data && memcmp(io_dldi_data->friendlyName, "Default", 7) != 0)
			ret = dldiPatchLoader((data_t*)bootloader, load_bin_size, false);
		else
			bootloader[3] = 0;
		if (isRunFromSD)
			bootloader[7] = 1;
	} else {
		ret = dldiPatchLoader((data_t*)bootloader, load_bin_size, false);
	}
	bootstub->arm9reboot = (VoidFn)(((u32)bootstub->arm9reboot)+fake_heap_end);
	bootstub->arm7reboot = (VoidFn)(((u32)bootstub->arm7reboot)+fake_heap_end);
	bootstub->bootsize = load_bin_size;

	memcpy(exceptionstub,exceptionstub_bin,exceptionstub_bin_size);

	exceptionstub();

	DC_FlushAll();

	return ret;
#else
	return true;
#endif

}
