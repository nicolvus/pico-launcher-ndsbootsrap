# Loader Backend — Deprecated

## Notice

**This document describes functionality that has been removed as of the latest version.**

The dual loader backend system has been replaced with a simpler approach that extracts the loader directly from `/_nds/nds-bootstrap-release.nds`.

## Current Behavior

Pico Launcher now:
- Reads the embedded loader from the beginning of `/_nds/nds-bootstrap-release.nds`
- No longer requires separate `load.bin` files
- Automatically detects the boot volume by checking for `/_nds/nds-bootstrap-release.nds`

## Required Files

To use nds-bootstrap with Pico Launcher, you only need:
- `/_nds/nds-bootstrap-release.nds` - The nds-bootstrap executable with embedded loader

## Migration from Old Setup

If you were previously using:
- `/_pico/load.bin` - No longer needed, can be deleted
- `/_nds/nds-bootstrap.nds` (as loader) - No longer needed, can be deleted

Settings in `/_pico/launcher.json`:
- `loaderBackend` - No longer used, will be ignored
- `legacyLoaderPath` - No longer used, will be ignored
- `bootstrapLoaderPath` - No longer used, will be ignored

These settings can be safely removed from your configuration file, though they will be ignored if present.

---

## Historical Documentation (Deprecated)

The following describes the old system for reference only:

### Overview

Pico Launcher previously supported two on-SD locations for the Chishm ARM loader binary:

| Backend | Default path | Notes |
|---------|-------------|-------|
| **Legacy** | `/_pico/load.bin` | Original Pico Launcher location |
| **Bootstrap** | `/_nds/nds-bootstrap.nds` | Bundled with older nds-bootstrap releases |

This system has been removed in favor of extracting the loader directly from the main nds-bootstrap executable.
