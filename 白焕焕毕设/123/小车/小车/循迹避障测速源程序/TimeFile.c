/////////��ʱ����

#include "std_for_wym.h"

//bit bTime_50Ms = 0;
bit bTime_500Ms = 0;
bit time_1000ms = 0;
//��ʱ����ʼ������
void InitTimer(void)
{
	TMOD&=0X0F;
	TMOD|=0X10;     //��ʱ��0�����ڷ�ʽ1
	TH1=0X4C;
	TL1=0X00;       //50MS
	ET1=1;          //�򿪶�ʱ��0
	TR1=1;          //������ʱ��0
	EA=1;           //�����ж�
}

void Time1(void) interrupt 3   
{
	//static char num;
	static char num1;
	TH1=0X4C;
	TL1=0X00;       //50MS 11.0592MHz
	//bTime_50Ms = 1;
//	if(++num >= 10)
//	{
//		num = 0;
//		bTime_500Ms = 1;
//	}
	if(++num1 >= 20)
	{
		num1 = 0;
		time_1000ms=1;
	}
}




