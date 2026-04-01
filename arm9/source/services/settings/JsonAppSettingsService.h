#pragma once
#include "IAppSettingsService.h"
#include "JsonAppSettingsSerializer.h"

class JsonAppSettingsService : public IAppSettingsService
{
    JsonAppSettingsSerializer _serializer;
    AppSettings _appSettings;
    const char* _filePath;
public:
    explicit JsonAppSettingsService(const char* filePath);

    AppSettings& GetAppSettings() override { return _appSettings; }
    const AppSettings& GetAppSettings() const override { return _appSettings; }

    void Save() const override
    {
        _serializer.Serialize(&_appSettings, _filePath);
    }
};