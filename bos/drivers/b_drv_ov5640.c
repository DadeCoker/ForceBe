/**
 *!
 * \file        b_drv_pcf8574.c
 * \version     v0.0.1
 * \date        2020/03/25
 * \author      Bean(notrynohigh@outlook.com)
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2020 Bean
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
#include "drivers/inc/b_drv_ov5640.h"

#include "drivers/inc/b_drv_ov5640af.h"
#include "drivers/inc/b_drv_ov5640cfg.h"

/**
 * \addtogroup B_DRIVER
 * \{
 */

/**
 * \addtogroup OV5640
 * \{
 */

/**
 * \defgroup OV5640_Private_TypesDefinitions
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OV5640_Private_Defines
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OV5640_Private_Macros
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OV5640_Private_Variables
 * \{
 */
const static bOV5640_HalIf_t bOV5640_HalIf = HAL_OV5640_IF;
bOV5640_Driver_t             bOV5640_Driver;

const static uint8_t OV5640_LightModeTable[5][7] = {
    0x04, 0X00, 0X04, 0X00, 0X04, 0X00, 0X00,  // Auto
    0x06, 0X1C, 0X04, 0X00, 0X04, 0XF3, 0X01,  // Sunny
    0x05, 0X48, 0X04, 0X00, 0X07, 0XCF, 0X01,  // Office
    0x06, 0X48, 0X04, 0X00, 0X04, 0XD3, 0X01,  // Cloudy
    0x04, 0X10, 0X04, 0X00, 0X08, 0X40, 0X01,  // Home
};

const static uint8_t OV5640_SaturationTable[7][6] = {
    0X0C, 0x30, 0X3D, 0X3E, 0X3D, 0X01,  //-3
    0X10, 0x3D, 0X4D, 0X4E, 0X4D, 0X01,  //-2
    0X15, 0x52, 0X66, 0X68, 0X66, 0X02,  //-1
    0X1A, 0x66, 0X80, 0X82, 0X80, 0X02,  //+0
    0X1F, 0x7A, 0X9A, 0X9C, 0X9A, 0X02,  //+1
    0X24, 0x8F, 0XB3, 0XB6, 0XB3, 0X03,  //+2
    0X2B, 0xAB, 0XD6, 0XDA, 0XD6, 0X04,  //+3
};

/**
 * \}
 */

/**
 * \defgroup OV5640_Private_FunctionPrototypes
 * \{
 */

/**
 * \}
 */

/**
 * \defgroup OV5640_Private_Functions
 * \{
 */

static void _OV5640_WriteReg(uint16_t reg, uint8_t data)
{
    SCCB_Start(bOV5640_HalIf.sccb);
    SCCB_SendByte(bOV5640_HalIf.sccb, bOV5640_HalIf.sccb_addr);
    SCCB_SendByte(bOV5640_HalIf.sccb, reg >> 8);
    SCCB_SendByte(bOV5640_HalIf.sccb, reg);
    SCCB_SendByte(bOV5640_HalIf.sccb, data);
    SCCB_Stop(bOV5640_HalIf.sccb);
}

static uint8_t _OV5640_ReadReg(uint16_t reg)
{
    uint8_t val = 0;
    SCCB_Start(bOV5640_HalIf.sccb);
    SCCB_SendByte(bOV5640_HalIf.sccb, bOV5640_HalIf.sccb_addr);
    SCCB_SendByte(bOV5640_HalIf.sccb, reg >> 8);
    SCCB_SendByte(bOV5640_HalIf.sccb, reg);
    SCCB_Stop(bOV5640_HalIf.sccb);

    SCCB_Start(bOV5640_HalIf.sccb);
    SCCB_SendByte(bOV5640_HalIf.sccb, bOV5640_HalIf.sccb_addr | 0x1);

    val = SCCB_ReceiveByte(bOV5640_HalIf.sccb);
    SCCB_NoAck(bOV5640_HalIf.sccb);
    SCCB_Stop(bOV5640_HalIf.sccb);
    return val;
}

static void _OV5640_FlashCtrl(uint8_t sw)
{
    _OV5640_WriteReg(0x3016, 0X02);
    _OV5640_WriteReg(0x301C, 0X02);
    if (sw)
    {
        _OV5640_WriteReg(0X3019, 0X02);
    }
    else
    {
        _OV5640_WriteReg(0X3019, 0X00);
    }
}

static void _OV5640_RGB565Mode()
{
    uint16_t i = 0;
    for (i = 0; i < (sizeof(ov5640_rgb565_reg_tbl) / 4); i++)
    {
        _OV5640_WriteReg(ov5640_rgb565_reg_tbl[i][0], ov5640_rgb565_reg_tbl[i][1]);
    }
}

static void _OV5640_LightMode(uint8_t mode)
{
    uint8_t i;
    _OV5640_WriteReg(0x3212, 0x03);  // start group 3
    for (i = 0; i < 7; i++)
    {
        _OV5640_WriteReg(0x3400 + i, OV5640_LightModeTable[mode][i]);
    }
    _OV5640_WriteReg(0x3212, 0x13);  // end group 3
    _OV5640_WriteReg(0x3212, 0xa3);  // launch group 3
}

static void _OV5640_ColorSaturation(uint8_t sat)
{
    uint8_t i;
    _OV5640_WriteReg(0x3212, 0x03);  // start group 3
    _OV5640_WriteReg(0x5381, 0x1c);
    _OV5640_WriteReg(0x5382, 0x5a);
    _OV5640_WriteReg(0x5383, 0x06);
    for (i = 0; i < 6; i++)
    {
        _OV5640_WriteReg(0x5384 + i, OV5640_SaturationTable[sat][i]);
    }
    _OV5640_WriteReg(0x538b, 0x98);
    _OV5640_WriteReg(0x538a, 0x01);
    _OV5640_WriteReg(0x3212, 0x13);  // end group 3
    _OV5640_WriteReg(0x3212, 0xa3);  // launch group 3
}

static void _OV5640_Brightness(uint8_t bright)
{
    uint8_t brtval;
    if (bright < 4)
    {
        brtval = 4 - bright;
    }
    else
    {
        brtval = bright - 4;
    }
    _OV5640_WriteReg(0x3212, 0x03);  // start group 3
    _OV5640_WriteReg(0x5587, brtval << 4);
    if (bright < 4)
    {
        _OV5640_WriteReg(0x5588, 0x09);
    }
    else
    {
        _OV5640_WriteReg(0x5588, 0x01);
    }
    _OV5640_WriteReg(0x3212, 0x13);  // end group 3
    _OV5640_WriteReg(0x3212, 0xa3);  // launch group 3
}

static void _OV5640_Contrast(uint8_t contrast)
{
    uint8_t reg0val = 0X00;  // contrast=3
    uint8_t reg1val = 0X20;
    switch (contrast)
    {
        case 0:  //-3
            reg1val = reg0val = 0X14;
            break;
        case 1:  //-2
            reg1val = reg0val = 0X18;
            break;
        case 2:  //-1
            reg1val = reg0val = 0X1C;
            break;
        case 4:  // 1
            reg0val = 0X10;
            reg1val = 0X24;
            break;
        case 5:  // 2
            reg0val = 0X18;
            reg1val = 0X28;
            break;
        case 6:  // 3
            reg0val = 0X1C;
            reg1val = 0X2C;
            break;
    }
    _OV5640_WriteReg(0x3212, 0x03);  // start group 3
    _OV5640_WriteReg(0x5585, reg0val);
    _OV5640_WriteReg(0x5586, reg1val);
    _OV5640_WriteReg(0x3212, 0x13);  // end group 3
    _OV5640_WriteReg(0x3212, 0xa3);  // launch group 3
}

static void _OV5640_Sharpness(uint8_t sharp)
{
    if (sharp < 33)
    {
        _OV5640_WriteReg(0x5308, 0x65);
        _OV5640_WriteReg(0x5302, sharp);
    }
    else
    {
        _OV5640_WriteReg(0x5308, 0x25);
        _OV5640_WriteReg(0x5300, 0x08);
        _OV5640_WriteReg(0x5301, 0x30);
        _OV5640_WriteReg(0x5302, 0x10);
        _OV5640_WriteReg(0x5303, 0x00);
        _OV5640_WriteReg(0x5309, 0x08);
        _OV5640_WriteReg(0x530a, 0x30);
        _OV5640_WriteReg(0x530b, 0x04);
        _OV5640_WriteReg(0x530c, 0x06);
    }
}

static uint8_t _OV5640_OutsizeSet(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    _OV5640_WriteReg(0X3212, 0X03);
    _OV5640_WriteReg(0x3808, width >> 8);
    _OV5640_WriteReg(0x3809, width & 0xff);
    _OV5640_WriteReg(0x380a, height >> 8);
    _OV5640_WriteReg(0x380b, height & 0xff);
    _OV5640_WriteReg(0x3810, offx >> 8);
    _OV5640_WriteReg(0x3811, offx & 0xff);

    _OV5640_WriteReg(0x3812, offy >> 8);
    _OV5640_WriteReg(0x3813, offy & 0xff);

    _OV5640_WriteReg(0X3212, 0X13);
    _OV5640_WriteReg(0X3212, 0Xa3);
    return 0;
}

static uint8_t _OV5640_FocusInit()
{
    uint16_t i;
    uint16_t addr  = 0x8000;
    uint8_t  state = 0x8F;
    _OV5640_WriteReg(0x3000, 0x20);
    for (i = 0; i < sizeof(OV5640_AF_Config); i++)
    {
        _OV5640_WriteReg(addr, OV5640_AF_Config[i]);
        addr++;
    }
    _OV5640_WriteReg(0x3022, 0x00);
    _OV5640_WriteReg(0x3023, 0x00);
    _OV5640_WriteReg(0x3024, 0x00);
    _OV5640_WriteReg(0x3025, 0x00);
    _OV5640_WriteReg(0x3026, 0x00);
    _OV5640_WriteReg(0x3027, 0x00);
    _OV5640_WriteReg(0x3028, 0x00);
    _OV5640_WriteReg(0x3029, 0x7f);
    _OV5640_WriteReg(0x3000, 0x00);
    i = 0;
    do
    {
        state = _OV5640_ReadReg(0x3029);
        bUtilDelayMS(5);
        i++;
        if (i > 1000)
            return 1;
    } while (state != 0x70);
    return 0;
}

/************************************************************************************************************driver
 * interface*******/
static int _OV5640_Ctl(bOV5640_Driver_t *pdrv, uint8_t cmd, void *param)
{
    bCameraCfgStruct_t * config;
    bCameraCfgOutsize_t *config_outsize;
    if (param == NULL)
    {
        return -1;
    }
    config         = (bCameraCfgStruct_t *)param;
    config_outsize = (bCameraCfgOutsize_t *)param;
    switch (cmd)
    {
        case bCMD_CONF_LIGHTMODE:
            if (config->config_val >= 5)
            {
                config->config_val = 4;
            }
            _OV5640_LightMode(config->config_val);
            break;
        case bCMD_CONF_COLOR_SAT:
            if (config->config_val >= 7)
            {
                config->config_val = 6;
            }
            _OV5640_ColorSaturation(config->config_val);
            break;
        case bCMD_CONF_BRIGHTNESS:
            _OV5640_Brightness(config->config_val);
            break;
        case bCMD_CONF_CONTRAST:
            if (config->config_val >= 7)
            {
                config->config_val = 6;
            }
            _OV5640_Contrast(config->config_val);
            break;
        case bCMD_CONF_SHARPNESS:
            _OV5640_Sharpness(config->config_val);
            break;
        case bCMD_CONF_FLASH_LED:
            if (config->config_val == 0)
            {
                _OV5640_FlashCtrl(0);
            }
            else
            {
                _OV5640_FlashCtrl(1);
            }
            break;
        case bCMD_CONF_OUTSIZE:
            _OV5640_OutsizeSet(config_outsize->xoff, config_outsize->yoff, config_outsize->xsize,
                               config_outsize->ysize);
            break;
        default:
            break;
    }
    return 0;
}

/**
 * \}
 */

/**
 * \addtogroup OV5640_Exported_Functions
 * \{
 */
int bOV5640_Init()
{
    uint16_t reg, i;

    bHalGPIO_WritePin(bOV5640_HalIf.reset.port, bOV5640_HalIf.reset.pin, 0);
    bUtilDelayMS(20);
    bHalGPIO_WritePin(bOV5640_HalIf.reset.port, bOV5640_HalIf.reset.pin, 1);
    bUtilDelayMS(20);

    reg = _OV5640_ReadReg(OV5640_CHIPIDH);
    reg <<= 8;
    reg |= _OV5640_ReadReg(OV5640_CHIPIDL);

    b_log("camera id: %x\r\n", reg);

    if (reg != OV5640_ID)
    {
        bOV5640_Driver.status = -1;
        return -1;
    }

    _OV5640_WriteReg(0x3103, 0X11);
    _OV5640_WriteReg(0X3008, 0X82);
    bUtilDelayMS(10);

    for (i = 0; i < sizeof(ov5640_uxga_init_reg_tbl) / 4; i++)
    {
        _OV5640_WriteReg(ov5640_uxga_init_reg_tbl[i][0], ov5640_uxga_init_reg_tbl[i][1]);
    }

    _OV5640_FocusInit();
    _OV5640_RGB565Mode();
    _OV5640_OutsizeSet(16, 4, 320, 240);

    bOV5640_Driver.status = 0;
    bOV5640_Driver.close  = NULL;
    bOV5640_Driver.read   = NULL;
    bOV5640_Driver.ctl    = _OV5640_Ctl;
    bOV5640_Driver.open   = NULL;
    bOV5640_Driver.write  = NULL;
    return 0;
}

bDRIVER_REG_INIT(bOV5640_Init);

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
