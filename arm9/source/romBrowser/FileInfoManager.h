#pragma once
#include "common.h"
#include <memory>
#include "FileInfo.h"
#include "FileType/FileCover.h"
#include "ICoverRepository.h"
#include "core/SharedPtr.h"
#include "FileType/InternalFileInfo.h"

class FileInfoManager
{
public:
    FileInfoManager(std::unique_ptr<const FileInfo*[]> items, u32 itemCount, const ICoverRepository& coverRepository);
    ~FileInfoManager();

    const InternalFileInfo* GetInternalFileInfo(int index)
    {
        return _extraFileInfo[index].internalFileInfo;
    }

    SharedPtr<FileCover> GetFileCover(int index)
    {
        return _extraFileInfo[index].fileCover;
    }

    void LoadFileInfo(int index)
    {
        auto internalFileInfo = GetInternalFileInfo(index);
        if (!internalFileInfo)
        {
            internalFileInfo = _items[index]->CreateInternalFileInfo();
        }

        if (!_extraFileInfo[index].fileCover.IsValid())
        {
            _extraFileInfo[index].fileCover = SharedPtr(_coverRepository.GetCoverForFile(*_items[index], internalFileInfo));
        }

        _extraFileInfo[index].internalFileInfo = internalFileInfo;
    }

    void ReleaseFileInfo(int index)
    {
        auto internalFileInfo = GetInternalFileInfo(index);
        if (internalFileInfo)
        {
            delete internalFileInfo;
            _extraFileInfo[index].internalFileInfo = nullptr;
        }

        _extraFileInfo[index].fileCover.Reset();
    }

    int GetItemIndex(const char* fileName);

    const FileInfo& GetItem(int index) const { return *_items[index]; }
    u32 GetItemCount() const { return _itemCount; }

private:
    struct ExtraFileInfo
    {
        const InternalFileInfo* internalFileInfo;
        SharedPtr<FileCover> fileCover;
    };

    std::unique_ptr<const FileInfo*[]> _items;
    u32 _itemCount;
    std::unique_ptr<ExtraFileInfo[]> _extraFileInfo;
    const ICoverRepository& _coverRepository;
};