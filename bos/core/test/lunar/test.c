#include <stdio.h>
#include "b_lunar.h"



int main()
{
    int year, month, day;
    bLunarInfo_t result;
    while(1)
    {
        printf(">>enter year month day\r\n");
        scanf("%d %d %d", &year, &month, &day);
        bSolar2Lunar(year, month, day, &result);
        printf("lunar:%d-%d-%d\r\n", result.year, result.month, result.day);
    }
    return 0;
}
