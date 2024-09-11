#include <stdio.h>
#include "b_fifo.h"


uint8_t fifo_buf[1024];
uint8_t test_buf[32];

int main()
{
    int type, val;
    printf("hello\r\n");
    int fifo_no = bFIFO_Regist(fifo_buf, 1024);
    printf("regist fifo_no:%d\r\n",fifo_no);
    while(1)
    {
        scanf("%d %d",&type, &val );
        printf("t:%d num:%d\r\n", type, val);
        if(type)
	{
            val = (val <= 11) ? val : 11;
            bFIFO_Write(fifo_no, "hello world", val);
           if(val < 0)
	   {
               printf("write error \r\n");
	       continue;
           }
        }
	else
	{
	   val =  bFIFO_Read(fifo_no, test_buf, val);
           if(val < 0)
	   {
               printf("read error \r\n");
	       continue;
           }
	   test_buf[val] = 0;
           printf("%d s:%s\r\n", val, test_buf);
        }
        uint16_t f_len;
        bFIFO_Length(fifo_no, &f_len);
        printf("len:%d\r\n", f_len);
    }
    return 0;
}
