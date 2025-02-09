![GitHub](https://img.shields.io/github/license/notrynohigh/BabyOS)![GitHub language count](https://img.shields.io/github/languages/count/notrynohigh/BabyOS)![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/notrynohigh/BabyOS)![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/notrynohigh/BabyOS)

# BabyOS

![logo](https://github.com/notrynohigh/BabyOS/raw/master/doc/2.png)

##  BabyOS是什么？

​        BabyOS适用于MCU项目，她是一套管理功能模块和外设驱动的框架。

​        **对项目而言，缩短开发周期**。项目开发时选择适用的功能模块及驱动。直接进入功能代码编写的阶段。

​        **对工程师而言，减少重复工作**。调试过的功能模块和驱动代码放入BabyOS中进行管理，以后项目可以直接使用，去掉重复调试的工作。

------

## 适用项目

​        **使用裸机开发的项目推荐基于BabyOS进行**。

​        

## 前世今生

​        说一说编写BabyOS原由

​        ................

​        目前使用MCU裸机开发的项目不会很庞大，大多有两个要求：**开发时间**和**产品功耗**。99.874%产品是电池供电，功耗是重点考虑对象。工程师开发的多个项目之间总会碰到相同的功能点，那么是否可以有套代码框架可以容纳已经做过的功能点，去掉重复的工作，加快产品或者demo的开发。

### 功耗的考量

​        出于功耗考虑，对外设的操作是：唤醒外设，操作，最后进入休眠。这样的操作形式和文件的操作很类似，文件的操作步骤是打开到编辑到关闭。

​       因此将外设的操作看作是对文件的操作进行。每个外设打开后返回一个描述符，后续代码中对外设的操作都是基于这个描述符进行。关闭外设后回收描述符。

​       所以外设的驱动中打开和关闭的操作执行对设备的唤醒和睡眠。利用描述符来操作外设还有一个好处是，当更换外设后，只需更换驱动接口，业务部分的代码不需要变动。

### 快速开发

​        小型项目的开发中，有较多使用率高的功能模块，例如：UTC、错误管理、电池电量、存储数据、上位机通信、固件升级等等。将这些功能都做成不依赖于硬件的模块交给BabyOS管理。将调试好的外设驱动也交给BabyOS管理。再次启动项目时，通过配置文件，选择当前项目使用的功能模块。以搭积木的方式缩短开发时间。

![opt](https://github.com/notrynohigh/BabyOS/raw/master/doc/1.png)

​       

## 使用方法

###   1、添加文件

​        bos/core/src       核心文件及功能模块全部添加至工程

​        bos/driver/src    选择需要的驱动添加至工程

​        bos/hal/              添加至工程，根据具体平台进行修改

###   2、选择功能模块

​        对于b_config.h进行配置，根据自己的需要选择功能模块。



###   3、列出需要使用的设备

​           找到b_device_list.h，在里面添加使用的外设。例如当前项目只需要使用flash和模拟串口，那么添加如下代码：    

```c
//           设备        驱动接口      描述
B_DEVICE_REG(W25QXX, bW25X_Driver, "flash")
B_DEVICE_REG(SUART, SUART_Driver, "suart")
```

###   4、使用范例

​    

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
    b_log("save ip, name\r\n");
    bKV_Set((uint8_t *)"ip", (uint8_t *)"192.168.1.155", sizeof("192.168.1.155"));
    bKV_Set((uint8_t *)"name", (uint8_t *)"BabyOS", sizeof("BabyOS"));
    //获取值
    b_log("read ip, name...\r\n");
    bKV_Get((uint8_t *)"ip", buf);
    b_log("ip: %s\r\n", buf);
    bKV_Get((uint8_t *)"name", buf);
    b_log("name %s\r\n", buf); 
    //修改键值对的值
    b_log("change name...\r\n");
    bKV_Set((uint8_t *)"name", (uint8_t *)"abcde", sizeof("abcde"));
    //重新获取值
    bKV_Get((uint8_t *)"name", buf);
    b_log("new name: %s\r\n", buf); 
    //......
    while(1)
    {
        //.....
        bExec();      //循环调用此函数
        //.....
    }
}


```



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



## Baby如何成长

​        之所以称之为BabyOS，从上面的介绍可以看出，她如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。

​       希望借助广大网友的力量，一起“喂养”她，是她成为MCU裸机开发中不可缺少的一部分。

​       码云：<https://gitee.com/notrynohigh/BabyOS>

​       github：<https://github.com/notrynohigh/BabyOS>



# 友情项目

​	BabyOS包含了第三方开源代码，这部分代码都是MCU项目中比较实用的。

​        b_shell 功能模块基于开源项目nr_micro_shell，<https://gitee.com/nrush/nr_micro_shell>，感谢作者Nrush

​        b_button 功能模块基于开源项目FlexibleButton，<https://github.com/murphyzhao/FlexibleButton>，感谢作者Murphy



**如果您觉得这套开源代码有意义，请给个Star表示支持，谢谢！**



管理员邮箱：notrynohigh@outlook.com

开发小组群：

![qqg](https://github.com/notrynohigh/BabyOS/raw/master/doc/qqg.png)



## 更新记录

| 日期    | 新增项                                                    | 备注                                                         |
| ------- | --------------------------------------------------------- | ------------------------------------------------------------ |
| 2019.12 | 功能模块：FIFO, AT, Nr_micro_shell, Lunar calendar        | [详情见wiki](https://github.com/notrynohigh/BabyOS/wiki/2019%E5%B9%B412%E6%9C%88%E6%9B%B4%E6%96%B0%E8%AF%B4%E6%98%8E) |
| 2020.01 | 功能模块：KV存储                                          | [详情见wiki](https://github.com/notrynohigh/BabyOS/wiki/2020%E5%B9%B41%E6%9C%88%E6%9B%B4%E6%96%B0%E8%AF%B4%E6%98%8E) |
| 2020.02 | 功能模块：Xmodem128, Ymodem, FlexibleButton 驱动：xpt2046 | [详情见wiki](https://github.com/notrynohigh/BabyOS/wiki/2020%E5%B9%B42%E6%9C%88%E6%9B%B4%E6%96%B0%E8%AF%B4%E6%98%8E) |

