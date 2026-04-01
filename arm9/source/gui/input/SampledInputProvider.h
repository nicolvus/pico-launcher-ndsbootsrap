#pragma once
#include "InputProvider.h"
#include "IInputSource.h"

/// @brief Input provider providing input from an \see IInputSource.
class SampledInputProvider : public InputProvider
{
public:
    explicit SampledInputProvider(const IInputSource* inputSource)
        : _inputSource(inputSource), _inputBufferReadPtr(0), _inputBufferWritePtr(0) { }

    void Update() override;

    /// @brief Samples the input source.
    void Sample()
    {
        _inputBuffer[_inputBufferWritePtr] = _inputSource->Sample();
        _inputBufferWritePtr = (_inputBufferWritePtr + 1) & 3;
    }

    void Reset() override
    {
        InputProvider::Reset();
        _inputBufferReadPtr = 0;
        _inputBufferWritePtr = 0;
    }

private:
    const IInputSource* _inputSource;

    InputKey _inputBuffer[4];
    u8 _inputBufferReadPtr;
    u8 _inputBufferWritePtr;
};
