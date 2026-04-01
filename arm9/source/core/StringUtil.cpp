#include "common.h"
#include <string.h>
#include "StringUtil.h"

u32 StringUtil::Copy(char* dst, const char* src, u32 dstLength)
{
    if (!dst || dstLength == 0)
    {
        return 0;
    }

    u32 i = 0;
    if (src)
    {
        for (; i < dstLength - 1; i++)
        {
            char c = src[i];
            dst[i] = c;
            if (c == 0)
            {
                return i;
            }
        }
    }

    dst[i] = 0;
    return i;
}

u32 StringUtil::Copy(char16_t* dst, const char16_t* src, u32 dstLength)
{
    if (!dst || dstLength == 0)
    {
        return 0;
    }

    u32 i = 0;
    if (src)
    {
        for (; i < dstLength - 1; i++)
        {
            char16_t c = src[i];
            dst[i] = c;
            if (c == 0)
            {
                return i;
            }
        }
    }

    dst[i] = 0;
    return i;
}

u32 StringUtil::Copy(char16_t* dst, const char* src, u32 dstLength)
{
    if (!dst || dstLength == 0)
    {
        return 0;
    }

    u32 i = 0;
    if (src)
    {
        for (; i < dstLength - 1; i++)
        {
            char c0 = *src++;

            // decode UTF-8
            if ((c0 & 0x80) == 0)
            {
                // 1 byte
                dst[i] = c0 & 0x7F;
            }
            else if ((c0 & 0xE0) == 0xC0)
            {
                // 2 bytes
                char c1 = *src++;
                dst[i] = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
            }
            else if ((c0 & 0xF0) == 0xE0)
            {
                // 3 bytes
                char c1 = *src++;
                char c2 = *src++;
                dst[i] = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            }
            else
            {
                // 4 bytes; not supported
                src += 3;
                dst[i] = '?'; // substitute with question mark
            }

            if (c0 == 0)
            {
                return i;
            }
        }
    }

    dst[i] = 0;
    return i;
}
