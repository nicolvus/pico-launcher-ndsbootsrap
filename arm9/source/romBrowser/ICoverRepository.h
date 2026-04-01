#pragma once
#include "FileType/FileCover.h"

class FileInfo;
class InternalFileInfo;

class ICoverRepository
{
public:
    virtual ~ICoverRepository() = 0;

    virtual void Initialize() = 0;
    virtual FileCover* GetCoverForFile(
        const FileInfo& fileInfo, const InternalFileInfo* internalFileInfo) const = 0;
};

inline ICoverRepository::~ICoverRepository() { }
