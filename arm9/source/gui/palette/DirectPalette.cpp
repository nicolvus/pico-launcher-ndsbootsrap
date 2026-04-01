#include "common.h"
#include "DirectPalette.h"

u32 DirectPalette::GetHashCode() const
{
    u32 hashcode = 1430287;
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++)
    {
        hashcode = hashcode * 7302013 + ((u32*)_colors)[i];
    }
    return hashcode;
}
