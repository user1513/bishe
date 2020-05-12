#include<reg52.h>
#include "lcd.h"



void delay(uint z)
{
	uint i,j;
	for(i=100;i>0;i--)
		for(j=z;j>0;j--);
}
void write_com(uchar com)						   //对LCD1602写指令操作，时序可看芯片手册
{

	lcdrs=0;
	P2=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar dat)						 //对LCD1602写数据操作，时序可看芯片手册
{
	lcdrs=1;
	P2=dat;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}

void lcd_init()								  //初始化LCD，并且光标从第一行第一个位置开始写
{
	lcden=0;
	lcdrw = 0 ;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);

}

//void display_string(uchar num, uchar *string)		 //LCD显示字符串函数，输入参数可设置写入位置
//{
//	write_com(0x80 + num);
//	while(*string != '\0')
//	{
//		write_data(*string++);
//	}
//}
void display_res(uint pos, uint d_tab)						//显示最终距离函数
{
	uchar a,b,c,d,e = 0;
	a = d_tab / 10000;
	b = d_tab / 1000 %10;
	c = d_tab /100 %10;
	d = d_tab /10 % 10;
	e = d_tab % 10;

	write_com(pos);
	write_data(0x30 + a);
	write_data(0x30 + b);
	write_data(0x30 + c);
	write_data(0x30 + d);
	write_data(0x30 + e);
	write_com(0x02);
}