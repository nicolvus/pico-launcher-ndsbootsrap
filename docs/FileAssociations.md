# File Associations
Pico Launcher supports custom file associations for file extensions other than `.nds`.
By setting up a file association, files with the specified extension will be opened using the specified DS application.
The file path is passed via argv.

## How to setup file associations
1. Open `/_pico/settings.json`.
2. If not present yet, add a `fileAssociations` key.
3. For each file association add a key inside the `fileAssociations` key. For example:
    ```json
    "fileAssociations": {
      "xyz": {
        "appPath": "/path/to/xyz/app.nds"
      },
      "gba": {
        "appPath": "/myGbaApp.nds"
      }
    }
    ```
    With this setup `.xyz` files will be opened by `/path/to/xyz/app.nds` and `.gba` files will be opened by `/myGbaApp.nds`.
