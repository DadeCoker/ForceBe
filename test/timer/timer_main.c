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

bTIMER_INSTANCE(timer1, 1000, 1);
bTIMER_INSTANCE(timer2, 2000, 1);

void Timer1Handler()
{
    b_log("babyos\r\n");
}

void Timer2Handler()
{
    b_log("hello \r\n");
}

int main()
{
    port_init();
    bInit();
     
    bSoftTimerStart(&timer1, Timer1Handler);
    bSoftTimerStart(&timer2, Timer2Handler);

    while (1)
    {
        bExec();
    }
    return 0;
}
