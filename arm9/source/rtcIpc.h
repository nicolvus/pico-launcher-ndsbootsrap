#pragma once
#include <../../libtwl7/include/libtwl/sio/sioRtc.h>

void rtc_init();
void rtc_readDateTime(rtc_datetime_t* dateTime);
