#include "common.h"
#include <string.h>
#include "Pcm16FileAudioStream.h"

bool Pcm16FileAudioStream::Open(const TCHAR* filePath)
{
    if (_audioFile.Open(filePath, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        return false;

    if (_audioFile.CreateClusterTable(_clusterTable, sizeof(_clusterTable)) != FR_OK)
        return false;

    return true;
}

void Pcm16FileAudioStream::Close()
{
    _audioFile.Close();
}

void Pcm16FileAudioStream::ReadSamples(s16 *left, s16 *right, u32 count)
{
    u32 bytesRead = 0;
    _audioFile.Read(left, count * sizeof(s16), bytesRead);
    memcpy(right, left, count * sizeof(s16));
}
