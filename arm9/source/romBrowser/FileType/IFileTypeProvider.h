#pragma once
class FileType;

/// @brief Interface for getting a file type based on a file path.
class IFileTypeProvider
{
public:
    virtual ~IFileTypeProvider() = 0;

    virtual const FileType* GetFileType(const TCHAR* path) const = 0;
};

inline IFileTypeProvider::~IFileTypeProvider() { }
