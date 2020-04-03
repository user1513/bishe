#ifndef BSP__H
#define BSP__H

#define LEDTIMEOUTFLAG      (1 << 0)/*LED超时标志位*/
#define DHT22TIMEOUTFLAG    (1 << 1)/*DHT22超时标志位*/
#define OLEDTIMEOUTFLAG     (1 << 2)/*OLED超时标志位*/




#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_led.h"
#include "EventRecorder.h"
#include "bsp_timer.h"
#include "bsp_dhtxx.h"
#include "bsp_oled.h"
#include "bsp_buzzer.h"
#include "bsp_swi2c.h"
#include "bsp_sgp30.h"


#include "CRC.h" /*CRC8_CRC16_CRC32检验*/

void bspInit(void);

#endif



