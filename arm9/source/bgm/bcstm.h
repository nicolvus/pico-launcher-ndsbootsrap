#pragma once
#include "common.h"

#define BCSTM_FORMAT_DSP_ADPCM      2

struct bcstm_ref_t
{
    u16 typeId;
    u16 padding;
    u32 offset;
};

struct bcstm_sized_ref_t
{
    u16 typeId;
    u16 padding;
    u32 offset;
    u32 size;
};

struct bcstm_ref_table_t
{
    u32 count;
    bcstm_ref_t references[1];
};

struct bcstm_header_t
{
    u32 signature;
    u16 endianness;
    u16 headerSize;
    u32 version;
    u32 fileSize;
    u16 nrBlocks;
    u16 reserved;
    bcstm_sized_ref_t infoBlockRef;
    bcstm_sized_ref_t seekBlockRef;
    bcstm_sized_ref_t dataBlockRef;
};

struct bcstm_info_stream_t
{
    u8 format;
    u8 loop;
    u8 nrChannels;
    u8 padding;
    u32 sampleRate;
    u32 loopStart;
    u32 loopEnd;
    u32 nrBlocks;
    u32 blockSize;
    u32 blockSampleCount;
    u32 lastBlockSize;
    u32 lastBlockSampleCount;
    u32 lastBlockPaddedSize;
    u32 seekEntrySize;
    u32 seekInterval;
    bcstm_ref_t dataRef;
};

struct bcstm_info_channel_t
{
    bcstm_ref_t codecInfoRef;
};

struct bcstm_dspadpcm_context_t
{
    u8 predictorAndScale;
    u8 reserved;
    s16 last1;
    s16 last2;
};

struct alignas(4) bcstm_dspadpcm_t
{
    s16 coefs[16];
    bcstm_dspadpcm_context_t context;
    bcstm_dspadpcm_context_t loopContext;
};

struct bcstm_info_t
{
    u32 signature;
    u32 sectionSize;
    bcstm_ref_t streamInfoRef;
    bcstm_ref_t trackInfoRef;
    bcstm_ref_t channelInfoRef;
};

struct bcstm_seek_t
{
    u32 signature;
    u32 sectionSize;
    u8 seekData[1];
};

struct bcstm_data_t
{
    u32 signature;
    u32 sectionSize;
    u8 data[1];
};
