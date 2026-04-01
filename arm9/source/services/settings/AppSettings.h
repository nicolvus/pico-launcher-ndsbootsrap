#pragma once
#include <memory>
#include "core/String.h"
#include "RomBrowserDisplaySettings.h"
#include "NdsLoaderKind.h"
#include "LoaderBackend.h"
#include "FileAssociation.h"

class AppSettings
{
public:
    String<char, 16> language = "english";
    String<char, 64> theme = "material";
    String<char, 256> lastUsedFilePath = "";
    RomBrowserDisplaySettings romBrowserDisplaySettings;
    NdsLoaderKind ndsLoaderKind = NdsLoaderKind::PicoLoader;

    /// Which loader binary to prefer when launching via nds-bootstrap.
    LoaderBackend loaderBackend = LoaderBackend::Auto;
    /// Override for the legacy loader path (/_pico/load.bin). Empty string = use default.
    String<char, 64> legacyLoaderPath = "";
    /// Override for the bootstrap loader path (/_nds/nds-bootstrap.nds). Empty string = use default.
    String<char, 64> bootstrapLoaderPath = "";

    std::unique_ptr<FileAssociation[]> fileAssociations;
    u32 numberOfFileAssociations = 0;
};