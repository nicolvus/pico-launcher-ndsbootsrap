#include "common.h"
#include "gui/IVramManager.h"
#include "gui/GraphicsContext.h"
#include "BannerListItemView.h"

BannerListItemView::BannerListItemView(std::unique_ptr<LabelView> firstLine,
    std::unique_ptr<LabelView> secondLine, std::unique_ptr<LabelView> thirdLine)
    : _firstLine(std::move(firstLine))
    , _secondLine(std::move(secondLine))
    , _thirdLine(std::move(thirdLine))
{
    AddChildTail(_firstLine.get());
    AddChildTail(_secondLine.get());
    AddChildTail(_thirdLine.get());
}

void BannerListItemView::Update()
{
    ViewContainer::Update();

    if (_icon)
    {
        _icon->Update();
    }
}
