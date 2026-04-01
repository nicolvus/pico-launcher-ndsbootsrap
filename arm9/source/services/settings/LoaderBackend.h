#pragma once
#include "common.h"

/// @brief Policy for resolving the Chishm ARM loader binary used by the NDS-Bootstrap launch path.
///
/// The launcher supports two on-SD locations for the loader binary:
///   - Legacy  : /_pico/load.bin          (original Pico Launcher location)
///   - Bootstrap: /_nds/nds-bootstrap.nds (new, distributable-with-nds-bootstrap location)
///
/// The policy controls which path is tried first and which is used as fallback.
enum class LoaderBackend : u8
{
    /// @brief Try the Bootstrap path first; fall back to the Legacy path. (default)
    Auto = 0,
    /// @brief Try the Legacy path (/_pico/load.bin) first; fall back to Bootstrap path.
    Legacy = 1,
    /// @brief Try the Bootstrap path (/_nds/nds-bootstrap.nds) first; fall back to Legacy path.
    Bootstrap = 2,
};
