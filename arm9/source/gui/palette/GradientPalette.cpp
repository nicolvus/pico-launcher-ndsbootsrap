#include "common.h"
#include "GradientPalette.h"

u32 GradientPalette::GetHashCode() const
{
    u32 hashcode = 1430287;
    hashcode = hashcode * 7302013 + _fromColor.r;
    hashcode = hashcode * 7302013 + _fromColor.g;
    hashcode = hashcode * 7302013 + _fromColor.b;
    hashcode = hashcode * 7302013 + _toColor.r;
    hashcode = hashcode * 7302013 + _toColor.g;
    hashcode = hashcode * 7302013 + _toColor.b;
    return hashcode;
}
