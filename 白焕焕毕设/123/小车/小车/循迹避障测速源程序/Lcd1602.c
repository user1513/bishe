
#include "std_for_wym.h"


//Һ��д����
void LCDWriteCom(unsigned char com)
{
	RS_L;
	RW_L;
	READ_DATA = com;
	EN_H;
	DelayMs(2);
	EN_L;
}
//Һ��д����
void LCDWriteData(unsigned char dat)
{
	RS_H;
	RW_L;
	READ_DATA = dat;
	EN_H;
	DelayMs(2);
	EN_L;
}
/*******************************************************************************
**�������ƣ�LCD_Write_str()
**�������ܣ���LCD��д��һ���ַ�
**�� �� �ڣ�hang : Ҫд����У�add Ҫд����   *sҪд���ָ������
**�� �� �ڣ���
**�� �� ֵ����
**��    ע��strlen()�����ÿ⺯��string.h  ,�����������ĳ���
**��    �ڣ�2014.2.22
*******************************************************************************/
void LcdWriteStr(unsigned char hang,unsigned char add,char*s)
{
    unsigned char i;
    unsigned char length = 0;
	if(hang==1)   
		LCDWriteCom(0x80+add);
	else
		LCDWriteCom(0x80+0x40+add);
    length = strlen(s);
    for(i=0;i<length;i++)
      LCDWriteData(*s++);          //ָ���������ݺ��Լ�һ
}
/*void LcdWriteChar(unsigned char hang,unsigned char add,char Ch)
{
	if(hang == 1)LCDWriteCom(0x80+add);
	else LCDWriteCom(0x80+0x40+add);
	LCDWriteData(Ch);
}*/

//����printfһ��ʹ��
void Lcd1602Printf( unsigned char x, unsigned char y, unsigned char *p,...)
{
	char idata LcdBuf[17];
  va_list ap;
  va_start(ap, p);
	vsprintf(LcdBuf,p,ap);
	va_end(ap);
  LcdWriteStr(x,y,LcdBuf);
}

void Init_1602(void)
{
	 LCDWriteCom(0x38);	  //����16*2��ʾ��5*7����8λ���ݽӿ�
	 LCDWriteCom(0x0c);	  //����ʾ������ʾ���
	 LCDWriteCom(0x06);	  //��ַ��1����д�����ݵ�ʱ��������
	 LCDWriteCom(0x01);	  // ����
	 DelayMs(2);
	 //Lcd1602Printf(1,0," Lcd1602 Normal ");    //����Һ��1602��ʾ��  Normal:����
}









