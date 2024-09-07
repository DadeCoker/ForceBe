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

​        某天、老大说，现在对项目开发有两个要求：**开发时间**和**产品功耗**。99.874%产品是电池供电，功耗是重点考虑对象。产品的功能都不复杂，项目之间有可复用的功能，是否有套代码能够去掉重复的工作，加速产品demo的开发。

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

###   4、使用设备

​    

```c
bInit();    //初始化，外设的初始化会在此处调用
//下面举例使用设备
int fd;
fd = bOpen(SUART, BCORE_FLAG_RW);    //其中SUART是在b_device_list.h中添加的设备
if(fd >= 0)
{
     bWrite(fd, (uint8_t *)"hello world\r\n", strlen("hello world\r\n")); //发送字符串
     bClose(fd);
}
```

   如果一个设备被打开正在使用，那么无法再次被打开。

###   5、使用功能模块

```c
int sdb_no;
sdb_no = bSDB_Regist(0, 1, W25QXX);//创建B类数据存储实例，指定设备W25QXX，获的功能模块实例IDsdb_no
//sdb_no大于等于0则有效
int bSDB_Write(int no, uint8_t *pbuf);
int bSDB_Read(int no, uint8_t *pbuf);
//读写函数传入实例ID sdb_no
```


更多使用介绍： https://github.com/notrynohigh/BabyOS/wiki



## Baby如何成长

​        之所以称之为BabyOS，从上面的介绍可以看出，她如果能在项目中发挥大的作用就需要有足够的功能模块以及驱动代码。希望借助广大网友的力量，一起“喂养”她，是她成为MCU裸机开发中不可缺少的一部分。

notrynohigh@outlook.com

![qqg](https://github.com/notrynohigh/BabyOS/raw/master/doc/qqg.png)