#pragma once
#include "services/settings/LoaderBackend.h"

void SetNdsBootstrapLaunchRomPath(const char* path);
const char* GetNdsBootstrapLaunchRomPath();

/// Loader backend policy used when TryLaunchNdsBootstrap() resolves the loader binary.
void SetNdsBootstrapLoaderBackend(LoaderBackend backend);
LoaderBackend GetNdsBootstrapLoaderBackend();

/// Optional path overrides for the loader binary.  Pass "" to use the compiled-in defaults.
void SetNdsBootstrapLegacyLoaderPath(const char* path);
const char* GetNdsBootstrapLegacyLoaderPath();

void SetNdsBootstrapBootstrapLoaderPath(const char* path);
const char* GetNdsBootstrapBootstrapLoaderPath();
