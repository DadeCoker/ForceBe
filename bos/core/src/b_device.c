/**
 *!
 * \file        b_device.c
 * \version     v0.0.1
 * \date        2019/06/05
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 * 
 * Copyright (c) 2019 Bean
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 */
   
/*Includes ----------------------------------------------*/
#include "b_driver.h"
#include "b_device.h"
#include <string.h>
/** 
 * \addtogroup BABYOS
 * \{
 */

/** 
 * \addtogroup DEVICE
 * \{
 */


/** 
 * \defgroup DEVICE_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */
   
/** 
 * \defgroup DEVICE_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup DEVICE_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup DEVICE_Private_Variables
 * \{
 */
 
static bDriverInterface_t   bNullDriver;  

static bDriverInterface_t* bDriverTable[bDEV_MAX_NUM] = {
    #define B_DEVICE_REG(dev, driver, desc)    &driver,
    #include "b_device_list.h"
};

static const char *bDeviceDescTable[bDEV_MAX_NUM] = {
    #define B_DEVICE_REG(dev, driver, desc)    desc,
    #include "b_device_list.h"
};


static uint8_t bDeviceStatusTable[bDEV_MAX_NUM];

static bDriverInterface_t * volatile pCurrentDriver = NULL; 
static bDriverInterface_t * volatile pCurrentDriverBak = NULL; 
/**
 * \}
 */
   
/** 
 * \defgroup DEVICE_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/** 
 * \defgroup DEVICE_Private_Functions
 * \{
 */


/**
 * \}
 */
   
/** 
 * \addtogroup DEVICE_Exported_Functions
 * \{
 */


/**
 * \brief Device Initialize
 * \retval Number of abnormal device
 */
int bDeviceInit()  
{
    uint8_t i = 0;
    uint8_t retval = 0;
    if(bDEV_MAX_NUM == 0)
    {
        return -1;
    }
    memset(&bNullDriver, 0, sizeof(bNullDriver));
    memset(bDeviceStatusTable, 0, sizeof(bDeviceStatusTable));
    for(i = 0;i < bDEV_MAX_NUM;i++)
    {
        if(bDriverTable[i]->init != NULL)
        {
            if(bDriverTable[i]->init(bDriverTable[i]) < 0)
            {
                bDeviceStatusTable[i] = BDEVICE_ERROR;
                b_log_e("%s init error\r\n", bDeviceDescTable[i]);
                retval++;
            }
            else
            {
                b_log("%s init ok\r\n", bDeviceDescTable[i]);
            }
        }
    }
    return retval;
}  




int bDeviceOpen(uint8_t no)
{
    int retval = 0;
    if(no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if(bDeviceStatusTable[no] == 0)
    {
        if(bDriverTable[no]->open != NULL)
        {
            pCurrentDriver = bDriverTable[no];
            retval = bDriverTable[no]->open();
        }
    }
    else
    {
        retval = -255;
    }
    return retval;
}


int bDeviceRead(int no, uint32_t address, uint8_t *pdata, uint16_t len)
{
    int retval = 0;
    if(no >= bDEV_MAX_NUM || pdata == NULL)
    {
        return -1;
    }
    if(bDeviceStatusTable[no] == 0)
    {
        if(bDriverTable[no]->read != NULL)
        {
            pCurrentDriver = bDriverTable[no];
            retval = bDriverTable[no]->read(address, pdata, len);
        }
    }
    else
    {
        retval = -255;
    }
    return retval;
}

int bDeviceWrite(int no, uint32_t address, uint8_t *pdata, uint16_t len)
{
    int retval = 0;
    if(no >= bDEV_MAX_NUM || pdata == NULL)
    {
        return -1;
    }
    if(bDeviceStatusTable[no] == 0)
    {
        if(bDriverTable[no]->write != NULL)
        {
            pCurrentDriver = bDriverTable[no];
            retval = bDriverTable[no]->write(address, pdata, len);
        }
    }
    else
    {
        retval = -255;
    }
    return retval;
}



int bDeviceClose(int no)
{
    int retval = 0;
    if(no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if(bDeviceStatusTable[no] == 0)
    {
        if(bDriverTable[no]->close != NULL)
        {
            pCurrentDriver = bDriverTable[no];
            retval = bDriverTable[no]->close();
        }
    }
    else
    {
        retval = -255;
    }
    return retval;
}


int bDeviceCtl(int no, uint8_t cmd, void *param)
{
    int retval = 0;
    if(no >= bDEV_MAX_NUM)
    {
        return -1;
    }
    if(bDeviceStatusTable[no] == 0)
    {
        if(bDriverTable[no]->ctl != NULL)
        {
            pCurrentDriver = bDriverTable[no];
            retval = bDriverTable[no]->ctl(cmd, param);
        }
    }
    else
    {
        retval = -255;
    }
    return retval;
}

int bDeviceGetCurrentDrv(bDriverInterface_t **pdrv)
{
    if(pdrv == NULL || pCurrentDriver == NULL)
    {
        return -1;
    }
    *pdrv = pCurrentDriver;
    return 0;
}

void bDeviceEnterInterrupt()
{
    pCurrentDriverBak = pCurrentDriver;
}

void bDeviceExitInterrupt()
{
    pCurrentDriver = pCurrentDriverBak;
}

/**
 * \}
 */

/**
 * \}
 */

/**
 * \}
 */

/************************ Copyright (c) 2019 Bean *****END OF FILE****/


