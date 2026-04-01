#include "common.h"
#include "LinearCongruentialGenerator.h"

u32 LinearCongruentialGenerator::Next()
{
    _state = _state * 0x5D588B656C078965LL + 0x269EC3LL;
    return _state >> 32;
}
