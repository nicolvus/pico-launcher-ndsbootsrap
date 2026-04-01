#pragma once
#include "FileCover.h"
#include <libtwl/dma/dmaNitro.h>

class StaticFileCover : public FileCover
{
public:
    StaticFileCover(const void* bitmapData, const void* paletteData)
        : _bitmapData(bitmapData), _paletteData(paletteData) { }

    VBlankTextureLoadRequest CreateTextureLoadRequest() const override
    {
        return VBlankTextureLoadRequest(
            _bitmapData, 128 * 96, _texVramOffset,
            _paletteData, 512, _plttVramOffset,
            nullptr, nullptr);
    }

    void Upload2DCoverBitmap(void* destination) const override
    {
        cover_bitmapToTiledCopy(_bitmapData, destination);
    }

    void Upload2DCoverPalette(void* destination) const override
    {
        dma_ntrCopy32(3, _paletteData, destination, 512);
    }

    bool IsActualCover() const override { return false; }

private:
    const void* _bitmapData;
    const void* _paletteData;
};
