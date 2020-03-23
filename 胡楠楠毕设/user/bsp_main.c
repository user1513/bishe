#include "inc.h"




void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	i = 2;
	j = 103;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



void lcd_display(void);
char xdata str[17] = 0;
void main(void)
{

	double *val = (void*)0;
	lcd_display();
	Timer0Init();
	UartInit();
	while(1)
	{
		ET0 = 0;
		val = Get_ES_Table_Val();
		sprintf(str, "v1:%5.3fv2:%5.3f\n",(float)val[0],(float)val[1]);
		Send_String(str);
		LcdShowStrToLength(0x80 ,str,16);
		sprintf(str, "v3:%5.3fv4:%5.3f\n",(float)val[2],(float)val[3]);
		Send_String(str);
		LcdShowStrToLength(0x80+ 0x40,str,16);
		ET0 = 1;
		Delay200ms();

	}
}

void lcd_display(void)
{
	LcdInit();	
	LcdShowStr(0x80 ,"   Welcome use  ");
	LcdShowStr(0x80 + 0x40 ,"Electronic scale");
	Delay200ms();
	Delay200ms();
	LcdShowStr(0x80 ,"  class: b16   ");
	LcdShowStr(0x80+ 0x40,"   name: XXX    ");
}

































