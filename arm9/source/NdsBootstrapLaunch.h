#pragma once

using NdsBootstrapLogFn = void (*)(const char*);

/// @brief Prepares nds-bootstrap.ini, then runs Chishm loader from /_pico/load.bin to start nds-bootstrap.
/// @param logFn Optional callback to receive per-step debug messages during launch.
/// @return false if files are missing or I/O fails (caller may show a message). Does not return if launch succeeds.
bool TryLaunchNdsBootstrap(NdsBootstrapLogFn logFn = nullptr);
