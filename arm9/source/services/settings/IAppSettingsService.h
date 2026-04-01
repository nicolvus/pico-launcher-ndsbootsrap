#pragma once
#include "AppSettings.h"

class IAppSettingsService
{
public:
    virtual ~IAppSettingsService() { }

    virtual AppSettings& GetAppSettings() = 0;
    virtual const AppSettings& GetAppSettings() const = 0;
    virtual void Save() const = 0;
};