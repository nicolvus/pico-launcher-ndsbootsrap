#include "common.h"
#include <cstdio>
#include <cstring>
#include <strings.h>
#include "core/StringUtil.h"
#include "NdsBootstrapLaunch.h"
#include "NdsBootstrapLaunchParams.h"
#include "core/Environment.h"
#include "fat/ff.h"
#include "fat/File.h"
#include "nds_loader/nds_loader_arm9.h"
#include <cstdlib>

namespace
{
static constexpr const char* kNdsBootstrapExe = "/_nds/nds-bootstrap-release.nds";
static constexpr const char* kBootstrapIniPath = "/_nds/nds-bootstrap.ini";
static constexpr const char* kQuitPath = "/_pico/LAUNCHER.nds";
static constexpr const char* kExternalLoaderSdPath = "sd:/_pico/load.bin";
static constexpr const char* kExternalLoaderFatPath = "fat:/_pico/load.bin";
static constexpr u32 kMaxLoaderBinBytes = 65536;

alignas(32) static u8 sLoaderBin[kMaxLoaderBinBytes];

static void EmitLog(NdsBootstrapLogFn logFn, const char* msg)
{
    if (logFn)
        logFn(msg);
}

static bool HasVolumePrefix(const char* path)
{
    if (!path)
        return false;

    return strncasecmp(path, "sd:/", 4) == 0
        || strncasecmp(path, "fat:/", 5) == 0
        || strncasecmp(path, "nand:/", 6) == 0;
}

static const char* DetectVolumePrefix()
{
    FILINFO finfo;
    memset(&finfo, 0, sizeof(finfo));

    // Check for nds-bootstrap-release.nds to identify the active boot drive.
    if (f_stat("sd:/_nds/nds-bootstrap-release.nds", &finfo) == FR_OK)
        return "sd:/";
    if (f_stat("fat:/_nds/nds-bootstrap-release.nds", &finfo) == FR_OK)
        return "fat:/";
    if (f_stat("sd:/", &finfo) == FR_OK)
        return "sd:/";
    return "fat:/";
}

static const char* GetVolumePrefixForPath(const char* path)
{
    if (path && strncasecmp(path, "sd:/", 4) == 0)
        return "sd:/";
    if (path && strncasecmp(path, "fat:/", 5) == 0)
        return "fat:/";
    return DetectVolumePrefix();
}

static void BuildAbsolutePath(const char* path, const char* volumePrefix, char* out, u32 outLen)
{
    if (!out || outLen == 0)
        return;

    if (!path)
    {
        out[0] = '\0';
        return;
    }

    if (HasVolumePrefix(path))
    {
        StringUtil::Copy(out, path, outLen);
        return;
    }

    StringUtil::Copy(out, volumePrefix, outLen);
    while (*path == '/')
        path++;
    strlcat(out, path, outLen);
}

static void BuildSavPath(const char* romPath, char* out, u32 outLen)
{
    StringUtil::Copy(out, romPath, outLen);
    char* dot = strrchr(out, '.');
    if (dot && strcasecmp(dot, ".nds") == 0)
        StringUtil::Copy(dot, ".sav", outLen - (u32)(dot - out));
    else
        strlcat(out, ".sav", outLen);
}

/// TWiLight `checkDsiBinaries` equivalent — when false, nds-bootstrap must run in DS mode.
static bool RomHasDsiBinaries(const char* romPathAbs)
{
    File file;
    if (file.Open(romPathAbs, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        return false;

    u8 unitCode = 0;
    if (file.Seek(0x12) != FR_OK || !file.ReadExact(&unitCode, 1))
        return false;
    if (unitCode == 0)
        return true;

    u32 arm9irom = 0;
    u32 arm7irom = 0;
    if (file.Seek(0x1C0) != FR_OK || !file.ReadExact(&arm9irom, 4))
        return false;
    if (file.Seek(0x1D0) != FR_OK || !file.ReadExact(&arm7irom, 4))
        return false;

    if (arm9irom < 0x8000 || arm9irom >= 0x20000000 || arm7irom < 0x8000 || arm7irom >= 0x20000000)
        return false;

    u32 arm9Sig[3][4];
    if (file.Seek(0x8000) != FR_OK || !file.ReadExact(arm9Sig[0], sizeof(arm9Sig[0])))
        return false;
    if (file.Seek(arm9irom) != FR_OK || !file.ReadExact(arm9Sig[1], sizeof(arm9Sig[1])))
        return false;
    if (file.Seek(arm7irom) != FR_OK || !file.ReadExact(arm9Sig[2], sizeof(arm9Sig[2])))
        return false;

    for (int i = 1; i < 3; i++)
    {
        if (arm9Sig[i][0] == arm9Sig[0][0] && arm9Sig[i][1] == arm9Sig[0][1] && arm9Sig[i][2] == arm9Sig[0][2]
            && arm9Sig[i][3] == arm9Sig[0][3])
            return false;
        if (arm9Sig[i][0] == 0 && arm9Sig[i][1] == 0 && arm9Sig[i][2] == 0 && arm9Sig[i][3] == 0)
            return false;
        if (arm9Sig[i][0] == 0xFFFFFFFF && arm9Sig[i][1] == 0xFFFFFFFF && arm9Sig[i][2] == 0xFFFFFFFF
            && arm9Sig[i][3] == 0xFFFFFFFF)
            return false;
    }
    return true;
}

static bool WriteBootstrapIni(NdsBootstrapLogFn logFn, const char* ndsPath, const char* savPath, int dsiModeIni,
                              bool boostCpu, bool boostVram)
{
    const char* volumePrefix = GetVolumePrefixForPath(ndsPath);
    char bootstrapIniPath[96];
    char quitPath[96];
    BuildAbsolutePath(kBootstrapIniPath, volumePrefix, bootstrapIniPath, sizeof(bootstrapIniPath));
    BuildAbsolutePath(kQuitPath, volumePrefix, quitPath, sizeof(quitPath));

    char body[768];
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
    if (n <= 0 || (u32)n >= sizeof(body))
    {
        EmitLog(logFn, "INI build overflow");
        LOG_FATAL("nds-bootstrap ini buffer overflow\n");
        return false;
    }

    File file;
    if (file.Open(bootstrapIniPath, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        EmitLog(logFn, "Cannot open nds-bootstrap.ini");
        LOG_FATAL("Could not write %s\n", bootstrapIniPath);
        return false;
    }
    u32 written = 0;
    u32 len = (u32)strlen(body);
    if (file.Write(body, len, written) != FR_OK || written != len)
    {
        EmitLog(logFn, "Failed to write nds-bootstrap.ini");
        LOG_FATAL("nds-bootstrap ini write failed\n");
        return false;
    }

    EmitLog(logFn, "nds-bootstrap.ini written");
    return true;
}

static bool IsExternalLoaderEnabled()
{
    // External loader must be opt-in; default is enabled, but can be disabled via environment.
    const char* env = std::getenv("PICO_EXTERNAL_LOADER");
    if (!env || env[0] == '\0')
        return true;
    if (strcasecmp(env, "1") == 0 || strcasecmp(env, "true") == 0 || strcasecmp(env, "yes") == 0
        || strcasecmp(env, "on") == 0)
        return true;
    return false;
}

static bool LoaderContainsDldiMagic(const u8* data, u32 size)
{
    static constexpr u8 kMagic[] = { 0xEE, 0xA5, 0x8D, 0xBF, ' ', 'C', 'h', 'i', 's', 'h', 'm' };
    if (!data || size < sizeof(kMagic))
        return false;

    for (u32 i = 0; i + sizeof(kMagic) <= size; ++i)
    {
        if (memcmp(data + i, kMagic, sizeof(kMagic)) == 0)
            return true;
    }
    return false;
}

static bool IsLoaderValid(const u8* data, u32 size)
{
    if (!data || size < 0x4000 || size > kMaxLoaderBinBytes)
        return false;

    bool anyNonZero = false;
    bool anyNonFF = false;
    const u32 sample = (size > 1024) ? 1024 : size;
    for (u32 i = 0; i < sample; ++i)
    {
        anyNonZero |= (data[i] != 0);
        anyNonFF |= (data[i] != 0xFF);
    }

    if (!anyNonZero || !anyNonFF)
        return false;

    return LoaderContainsDldiMagic(data, size);
}

static bool ReadLoaderFromFile(const char* path, u8* outData, u32 maxSize, u32& outSize)
{
    outSize = 0;
    if (!path || !outData || maxSize == 0)
        return false;

    File file;
    if (file.Open(path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        return false;

    u32 size = (u32)file.GetSize();
    if (size > maxSize)
        size = maxSize;

    u32 read = 0;
    if (file.Read(outData, size, read) != FR_OK || read != size)
        return false;

    outSize = size;
    return true;
}
} // namespace

bool TryLaunchNdsBootstrap(NdsBootstrapLogFn logFn)
{
    EmitLog(logFn, "TryLaunchNdsBootstrap enter");

    const char* romPath = GetNdsBootstrapLaunchRomPath();
    if (strlen(romPath) == 0)
    {
        EmitLog(logFn, "ROM path is empty");
        LOG_FATAL("NDS ROM path not set.\n");
        return false;
    }

    {
        char msg[200];
        snprintf(msg, sizeof(msg), "ROM selected: %.160s", romPath);
        EmitLog(logFn, msg);
    }

    const char* volumePrefix = GetVolumePrefixForPath(romPath);
    {
        char msg[64];
        snprintf(msg, sizeof(msg), "Volume prefix: %s", volumePrefix);
        EmitLog(logFn, msg);
    }

    char romPathAbs[288];
    char ndsBootstrapExePath[96];
    char ndsBootstrapDirPath[96];

    BuildAbsolutePath(romPath, volumePrefix, romPathAbs, sizeof(romPathAbs));
    BuildAbsolutePath(kNdsBootstrapExe, volumePrefix, ndsBootstrapExePath, sizeof(ndsBootstrapExePath));
    BuildAbsolutePath("/_nds/nds-bootstrap", volumePrefix, ndsBootstrapDirPath, sizeof(ndsBootstrapDirPath));

    {
        char msg[220];
        snprintf(msg, sizeof(msg), "ROM abs: %.180s", romPathAbs);
        EmitLog(logFn, msg);
    }

    {
        char msg[128];
        snprintf(msg, sizeof(msg), "Will launch: %.96s", ndsBootstrapExePath);
        EmitLog(logFn, msg);
    }
    EmitLog(logFn, "Checking nds-bootstrap-release.nds");

    FILINFO finfoBootstrap;
    memset(&finfoBootstrap, 0, sizeof(finfoBootstrap));
    if (f_stat(ndsBootstrapExePath, &finfoBootstrap) != FR_OK)
    {
        char msg[128];
        snprintf(msg, sizeof(msg), "nds-bootstrap-release.nds not found at: %.96s", ndsBootstrapExePath);
        EmitLog(logFn, msg);
        LOG_FATAL("nds-bootstrap not found at %s (install DS-Homebrew release build).\n", ndsBootstrapExePath);
        return false;
    }
    if (finfoBootstrap.fclust == 0)
    {
        EmitLog(logFn, "Invalid cluster for nds-bootstrap exe");
        LOG_FATAL("Invalid cluster for %s\n", ndsBootstrapExePath);
        return false;
    }

    f_mkdir(ndsBootstrapDirPath);
    EmitLog(logFn, "Ensured /_nds/nds-bootstrap dir");

    char savPath[288];
    BuildSavPath(romPathAbs, savPath, sizeof(savPath));
    EmitLog(logFn, "Save path prepared");

    const bool romDsiBinariesOk = RomHasDsiBinaries(romPathAbs);
    const int dsiModeIni = (!romDsiBinariesOk) ? 0 : (Environment::IsDsiMode() ? 1 : 0);
    // Do not NTR-switch while launching nds-bootstrap itself.
    // nds-bootstrap decides the target runtime mode from DSI_MODE in its INI.
    const bool dsModeSwitch = false;
    const bool isRunFromSd = (strncasecmp(volumePrefix, "sd:", 3) == 0);
    constexpr bool kBoostCpu = false;
    constexpr bool kBoostVram = false;

    {
        char msg[96];
        snprintf(msg, sizeof(msg), "DSI_MODE=%d romDsi=%d", dsiModeIni, romDsiBinariesOk ? 1 : 0);
        EmitLog(logFn, msg);
    }

    if (!WriteBootstrapIni(logFn, romPathAbs, savPath, dsiModeIni, kBoostCpu, kBoostVram))
        return false;

    static char sExePathBuf[64];
    StringUtil::Copy(sExePathBuf, ndsBootstrapExePath, sizeof(sExePathBuf));
    const char* argv[] = { sExePathBuf };

    char finalFailureReason[160];
    StringUtil::Copy(finalFailureReason, "unknown", sizeof(finalFailureReason));

    if (IsExternalLoaderEnabled())
    {
        const char* externalCandidates[] = { kExternalLoaderSdPath, kExternalLoaderFatPath };
        for (u32 i = 0; i < 2; ++i)
        {
            const char* externalPath = externalCandidates[i];
            {
                char msg[160];
                snprintf(msg, sizeof(msg), "Trying external loader: %s", externalPath);
                EmitLog(logFn, msg);
            }

            u32 loadSize = 0;
            const bool externalReadOk = ReadLoaderFromFile(externalPath, sLoaderBin, sizeof(sLoaderBin), loadSize);
            const bool externalValid = externalReadOk && IsLoaderValid(sLoaderBin, loadSize);
            EmitLog(logFn, externalValid ? "External loader validation: OK" : "External loader validation: FAIL");
            if (!externalValid)
                continue;

            EmitLog(logFn, "Launch method selected: external");
            eRunNdsRetCode rc = runNdsWithExternalLoader(sLoaderBin, loadSize, finfoBootstrap.fclust, ndsBootstrapExePath,
                                                         1, argv, isRunFromSd, dsModeSwitch, kBoostCpu, kBoostVram,
                                                         -1);
            if (rc == RUN_NDS_OK)
                return true;

            {
                char msg[64];
                snprintf(msg, sizeof(msg), "runNdsFile rc=%d", (int)rc);
                EmitLog(logFn, msg);
            }
            StringUtil::Copy(finalFailureReason, "external loader launch failed", sizeof(finalFailureReason));
        }
    }
    else
    {
        StringUtil::Copy(finalFailureReason, "external loader disabled", sizeof(finalFailureReason));
    }

    {
        char msg[160];
        snprintf(msg, sizeof(msg), "Trying embedded loader from: %s", ndsBootstrapExePath);
        EmitLog(logFn, msg);
    }

    u32 embeddedSize = 0;
    const bool embeddedReadOk = ReadLoaderFromFile(ndsBootstrapExePath, sLoaderBin, sizeof(sLoaderBin), embeddedSize);
    const bool embeddedValid = embeddedReadOk && IsLoaderValid(sLoaderBin, embeddedSize);
    EmitLog(logFn, embeddedValid ? "Embedded loader validation: OK" : "Embedded loader validation: FAIL");
    if (embeddedValid)
    {
        EmitLog(logFn, "Launch method selected: embedded");
        eRunNdsRetCode rc = runNdsWithExternalLoader(sLoaderBin, embeddedSize, finfoBootstrap.fclust, ndsBootstrapExePath,
                                                     1, argv, isRunFromSd, dsModeSwitch, kBoostCpu, kBoostVram, -1);
        if (rc == RUN_NDS_OK)
            return true;

        {
            char msg[64];
            snprintf(msg, sizeof(msg), "runNdsFile rc=%d", (int)rc);
            EmitLog(logFn, msg);
        }
        StringUtil::Copy(finalFailureReason, "embedded loader launch failed", sizeof(finalFailureReason));
    }
    else
    {
        StringUtil::Copy(finalFailureReason, "embedded loader invalid", sizeof(finalFailureReason));
    }

    EmitLog(logFn, "Launch method selected: direct");
    eRunNdsRetCode rcDirect = runNdsFile(ndsBootstrapExePath, 1, argv);
    {
        char msg[64];
        snprintf(msg, sizeof(msg), "runNdsFile rc=%d", (int)rcDirect);
        EmitLog(logFn, msg);
    }
    if (rcDirect != RUN_NDS_OK)
    {
        char msg[220];
        snprintf(msg, sizeof(msg), "Final launch failure reason: %s", finalFailureReason);
        EmitLog(logFn, msg);
        LOG_FATAL("Direct launch failed for %s rc=%d\n", ndsBootstrapExePath, (int)rcDirect);
        return false;
    }

    return true;
}
