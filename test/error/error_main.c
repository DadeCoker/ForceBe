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

#define BAT_LOW (0)
#define MEM_ERR (1)

void SystemErrCallback(uint8_t err)
{
    b_log_e("err:%d\r\n", err);
}

int main()
{
    port_init();
    bInit();
     
    bErrorInit(SystemErrCallback);
    bErrorRegist(BAT_LOW, 3000, BERROR_LEVEL_1);
    bErrorRegist(MEM_ERR, 0, BERROR_LEVEL_0);

    while (1)
    {
        bExec();
    }
    return 0;
}
