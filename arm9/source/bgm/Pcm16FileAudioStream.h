#pragma once
#include "common.h"
#include "fat/File.h"
#include "IAudioStream.h"

/// @brief Raw mono 16 bit pcm audio file stream.
///        Assumes a sample rate of 32728 Hz.
class alignas(32) Pcm16FileAudioStream : public IAudioStream
{
public:
    /// @brief Opens the given file. Call this function before
    ///        using ReadSamples.
    /// @param filePath The path of the file to open.
    /// @return True if the file was successfully opened, or false otherwise.
    bool Open(const TCHAR* filePath);

    void Close() override;
    void ReadSamples(s16* left, s16* right, u32 count) override;

    u32 GetSampleRate() const override
    {
        return 32728;
    }

    ~Pcm16FileAudioStream()
    {
        Close();
    }

private:
    File _audioFile alignas(32);
    DWORD _clusterTable[1024] alignas(32);
};
