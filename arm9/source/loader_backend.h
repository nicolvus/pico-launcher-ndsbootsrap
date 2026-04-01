#pragma once
#include "common.h"
#include "services/settings/LoaderBackend.h"
#include "NdsBootstrapLaunch.h"

/// Default SD-relative path for the legacy (original Pico Launcher) loader binary.
static constexpr const char* kDefaultLegacyLoaderPath = "/_pico/load.bin";

/// Default SD-relative path for the bootstrap-bundled loader binary.
static constexpr const char* kDefaultBootstrapLoaderPath = "/_nds/nds-bootstrap.nds";

/// @brief Resolves the absolute path to the Chishm ARM loader binary according to the given policy.
///
/// Checks file existence on the mounted FAT volume and returns the first available path.
/// If the primary path is not found the fallback is tried. If neither exists, the function
/// returns false, writes an error via logFn, and clears @p out.
///
/// @param policy            Selection policy (Auto / Legacy / Bootstrap).
/// @param legacyOverride    Override for the legacy path, or nullptr / "" to use kDefaultLegacyLoaderPath.
/// @param bootstrapOverride Override for the bootstrap path, or nullptr / "" to use kDefaultBootstrapLoaderPath.
/// @param volumePrefix      Volume prefix for relative paths (e.g. "sd:/").
/// @param out               Output buffer that receives the resolved absolute path.
/// @param outLen            Size of @p out in bytes.
/// @param usedFallback      Set to true when the fallback path was chosen instead of the primary.
/// @param logFn             Optional per-step log callback (may be nullptr).
/// @return true if a loader path was resolved and written to @p out; false if no loader was found.
bool ResolveLoaderBinPath(
    LoaderBackend policy,
    const char* legacyOverride,
    const char* bootstrapOverride,
    const char* volumePrefix,
    char* out,
    u32 outLen,
    bool& usedFallback,
    NdsBootstrapLogFn logFn = nullptr);
