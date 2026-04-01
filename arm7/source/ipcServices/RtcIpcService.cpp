#include "common.h"
#include <libtwl/sio/sioRtc.h>
#include "RtcIpcService.h"

void RtcIpcService::HandleMessage(u32 data)
{
    rtc_readDateTime(reinterpret_cast<rtc_datetime_t*>(data << 2));
    SendResponseMessage(1);
}
