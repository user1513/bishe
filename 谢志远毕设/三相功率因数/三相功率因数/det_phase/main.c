#include<reg52.h>

#include "lcd.h"
#include "uart.h"


uchar flag_edge = 0;
uint times = 0;


uint freq = 0;
uint period = 0;


uint flag2 = 0;
uint time_1s = 0;
ulong final_res[4] = {12345,23456,98765,6789};


void timer_init()
{
	ET1 = 0;
	TMOD=0x10;
	TH1 = 0;		  
	TL1 = 0;

	EX0 = 1;
	EX1 = 1;

	TR1=0;
	EA=1;					  //¿ªÆôÖÕ¶Ë¿ª¹Ø
}
void main()
{
	uint phase_num = 0;
	uchar channel_num = 0;
	lcd_init();
	uart_init();
	timer_init();

	while(1)
	{ 
		if(flag_edge == 1)
		{
			flag_edge = 0;
			//display_res(0x80,times);
		} 	
	}
}



void INT0_isr() interrupt 0
{
	TR1 = 1;
}

void INT1_isr() interrupt 2
{
	TR1 = 0;
	times = (TH1 << 8) + TL1;
	display_res(0x80,times);
	TH1 = 0;
	TL1 = 0;
	flag_edge = 1;
	


}