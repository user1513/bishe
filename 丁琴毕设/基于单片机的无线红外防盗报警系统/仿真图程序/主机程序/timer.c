#include "timer.h"

extern const uint8_t code HZ_tab[][16];
/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer0Init(void)
{
	TMOD|=0X01; //选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	ET0=1;      //打开定时器0中断允许
	EA=1;       //打开总中断
	TR0=1;      //打开定时器			
}

	uint8_t * _pDs18b20 = (uint8_t *)0;
	uint8_t * _pDs1302 = (uint8_t *)0;
	uint8_t key_val = 1;
void Timer0() interrupt 1
{
	static uint8_t delay_10ms = 0;
	uint8_t tmp = 0;
	int i = 0;
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
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