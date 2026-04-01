#pragma once
#include <memory>
#include "IFileTypeProvider.h"
#include "services/settings/AppSettings.h"
#include "CustomFileType.h"

/// @brief Implementation of \see IFileTypeProvider that uses extensions to identify file types.
class ExtensionFileTypeProvider : public IFileTypeProvider
{
public:
    explicit ExtensionFileTypeProvider(const AppSettings& appSettings);

    const FileType* GetFileType(const TCHAR* path) const override;

private:
    const AppSettings& _appSettings;
    std::unique_ptr<CustomFileType[]> _customFileTypes;
};