#pragma once

using NdsBootstrapLogFn = void (*)(const char*);

/// @brief Prepares nds-bootstrap.ini, resolves the loader binary via the configured backend policy,
///        and runs the Chishm loader to start nds-bootstrap.
/// @param logFn Optional callback to receive per-step debug messages during launch.
/// @return false if files are missing or I/O fails (caller may show a message). Does not return if launch succeeds.
bool TryLaunchNdsBootstrap(NdsBootstrapLogFn logFn = nullptr);
