#include "common.h"
#include "JsonAppSettingsService.h"

JsonAppSettingsService::JsonAppSettingsService(const char* filePath)
    : _filePath(filePath)
{
    if (!_serializer.Deserialize(&_appSettings, _filePath))
        Save();
}
