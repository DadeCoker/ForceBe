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

void check_name()
{
    uint8_t name[64];
    b_log("read name:");
    bKV_Get("name", name);
    b_log("name: %s\n", name);
}

int main()
{
    port_init();
    bInit();
    bKV_Init(bTESTFLASH, 0, 40960, 4096);
    bKV_Set("name", "babyos", strlen("babyos"));
    while (1)
    {
        bExec();
        BOS_PERIODIC_TASK(check_name, 1000);
    }
    return 0;
}
