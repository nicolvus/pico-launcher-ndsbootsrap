#pragma once
#include "gui/VBlankTextureLoadRequest.h"

extern "C" void cover_bitmapToTiledCopy(const void* bitmap, void* vram);

#define COVER_WIDTH         106
#define COVER_HEIGHT        96

/// @brief Abstract base class for a file cover.
class FileCover
{
public:
    virtual ~FileCover() = 0;

    /// @brief Sets the texture and texture palette offsets of this cover after it has been loaded to vram.
    /// @param texVramOffset The texture vram offset.
    /// @param plttVramOffset The texture palette vram offset.
    void SetTexVramOffset(u32 texVramOffset, u32 plttVramOffset)
    {
        _texVramOffset = texVramOffset;
        _plttVramOffset = plttVramOffset;
    }

    /// @brief Creates a texture load request to load this cover.
    /// @return The constructed texture load request.
    virtual VBlankTextureLoadRequest CreateTextureLoadRequest() const = 0;

    /// @brief Uploads the 8bpp bitmap graphics of this cover to BG vram.
    /// @param destination The address to load to.
    virtual void Upload2DCoverBitmap(void* destination) const = 0;

    /// @brief Uploads the 256 color palette of this cover to palette ram.
    /// @param destination The address to load to.
    virtual void Upload2DCoverPalette(void* destination) const = 0;

    /// @brief Returns whether this cover is an actual cover graphic of the file.
    /// @return True when this cover is an actual cover graphic of the file,
    ///         or false when the graphic is a (generic) placeholder.
    virtual bool IsActualCover() const = 0;

protected:
    u32 _texVramOffset = 0;
    u32 _plttVramOffset = 0;
};

inline FileCover::~FileCover() { }
