/*
    The MIT License (MIT)
    Author: Hongtai Liu (lht856@foxmail.com)
    Copyright (C) 2020  Seeed Technology Co.,Ltd.
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifdef __SAMD51__
#ifndef __RTC_SAMD51_H__
#define __RTC_SAMD51_H__
#include "DateTime.h"


typedef void (*rtcCallBack)(uint32_t flag); 

// Default date & time after reset
#define DEFAULT_YEAR 2000 // 2000..2063
#define DEFAULT_MONTH 1   // 1..12
#define DEFAULT_DAY 1     // 1..31
#define DEFAULT_HOUR 0    // 1..23
#define DEFAULT_MINUTE 0  // 0..59
#define DEFAULT_SECOND 0  // 0..59

class RTC_SAMD51
{
public:
    enum Alarm_Match : uint8_t // Should we have this enum or just use the identifiers from /component/rtc.h ?
    {
        MATCH_OFF = RTC_MODE2_MASK_SEL_OFF_Val,                  // Never
        MATCH_SS = RTC_MODE2_MASK_SEL_SS_Val,                    // Every Minute
        MATCH_MMSS = RTC_MODE2_MASK_SEL_MMSS_Val,                // Every Hour
        MATCH_HHMMSS = RTC_MODE2_MASK_SEL_HHMMSS_Val,            // Every Day
        MATCH_DHHMMSS = RTC_MODE2_MASK_SEL_DDHHMMSS_Val,         // Every Month
        MATCH_MMDDHHMMSS = RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val,    // Every Year
        MATCH_YYMMDDHHMMSS = RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val // Once, on a specific date and a specific time
    };

    boolean begin();
    void adjust(const DateTime &dt);
    DateTime now();
    void setAlarm(uint8_t id, const DateTime &dt);
    DateTime alarm(uint8_t id);
    void enableAlarm(uint8_t id, Alarm_Match match);
    void disableAlarm(uint8_t id);
    void attachInterrupt(rtcCallBack callback);
    void detachInterrupt();
    void standbyMode();

private:
    void config32kOSC(void);
    void configureClock(void);
    void RTCreadRequest();
    bool RTCisSyncing(uint32_t flag = RTC_MODE2_SYNCBUSY_MASK_);
    void RTCdisable();
    void RTCenable();
    void RTCreset();
    void RTCresetRemove();
};
#endif
#endif