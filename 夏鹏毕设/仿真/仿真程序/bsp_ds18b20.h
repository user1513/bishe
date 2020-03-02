#ifndef __BSP_DS18B20_H_
#define __BSP_DS18B20_H_

#include <reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--定义使用的IO口--//
sbit DQ = P2^7;			//定义DQ引脚为P3.4

//--声明全局函数--//
int Ds18b20ReadTemp(void);

#endif
