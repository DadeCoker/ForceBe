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

bKV_INSTANCE(gKVInstance, bTESTFLASH, 0, 10 * 4 * 1024, 4 * 1024);

void check_name()
{
    static uint8_t flag    = 0;
    uint8_t        write[] = "babyos1";
    uint8_t        name[64];
    memset(name, 0, sizeof(name));
    flag     = (flag + 1) % 2;
    write[6] = write[6] + flag;
    bKVSetValue(&gKVInstance, "name", write, strlen(write));
    b_log("read name: %d\r\n", bKVGetValueLength(&gKVInstance, "name"));
    bKVGetValue(&gKVInstance, "name", name, sizeof(name), NULL);
    b_log("name: %s\r\n", name);
}

int main()
{
    port_init();
    bInit();
    bKVInit(&gKVInstance);
    while (1)
    {
        bExec();
        BOS_PERIODIC_TASK(check_name, 1000);
    }
    return 0;
}
