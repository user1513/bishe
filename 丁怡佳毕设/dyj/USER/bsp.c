#include "bsp.h"

void bspInit(void)
{
    delay_init(168);		        /*��ʼ����ʱ����*/

	bspLedInit();		            /*��ʼ��LED�˿�*/

	EventRecorderInitialize(EventRecordAll, 1U);/*�¼���¼����ʼ��*/

	EventRecorderStart();           /*�¼���¼������*/

    TIM3_Int_Init(9,8399);          /*timer3 = 84Mhz, �趨���ʱ��Ϊ1ms*/

    vDhtxx_Init();                  /*DHT22��ʼ������*/
	
	OLED_Init();					/*OLED��ʼ������*/

	bspBuzzerInit();				/*��������ʼ��*/

	bspBuzzerFreq(50, 1500);		/*��������������100ms,����Ӧʱ��Ϊ5000ms*/

	bsp_sgp30_io_init();			/*sgp30�����������ʼ��*/

	bsp_adc_init();					/*adc1ͨ��0��ʼ��*/

	TIM2_PWM_Init(999, 839);		/*PWM���ڳ�ʼ��*/

	TIM_SetCompare2(TIM2, 32);			/*PWMռ�ձȳ�ʼ��*/

}

