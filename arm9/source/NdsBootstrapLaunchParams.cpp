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
