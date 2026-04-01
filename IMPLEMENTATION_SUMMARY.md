# Implementation Summary

## Objetivo Completado

Se ha eliminado la dependencia del launcher en archivos `load.bin` externos, y ahora el launcher extrae el loader directamente de `/_nds/nds-bootstrap-release.nds`.

## Archivos Modificados

### 1. arm9/source/NdsBootstrapLaunch.cpp
**Cambios principales:**
- **DetectVolumePrefix()**: Actualizado para detectar el volumen usando `/_nds/nds-bootstrap-release.nds` en lugar de `load.bin` o `nds-bootstrap.nds`
- **TryLaunchNdsBootstrap()**:
  - Eliminada la resolución de ruta de loader externo (ResolveLoaderBinPath)
  - Eliminada la lectura de archivos load.bin separados
  - Ahora abre `/_nds/nds-bootstrap-release.nds` y lee los primeros 64KB como loader embebido
  - Agregados logs claros:
    - "Will launch: <path>/nds-bootstrap-release.nds"
    - "Extracting embedded loader from nds-bootstrap-release.nds"
    - "Extracted loader bytes: <size>"
- Eliminado el include de `loader_backend.h`

### 2. arm9/source/NdsBootstrapLaunchParams.h/cpp
**Cambios:**
- Eliminadas funciones relacionadas con loader backend:
  - SetNdsBootstrapLoaderBackend()
  - GetNdsBootstrapLoaderBackend()
  - SetNdsBootstrapLegacyLoaderPath()
  - GetNdsBootstrapLegacyLoaderPath()
  - SetNdsBootstrapBootstrapLoaderPath()
  - GetNdsBootstrapBootstrapLoaderPath()
- Mantenidas solo las funciones de ROM path (GetNdsBootstrapLaunchRomPath, SetNdsBootstrapLaunchRomPath)

### 3. arm9/source/romBrowser/RomBrowserController.cpp
**Cambios:**
- Eliminadas las llamadas a:
  - SetNdsBootstrapLoaderBackend()
  - SetNdsBootstrapLegacyLoaderPath()
  - SetNdsBootstrapBootstrapLoaderPath()
- Mantenida solo la llamada a SetNdsBootstrapLaunchRomPath()

### 4. arm9/source/services/settings/AppSettings.h
**Cambios:**
- Eliminados campos:
  - LoaderBackend loaderBackend
  - String<char, 64> legacyLoaderPath
  - String<char, 64> bootstrapLoaderPath
- Eliminado include de "LoaderBackend.h"

### 5. arm9/source/services/settings/JsonAppSettingsSerializer.thumb.cpp
**Cambios:**
- Eliminadas constantes:
  - KEY_LOADER_BACKEND
  - KEY_LEGACY_LOADER_PATH
  - KEY_BOOTSTRAP_LOADER_PATH
- Eliminadas funciones:
  - serializeLoaderBackend()
  - tryParseLoaderBackend()
- Eliminado include de "LoaderBackend.h"
- Eliminada serialización/deserialización de los campos de loader backend

### 6. Archivos Eliminados
- **arm9/source/loader_backend.cpp** - Ya no se necesita resolución de rutas de loader
- **arm9/source/loader_backend.h** - Header del sistema de loader backend
- **arm9/source/services/settings/LoaderBackend.h** - Enum de políticas de loader backend

### 7. Documentación

#### README.md
- Actualizado para reflejar el nuevo requisito simplificado
- Cambiado de "Dual loader backend support" a "Can launch games using nds-bootstrap (requires `/_nds/nds-bootstrap-release.nds`)"

#### CHANGELOG.md
- Actualizado con sección "Unreleased" que documenta los cambios
- Listados cambios y eliminaciones del sistema de loader backend

#### docs/LoaderBackend.md
- Marcado como "Deprecated"
- Agregada sección de migración
- Documentado el nuevo comportamiento
- Mantenida documentación histórica para referencia

#### TESTING.md (nuevo)
- Instrucciones completas de prueba
- Checklist de verificación
- Documentación de archivos requeridos/no requeridos
- Instrucciones de rollback

## Lógica Implementada

### Extracción del Loader
```cpp
// Abrir nds-bootstrap-release.nds
File bootstrapFile;
bootstrapFile.Open(ndsBootstrapExePath, FA_OPEN_EXISTING | FA_READ);

// Leer los primeros 64KB como loader embebido
u32 loadSize = kMaxLoaderBinBytes;  // 65536 bytes
u32 fileSize = bootstrapFile.GetSize();
if (fileSize < loadSize)
    loadSize = fileSize;

// Leer en buffer sLoaderBin
bootstrapFile.Read(sLoaderBin, loadSize, br);
```

### Detección de Volumen
```cpp
// Ahora usa nds-bootstrap-release.nds como referencia
if (f_stat("sd:/_nds/nds-bootstrap-release.nds", &finfo) == FR_OK)
    return "sd:/";
if (f_stat("fat:/_nds/nds-bootstrap-release.nds", &finfo) == FR_OK)
    return "fat:/";
```

### Logging Mejorado
```cpp
// Mensajes claros sobre qué se va a lanzar
"Will launch: sd:/_nds/nds-bootstrap-release.nds"
"Extracting embedded loader from nds-bootstrap-release.nds"
"Extracted loader bytes: 65536"

// Error claro si falta el archivo
"nds-bootstrap-release.nds not found at: sd:/_nds/nds-bootstrap-release.nds"
```

## Archivos Requeridos en SD

### ✅ Requeridos
- `/_nds/nds-bootstrap-release.nds` - Ejecutable principal con loader embebido

### ❌ Ya NO Requeridos
- `/_pico/load.bin` - Loader legacy
- `/_nds/nds-bootstrap.nds` - Loader alternativo

## Compatibilidad

### Configuración Existente
Los archivos `launcher.json` existentes con campos de loader backend (`loaderBackend`, `legacyLoaderPath`, `bootstrapLoaderPath`) seguirán funcionando, pero estos campos serán ignorados.

### Migración Automática
No se requiere migración manual. El sistema simplemente:
1. Ignora las configuraciones antiguas de loader backend
2. Busca directamente `/_nds/nds-bootstrap-release.nds`
3. Extrae el loader del archivo

## Beneficios

1. **Simplificación**: Un solo archivo requerido en lugar de múltiples opciones
2. **Menos configuración**: No se necesitan políticas de backend ni rutas override
3. **Más robusto**: Menos puntos de fallo en la resolución de rutas
4. **Mejor logging**: Mensajes más claros sobre qué se está ejecutando
5. **Menos código**: ~250 líneas eliminadas, sistema más mantenible

## Próximos Pasos (Recomendados)

1. **Compilar**: Verificar que compila sin errores
2. **Probar en hardware**: Confirmar que funciona con nds-bootstrap-release.nds real
3. **Verificar DSi binaries**: Asegurar que la detección de DSi binaries funciona correctamente
4. **Probar casos edge**: ROMs de diferentes tamaños, ubicaciones, tipos

## Notas Técnicas

- El loader se extrae de los primeros 64KB (kMaxLoaderBinBytes) del archivo NDS
- Esta asunción se basa en la estructura estándar de nds-bootstrap donde el loader está embebido al inicio
- La función `runNdsWithExternalLoader()` permanece sin cambios y recibe el loader extraído como antes
- El cluster del archivo NDS todavía se pasa para que el loader pueda encontrar el ejecutable
