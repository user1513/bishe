#ifndef BUZZER__H
#define BUZZER__H

#include "bsp.h"

#define BEEP PAout(5)

void bspBuzzerInit(void);

void bspBuzzerFlash(void);

/*��������������*/
/*���Ƶ�� 500Hz*/
/*������� 2ms*/

void bspBuzzerFreq(uint16_t  _usPeriod,uint16_t _usTotalPeriod);

/*������������*/

void bspBeepHandler(void);

/*����������ʹ��*/

void bspBeepEnable(void);

/*����������ʧ��*/

void bspBeepDisable(void);

#endif
