#include<reg52.h>
#include"lcd.h"
#include"uart.h"


void uart_init()
{
	SCON=0x40;
	PCON = 0X00;				//�����ʲ��ӱ�
	REN=1;						//���ͽ�������
	T2MOD = 0X01;
	T2CON = 0X30;			   	//����T2λ������ʱ��
	RCAP2H = 0xff;			   	//65536-11059200/32��9600
	RCAP2L = 0xdc;			  	//��ʼ������ֵ 11.0592Mhz 9600������
	TH2 = 0xff;				  	//��ʼ������ֵ 11.0592Mhz 9600������
	TL2 = 0xdc;				  
	SM0=0;					  	//����ģʽ8Ϊ����Ϊ��1λֹͣλ
	SM1=1;					  	//����ģʽ8Ϊ����Ϊ��1λֹͣλ
	TR2=1;					  	//��ʱ��2��ʼ����
	TI=0;					  	//����жϱ�־λ
}



void send_char(uchar dat)		//����һ���ֽ�����
{
	SBUF=dat;
	while(!TI);
	TI=0;
}
void send_string(uchar *da)		//�����ַ�������
{
	while(*da!='\0')
	{
		send_char(*da);
		da++;
	}
}

void send_num(uint *num)		 //���Ͳ������ݣ�ÿ��������������ֽڣ����ֽ���ǰ���ֽ��ں󣬹�����10���ֽڣ���������ֽ�Ϊ0d0a����֡
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