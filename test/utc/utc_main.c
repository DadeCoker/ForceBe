/**
 * \file kv_main.c
 * \brief
 * \version 0.1
 * \date 2022-10-29
 * \author notrynohigh (notrynohigh@outlook.com)
 *
 * Copyright (c) 2020 by notrynohigh. All Rights Reserved.
 */
#include "../port.h"
#include "b_os.h"

int main()
{
    bUTC_DateTime_t tm;
    bUTC_t utc = 1680019757;
    port_init();
    bInit();
     
    bUTC2Struct(&tm, utc, 8);

    b_log("%d-%d-%d %d:%d:%d [%d]\r\n", tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second, tm.week);

    tm.year = 2023;
    tm.month = 3;
    tm.day = 29;
    tm.hour = 0;
    tm.minute = 9;
    tm.second = 17;
    bUTC_t test_utc = bStruct2UTC(tm, 8);
    b_log("utc: %d\r\n", test_utc);

    while (1)
    {
        bExec();
    }
    return 0;
}
