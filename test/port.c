/**
 * \file port.c
 * \brief
 * \version 0.1
 * \date 2022-10-29
 * \author notrynohigh (notrynohigh@outlook.com)
 *
 * Copyright (c) 2020 by notrynohigh. All Rights Reserved.
 */
#include "port.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "b_os.h"

typedef void (*pfunc_t)(void);

void *port_tick(void *arg)
{
    while (1)
    {
        if (arg)
        {
            ((pfunc_t)arg)();
        }
        usleep(1000);
    }
    return NULL;
}

int bMcuUartSend(bHalUartNumber_t uart, const uint8_t *pbuf, uint16_t len)
{
    printf("%s", pbuf);
    return len;
}

void port_init()
{
    pthread_t tick_thread;
    pthread_create(&tick_thread, NULL, port_tick, bHalIncSysTick);
}
