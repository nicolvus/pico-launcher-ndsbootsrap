#pragma once
#include "ff.h"

class Directory
{
    DIR _directory;

public:
    FRESULT Open(const TCHAR* path) { return f_opendir(&_directory, path); }
    FRESULT Close() { return f_closedir(&_directory); }
    FRESULT Read(FILINFO* fileInfo) { return f_readdir(&_directory, fileInfo); }
    FRESULT Rewind() { return f_rewinddir(&_directory); }

    ~Directory()
    {
        Close();
    }

    DIR* GetFatFsDirectory() { return &_directory; }
};