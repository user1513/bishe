#include "timer.h"

extern const uint8_t code HZ_tab[][16];
/*******************************************************************************
* �� �� ��         : Timer0Init
* ��������		   : ��ʱ��0��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0Init(void)
{
	TMOD|=0X01; //ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	ET0=1;      //�򿪶�ʱ��0�ж�����
	EA=1;       //�����ж�
	TR0=1;      //�򿪶�ʱ��			
}

	uint8_t * _pDs18b20 = (uint8_t *)0;
	uint8_t * _pDs1302 = (uint8_t *)0;
	uint8_t key_val = 1;
void Timer0() interrupt 1
{
	static uint8_t delay_10ms = 0;
	uint8_t tmp = 0;
	int i = 0;
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xDC;		//���ö�ʱ��ֵ
	delay_10ms ++;
	tmp = kscanf();
	if(delay_10ms == 50)
	{
		if(tmp != key_val)
		{
			key_val = tmp;
			if(key_val)
			{
				LCD_ShowChinaLanguage(6 ,3 , &HZ_tab[43*2][0], 2);
			}
			else
			{
				LCD_ShowChinaLanguage(6 ,3 , &HZ_tab[45*2][0], 2);
			}
		}
	}
	if(delay_10ms == 50)
	{	
		delay_10ms = 0;
		
	}
	
	
	
}