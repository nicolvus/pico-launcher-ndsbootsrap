#pragma once
#include "common.h"

/// @brief Which backend is used to launch Nintendo DS ROMs (.nds).
enum class NdsLoaderKind : u8
{
    /// @brief Pico Loader (pico-loader binaries under /_pico/).
    PicoLoader = 0,
    /// @brief nds-bootstrap (/_nds/nds-bootstrap-release.nds and Chishm loader in /_pico/load.bin).
    NdsBootstrap = 1,
};
