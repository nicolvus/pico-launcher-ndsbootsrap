#pragma once
#include "../FileRecyclerAdapter.h"
#include "../views/IconGridItemView.h"

class IRomBrowserViewFactory;

class IconGridFileRecyclerAdapter : public FileRecyclerAdapter
{
public:
    IconGridFileRecyclerAdapter(FileInfoManager* fileInfoManager,
        TaskQueueBase* taskQueue, const IThemeFileIconFactory* themeFileIconFactory,
        const IRomBrowserViewFactory* romBrowserViewFactory)
        : FileRecyclerAdapter(fileInfoManager, taskQueue, themeFileIconFactory)
        , _romBrowserViewFactory(romBrowserViewFactory) { }

    void GetViewSize(int& width, int& height) const override;
    View* CreateView() const override;
    void DestroyView(View* view) const override;
    void BindView(View* view, int index) const override;
    void ReleaseView(View* view, int index) const override;

    void InitVram(const VramContext& vramContext) override;

private:
    const IRomBrowserViewFactory* _romBrowserViewFactory;
    IconGridItemView::VramToken _iconGridItemViewGraphics;

    TaskResult<void> BindView(View* view, int index,
        const InternalFileInfo* internalFileInfo, const vu8& cancelRequested) const override;
};
