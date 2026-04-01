#include "common.h"
#include "../FileInfoManager.h"
#include "core/task/TaskQueue.h"
#include "../views/CoverView.h"
#include "../Theme/IRomBrowserViewFactory.h"
#include "../FileType/UnknownFileCover.h"
#include "romBrowser/ICoverRepository.h"
#include "CoverFlowFileRecyclerAdapter.h"

void CoverFlowFileRecyclerAdapter::GetViewSize(int& width, int& height) const
{
    width = 44;
    height = 44;
}

View* CoverFlowFileRecyclerAdapter::CreateView() const
{
    return new CoverView(_vblankTextureLoader);
}

void CoverFlowFileRecyclerAdapter::DestroyView(View* view) const
{
    auto coverView = static_cast<CoverView*>(view);
    delete coverView;
}

TaskResult<void> CoverFlowFileRecyclerAdapter::BindView(View* view, int index,
    const InternalFileInfo* internalFileInfo, const vu8& cancelRequested) const
{
    auto coverView = static_cast<CoverView*>(view);
    auto cover = _fileInfoManager->GetFileCover(index);
    if (cancelRequested)
    {
        _fileInfoManager->ReleaseFileInfo(index);
        return TaskResult<void>::Canceled();
    }
    coverView->SetCover(std::move(cover));
    coverView->UploadCoverGraphics();
    if (cancelRequested)
    {
        coverView->ClearCover();
        _fileInfoManager->ReleaseFileInfo(index);
        return TaskResult<void>::Canceled();
    }
    return TaskResult<void>::Completed();
}

void CoverFlowFileRecyclerAdapter::ReleaseView(View* view, int index) const
{
    LOG_DEBUG("Releasing %d\n", index);
    auto coverView = static_cast<CoverView*>(view);
    coverView->ClearCover();
    _fileInfoManager->ReleaseFileInfo(index);
}

void CoverFlowFileRecyclerAdapter::InitVram(const VramContext& vramContext)
{
    // _iconGridItemViewGraphics = _romBrowserViewFactory->UploadIconGridItemViewGraphics(vramManager);
}
