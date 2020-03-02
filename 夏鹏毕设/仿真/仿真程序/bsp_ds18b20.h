#ifndef __BSP_DS18B20_H_
#define __BSP_DS18B20_H_

#include <reg52.h>
//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--����ʹ�õ�IO��--//
sbit DQ = P2^7;			//����DQ����ΪP3.4

//--����ȫ�ֺ���--//
int Ds18b20ReadTemp(void);

#endif
