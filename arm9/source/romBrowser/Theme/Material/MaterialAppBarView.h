#pragma once
#include "../../views/AppBarView.h"

class MaterialAppBarView : public AppBarView
{
public:
    MaterialAppBarView(int x, int y, Orientation orientation,
        int startButtonCount, int endButtonCount, const MaterialColorScheme* materialColorScheme);

    void InitVram(const VramContext& vramContext) override;
};
