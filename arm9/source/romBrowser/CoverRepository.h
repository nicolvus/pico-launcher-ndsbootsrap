#pragma once
#include "ICoverRepository.h"
#include "SdFolder.h"

class CoverRepository : public ICoverRepository
{
public:
    void Initialize() override;
    FileCover* GetCoverForFile(
        const FileInfo& fileInfo, const InternalFileInfo* internalFileInfo) const override;

private:
    std::unique_ptr<SdFolder> _ndsCoversFolder;
    std::unique_ptr<SdFolder> _gbaCoversFolder;
    std::unique_ptr<SdFolder> _userCoversFolder;

    const SdFolder* GetCoverFolder(const char* coverFolderName) const;
};
