#include "common.h"
#include "gui/VramContext.h"
#include "gui/Gx.h"
#include "core/math/ColorConverter.h"
#include "Rgb6Palette.h"

#define PLTT_IDX_15     0
#define PLTT_IDX_30     1
#define PLTT_IDX_31     2

struct color_mapping_t
{
    u8 vtxColorComponent;
    u8 plttIdx;
};

static const color_mapping_t sMappingTable[64] =
{
    { 0, PLTT_IDX_31 }, // 0
    { 1, PLTT_IDX_15 }, // 1
    { 2, PLTT_IDX_15 }, // 2
    { 1, PLTT_IDX_31 }, // 3
    { 4, PLTT_IDX_15 }, // 4
    { 2, PLTT_IDX_31 }, // 5
    { 6, PLTT_IDX_15 }, // 6
    { 3, PLTT_IDX_31 }, // 7
    { 8, PLTT_IDX_15 }, // 8
    { 4, PLTT_IDX_31 }, // 9
    { 10, PLTT_IDX_15 }, // 10
    { 5, PLTT_IDX_31 }, // 11
    { 12, PLTT_IDX_15 }, // 12
    { 6, PLTT_IDX_31 }, // 13
    { 14, PLTT_IDX_15 }, // 14
    { 7, PLTT_IDX_31 }, // 15
    { 16, PLTT_IDX_15 }, // 16
    { 8, PLTT_IDX_31 }, // 17
    { 18, PLTT_IDX_15 }, // 18
    { 9, PLTT_IDX_31 }, // 19
    { 20, PLTT_IDX_15 }, // 20
    { 10, PLTT_IDX_31 }, // 21
    { 22, PLTT_IDX_15 }, // 22
    { 11, PLTT_IDX_31 }, // 23
    { 24, PLTT_IDX_15 }, // 24
    { 12, PLTT_IDX_31 }, // 25
    { 26, PLTT_IDX_15 }, // 26
    { 13, PLTT_IDX_31 }, // 27
    { 28, PLTT_IDX_15 }, // 28
    { 14, PLTT_IDX_31 }, // 29
    { 30, PLTT_IDX_15 }, // 30
    { 15, PLTT_IDX_31 }, // 31
    { 16, PLTT_IDX_30 }, // 32
    { 16, PLTT_IDX_31 }, // 33
    { 17, PLTT_IDX_30 }, // 34
    { 17, PLTT_IDX_31 }, // 35
    { 18, PLTT_IDX_30 }, // 36
    { 18, PLTT_IDX_31 }, // 37
    { 19, PLTT_IDX_30 }, // 38
    { 19, PLTT_IDX_31 }, // 39
    { 20, PLTT_IDX_30 }, // 40
    { 20, PLTT_IDX_31 }, // 41
    { 21, PLTT_IDX_30 }, // 42
    { 21, PLTT_IDX_31 }, // 43
    { 22, PLTT_IDX_30 }, // 44
    { 22, PLTT_IDX_31 }, // 45
    { 23, PLTT_IDX_30 }, // 46
    { 23, PLTT_IDX_31 }, // 47
    { 24, PLTT_IDX_30 }, // 48
    { 24, PLTT_IDX_31 }, // 49
    { 25, PLTT_IDX_30 }, // 50
    { 25, PLTT_IDX_31 }, // 51
    { 26, PLTT_IDX_30 }, // 52
    { 26, PLTT_IDX_31 }, // 53
    { 27, PLTT_IDX_30 }, // 54
    { 27, PLTT_IDX_31 }, // 55
    { 28, PLTT_IDX_30 }, // 56
    { 28, PLTT_IDX_31 }, // 57
    { 29, PLTT_IDX_30 }, // 58
    { 29, PLTT_IDX_31 }, // 59
    { 30, PLTT_IDX_30 }, // 60
    { 31, PLTT_IDX_30 }, // 61
    { 31, PLTT_IDX_30 }, // 62 -> 61
    { 31, PLTT_IDX_31 }  // 63
};

void Rgb6Palette::UploadGraphics(const VramContext& vramContext)
{
    const auto paletteVramManager = vramContext.GetTexPlttVramManager();

    if (paletteVramManager)
    {
        _vramOffset = paletteVramManager->Alloc(432);
        static const u8 colorValues[3] = { 15, 30, 31 };
        for (int r = 0; r < 3; r++)
        {
            for (int g = 0; g < 3; g++)
            {
                for (int b = 0; b < 3; b++)
                {
                    int index = b * 3 * 3 + g * 3 + r;
                    paletteVramManager->GetVramAddress(_vramOffset)[index * 8]
                        = ColorConverter::ToXBGR555(Rgb<5, 5, 5>(colorValues[r], colorValues[g], colorValues[b]));
                }
            }
        }
    }
}

void Rgb6Palette::ApplyColor(const Rgb<6,6,6>& color) const
{
    const auto& rConversion = sMappingTable[color.r];
    const auto& gConversion = sMappingTable[color.g];
    const auto& bConversion = sMappingTable[color.b];

    Gx::Color(ColorConverter::ToXBGR555(Rgb<5, 5, 5>(
        rConversion.vtxColorComponent, gConversion.vtxColorComponent, bConversion.vtxColorComponent)));
    u32 colorOffset = bConversion.plttIdx * 3 * 3 + gConversion.plttIdx * 3 + rConversion.plttIdx;
    Gx::TexPlttBase((_vramOffset >> 4) + colorOffset);
}
