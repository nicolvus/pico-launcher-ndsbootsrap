#pragma once
#include <string.h>

#define SYS_CLOSE           0x02
#define SYS_CLOCK           0x10
#define SYS_ELAPSED         0x30
#define SYS_ERRNO           0x13
#define SYS_FLEN            0x0C
#define SYS_GET_CMDLINE     0x15
#define SYS_HEAPINFO        0x16
#define SYS_ISERROR         0x08
#define SYS_ISTTY           0x09
#define SYS_OPEN            0x01
#define SYS_READ            0x06
#define SYS_READC           0x07
#define SYS_REMOVE          0x0E
#define SYS_SEEK            0x0A
#define SYS_SYSTEM          0x12
#define SYS_TICKFREQ        0x31
#define SYS_TIME            0x11
#define SYS_TMPNAM          0x0D
#define SYS_WRITE           0x05
#define SYS_WRITEC          0x03
#define SYS_WRITE0          0x04

#define SH_OPEN_MODE_R          0
#define SH_OPEN_MODE_RB         1
#define SH_OPEN_MODE_R_PLUS     2
#define SH_OPEN_MODE_R_PLUS_B   3
#define SH_OPEN_MODE_W          4
#define SH_OPEN_MODE_WB         5
#define SH_OPEN_MODE_W_PLUS     6
#define SH_OPEN_MODE_W_PLUS_B   7
#define SH_OPEN_MODE_A          8
#define SH_OPEN_MODE_AB         9
#define SH_OPEN_MODE_A_PLUS    10
#define SH_OPEN_MODE_A_PLUS_B  11

typedef struct
{
    const char* path;
    u32 mode;
    u32 pathLength;
} semihosting_open_params_t;

typedef struct
{
    int handle;
    void* buffer;
    u32 length;
} semihosting_read_params_t;

typedef struct
{
    int handle;
    const void* buffer;
    u32 length;
} semihosting_write_params_t;

typedef struct
{
    int handle;
    u32 position;
} semihosting_seek_params_t;

#ifdef __cplusplus
extern "C" {
#endif

int sh_callSemihostingFunction(int function, const void* parameters);

static inline void sh_writeChar(char c)
{
    sh_callSemihostingFunction(SYS_WRITEC, &c);
}

static inline void sh_writeString(const char* str)
{
    sh_callSemihostingFunction(SYS_WRITE0, str);
}

static inline int sh_openFile(const char* path, u32 mode)
{
    semihosting_open_params_t params =
    {
        path,
        mode,
        strlen(path)
    };
    return sh_callSemihostingFunction(SYS_OPEN, &params);
}

static inline int sh_readFile(int handle, void* buffer, u32 length)
{
    semihosting_read_params_t params =
    {
        handle,
        buffer,
        length
    };
    return sh_callSemihostingFunction(SYS_READ, &params);
}

static inline int sh_writeFile(int handle, const void* buffer, u32 length)
{
    semihosting_write_params_t params =
    {
        handle,
        buffer,
        length
    };
    return sh_callSemihostingFunction(SYS_WRITE, &params);
}

static inline int sh_seekFile(int handle, u32 position)
{
    semihosting_seek_params_t params =
    {
        handle,
        position
    };
    return sh_callSemihostingFunction(SYS_SEEK, &params);
}

static inline int sh_closeFile(int handle)
{
    return sh_callSemihostingFunction(SYS_CLOSE, &handle);
}

static inline int sh_getFileSize(int handle)
{
    return sh_callSemihostingFunction(SYS_FLEN, &handle);
}

#ifdef __cplusplus
}
#endif