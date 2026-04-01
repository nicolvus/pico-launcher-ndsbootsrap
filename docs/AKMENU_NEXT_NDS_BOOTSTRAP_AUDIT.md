# Auditoría de lanzamiento nds-bootstrap (akmenu-next → pico-launcher)

Fecha: 2026-04-01  
Origen auditado: `coderkei/akmenu-next`  
Destino de port: `nicolvus/pico-launcher-ndsbootsrap`

## A) Archivos relevantes (ruta completa) + razón breve

### En `coderkei/akmenu-next`
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/launcher/NdsBootstrapLauncher.cpp`  
  Flujo principal de arranque nds-bootstrap (retail/homebrew), escritura de INI, selección release/nightly/hb.
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/launcher/NdsBootstrapLauncher.h`  
  Interfaz del launcher nds-bootstrap y estado usado durante lanzamiento.
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/launcher/nds_loader_arm9.h`  
  API real usada para arrancar: `runNdsFile(...)` / `runNds(...)`.
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/launcher/nds_loader_arm9.c`  
  Implementación de loader Chishm embebido (`load_bin.h`), patch DLDI, `installBootStub`, retorno `eRunNdsRetCode`.
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/romlauncher.cpp`  
  Punto de decisión del backend (`NdsBootstrapLauncher` vs `DSpicoLauncher`) y condiciones.
- `https://github.com/coderkei/akmenu-next/blob/main/arm9/source/fsmngr.cpp`  
  Resolución de volumen raíz (`fat:`/`sd:`) vía `resolveSystemPath(...)`.

### En `nicolvus/pico-launcher-ndsbootsrap` (actual)
- `/home/runner/work/pico-launcher-ndsbootsrap/pico-launcher-ndsbootsrap/arm9/source/NdsBootstrapLaunch.cpp`  
  Flujo actual: detecta volumen, escribe INI, extrae loader de `nds-bootstrap-release.nds`, intenta `runNdsWithExternalLoader`, fallback `runNdsFile`.
- `/home/runner/work/pico-launcher-ndsbootsrap/pico-launcher-ndsbootsrap/arm9/source/NdsBootstrapLaunch.h`  
  Interfaz de lanzamiento nds-bootstrap.
- `/home/runner/work/pico-launcher-ndsbootsrap/pico-launcher-ndsbootsrap/arm9/source/nds_loader/nds_loader_arm9.h`  
  API disponible en pico-launcher: `runNdsWithExternalLoader(...)`, `runNdsFile(...)`.
- `/home/runner/work/pico-launcher-ndsbootsrap/pico-launcher-ndsbootsrap/arm9/source/nds_loader/nds_loader_arm9.c`  
  Implementación actual; `runNdsFile(...)` devuelve `RUN_NDS_STAT_FAILED` (stub).

---

## B) Funciones exactas por archivo clave y qué hacen

### `akmenu-next/arm9/source/launcher/NdsBootstrapLauncher.cpp`
- `bool NdsBootstrapLauncher::prepareIni(bool hb)`
  - Escribe `/_nds/nds-bootstrap.ini`.
  - Claves observadas: `NDS_PATH`, `SAV_PATH`, `QUIT_PATH`, `CONSOLE_MODEL`, `LANGUAGE`, `DSI_MODE` (en casos), `HOTKEY`, `LOGGING`, `DEBUG`, `PHAT_COLORS`.
  - Para homebrew (`hb == true`): fuerza `DSI_MODE=0`, guarda INI y sale.
- `bool NdsBootstrapLauncher::launchRom(std::string romPath, std::string savePath, ..., bool hb)`
  - Resuelve rutas:
    - `/_nds/nds-bootstrap-release.nds`
    - `/_nds/nds-bootstrap-nightly.nds`
    - `/_nds/nds-bootstrap-hb-release.nds`
  - Asegura `/_nds/nds-bootstrap/`.
  - Limpia `/_nds/nds-bootstrap/nds-bootstrap.ini` legacy si existe.
  - Llama `prepareIni(false)` y luego `runNdsFile(argv[0], argv.size(), &argv[0])`.
- `bool launchHbStrap(std::string romPath)`
  - Homebrew strap: `runNdsFile(ndsHbBootstrapPath, argv.size(), &argv[0])`.

### `akmenu-next/arm9/source/launcher/nds_loader_arm9.h/.c`
- `eRunNdsRetCode runNdsFile(const char* filename, int argc, const char** argv);`
  - `stat(filename)` para obtener cluster (`st.st_ino`).
  - `installBootStub(havedsiSD)`.
  - Lanza con `runNds(load_bin, load_bin_size, st.st_ino, true, true, argc, argv);`
- `eRunNdsRetCode runNds(...)`
  - Copia `load_bin` a VRAM, setea parámetros (cluster, patch flags, argv).
  - Patching DLDI y soft reset.
- `bool installBootStub(bool havedsiSD)`
  - Configura bootstub + bootloader.
  - Si `havedsiSD`, desactiva DLDI patch y marca uso SD interna.

### `akmenu-next/arm9/source/romlauncher.cpp`
- `TLaunchResult launchRom(...)`
  - Selecciona `NdsBootstrapLauncher` como camino estándar cuando no corresponde Pico Loader.
  - Es el entrypoint de usuario al backend nds-bootstrap.

### `pico-launcher/arm9/source/NdsBootstrapLaunch.cpp` (actual)
- `static bool WriteBootstrapIni(...)`
  - Escribe INI con claves mínimas:
    - `NDS_PATH`, `SAV_PATH`, `QUIT_PATH`, `LANGUAGE=-1`, `DSI_MODE`, `BOOST_CPU`, `BOOST_VRAM`, `CONSOLE_MODEL=0`.
- `bool TryLaunchNdsBootstrap(NdsBootstrapLogFn logFn)`
  - Busca `/_nds/nds-bootstrap-release.nds`, obtiene cluster.
  - Extrae primeros 64KB como loader embebido y valida firma DLDI custom.
  - Intenta `runNdsWithExternalLoader(...)`.
  - Si falla, fallback `runNdsFile(...)`.

---

## C) Receta mínima de port a pico-launcher (paso 1..N)

Objetivo: replicar el camino **estable por defecto** de akmenu-next.

1. **Resolver path absoluto del bootstrap** en mismo volumen operativo:
   - `sd:/_nds/nds-bootstrap-release.nds` o `fat:/_nds/nds-bootstrap-release.nds`.
2. **Crear/actualizar INI** `/_nds/nds-bootstrap.ini` con claves mínimas:
   - `NDS_PATH=<rom abs>`
   - `SAV_PATH=<sav abs>`
   - `QUIT_PATH=<launcher abs>`
   - `LANGUAGE`, `DSI_MODE`, `BOOST_CPU`, `BOOST_VRAM`, `CONSOLE_MODEL`.
3. **Asegurar directorio** `/_nds/nds-bootstrap/`.
4. **Lanzar directo con runNdsFile (camino estable akmenu-next)**:
   ```cpp
   std::vector<const char*> argv;
   argv.push_back(ndsBootstrapPath.c_str());
   eRunNdsRetCode rc = runNdsFile(argv[0], argv.size(), &argv[0]);
   ```
5. **Manejo de retorno/log**:
   - `RUN_NDS_OK` => éxito.
   - Cualquier otro => log detallado + error al usuario.
6. **Opcional homebrew separado** (igual que akmenu-next):
   - Ejecutar `/_nds/nds-bootstrap-hb-release.nds` con `runNdsFile(...)`.

### Llamadas exactas y parámetros (akmenu-next default)
```cpp
// retail/nightly
eRunNdsRetCode rc = runNdsFile(argv[0], argv.size(), &argv[0]);

// hb
eRunNdsRetCode rc = runNdsFile(ndsHbBootstrapPath.c_str(), argv.size(), &argv[0]);
```

**Default estable en akmenu-next:** `runNdsFile(...)` con `load_bin` compilado (no external loader runtime).

---

## D) Diferencias críticas que explican fallos actuales

1. **`rc=1` en `runNdsFile` (pico-launcher)**
   - En pico-launcher actual: `runNdsFile(...)` está stub y retorna `RUN_NDS_STAT_FAILED` siempre.
   - En akmenu-next: `runNdsFile(...)` está implementado y lanza realmente con `load_bin`.
   - Efecto: fallback directo en pico-launcher falla siempre con `rc=1`.

2. **“loader inválido”**
   - Pico-launcher extrae 64KB desde `nds-bootstrap-release.nds` y valida magia DLDI (`0xEE A5 8D BF " Chishm"`).
   - Si la release no contiene ese bloque en el offset esperado, se marca inválido.
   - Akmenu-next evita este riesgo: usa `load_bin` embebido en build, no extracción dinámica.

3. **Pantalla negra**
   - Típicamente tras salto de loader con estado/patch no consistente.
   - Diferencia clave: pico-launcher depende de path externo + loader extraído + `runNdsWithExternalLoader`; akmenu-next usa camino único `runNdsFile` probado con `load_bin`.
   - En pico, si external “parece válido” pero no corresponde al formato esperado por runtime, puede resetear y quedar en negro.

---

## E) Recomendación final

1. **Método a copiar tal cual (recomendado):**
   - Camino **directo** estilo akmenu-next: `runNdsFile(...)` con implementación real (no stub).
2. **Qué desactivar/eliminar en pico-launcher:**
   - Desactivar por defecto el camino `runNdsWithExternalLoader(...)` basado en extracción de 64KB.
   - No depender de validación runtime de “embedded loader” para el flujo principal.
3. **Estrategia segura:**
   - Mantener external sólo como experimental/feature flag.
   - Camino principal de producción = directo `runNdsFile`.

---

## F) Snippets concretos (launch + escritura ini)

### 1) Escritura INI (pico-launcher actual, `WriteBootstrapIni`)
```cpp
const int n = snprintf(body, sizeof(body),
                       "[NDS-BOOTSTRAP]\n"
                       "NDS_PATH=%s\n"
                       "SAV_PATH=%s\n"
                       "QUIT_PATH=%s\n"
                       "LANGUAGE=-1\n"
                       "DSI_MODE=%d\n"
                       "BOOST_CPU=%d\n"
                       "BOOST_VRAM=%d\n"
                       "CONSOLE_MODEL=0\n",
                       ndsPath, savPath, quitPath, dsiModeIni, boostCpu ? 1 : 0, boostVram ? 1 : 0);
```

### 2) Launch actual pico (external + fallback)
```cpp
eRunNdsRetCode rc = runNdsWithExternalLoader(
    sLoaderBin, loadSize, finfoBootstrap.fclust, ndsBootstrapExePath,
    1, argv, isRunFromSd, dsModeSwitch, kBoostCpu, kBoostVram, -1);

eRunNdsRetCode rcDirect = runNdsFile(ndsBootstrapExePath, 1, argv);
```

### 3) Launch estable en akmenu-next (directo)
```cpp
std::vector<const char*> argv;
argv.push_back(ndsBootstrapPath.c_str());
eRunNdsRetCode rc = runNdsFile(argv[0], argv.size(), &argv[0]);
if (rc == RUN_NDS_OK) return true;
```

### 4) Implementación real akmenu-next de `runNdsFile` (resumen)
```c
if (stat(filename, &st) < 0) return RUN_NDS_STAT_FAILED;
installBootStub(havedsiSD);
return runNds(load_bin, load_bin_size, st.st_ino, true, true, argc, argv);
```

---

## Nota sobre caminos alternativos en akmenu-next
- Hay variantes `release`, `nightly` y `hb-release`.
- El **camino estable por defecto** para retail es `/_nds/nds-bootstrap-release.nds` + `runNdsFile(...)`.
- `nightly` depende de setting explícito del usuario.
