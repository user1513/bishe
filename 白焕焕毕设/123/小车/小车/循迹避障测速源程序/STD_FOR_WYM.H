
#ifndef __STD_FOR_WYM_H__
#define __STD_FOR_WYM_H__

#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>    //va_list 的头文件


//类型声明 
typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;
typedef unsigned char BYTE;

//包含所需要的头文件
#include "delay.h"
#include "TimeFile.h"
#include "Lcd1602.h"
#include "Csb.h"

#define PI 314   //圆周率
#define D  67L        //单位：米

extern void xunnji();

#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)

#endif


