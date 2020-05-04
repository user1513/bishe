#include "app.h"
#include "NRF24L01.h"
#include "ds1302.h"
#include "usart.h"
#include "timer.h"
#include "lcd12864.h"
#include "stdio.h"
#include "temp.h"
#include "VS1838B.h"


extern xdata unsigned char IRCOM[];
extern uint8_t xdata RX_BUF[];




uint32_t STATE_FLAG = 0; //״̬��־λ


bit vs1838b_flag = 0;
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
xdata uint8_t able[16] = {0};
void main()
{
	
	uint8_t data_ = 0;
	uint16_t temp = 0;
	P3M0=0x08;
	P33 = 0;
	UartInit();
	Delay1000ms();
	Lcm_Init();
	Delay1000ms();
	Lcm_Init();
	Delay1000ms();
	Lcm_Init();
	Wr_Data_String(0x80, (unsigned char *)"****************");
	Wr_Data_String(0x90, (unsigned char *)"���ڵ�Ƭ��������");
	Wr_Data_String(0x88, (unsigned char *)"�����������ϵͳ");
	Wr_Data_String(0x98, (unsigned char *)"****************");
	Delay1000ms();
	Delay1000ms();
	P33 = 1;
	Delay1000ms();
	P33 = 0;
	Wr_Data_String(0x80, (unsigned char *)"������  ��  ��  ");
	Wr_Data_String(0x90, (unsigned char *)"ѧ�ţ�160211202 ");
	Wr_Data_String(0x88, (unsigned char *)"���棺һ�а�ȫ!!");
	Wr_Data_String(0x98, (unsigned char *)"ģʽ��  ��  ��  ");
	//NRF24L01��ʼ��
	init_io();		           //���߳�ʼ��IO
	RX_Mode();		           //����Ϊ����ģʽ
	Timer0Init();
	vs1838b_Init();
	while(1)
	{
		if(STATE_FLAG & (1 << 0 ))
		{
			STATE_FLAG &= ~(1 << 0);

			if(IRCOM[6] == 0x34 && IRCOM[5] == 0x36)
			{
				IRCOM[6] = 0;
				IRCOM[5] = 0;
				vs1838b_flag = ~vs1838b_flag;
				if(vs1838b_flag == 0)
				{
					Wr_Data_String(0x9C, "��  ��  ");
				}
				else
				{
					Wr_Data_String(0x9C, "��  ��   ");
				}
			}
			
			
			if(vs1838b_flag == 0 && RX_BUF[2] == 0x01)
			{
				SendByte(0x88);
				P33 = ~P33;
			}	
			else
			{
				P33 = 0;
			}
			
			if(RX_BUF[2] == 0x01)
			{
				Wr_Data_String(0x8B, (unsigned char *)"���˿���!!");
			}
			else
			{
				Wr_Data_String(0x8B, (unsigned char *)"һ�а�ȫ!!");
			}
			NRF24L01_Send();
		}
			
			
//		Delay1000ms();
		
	}
}