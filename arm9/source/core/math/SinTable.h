#pragma once
#include <cmath>
#include "fixed.h" 

template<int N>
class SinTable
{
    struct sin_cos_t
    {
        fix16<14> sin = 0;
        fix16<14> cos = 0;
    };

    sin_cos_t _table[N];

public:
    constexpr SinTable()
        : _table()
    {
        for (auto i = 0; i < N; i++)
            _table[i] = { sin(i * 2 * M_PI / N), cos(i * 2 * M_PI / N) };
    }

    constexpr fix16<14> Sin(u16 angle) const { return _table[(angle * N) >> 16].sin; }
    constexpr fix16<14> Cos(u16 angle) const { return _table[(angle * N) >> 16].cos; }
    constexpr const sin_cos_t& SinCos(u16 angle) const { return _table[(angle * N) >> 16]; }
};

constexpr SinTable<4096> gSinTable = SinTable<4096>();