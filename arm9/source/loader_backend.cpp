#include "common.h"
#include <cstring>
#include <cstdio>
#include <strings.h>
#include "fat/ff.h"
#include "core/StringUtil.h"
#include "loader_backend.h"

namespace
{
static void EmitLog(NdsBootstrapLogFn logFn, const char* msg)
{
    if (logFn)
        logFn(msg);
}

static bool HasVolumePrefix(const char* path)
{
    if (!path)
        return false;
    return strncasecmp(path, "sd:/", 4) == 0
        || strncasecmp(path, "fat:/", 5) == 0
        || strncasecmp(path, "nand:/", 6) == 0;
}

/// Builds an absolute path from a (potentially relative) path and a volume prefix.
static void BuildLoaderAbsPath(const char* path, const char* volumePrefix, char* out, u32 outLen)
{
    if (!out || outLen == 0)
        return;
    if (!path || !path[0])
    {
        out[0] = '\0';
        return;
    }
    if (HasVolumePrefix(path))
    {
        StringUtil::Copy(out, path, outLen);
        return;
    }
    StringUtil::Copy(out, volumePrefix, outLen);
    const char* p = path;
    while (*p == '/')
        p++;
    strlcat(out, p, outLen);
}

static bool PathExists(const char* absPath)
{
    FILINFO fi;
    memset(&fi, 0, sizeof(fi));
    return f_stat(absPath, &fi) == FR_OK;
}
} // namespace

bool ResolveLoaderBinPath(
    LoaderBackend policy,
    const char* legacyOverride,
    const char* bootstrapOverride,
    const char* volumePrefix,
    char* out,
    u32 outLen,
    bool& usedFallback,
    NdsBootstrapLogFn logFn)
{
    usedFallback = false;

    const char* legacyRel    = (legacyOverride    && legacyOverride[0])    ? legacyOverride    : kDefaultLegacyLoaderPath;
    const char* bootstrapRel = (bootstrapOverride && bootstrapOverride[0]) ? bootstrapOverride : kDefaultBootstrapLoaderPath;

    // Determine search order based on policy.
    // Auto and Bootstrap both prefer the bootstrap path; Legacy prefers the legacy path.
    const char* primary;
    const char* fallback;
    switch (policy)
    {
        case LoaderBackend::Legacy:
            primary  = legacyRel;
            fallback = bootstrapRel;
            break;
        case LoaderBackend::Auto:
        case LoaderBackend::Bootstrap:
        default:
            primary  = bootstrapRel;
            fallback = legacyRel;
            break;
    }

    {
        char msg[160];
        snprintf(msg, sizeof(msg), "LoaderBackend: primary=%.60s fallback=%.60s", primary, fallback);
        EmitLog(logFn, msg);
        LOG_DEBUG("LoaderBackend: primary=%s fallback=%s\n", primary, fallback);
    }

    // Build absolute paths once so both the existence check and the error message share them.
    char primaryAbs[96];
    char fallbackAbs[96];
    BuildLoaderAbsPath(primary,  volumePrefix, primaryAbs,  sizeof(primaryAbs));
    BuildLoaderAbsPath(fallback, volumePrefix, fallbackAbs, sizeof(fallbackAbs));

    // Try primary
    if (PathExists(primaryAbs))
    {
        StringUtil::Copy(out, primaryAbs, outLen);
        {
            char msg[128];
            snprintf(msg, sizeof(msg), "Loader resolved (primary): %.96s", out);
            EmitLog(logFn, msg);
        }
        LOG_DEBUG("LoaderBackend: using primary %s\n", out);
        return true;
    }

    EmitLog(logFn, "Primary loader not found, trying fallback");
    LOG_DEBUG("LoaderBackend: primary not found (%s), trying fallback %s\n", primaryAbs, fallbackAbs);

    // Try fallback
    if (PathExists(fallbackAbs))
    {
        StringUtil::Copy(out, fallbackAbs, outLen);
        usedFallback = true;
        {
            char msg[128];
            snprintf(msg, sizeof(msg), "Loader resolved (fallback): %.96s", out);
            EmitLog(logFn, msg);
        }
        LOG_DEBUG("LoaderBackend: using fallback %s\n", out);
        return true;
    }

    // Neither path exists — emit a clear error with the exact paths the user must provide.
    {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "Loader not found. Need one of:\n  %.90s\n  %.90s",
                 primaryAbs, fallbackAbs);
        EmitLog(logFn, msg);
        LOG_FATAL("LoaderBackend: no loader found. Tried primary=%s and fallback=%s\n",
                  primaryAbs, fallbackAbs);
    }
    out[0] = '\0';
    return false;
}
