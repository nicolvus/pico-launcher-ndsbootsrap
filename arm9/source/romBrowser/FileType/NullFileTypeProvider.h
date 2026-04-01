#pragma once
#include "IFileTypeProvider.h"
#include "UnknownFileType.h"

/// @brief Dummy implementation of \see IFileTypeProvider that always returns \see UnknownFileType.
class NullFileTypeProvider : public IFileTypeProvider
{
public:
    const FileType* GetFileType(const TCHAR* path) const override
    {
        return &UnknownFileType::sInstance;
    }
};