![GitHub](https://img.shields.io/github/license/notrynohigh/BabyOS)![GitHub language count](https://img.shields.io/github/languages/count/notrynohigh/BabyOS)![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/notrynohigh/BabyOS)![GitHub commits since latest release (by SemVer)](https://img.shields.io/github/commits-since/notrynohigh/BabyOS/V7.0.0)![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/notrynohigh/BabyOS)

# BabyOS

![logo](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/2.png)



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

# 1 代码结构

![frame](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/frame.bmp)

# 2 适用项目

MCU裸机开发项目。编译器勾选C99   

# 3 前世今生

说一说编写BabyOS原由

 ................

使用MCU开发的项目大多有两个要求：**产品功耗**和**开发周期**

## 3.1 功耗的考量

设备的操作：唤醒设备，操作，设置休眠。

文件的操作：打开文件，编辑，关闭文件。

因此BabyOS中对设备的操作由统一的接口，即将设备看做文件一样对待。

## 3.2 缩短开发周期

项目中有较多使用率高的功能模块和外设，BabyOS提供友好的框架对功能模块和设备驱动进行收集和管理。

新项目启动时，以搭积木的方式即可完成一部分工作，以此来缩短开发时间。

# 4 使用方法

【STM32F107 使用STM32标准库函数为例】

## 4.1 准备基础工程

基础工程是基于STM32标准库建立的工程。需要用到的MCU资源初始化完成。

## 4.2 加入BabyOS代码

### 4.2.1 添加BabyOS代码

加入BabyOS代码有两种方式：

①下载代码，复制 bos 和 _config 目录到自己的代码目录。

② 当作子模块加入：`git submodule add https://gitee.com/notrynohigh/BabyOS.git babyos` bos

​     babyos/_config 用户根据需要进行修改

​     babyos/bos BabyOS的主要代码

### 4.2.2 加入代码至工程

| 路径           | 部分/全部    | 备注                                                         |
| -------------- | ------------ | ------------------------------------------------------------ |
| bos/algorithm  | 根据需要添加 |                                                              |
| bos/core       | 全部添加     |                                                              |
| bos/drivers    | 根据需要添加 | 不需要的驱动不要添加到工程中                                 |
| bos/hal        | 全部添加     |                                                              |
| bos/mcu        | 根据需要添加 | 根据平台添加对应的代码，对应b_config.h中的配置               |
| bos/modules    | 全部添加     | 全部添加后，可在b_config中配置                               |
| bos/thirdparty | 根据需要添加 | CmBacktrace 错误跟踪<br>FatFS 文件系统<br>littlefs 文件系统<br>FlexibleButton 使用按键模块则需要添加<br>nr_micro_shell 命令行<br>SFUD 使用SPIFLASH驱动需要添加<br>UGUI 简单的GUI库 |
| bos/utils      | 全部添加     | 通用代码，有模拟的IIC和SPI代码，模拟串口接收空闲事件         |
| bos/_config    |              | b_config.h BabyOS 配置文件<br>b_device_list.h 注册设备的文件<br>b_hal_if.h 驱动接口文件 |

### 4.2.3 添加头文件路径

添加两个路径即可：

`bos/`

 `_config/`   如果配置文件拷贝到其他路径了，则添加相应路径即可。 

### 4.2.4 修改配置

| 配置项                   | 说明                              | 备注 |
| ------------------------ | --------------------------------- | ---- |
| Version Configuration    | 版本配置项，硬件和固件版本        |      |
| Platform Configuration   | 平台配置项，指定心跳频率和MCU平台 |      |
| Modules Configuration    | 模块配置项，各个功能模块的配置    |      |
| Thirdparty Configuration | 第三方开源代码配置项              |      |

### 4.2.5 调用必要的函数

Include头文件 `b_os.h`

①滴答定时器中断服务函数调用 `bHalIncSysTick();`

②初始化代码 `bInit();`

③主循环调用 `bExec();`

## 4.3 配置接口&注册设备

可以由配置工具生成如下代码。配置工具：https://gitee.com/notrynohigh/bconfig-tool/releases/V0.0.1

![](https://images.gitee.com/uploads/images/2021/1212/020923_ec27eb55_1789704.png)

加入drivers内文件至工程后，则需要配置驱动的接口：`b_hal_if.h`

例如24C02芯片，IIC引脚 PB6-SCL   PB7-SDA

```C
// 24cxx
#define HAL_24CXX_IF                                             \
    {                                                            \
        {                                                        \
            .dev_addr               = 0xa0,                      \
            .is_simulation          = 1,                         \
            ._if.simulating_i2c.clk = {B_HAL_GPIOB, B_HAL_PIN6}, \
            ._if.simulating_i2c.sda = {B_HAL_GPIOB, B_HAL_PIN7}, \
        },                                                       \
    }
```

注册设备：`b_device_list.h`

```C
B_DEVICE_REG(b24CXX, b24CXX_Driver[0], "24cxx")
```

## 4.4 使用设备

编写测试代码：

```C
static void _24C02Test()
{
    int fd = -1;
    static uint8_t WValue = 0;
    static uint8_t RFlag = 0;
    uint8_t tmp = 0;
    fd = bOpen(_24C02, BCORE_FLAG_RW);
    if(fd < 0)
    {
        return;
    }
    if(RFlag)
    {
        bLseek(fd, 0);
        bRead(fd, &tmp, 1);
        b_log("r:%d\n", tmp);
        RFlag = 0;
        WValue++;
    }
    else
    {
        bLseek(fd, 0);
        bWrite(fd, &WValue, 1);
        RFlag = 1;
    }
    bClose(fd);
}
```

打开2402然后写入1个字节，然后再读出来并打印：

```C
_____________________________________________
    ____                         __       __  
    /   )          /           /    )   /    \
---/__ /-----__---/__---------/----/----\-----
  /    )   /   ) /   ) /   / /    /      \    
_/____/___(___(_(___/_(___/_(____/___(____/___
                         /                    
                     (_ /                     
HW:21.10.26 FW:6.1.0 COMPILE:Oct 30 2021-23:38:31
device number:1
r:0
r:1
r:2
r:3
r:4


```

# 5 BabyOS教程

教程的代码仓库中不同分支对应着不同实验

<https://gitee.com/notrynohigh/BabyOS_Example>  【基于STM32F107CV】

<https://gitee.com/notrynohigh/BabyOS_Protocol> 【BabyOS私有协议上位机Demo】

https://gitee.com/notrynohigh/bconfig-tool/releases/V0.0.1【BabyOS配置工具】



# 6 Baby如何成长

之所以称之为BabyOS，从上面的介绍可以看出，她如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。希望借助广大网友的力量，一起“喂养”她，是她成为MCU裸机开发中不可缺少的一部分。

**码云**（主仓库，开发者提交代码于dev分支，由管理员合并至master分支）：

<https://gitee.com/notrynohigh/BabyOS>

**github**（自动同步）：

<https://github.com/notrynohigh/BabyOS>



# 友情项目

BabyOS包含了第三方开源代码，这部分代码都是MCU项目中比较实用的。

Shell功能模块基于开源项目nr_micro_shell，<https://gitee.com/nrush/nr_micro_shell>，感谢作者Nrush

Button 功能模块基于开源项目FlexibleButton，<https://github.com/murphyzhao/FlexibleButton>，感谢作者Murphy

GUI功能模块基于开源项目uGUI, <https://github.com/achimdoebler/UGUI>, 感谢作者Achimdoebler

Trace功能模块基于开源项目 CmBacktrace,https://gitee.com/Armink/CmBacktrace/tree/master 感谢作者Armink

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
| 2021.01 | 去掉多余的文件，修改section部分的内容。                      |      |
| 2021.02 | 增加vscode+arm-gcc+openocd 编译调试的例子。doc目录增加sections链接文件以及示例makefile等 |      |
| 2021.05 | 支持编译器AC6                                                |      |
| 2021.06 | 大版本更新，对整体代码重新梳理一遍。可使用MCU内部FLASH使用KV存储、完善驱动部分等... |      |
| 2021.07 | 修改gcc环境下链接文件的修改方法                              |      |
| 2021.10 | 修改将分离的两个仓库内容重新加入进来，改变HAL代码的方式以及各个驱动的接口结构。V7.0.0 |      |
| 2021.11 | 针对警告和报错点进行优化, mcu目录下的代码默认用寄存器操作方式 |      |
| 2021.12 | 优化整体结构，增加对应配置工具，方便使用。                   |      |

