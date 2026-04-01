#pragma once
class AppSettings;

class JsonAppSettingsSerializer
{
public:
    void Serialize(const AppSettings* appSettings, const char* filePath) const;
    bool Deserialize(AppSettings* appSettings, const char* filePath) const;
};