#include "b_os.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
static int count = 0;
static struct itimerval oldtv;
struct itimerval itv;

void set_timer()
{
        itv.it_interval.tv_sec = 0;
        itv.it_interval.tv_usec = 1000;  //启动后的定时器每隔990ms唤醒一次
        itv.it_value.tv_sec = 0;
        itv.it_value.tv_usec = 50000;   //定时器在50ms后启动
        setitimer(ITIMER_REAL, &itv, &oldtv); 
}

void shut_timer()
{
        itv.it_value.tv_sec = 0;        //将启动参数设置为0,表示定时器不启动
        itv.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &itv, &oldtv);
}

void signal_handler(int m)
{
        bHalIncSysTick();
}

void test_log()
{
	b_log_i("hello bos\r\n");
}



int main()
{
        signal(SIGALRM, signal_handler);  //将SIGALRM信号与signal_handler函数建立关系,当信号触发时便会调用该函数.
        set_timer();
        bInit();
	while(1)
	{
		bExec();
		BOS_PERIODIC_TASK(test_log, 3000);
	}
}



