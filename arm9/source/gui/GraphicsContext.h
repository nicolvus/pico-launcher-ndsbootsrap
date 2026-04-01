#pragma once
#include "core/math/Point.h"
#include "core/math/Rectangle.h"
#include "OamManager.h"
#include "PaletteManager.h"
#include "Rgb6Palette.h"

class GraphicsContext
{
public:
    GraphicsContext(OamManager* oamManager, PaletteManager* paletteManager, const Rgb6Palette* rgb6Palette)
        : _priority(3), _polygonId(0), _oamManager(oamManager)
        , _paletteManager(paletteManager), _rgb6Palette(rgb6Palette)
        , _clipArea(0, 0, 256, 192) { }

    u32 SetPriority(u32 priority)
    {
        u32 oldPriority = _priority;
        _priority = priority;
        return oldPriority;
    }

    constexpr u32 GetPriority() const { return _priority; }

    u32 SetPolygonId(u32 polygonId)
    {
        u32 oldPolygonId = _polygonId;
        _polygonId = polygonId;
        return oldPolygonId;
    }

    constexpr u32 GetPolygonId() const { return _polygonId; }

    OamManager& GetOamManager() const { return *_oamManager; }
    PaletteManager& GetPaletteManager() const { return *_paletteManager; }
    const Rgb6Palette* GetRgb6Palette() const { return _rgb6Palette; }

    virtual PaletteManager& GetPaletteManager(int scanline) const { return *_paletteManager; }

    void SetClipArea(const Rectangle& clipArea, bool inverse = false)
    {
        _clipArea = clipArea;
        _inverseClipArea = inverse;
    }

    constexpr bool IsVisible(const Rectangle& bounds) const
    {
        if (!Rectangle(0, 0, 256, 192).OverlapsWith(bounds))
            return false;
        return _inverseClipArea ? !_clipArea.Contains(bounds) : _clipArea.OverlapsWith(bounds);
    }

    void ResetClipArea()
    {
        SetClipArea(Rectangle(0, 0, 256, 192));
    }

private:
    u32 _priority;
    u8 _polygonId;

    OamManager* _oamManager;
    PaletteManager* _paletteManager;
    const Rgb6Palette* _rgb6Palette;
    Rectangle _clipArea;
    bool _inverseClipArea = false;
};