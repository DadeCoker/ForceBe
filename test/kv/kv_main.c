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

bKV_INSTANCE(gKVInstance, bTESTFLASH, 0, 100 * 1024, 4 * 1024);

void check_name()
{
    uint8_t name[64];
    b_log("read name:");
    bKVGetValue(&gKVInstance, "name", name, sizeof(name), NULL);
    b_log("name: %s\n", name);
}

int main()
{
    port_init();
    bInit();
    bKVInit(&gKVInstance);
    bKVSetValue(&gKVInstance, "name", "babyos", strlen("babyos"));
    while (1)
    {
        bExec();
        BOS_PERIODIC_TASK(check_name, 1000);
    }
    return 0;
}
