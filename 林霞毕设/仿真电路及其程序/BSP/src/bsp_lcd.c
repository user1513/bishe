#include "inc.h"

/*******************************************************************************
* �� �� ��         : Delay1ms
* ��������		   : ��ʱ��������ʱ1ms
* ��    ��         : c
* ��    ��         : ��
* ˵    ��         : �ú�������12MHZ�����£�12��Ƶ��Ƭ������ʱ��
*******************************************************************************/

//void Delay1ms(uchar z)		//@11.0592MHz
//{
//	unsigned char i, j,k;
//	for(k=0;k<z;k++)
//	{
//		_nop_();
//		i = 2;
//		j = 199;
//		do
//		{
//			while (--j);
//		} while (--i);
//	}
//}

void Delay100us()		//@11.0592MHz
{
	unsigned char i;

//	_nop_();
//	i = 43;
//	while (--i);
	

	_nop_();//ʵ����ʱ50us//����ʱ����
	i = 20;
	while (--i);
}


/*******************************************************************************
* �� �� ��         : LcdWriteCom
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : com
* ��    ��         : ��
*******************************************************************************/
#ifndef 	LCD1602_4PINS	 //��û�ж������LCD1602_4PINSʱ
void LcdWriteCom(uchar com)	  //д������
{
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	//Delay1ms(1);		//�ȴ������ȶ�
	Delay100us();
	LCD1602_E = 1;	          //д��ʱ��
	Delay100us();	  //����ʱ��
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)	  //д������
{
	LCD1602_E = 0;	 //ʹ������
	LCD1602_RS = 0;	 //ѡ��д������
	LCD1602_RW = 0;	 //ѡ��д��

	LCD1602_DATAPINS = com;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	 //д��ʱ��
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;

//	Delay1ms(1);
	LCD1602_DATAPINS = com << 4; //���͵���λ
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	 //д��ʱ��
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/		   
#ifndef 	LCD1602_4PINS		   
void LcdWriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;   //д��ʱ��
	Delay100us();   //����ʱ��
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	  //ʹ������
	LCD1602_RS = 1;	  //ѡ��д������
	LCD1602_RW = 0;	  //ѡ��д��

	LCD1602_DATAPINS = dat;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	  //д��ʱ��
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; //д�����λ
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	  //д��ʱ��
	Delay100us();
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* �� �� ��       : LcdInit()
* ��������		 : ��ʼ��LCD��
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/		   
#ifndef		LCD1602_4PINS
void LcdInit()						  //LCD��ʼ���ӳ���
{
 	LcdWriteCom(0x38);  //����ʾ
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
}
#else
void LcdInit()						  //LCD��ʼ���ӳ���
{
	LcdWriteCom(0x32);	 //��8λ����תΪ4λ����
	LcdWriteCom(0x28);	 //����λ���µĳ�ʼ��
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
}
#endif

void LcdShowStr(char cAddr,char *pStr)
{
	while(*pStr != '\0')
	{
		LcdWriteCom(cAddr);
		cAddr++;
		LcdWriteData(*pStr);
		pStr++;
	}
}

void lcdshowCelsius(char cAddr)//��ʾ���϶ȡ�
{
	LcdWriteCom(cAddr);
	cAddr++;
	LcdWriteData(0xdf);
	LcdWriteData('C');
}


