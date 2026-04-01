#include "common.h"
#include "SampledInputProvider.h"

void SampledInputProvider::Update()
{
    InputKey curKeys = _currentKeys;
    InputKey trig = InputKey::None;
    InputKey rel = InputKey::None;

    while (_inputBufferReadPtr != _inputBufferWritePtr)
    {
        InputKey nextKeys = _inputBuffer[_inputBufferReadPtr];
        trig |= (nextKeys ^ curKeys) & nextKeys;
        rel |= (nextKeys ^ curKeys) & curKeys;
        curKeys = nextKeys;
        _inputBufferReadPtr = (_inputBufferReadPtr + 1) & 3;
    }

    _triggeredKeys = trig;
    _releasedKeys = rel;
    _currentKeys = curKeys;
}
