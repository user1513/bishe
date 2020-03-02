#include "BSP_DS18B20.h"
#include "intrins.h"




/***********ds18b20�ӳ��Ӻ���������12MHz ��*******/ 
/************DS18B20��ʱ��Ҫ�����,��ֻ�ܳ����ܶ�
*************��11.0592M��Ҳ��,��Ϊʱ�䳤Щ********/
static void delay_18B20(unsigned int i)
{
	while(i--);
}

/**********ds18b20��ʼ������**********************/

static void Init_DS18B20(void) 
{
	 unsigned char x=0;
	 DQ = 1;          //DQ��λ
	 delay_18B20(8);  //������ʱ
	 DQ = 0;          //��Ƭ����DQ����
	 delay_18B20(80); //��ȷ��ʱ ���� 480us
	 DQ = 1;          //��������
	 delay_18B20(14);
	 x=DQ;            //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	 delay_18B20(20);
}

/***********ds18b20��һ���ֽ�**************/  

static unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	 {
		  DQ = 0; // �������ź�
		  dat>>=1;
		  DQ = 1; // �������ź�
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(4);
	 }
 	return(dat);
}

/*************ds18b20дһ���ֽ�****************/  

static void WriteOneChar(unsigned char dat)
{
 	unsigned char i=0;
 	for (i=8; i>0; i--)
 	{
  		DQ = 0;
 		DQ = dat&0x01;
    	delay_18B20(5);
 		DQ = 1;
    	dat>>=1;
 }
}

/**************��ȡds18b20��ǰ�¶�************/

int Ds18b20ReadTemp(void)
{	
	int temp = 0;
	Init_DS18B20();
	
	delay_18B20(80);       // this message is wery important
	Init_DS18B20();
	WriteOneChar(0xCC);    	// ����������кŵĲ���
	WriteOneChar(0x44); 	// �����¶�ת��

	delay_18B20(80);       // this message is wery important
	Init_DS18B20();
	WriteOneChar(0xCC); 	//����������кŵĲ���
	WriteOneChar(0xBE); 	//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	delay_18B20(80);

	temp = ReadOneChar();    	//��ȡ�¶�ֵ��λ
	temp += ReadOneChar() << 8;   		//��ȡ�¶�ֵ��λ
	return temp;
}




