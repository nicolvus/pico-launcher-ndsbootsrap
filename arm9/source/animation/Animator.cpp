#include "common.h"
#include "core/math/fixed.h"
#include "Animator.h"

template <>
bool Animator<int>::Update()
{
    if (++_frame >= _duration)
    {
        _frame = _duration;
        _value = _to;
        return true;
    }

    auto relativePos = _curve->Compute(_frame * _invDuration);
    _value = _from + (relativePos.LongMul(_to - _from) + 0.5).Int();

    return false;
}

template <>
bool Animator<fix32<12>>::Update()
{
    if (++_frame >= _duration)
    {
        _frame = _duration;
        _value = _to;
        return true;
    }

    auto relativePos = _curve->Compute(_frame * _invDuration);
    _value = _from + fix32<12>(relativePos.LongMul(_to - _from));

    return false;
}
