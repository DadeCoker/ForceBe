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

void md5_test()
{
    uint8_t test[] = "12345";
    uint8_t md5_result[32 + 1];
    memset(md5_result, 0, sizeof(md5_result));
    md5_32(test, strlen(test), md5_result);
    printf("md5_32: %s \r\n", md5_result);
    
    memset(md5_result, 0, sizeof(md5_result));
    md5_16(test, strlen(test), md5_result);
    printf("md5_16: %s \r\n", md5_result);
}

void crc_test()
{
     uint8_t test[] = "01234567890123456789";
     uint8_t i = 0;
     uint32_t crc[ALGO_CRC32_MPEG2 + 1];
     CRC_REG_SBS_HANDLE(tmp_crc, 0);
     for(i = 0;i <= ALGO_CRC32_MPEG2;i++)
     {
          crc[i] = crc_calculate(i, test, strlen(test));
          printf("crc %d : %x\r\n", i, crc[i]);
     }

     for(i = 0;i <= ALGO_CRC32_MPEG2;i++)
     {
          CRC_SBS_HANDLE_RESET(tmp_crc, i);
          crc_calculate_sbs(&tmp_crc, test, 10);
          crc_calculate_sbs(&tmp_crc, &test[10], 10);
          printf("crc %d : %x ----%d\r\n", i, tmp_crc.crc, crc[i] == tmp_crc.crc);
     }



}

int main()
{
    port_init();
    bInit();
     
    md5_test();
    crc_test();

    while (1)
    {
        bExec();
    }
    return 0;
}
