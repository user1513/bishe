#ifndef BSP__H
#define BSP__H

#define LEDTIMEOUTFLAG      (1 << 0)/*LED��ʱ��־λ*/
#define DHT22TIMEOUTFLAG    (1 << 1)/*DHT22��ʱ��־λ*/
#define OLEDTIMEOUTFLAG     (1 << 2)/*OLED��ʱ��־λ*/

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_led.h"
#include "EventRecorder.h"
#include "bsp_timer.h"
#include "bsp_dhtxx.h"
#include "bsp_oled.h"
#include "bsp_buzzer.h"
void bspInit(void);

#endif



