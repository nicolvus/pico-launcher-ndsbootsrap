# Testing Instructions

## Changes Summary

This update removes the dependency on external `load.bin` files and launches nds-bootstrap directly by extracting the embedded loader from `nds-bootstrap-release.nds`.

### Modified Files

1. **arm9/source/NdsBootstrapLaunch.cpp**
   - Removed dependency on external load.bin resolution
   - Now reads the loader directly from the beginning of `/_nds/nds-bootstrap-release.nds`
   - Updated `DetectVolumePrefix()` to check for `nds-bootstrap-release.nds` instead of legacy loader paths
   - Added clearer logging messages showing which executable will be launched

2. **arm9/source/NdsBootstrapLaunchParams.h/cpp**
   - Removed loader backend configuration functions
   - Kept only ROM path getter/setter

3. **arm9/source/romBrowser/RomBrowserController.cpp**
   - Removed calls to set loader backend settings

4. **arm9/source/services/settings/AppSettings.h**
   - Removed `loaderBackend`, `legacyLoaderPath`, and `bootstrapLoaderPath` settings

5. **arm9/source/services/settings/JsonAppSettingsSerializer.thumb.cpp**
   - Removed serialization/deserialization of loader backend settings

6. **Removed files:**
   - `arm9/source/loader_backend.cpp`
   - `arm9/source/loader_backend.h`
   - `arm9/source/services/settings/LoaderBackend.h`

7. **Documentation:**
   - Updated `README.md` to reflect simplified nds-bootstrap requirements
   - Updated `CHANGELOG.md` with changes

## Required Files on SD Card

### For nds-bootstrap launcher to work:
- **Required:** `/_nds/nds-bootstrap-release.nds` - The main nds-bootstrap executable (contains embedded loader)
- **Optional:** `/_nds/nds-bootstrap-hb-release.nds` - Homebrew version (if already supported by existing logic)

### No longer required:
- ❌ `/_pico/load.bin` - No longer needed
- ❌ `/_nds/nds-bootstrap.nds` - No longer needed as a separate loader file

## Testing Steps

### 1. Basic Launch Test
1. Place `/_nds/nds-bootstrap-release.nds` on your SD card
2. Place a commercial NDS ROM in any directory on the SD card
3. Launch Pico Launcher
4. Navigate to the ROM and select it
5. Choose to launch with nds-bootstrap
6. **Expected:** The ROM should launch successfully via nds-bootstrap

### 2. Volume Detection Test
1. Ensure `/_nds/nds-bootstrap-release.nds` exists on SD card
2. Launch Pico Launcher
3. Check debug logs for volume detection
4. **Expected:** Log should show volume detected as `sd:/` or `fat:/` based on `nds-bootstrap-release.nds` presence

### 3. Missing File Error Test
1. Remove `/_nds/nds-bootstrap-release.nds` from SD card
2. Try to launch a ROM with nds-bootstrap
3. **Expected:** Clear error message showing the exact path where `nds-bootstrap-release.nds` is expected

### 4. Launch Logging Test
1. Enable debug logging if available
2. Launch a ROM with nds-bootstrap
3. **Expected logs should include:**
   - `"Will launch: <path>/nds-bootstrap-release.nds"`
   - `"Checking nds-bootstrap-release.nds"`
   - `"Extracting embedded loader from nds-bootstrap-release.nds"`
   - `"Extracted loader bytes: <size>"`
   - `"Calling runNdsWithExternalLoader..."`

### 5. DSi Binary Detection Test
1. Test with a ROM that has DSi binaries
2. Test with a ROM that doesn't have DSi binaries
3. **Expected:** Correct DSI_MODE value in generated `/_nds/nds-bootstrap.ini`

### 6. Settings Migration Test
1. If you have an existing `/_pico/launcher.json` with old loader backend settings
2. Launch Pico Launcher
3. **Expected:** Old settings (`loaderBackend`, `legacyLoaderPath`, `bootstrapLoaderPath`) should be ignored without errors

## Verification Checklist

- [ ] Pico Launcher compiles without errors
- [ ] ROM launches successfully via nds-bootstrap with only `/_nds/nds-bootstrap-release.nds` present
- [ ] No references to `/_pico/load.bin` in code
- [ ] Volume detection works correctly
- [ ] Error messages are clear when `nds-bootstrap-release.nds` is missing
- [ ] Debug logs show correct file paths and operations
- [ ] Settings JSON is generated correctly without loader backend fields
- [ ] Existing functionality (Pico Loader backend) still works

## Known Limitations

- The embedded loader is read from the first 64KB of `nds-bootstrap-release.nds`
- This assumes the loader is embedded at the beginning of the NDS file (standard nds-bootstrap structure)

## Rollback Instructions

If issues are found:
1. Restore the removed loader backend files from git history
2. Restore the loader backend settings in AppSettings
3. Revert changes to NdsBootstrapLaunch.cpp
