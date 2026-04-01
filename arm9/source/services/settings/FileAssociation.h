#pragma once
#include "core/String.h"

class FileAssociation
{
public:
    FileAssociation() { }

    FileAssociation(const char* extension, const char* applicationPath)
        : extension(extension), applicationPath(applicationPath) { }

    String<char, 8> extension;
    String<char, 256> applicationPath;
};
