#include<reg52.h>
#include"lcd.h"
#include"uart.h"


void uart_init()
{
	SCON=0x40;
	PCON = 0X00;				//波特率不加倍
	REN=1;						//发送接收是能
	T2MOD = 0X01;
	T2CON = 0X30;			   	//设置T2位波特率时钟
	RCAP2H = 0xff;			   	//65536-11059200/32×9600
	RCAP2L = 0xdc;			  	//初始化计数值 11.0592Mhz 9600波特率
	TH2 = 0xff;				  	//初始化计数值 11.0592Mhz 9600波特率
	TL2 = 0xdc;				  
	SM0=0;					  	//串口模式8为数据为，1位停止位
	SM1=1;					  	//串口模式8为数据为，1位停止位
	TR2=1;					  	//定时器2开始计数
	TI=0;					  	//清除中断标志位
}



void send_char(uchar dat)		//发送一个字节数据
{
	SBUF=dat;
	while(!TI);
	TI=0;
}
void send_string(uchar *da)		//发送字符串数据
{
	while(*da!='\0')
	{
		send_char(*da);
		da++;
	}
}

void send_num(uint *num)		 //发送测量数据，每个数据最多两个字节，高字节在前低字节在后，共传输10个字节，最后两个字节为0d0a结束帧
{
	int i;
	uchar tab[10] = {0};
	
	for(i=0 ; i < 4; i++)
	{
		tab[2 * i + 0] = (num[i] & 0xff00) >> 8;
		tab[2 * i + 1] = (num[i] & 0x00ff);
	}
	tab[8] = 0x0d;
	tab[9] = 0x0a;
	 
	for(i=0;i<10;i++)
	{
		send_char(tab[i]);
	}
}