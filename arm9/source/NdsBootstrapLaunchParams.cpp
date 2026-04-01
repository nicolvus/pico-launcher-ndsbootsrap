#include "common.h"
#include "NdsBootstrapLaunchParams.h"
#include "core/StringUtil.h"

static char sRomPath[256];

void SetNdsBootstrapLaunchRomPath(const char* path)
{
    StringUtil::Copy(sRomPath, path, sizeof(sRomPath));
}

const char* GetNdsBootstrapLaunchRomPath()
{
    return sRomPath;
}

static LoaderBackend sLoaderBackend = LoaderBackend::Auto;
static char sLegacyLoaderPath[96]    = "";
static char sBootstrapLoaderPath[96] = "";

void SetNdsBootstrapLoaderBackend(LoaderBackend backend)
{
    sLoaderBackend = backend;
}

LoaderBackend GetNdsBootstrapLoaderBackend()
{
    return sLoaderBackend;
}

void SetNdsBootstrapLegacyLoaderPath(const char* path)
{
    StringUtil::Copy(sLegacyLoaderPath, path, sizeof(sLegacyLoaderPath));
}

const char* GetNdsBootstrapLegacyLoaderPath()
{
    return sLegacyLoaderPath;
}

void SetNdsBootstrapBootstrapLoaderPath(const char* path)
{
    StringUtil::Copy(sBootstrapLoaderPath, path, sizeof(sBootstrapLoaderPath));
}

const char* GetNdsBootstrapBootstrapLoaderPath()
{
    return sBootstrapLoaderPath;
}
