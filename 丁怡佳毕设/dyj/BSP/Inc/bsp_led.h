#ifndef __LED_H
#define __LED_H
#include "sys.h"
	


//LED�˿ڶ���
#define LED0 PCout(0)	// DS0 

#define FS0 PAout(6)	// DS0 

void bspLedInit(void);//��ʼ��	

void bspLedFlash(void);//PC0������˸

#endif
