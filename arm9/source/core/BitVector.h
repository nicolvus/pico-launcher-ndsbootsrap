#pragma once
#include <array>

template <u32 Length>
class BitVector
{
public:
    BitVector()
    {
        Clear();
    }

    u32 Get(u32 idx) const
    {
        return (_flags[idx >> 5] >> (idx & 0x1F)) & 1;
    }

    void Set(u32 idx, u32 value)
    {
        u32 f = _flags[idx >> 5];
        if (value)
            f |= 1 << (idx & 0x1F);
        else
            f &= ~(1 << (idx & 0x1F));
        _flags[idx >> 5] = f;
    }

    void Clear()
    {
        _flags.fill(0);
    }

    [[gnu::noinline]]
    constexpr int FindFirstZero()
    {
        u32 globalBitNr = 0;
        for (const u32 bits : _flags)
        {
            if (bits == ~0u)
            {
                globalBitNr += 32;
                continue;
            }
            u32 invBits = ~bits;
            globalBitNr += 31 - __builtin_clz(-invBits & invBits);
            return globalBitNr >= Length ? -1 : globalBitNr;
        }
        return -1;
    }

private:
    std::array<u32, (Length + 31) / 32> _flags;
};