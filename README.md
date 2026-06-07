![GitHub](https://img.shields.io/github/license/notrynohigh/BabyOS)![GitHub language count](https://img.shields.io/github/languages/count/notrynohigh/BabyOS)![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/notrynohigh/BabyOS)![GitHub commits since latest release (by SemVer)](https://img.shields.io/github/commits-since/notrynohigh/BabyOS/v3.3.0)![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/notrynohigh/BabyOS)

# BabyOS

![logo](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/2.png)


# 目录

- [BabyOS](#BabyOS)
    - [BabyOS是什么](#BabyOS是什么)
    - [适用项目](#适用项目)
    - [前世今生](#前世今生)
        - [功耗的考量](#功耗的考量)
        - [缩短开发周期](#缩短开发周期)
    - [使用方法](#使用方法)
        - [添加文件](#添加文件)
        - [增加系统定时器](#增加系统定时器)
        - [选择功能模块](#选择功能模块)
        - [注册设备](#注册设备)
        - [使用范例](#使用范例)
            - [指定硬件接口](#指定硬件接口)
            - [基于SPIFLASH使用KV功能](#基于SPIFLASH使用KV功能)
    - [BabyOS教程](#BabyOS教程)
    - [BabyOS私有协议上位机Demo](#BabyOS私有协议上位机Demo)
    - [Baby如何成长](#Baby如何成长)
    - [友情项目](#友情项目)
    - [更新记录](#更新记录)


##  BabyOS是什么

```
______________________________________________
    ____                         __       __  
    /   )          /           /    )   /    )
---/__ /-----__---/__---------/----/----\-----
  /    )   /   ) /   ) /   / /    /      \    
_/____/___(___(_(___/_(___/_(____/___(____/___
                         /                    
                     (_ /                     
```

BabyOS适用于MCU项目，她是一套管理功能模块和外设驱动的框架。

![frame](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/frame.png)

**对项目而言，缩短开发周期**。项目开发时选择适用的功能模块及驱动。直接进入功能代码编写的阶段。

**对工程师而言，减少重复工作**。调试过的功能模块和驱动代码放入BabyOS中进行管理，以后项目可以直接使用，去掉重复调试的工作。

------

## 适用项目

使用裸机开发的项目推荐基于BabyOS进行。

使用操作系统开发的项目，BabyOS可作为功能库及驱动库使用。

​        

## 前世今生

说一说编写BabyOS原由

 ................

使用MCU裸机开发的项目大多有两个要求：**产品功耗**和**开发周期**

### 功耗的考量

出于功耗考虑，对外设的操作是：唤醒外设，操作，最后进入休眠。这样的操作形式和文件的操作很类似，文件的操作步骤是打开到编辑到关闭。因此将外设的操作看作是对文件的操作进行。驱动中打开和关闭对应着唤醒和睡眠。

### 缩短开发周期

项目中，有较多使用率高的功能模块，例如：UTC、错误管理、电池电量、存储数据、上位机通信、固件升级等等。同时也有很多常用的芯片、传感器等。BabyOS便是将这些使用率高的功能模块以及常用的芯片、传感器驱动进行收集并管理。新项目启动时，以搭积木的方式完成一部分工作以此来缩短开发时间。

 

## 使用方法

###   添加文件

bos/core/         核心文件全部添加至工程

bos/config/       配置文件及设备列表文件，全部添加至工程

bos/driver/       选择需要的驱动添加至工程，将b_hal.h内定义的硬件接口取消注释

bos/hal/hal/      硬件抽象层，将需要的文件添加至工程，根据具体平台进行修改

bos/hal/utils/    底层实用代码，全部添加至工程

bos/modules/   功能模块，全部添加至工程

bos/algorithm/ 实用算法，选择需要的添加至工程

bos/thirdparty/ 第三方开源代码，将需要的添加至工程

### 增加系统定时器

例如使用滴答定时器，中断服务函数调用：void bHalIncSysTick(void);

注：定时器的周期与b_config.h里_TICK_FRQ_HZ要匹配

###   选择功能模块

b_config.h进行配置，根据自己的需要选择功能模块。

![config](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/1.png)

###   注册设备

b_device_list.h，在里面添加使用的外设。例如项目只需要使用SPIFlash，那么添加如下代码： 

```c
//           设备        驱动          描述
B_DEVICE_REG(W25QXX, bW25X_Driver, "flash")
//如果没有注册任何设备，取消B_DEVICE_REG(null, bNullDriver, "null")的注释    
//B_DEVICE_REG(null, bNullDriver, "null")   
```

###   使用范例  

以b_kv功能模块为例，先在b_config里面使能b_kv。

#### 取消硬件接口的注释

b_hal.h中取消如下部分的注释，并根据实际连接图修改GPIO和SPI号

```C
#define HAL_W25X_SPI                    B_HAL_SPI_2
#define HAL_W25X_CS_PORT                B_HAL_GPIOB             
#define HAL_W25X_CS_PIN                 B_HAL_PIN12
```

修改硬件抽象层b_hal_spi.c内SPI的操作（依赖硬件平台，使用STM32 HAL库为例）

```C
int bHalSPI_Send(bHalSPINumber_t spi, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(spi)
    {
        case B_HAL_SPI_1:
            HAL_SPI_Transmit(&hspi1, pbuf, len, 0xfff);
            break;        
        default:
            break;
    }
    return 0;
}

int bHalSPI_Receive(bHalSPINumber_t spi, uint8_t *pbuf, uint16_t len)
{
    if(pbuf == NULL)
    {
        return -1;
    }
    switch(spi)
    {
        case B_HAL_SPI_1:
            HAL_SPI_Receive(&hspi1, pbuf, len, 0xfff);
            break;        
        default:
            break;
    }
    return 0;
}
```

修改硬件抽象层b_hal_gpio.c内IO的操作（依赖硬件平台，使用STM32 HAL库为例）

```C
void bHalGPIO_WritePin(uint8_t port, uint8_t pin, uint8_t s)
{
    GPIO_PinState sta = (s) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(GPIO_PortTable[port], GPIO_PinTable[pin], sta);
}
```

#### 基于SPIFLASH使用KV功能

```c
#include "b_os.h"    //头文件
//b_config.h配置文件中使能KV存储
int main()
{
    uint8_t buf[128];
    //......     
    bInit();    //初始化，外设的初始化会在此处调用
    
    //下面举例使用：W25QXX和KV存储功能模块,其中W25QXX已经添加到b_device_list.h
    if(0 == bKV_Init(W25QXX, 0xA000, 4096 * 4, 4096)) //初始化KV存储，指定存储设备W25QXX
    {
        b_log("bKV_Init ok...\r\n");
    }
    //存储键值对（可用于存储系统配置信息）
    bKV_Set((uint8_t *)"name", (uint8_t *)"BabyOS", 7);
    bKV_Get((uint8_t *)"name", buf);
    b_log("name:%s\r\n", buf); 
    //......
    while(1)
    {
        //.....
        bExec();      //循环调用此函数
        //.....
    }
}


```

如果不使用功能模块，单独对设备进行操作，使用如下方式进行：

```c
//举例使用W25QXX读取数据，从0地址读取128个字节数据至buf
{
    int fd = -1;
    fd = bOpen(W25QXX, BCORE_FLAG_RW);
    if(fd == -1)
    {
        return;
    }
    bLseek(fd, 0);
    bRead(fd, buf, 128);
    bClose(fd); 
}
```



更多使用介绍： 

<https://gitee.com/notrynohigh/BabyOS/wikis>

https://github.com/notrynohigh/BabyOS/wiki



## BabyOS教程

教程的代码仓库中不同分支对应着不同实验

<https://gitee.com/notrynohigh/BabyOS_Example>  【基于STM32F107CV】

<https://gitee.com/notrynohigh/BabyOS_HDSC>      【基于华大MCU HC32L136K8TA】



## BabyOS私有协议上位机Demo

<https://gitee.com/notrynohigh/BabyOS_Protocol>



## Baby如何成长

之所以称之为BabyOS，从上面的介绍可以看出，她如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。希望借助广大网友的力量，一起“喂养”她，是她成为MCU裸机开发中不可缺少的一部分。

**码云**（主仓库，开发者提交代码于dev分支，由管理员合并至master分支）：

<https://gitee.com/notrynohigh/BabyOS>

**github**（由notrynohigh同步）：

<https://github.com/notrynohigh/BabyOS>



## 友情项目

BabyOS包含了第三方开源代码，这部分代码都是MCU项目中比较实用的。

b_shell 功能模块基于开源项目nr_micro_shell，<https://gitee.com/nrush/nr_micro_shell>，感谢作者Nrush

b_button 功能模块基于开源项目FlexibleButton，<https://github.com/murphyzhao/FlexibleButton>，感谢作者Murphy

b_gui 功能模块基于开源项目uGUI, <https://github.com/achimdoebler/UGUI>, 感谢作者Achimdoebler

b_trace功能模块基于开源项目CmBacktrace,<https://gitee.com/Armink/CmBacktrace>, 感谢作者Armink

**如果您觉得这套开源代码有意义，请给个Star表示支持，谢谢！**



管理员邮箱：notrynohigh@outlook.com

开发小组群：

![qq](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/qq.png)



## 更新记录

| 日期    | 新增项                                                       | 备注 |
| ------- | ------------------------------------------------------------ | ---- |
| 2019.12 | 功能模块：FIFO, AT, Nr_micro_shell, Lunar calendar           |      |
| 2020.01 | 功能模块：KV存储                                             |      |
| 2020.02 | 功能模块：Xmodem128, Ymodem, FlexibleButton 驱动：xpt2046    |      |
| 2020.03 | 功能模块：b_log, b_gui, b_menu, b_trace，b_heap 驱动：ssd1289 ili9341 |      |
| 2020.04 | 增加基于华大MCU例子，增加驱动ili9320 oled12864，优化b_mod_kv等功能模块，增加algorithm目录 |      |
| 2020.05 | 优化代码结构，增加：b_mod_timer b_mod_qpn等 ,优化b_mod_xmodem等，增加算法base64 sha1 sort等 |      |

