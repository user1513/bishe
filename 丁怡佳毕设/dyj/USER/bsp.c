#include "bsp.h"

void bspInit(void)
{
    delay_init(168);		        /*初始化延时函数*/

	bspLedInit();		            /*初始化LED端口*/

	EventRecorderInitialize(EventRecordAll, 1U);/*事件记录器初始化*/

	EventRecorderStart();           /*事件记录器启动*/

    TIM3_Int_Init(9,8399);          /*timer3 = 84Mhz, 设定溢出时间为1ms*/

    vDhtxx_Init();                  /*DHT22初始化函数*/
	
	OLED_Init();					/*OLED初始化函数*/

	bspBuzzerInit();				/*蜂鸣器初始化*/

	bspBuzzerFreq(50, 1500);		/*蜂鸣器跳变周期100ms,总响应时间为5000ms*/

	bsp_sgp30_io_init();			/*sgp30传感器外设初始化*/

	bsp_adc_init();					/*adc1通道0初始化*/

	TIM2_PWM_Init(999, 839);		/*PWM周期初始化*/

	TIM_SetCompare2(TIM2, 32);			/*PWM占空比初始化*/

}

