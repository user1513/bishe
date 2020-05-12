#include<reg52.h>
#include "lcd.h"



void delay(uint z)
{
	uint i,j;
	for(i=100;i>0;i--)
		for(j=z;j>0;j--);
}
void write_com(uchar com)						   //��LCD1602дָ�������ʱ��ɿ�оƬ�ֲ�
{

	lcdrs=0;
	P2=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar dat)						 //��LCD1602д���ݲ�����ʱ��ɿ�оƬ�ֲ�
{
	lcdrs=1;
	P2=dat;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}

void lcd_init()								  //��ʼ��LCD�����ҹ��ӵ�һ�е�һ��λ�ÿ�ʼд
{
	lcden=0;
	lcdrw = 0 ;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);

}

//void display_string(uchar num, uchar *string)		 //LCD��ʾ�ַ����������������������д��λ��
//{
//	write_com(0x80 + num);
//	while(*string != '\0')
//	{
//		write_data(*string++);
//	}
//}
void display_res(uint pos, uint d_tab)						//��ʾ���վ��뺯��
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