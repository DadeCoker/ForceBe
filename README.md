![GitHub](https://img.shields.io/github/license/notrynohigh/BabyOS)![GitHub language count](https://img.shields.io/github/languages/count/notrynohigh/BabyOS)![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/notrynohigh/BabyOS)

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

对项目而言，缩短开发周期。项目开发时选择适用的功能模块及驱动。直接进入功能代码编写的阶段。

对开发而言，减少重复工作。调试过的功能模块和驱动代码放入BabyOS中管理，以后项目可以直接使用。

# 1 代码结构

![BabyOS](https://gitee.com/notrynohigh/BabyOS/raw/dev/doc/BabyOS.png)

# 2 适用项目

MCU开发项目。编译器勾选C99   

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

# 4 BabyOS手册

 [《BabyOS设计和使用手册》](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md)

[1.项目介绍](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#1-%E9%A1%B9%E7%9B%AE%E4%BB%8B%E7%BB%8D)

[2.设计思路](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#2-%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF)

[3.快速体验](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#3-%E5%BF%AB%E9%80%9F%E4%BD%93%E9%AA%8C)

[4.进阶体验](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#4-%E8%BF%9B%E9%98%B6%E4%BD%93%E9%AA%8C)

[5.概要介绍](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#5%E6%A6%82%E8%A6%81%E4%BB%8B%E7%BB%8D)

[6.功能模块](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#6-%E5%8A%9F%E8%83%BD%E6%A8%A1%E5%9D%97)

[7.工具模块](https://gitee.com/notrynohigh/BabyOS/blob/master/doc/BabyOS%E8%AE%BE%E8%AE%A1%E5%92%8C%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C.md#7%E5%B7%A5%E5%85%B7%E6%A8%A1%E5%9D%97)

# 5 BabyOS教程

[第一讲介绍BabyOS](https://www.bilibili.com/video/BV1Ff4y1o7bZ?share_source=copy_web)

[第二讲BabyOS的快速体验和进阶体验](https://www.bilibili.com/video/BV1Lg411f7cH?share_source=copy_web)

[第三讲BabyOS的功能概述](https://www.bilibili.com/video/BV1iU4y1q7EJ?share_source=copy_web)

[第四讲BabyOS的功能组件](https://www.bilibili.com/video/BV1JZ4y1Y7S7?share_source=copy_web)

# 6 BabyOS例程

例程仓库中不同分支对应着不同实验：

<https://gitee.com/notrynohigh/BabyOS_Example>  

BabyOS私有协议上位机Demo：

<https://gitee.com/notrynohigh/BabyOS_Protocol> 

# 7 BabyOS配置工具

https://gitee.com/notrynohigh/bconfig-tool/releases/

![](https://gitee.com/notrynohigh/bconfig-tool/raw/master/doc/1.png)



# 8 Baby如何成长

BabyOS如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。希望借助广大网友的力量，一起“喂养”她，是她成为MCU开发中不可缺少的一部分。

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

开发小组QQ群&开发动态发布的公众号：

![qq](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/qq.png) ![qrcode](https://gitee.com/notrynohigh/BabyOS/raw/master/doc/qrcode.jpg)



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
| 2021.12 | 优化整体结构，增加对应配置工具，方便使用。支持V7.3.0及更新版本的 b_config.h |      |
| 2022.01 | 增加bReinit和bModifyHalIf接口，并修复模拟I2C中BUG            |      |
| 2022.02 | 调整结构，使得依赖关系更加合理。                             |      |
| 2022.03 | 增加cjson第三方代码，并配上内存池。修改b_config.h中宏的命名以及文件夹、文件命名。 |      |
| 2022.04 | 增加WIFI模块驱动，增加获取驱动私有信息的接口，增加QSPI       |      |
| 2022.05 | 重写配置工具、增加IAP功能模块                                |      |
| 2022.06 | 更新设计和使用文档，详细介绍软件模块。增加对矩阵按键的支持   |      |
| 2022.07 | 增加视频教程，在README中展现                                 |      |

