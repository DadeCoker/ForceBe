#ifndef __ALGO_SPEEDPID_H__
#define __ALGO_SPEEDPID_H__

#include <stdint.h>

struct PID
{
    double SetSpeed;     //定义设定值
    double ActualSpeed;  //定义实际值
    double err;          //定义偏差值
    double err_last;     //定义上一个偏差值
    double Kp, Ki, Kd;   //定义比例、积分、微分系数
    double voltage;      //定义电压值（控制执行器的变量）
    double integral;     //定义积分值
};

#endif
