![GitHub](https://img.shields.io/github/license/notrynohigh/BabyOS)![GitHub language count](https://img.shields.io/github/languages/count/notrynohigh/BabyOS)![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/notrynohigh/BabyOS)![GitHub commits since latest release (by SemVer)](https://img.shields.io/github/commits-since/notrynohigh/BabyOS/v5.0.0)![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/notrynohigh/BabyOS)

# BabyOS

![logo](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/2.png)


# 目录

- [BabyOS](#BabyOS)
    - [BabyOS是什么](#BabyOS是什么)
    - [代码结构](#代码结构)
    - [适用项目](#适用项目)
    - [前世今生](#前世今生)
    - [使用方法](#使用方法)
    - [BabyOS教程](#BabyOS教程)
    - [BabyOS私有协议上位机Demo](#BabyOS私有协议上位机Demo)
    - [Baby如何成长](#Baby如何成长)
    - [友情项目](#友情项目)
    - [更新记录](#更新记录)


#  BabyOS是什么

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

**对项目而言，缩短开发周期**。项目开发时选择适用的功能模块及驱动。直接进入功能代码编写的阶段。

**对工程师而言，减少重复工作**。调试过的功能模块和驱动代码放入BabyOS中管理，以后项目可以直接使用，去掉重复调试的工作。

# 代码结构

BabyOS代码分为3部分：

**[BabyOS仓库](https://gitee.com/notrynohigh/BabyOS)**：功能模块和驱动 ,一般情况下用户不需要改动，可以将此仓库作为子模块。 

**[BabyOS_Config](https://gitee.com/notrynohigh/BabyOS_Config)**：配置文件和设备注册文件  

**[BabyOS_Hal](https://gitee.com/notrynohigh/BabyOS_Hal)**：硬件抽象层, 不同分支对应不同硬件平台，选择对应平台下载，没有合适的则选择master分支

![frame](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/frame.png)



# 适用项目

使用裸机开发的项目推荐基于BabyOS进行。

使用操作系统开发的项目，BabyOS可作为功能库及驱动库使用。

​        

# 前世今生

说一说编写BabyOS原由

 ................

使用MCU开发的项目大多有两个要求：**产品功耗**和**开发周期**

#### 功耗的考量

设备的操作：唤醒设备，操作，设置休眠。

文件的操作：打开文件，编辑，关闭文件。

因此BabyOS中对设备的操作由统一的接口，即将设备看做文件一样对待。

#### 缩短开发周期

项目中有较多使用率高的功能模块和外设，BabyOS提供友好的框架对功能模块和设备驱动进行收集和管理。

新项目启动时，以搭积木的方式即可完成一部分工作，以此来缩短开发时间。



# 使用方法

**基于STM32F107上使用SPIFLASH及KV功能模块为例**

## 1.添加文件

```shell
BabyOS							//可作git子模块使用
├── bos
│   ├── algorithm               //常用算法，无需添加其中文件
│   ├── core					//核心文件，全部包含至工程
│   ├── drivers					//驱动文件，选择spiflash驱动添加至工程
│   ├── modules					//功能模块，全部添加至工程，由配置文件b_config.h配置
│   ├── thirdparty				//第三方代码，选择SFUD第三方代码添加至工程
│   └── utils					//实用代码，选择delay部分代码添加至工程
├── build						
├── CMakeLists.txt
├── doc							//相关文档
├── Examples					
│   └── qpn
├── LICENSE						//开源协议
└── README.md
BabyOS_Config					//克隆后放入工程目录，全部添加至工程
BabyOS_Hal						//克隆后放入工程目录，添加hal、gpio、uart、spi部分
```

```shell
//进入用户工程目录执行
git submodule add https://gitee.com/notrynohigh/BabyOS.git
git clone https://gitee.com/notrynohigh/BabyOS_Config.git    //克隆配置文件及设备注册文件
git clone https://gitee.com/notrynohigh/BabyOS_Hal.git		 //克隆后切换到对应平台的分支，如果没有则采用master分支作为模板
```



## 2.增加系统定时器

```C
//例如使用滴答定时器，中断服务函数调用：void bHalIncSysTick(void);

//注：定时器的周期与b_config.h里_TICK_FRQ_HZ要匹配
```



##   3.选择功能模块

b_config.h进行配置，勾选其中的KV Enable/Disable项

![config](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/1.png)

##   4.注册设备

```c
//b_device_list.h，在里面添加使用的外设。例如项目只需要使用SPIFlash，那么添加如下代码： 
//           设备        驱动          描述
B_DEVICE_REG(SPIFLASH, bSPIFLASH_Driver[0], "flash")

//如果没有注册任何设备，取消B_DEVICE_REG(null, bNullDriver, "null")的注释    
//B_DEVICE_REG(null, bNullDriver, "null")   
```

##   5.修改硬件接口

b_hal.h中根据实际连接图修改GPIO和SPI号

```C
#define HAL_SPIFLASH_QSPI_EN            0
#define HAL_SPIFLASH_TOTAL_NUMBER       1 
#define HAL_SPIFLASH_IF                 {{B_HAL_QSPI_INVALID, B_HAL_SPI_1, {B_HAL_GPIOB, B_HAL_PIN12}},}   
```

## 6.修改硬件抽象层SPI部分

（依赖硬件平台，使用STM32 HAL库为例）

```C
//b_hal_spi.c
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

## 7.修改硬件抽象层GPIO部分

（依赖硬件平台，使用STM32 HAL库为例）

```C
void bHalGPIO_WritePin(uint8_t port, uint8_t pin, uint8_t s)
{
    GPIO_PinState sta = (s) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(GPIO_PortTable[port], GPIO_PinTable[pin], sta);
}
```

## 8.基于SPIFLASH使用KV功能

```c
#include "b_os.h"    //头文件
//b_config.h配置文件中使能KV存储
int main()
{
    uint8_t buf[128]; 
    bInit();    //初始化，外设的初始化会在此处调用
    if(0 == bKV_Init(SPIFLASH, 0xA000, 4096 * 4, 4096)) //初始化KV存储，指定存储设备SPIFLASH
    {
        b_log("bKV_Init ok...\r\n");
    }
    //存储键值对（可用于存储系统配置信息）
    bKV_Set("name", (uint8_t *)"BabyOS", 7);
    bKV_Get("name", buf);
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
//举例使用SPIFLASH读取数据，从0地址读取128个字节数据至buf
{
    int fd = -1;
    fd = bOpen(SPIFLASH, BCORE_FLAG_RW);
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



# BabyOS教程

教程的代码仓库中不同分支对应着不同实验

<https://gitee.com/notrynohigh/BabyOS_Example>  【基于STM32F107CV】

<https://gitee.com/notrynohigh/BabyOS_HDSC>      【基于华大MCU HC32L136K8TA】

<https://www.bilibili.com/video/BV1ap4y1S7o8/>      【视频教程，BabyOS介绍】

https://www.bilibili.com/video/BV1pt4y1Q7vz/       【视频教程，移植BabyOS】

# BabyOS私有协议上位机Demo

<https://gitee.com/notrynohigh/BabyOS_Protocol>



# Baby如何成长

之所以称之为BabyOS，从上面的介绍可以看出，她如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。希望借助广大网友的力量，一起“喂养”她，是她成为MCU裸机开发中不可缺少的一部分。

**码云**（主仓库，开发者提交代码于dev分支，由管理员合并至master分支）：

<https://gitee.com/notrynohigh/BabyOS>

**github**（由notrynohigh同步）：

<https://github.com/notrynohigh/BabyOS>



# 友情项目

BabyOS包含了第三方开源代码，这部分代码都是MCU项目中比较实用的。

Shell功能模块基于开源项目nr_micro_shell，<https://gitee.com/nrush/nr_micro_shell>，感谢作者Nrush

Button 功能模块基于开源项目FlexibleButton，<https://github.com/murphyzhao/FlexibleButton>，感谢作者Murphy

GUI功能模块基于开源项目uGUI, <https://github.com/achimdoebler/UGUI>, 感谢作者Achimdoebler

Hardfault跟踪功能模块基于开源项目CmBacktrace,<https://gitee.com/Armink/CmBacktrace>, 感谢作者Armink

SPIFlash驱动基于开源项目SFUD,https://gitee.com/Armink/SFUD 感谢作者Armink

FS功能模块是基于FatFS和LittleFS,方便用户使用:

​				http://elm-chan.org/fsw/ff/archives.html FatFS

​				https://github.com/ARMmbed/littlefs LittleFS

------

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
| 2020.06 | 分解为3个仓库管理代码。增加SFUD作为SPIFLASH驱动，增加SD卡驱动，增加FatFS和LittleFS等 |      |
| 2020.07 | 内容未更新，增加两个视频教程用于快速了解BabyOS               |      |
| 2020.08 | BabyOS_Hal仓库增加ST标准库，增加标准库例子。对代码中细节进行优化 |      |

