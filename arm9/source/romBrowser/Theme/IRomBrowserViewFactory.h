#pragma once
#include <memory>
#include "../views/IconGridItemView.h"
#include "../views/BannerListItemView.h"
#include "../views/AppBarView.h"
#include "../views/BannerView.h"
#include "gui/views/RecyclerViewBase.h"

class VramContext;
class VBlankTextureLoader;
class RomBrowserViewModel;
class IThemeFileIconFactory;
class FileRecyclerAdapter;

class IRomBrowserViewFactory
{
public:
    virtual ~IRomBrowserViewFactory() = 0;

    virtual IconGridItemView* CreateIconGridItemView() const = 0;
    virtual IconGridItemView::VramToken UploadIconGridItemViewGraphics(
        const VramContext& vramContext) const { return IconGridItemView::VramToken(0); }

    virtual BannerListItemView* CreateBannerListItemView(VBlankTextureLoader* vblankTextureLoader) const = 0;
    virtual BannerListItemView::VramToken UploadBannerListItemViewGraphics(
        const VramContext& vramContext) const { return BannerListItemView::VramToken(0); }

    virtual std::unique_ptr<AppBarView> CreateAppBarView(int x, int y, AppBarView::Orientation orientation,
        int startButtonCount, int endButtonCount) const = 0;

    virtual std::unique_ptr<BannerView> CreateFileInfoView() const = 0;

    virtual std::unique_ptr<RecyclerViewBase> CreateCoverFlowRecyclerView() const = 0;

    virtual FileRecyclerAdapter* CreateCoverFlowRecyclerAdapter(
        RomBrowserViewModel* viewModel, const IThemeFileIconFactory* themeFileIconFactory,
        VBlankTextureLoader* vblankTextureLoader) const = 0;
};

inline IRomBrowserViewFactory::~IRomBrowserViewFactory() { }
