#include "inc.h"

void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	i = 2;
	j = 103;
	k = 0;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




void lcd_display(void);
extern uint8_t showflag;
void main(void)
{
    xdata float _fhumi,_ftemp;
	
	static uint8_t time_flag = 0;
	static float humi_flag = 0;
	static float temp_flag = 0;
	
	
	uchar* pTime = (void*) 0;
    xdata char table[17];
	lcd_display();
	Timer0Init();
	UartInit();
    s_connectionreset();
	ds1302_init(); //DS1302初始化
	ds1302_write_time(); //写入初始值
	while(1)
	{
        switch(showflag){
			case 0:
				_fhumi = fGetHumi();
				_ftemp = fGetTemp();
				if(temp_flag != _ftemp){
					sprintf(table, "TEMP:%05.1f  ", _ftemp);
					LcdShowStr(0x80,table);
					lcdshowCelsius(0x80 + 12);
					temp_flag = _ftemp;
				}
				if(humi_flag != _fhumi){
					sprintf(table, "HUMI:%05.1f  %%   ",_fhumi);
					LcdShowStr(0x80 + 0x40,table);	
					humi_flag = _fhumi;
				}
			break;
			case 1:
				pTime = GetTime();
				if(pTime[13] != time_flag)
				{
					temp_flag = 0;
					humi_flag = 0;
					time_flag = pTime[13];
					sprintf(table, "Y:%04dM:%02dD:%02d  ", (int)(pTime[0]*1000 + pTime[1]*100+pTime[2]*10 + pTime[3]),
														  (int)(pTime[4]*10 + pTime[5]),
														  (int)(pTime[6]*10 + pTime[7]));
					LcdShowStr(0x80,table);									
					sprintf(table, "H:%02dM:%02dS:%02d W:%d", (int)(pTime[8]*10 + pTime[9]),
														  (int)(pTime[10]*10 + pTime[11]),
														  (int)(pTime[12]*10 + pTime[13]),
														  (int)(pTime[14]));									  
					LcdShowStr(0x80 + 0x40,table);	
				}
			break;
			default:break;
		}
			
        
        
	}
}
void lcd_display(void)
{
	LcdInit();	
	LcdShowStr(0x80 ,"   Welcome use  ");
	LcdShowStr(0x80 + 0x40 ,"  Thermometer   ");
	Delay200ms();
	Delay200ms();
	LcdShowStr(0x80 ,"  class: b16   ");
	LcdShowStr(0x80+ 0x40,"   name: LL     ");
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	LcdShowStr(0x80,"HUMI:XXXXX  ");
	lcdshowCelsius(0x80 + 12);
	LcdShowStr(0x80 + 0x40,"TEMP:XXXXX  % ");	
}

































