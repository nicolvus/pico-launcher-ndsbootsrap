# Changelog

## [Unreleased]

### Changed
- nds-bootstrap launcher now extracts the loader directly from `/_nds/nds-bootstrap-release.nds`
- Removed dependency on external `load.bin` file
- Removed loader backend configuration options (`loaderBackend`, `legacyLoaderPath`, `bootstrapLoaderPath`)
- Simplified volume detection to use `/_nds/nds-bootstrap-release.nds` as reference

### Removed
- Support for `/_pico/load.bin` and `/_nds/nds-bootstrap.nds` as separate loader binaries
- Loader backend resolution system

## [v1.2.0] - 29 Mar 2026

### Added
- Support for cheats with Pico Loader API v3
- Hide files/dirs with hidden attribute, or with a name starting with a period
- New customization options for custom themes
    - Position of elements on the top screen
    - Text colors
    - Blend colors

### Changed
- File name on the top screen now uses marquee when too long

### Fixed
- Improve error handling for banners to better detect if a rom has a valid banner

## [v1.1.0] - 11 Jan 2026

### Added
- Support for Pico Loader API v2. This makes it possible to return to Pico Launcher from supported homebrew applications.

## [v1.0.0] - 25 Nov 2025
- Initial release