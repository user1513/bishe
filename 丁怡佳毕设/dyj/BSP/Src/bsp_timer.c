#include "bsp_timer.h"



//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz  84Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
extern uint8_t g_ucStateFlag;

uint16_t g_usDht22Timeout = 0;		//DHT22���������
#define DHT22TIMEOUT 2000			//DHT22���2000MS

uint16_t g_usLedTimeout = 0;		//LED��˸���������
#define LEDTIMEOUT 500				//LED��˸���500MS

uint16_t g_usOledTimeout = 0;		//Oled updateʱ�������
#define OLEDTIMEOUT 1500				//Oled ���»�����500MS

uint16_t g_usSgp30Timeout = 0;		//Oled updateʱ�������
#define SGP30TIMEOUT 1000				//Oled ���»�����500MS

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		if(g_usLedTimeout == 0)
			EventStartA(0);

		g_usLedTimeout++;
		
		if(g_usLedTimeout > LEDTIMEOUT)  //�����������趨��˸���
		{
			g_usLedTimeout = 0;
			EventStopA(0);
			g_ucStateFlag &= ~LEDTIMEOUTFLAG;//������λ����
		}
//		if(g_usDht22Timeout == 0)
//			EventStartA(0);
		g_usDht22Timeout++;	

		if(g_usDht22Timeout > DHT22TIMEOUT)  //�����������趨
		{
			g_usDht22Timeout = 0;	

			g_ucStateFlag &= ~DHT22TIMEOUTFLAG;//����һλ����	
		}
//		if(g_usOledTimeout == 0)
//			EventStartB(0);
		g_usOledTimeout++;

		if(g_usOledTimeout > OLEDTIMEOUT)  //�����������趨
		{
			g_usOledTimeout = 0;
			
			g_ucStateFlag &= ~OLEDTIMEOUTFLAG;//���ڶ�λ����
		}


		g_usSgp30Timeout++;

		if(g_usSgp30Timeout > SGP30TIMEOUT)  //�����������趨
		{
			g_usSgp30Timeout = 0;
			
			g_ucStateFlag &= ~SGP30TIMEOUTFLAG;//������λ����
		}


		bspBeepHandler(); 				/*beep������������*/
	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
