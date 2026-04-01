#pragma once
#include "services/settings/RomBrowserLayout.h"
#include "RomBrowserDisplayMode.h"

class RomBrowserDisplayModeFactory
{
public:
    const RomBrowserDisplayMode* GetRomBrowserDisplayMode(
        RomBrowserLayout romBrowserDisplayMode) const;
};
