#pragma once
#include "FileType.h"

class UnknownFileType : public FileType
{
public:
    static const UnknownFileType sInstance;

private:
    constexpr UnknownFileType()
        : FileType("other", FileTypeClassification::Unknown) { }
};