#include "app.h"

extern const uint8_t code HZ_tab[][16];

sbit buzzer = P3^7; 
sbit key = P2^7;
sbit LED = P3^6;

uint8_t flag = 0;
uint8_t able[2] = {0};

extern uint8_t key_val;

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


int main(void)
{
	int i = 0;
	
	buzzer = 1;
	LED = 1;
	InitLcd(); 
	LED_ShowString(0, 0, "****************");
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[0][0], 8);
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[8*2][0], 8);
	LED_ShowString(6, 0, "****************");
	
	Delay1000ms();
	Delay1000ms();
	SetClear();
	LCD_ShowChinaLanguage(0 ,0 , &HZ_tab[16*2][0], 5); //姓名：丁琴
	LCD_ShowChinaLanguage(2 ,0 , &HZ_tab[21*2][0], 3);//学号：
	LED_ShowString(2, 6, "160211202");
	LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[24*2][0], 8);
	LCD_ShowChinaLanguage(6 ,0 , &HZ_tab[40*2][0], 5);
	
	Timer0Init();
	uart_init();
	while(1)
	{
			
			if(flag == 1) // 如果收到数据 
		{ 
				ES = 0; //关闭串口中断

				flag = 0; //标志位清零
				for(i = 0; i < 2; i++)
			{
				SBUF = *(able + i); //将收到的数据发送至缓冲区 

				while(!TI); //等待数据发送结束
	
				TI=0; // 将发送结束标志清零
			}
			if(able[0] == 'O' && able[1] == 'K')
			{
				LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[32*2][0], 8);
			}
			else
			{
				LCD_ShowChinaLanguage(4 ,0 , &HZ_tab[24*2][0], 8);
			}
			if(able[0] == 'O' && able[1] == 'K'&&key_val == 1) {buzzer = 0; LED = 0;}
			else if(able[0] == 'N' && able[1] == 'O'){buzzer = 1; LED = 1;}
				ES=1; // 使能串口中断
		}
	}
}