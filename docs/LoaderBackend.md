# Loader Backend — Migration Notes

## Overview

Pico Launcher now supports two on-SD locations for the Chishm ARM loader binary that is
used when launching NDS ROMs with the **nds-bootstrap** backend:

| Backend | Default path | Notes |
|---------|-------------|-------|
| **Legacy** | `/_pico/load.bin` | Original Pico Launcher location |
| **Bootstrap** | `/_nds/nds-bootstrap.nds` | Bundled with newer nds-bootstrap releases |

A configurable *policy* decides which path is tried first and which is used as fallback.

---

## New configuration keys (`/_pico/launcher.json`)

```json
{
  "loaderBackend":        "auto",
  "legacyLoaderPath":     "",
  "bootstrapLoaderPath":  ""
}
```

### `loaderBackend`

| Value | Primary | Fallback |
|-------|---------|---------|
| `"auto"` *(default)* | `/_nds/nds-bootstrap.nds` | `/_pico/load.bin` |
| `"bootstrap"` | `/_nds/nds-bootstrap.nds` | `/_pico/load.bin` |
| `"legacy"` | `/_pico/load.bin` | `/_nds/nds-bootstrap.nds` |

`auto` and `bootstrap` are equivalent: the bootstrap path is preferred.

### `legacyLoaderPath` / `bootstrapLoaderPath`

Optional path overrides for the two loader locations.  
Leave empty (default) to use the built-in defaults shown in the table above.  
Volume prefixes (`sd:/`, `fat:/`) are accepted; relative paths are resolved against
the detected boot volume.

Example (custom locations):
```json
{
  "loaderBackend":        "legacy",
  "legacyLoaderPath":     "/_homebrew/loader.bin",
  "bootstrapLoaderPath":  ""
}
```

---

## Fallback behaviour

1. The launcher determines the primary path from the configured policy.
2. If the primary file **does not exist** on the SD card, the other path is tried automatically.
3. If **neither** file exists the launch is aborted and the exact paths that were checked
   are printed to the debug console and to `/_pico/launch_log.txt`.
4. If the fallback was used, a notice is printed in the debug log.

---

## Backward compatibility

Installations that already have `/_pico/load.bin` continue to work without any
configuration change.  The default policy (`auto`) will try
`/_nds/nds-bootstrap.nds` first; if that file is absent it transparently falls
back to `/_pico/load.bin`.

To restore the exact pre-migration behaviour (legacy path only, no fallback to
bootstrap), set:
```json
{ "loaderBackend": "legacy" }
```

---

## Manual test checklist

| Scenario | Expected result |
|----------|----------------|
| Only `/_pico/load.bin` present, `loaderBackend = "auto"` | Fallback used; launch succeeds |
| Only `/_nds/nds-bootstrap.nds` present, `loaderBackend = "auto"` | Primary used; launch succeeds |
| Both files present, `loaderBackend = "auto"` | Primary (`/_nds/nds-bootstrap.nds`) used |
| Both files present, `loaderBackend = "legacy"` | Primary (`/_pico/load.bin`) used |
| Neither file present | Launch aborts; exact paths printed to debug console |
| Corrupted primary file (e.g. 0-byte), fallback is valid | Launch fails on corrupt file read; error logged |
| `legacyLoaderPath` set to a custom path | Custom path used as legacy candidate |
