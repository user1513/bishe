#include <reg52.h>
#include <intrins.h>
#include "math.h"
#include "BSP_DS18B20.h"
#include "bsp_usart.h"
#include "stdio.h"
#define uchar unsigned char	// �Ժ�unsigned char�Ϳ�����uchar����
#define uint  unsigned int	// �Ժ�unsigned int �Ϳ�����uint ����
#define CLAC_SPEED(X)  		X / 10000.0
float 	Audio_speed = 0.0f; 			//voice sound
float 	Temp_val = 0.0f; 			//voice sound
sfr ISP_DATA  = 0xe2;			// ���ݼĴ���
sfr ISP_ADDRH = 0xe3;			// ��ַ�Ĵ����߰�λ
sfr ISP_ADDRL = 0xe4;			// ��ַ�Ĵ����Ͱ�λ
sfr ISP_CMD   = 0xe5;			// ����Ĵ���
sfr ISP_TRIG  = 0xe6;			// ������Ĵ���
sfr ISP_CONTR = 0xe7;			// ����Ĵ���

sbit LcdRs_P   = P1^1;		// 1602Һ����RS�ܽ�       
sbit LcdRw_P   = P1^2;		// 1602Һ����RW�ܽ� 
sbit LcdEn_P   = P1^3;		// 1602Һ����EN�ܽ�
sbit Trig1_P   = P2^5;		// ������ģ��1��Trig�ܽ�
sbit Echo1_P   = P2^6;		// ������ģ��1��Echo�ܽ�
sbit Trig2_P   = P3^5;		// ������ģ��2��Trig�ܽ�
sbit Echo2_P   = P3^6;		// ������ģ��2��Echo�ܽ�
sbit Trig3_P   = P3^2;		// ������ģ��3��Trig�ܽ�
sbit Echo3_P   = P3^3;		// ������ģ��3��Echo�ܽ�
sbit Trig4_P   = P1^4;		// ������ģ��4��Trig�ܽ�
sbit Echo4_P   = P1^5;		// ������ģ��4��Echo�ܽ�
sbit KeySet_P  = P2^2;		// ���ð����Ĺܽ�
sbit KeyDown_P = P2^1;		// �������Ĺܽ�
sbit KeyUp_P   = P2^0;		// �Ӱ����Ĺܽ�
sbit Buzzer_P  = P2^3;		// �������Ĺܽ�
sbit Led1_P    = P2^4;		// ������1������
sbit Led2_P    = P3^4;		// ������2������
sbit Led3_P    = P1^6;		// ������3������
sbit Led4_P    = P1^0;		// ������4������

uint gAlarm;							// �����������


//�¶�������
//����һ��ʽ��
//V=331������(1+T/273)(m/S)
//T���������¶ȣ�V����T��ʱ������(����)
float datapros(int temp) 	 
{
   	float tp;  
	if(temp< 0)				//���¶�ֵΪ����
  	{
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp = temp-1;
		temp = ~temp;
		tp = - temp*0.0625;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
 
  	}
 	else
  	{			
		//����¶���������ô����ô������ԭ����ǲ���������
		tp = temp*0.0625;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}
	return tp;
}

float Sound_Speed_func()
{
	Temp_val = datapros(Ds18b20ReadTemp());
	Audio_speed = 331 * sqrt(1 + (Temp_val) / 273.0);
	return Audio_speed;
}
//���������
//û���ҵ����ʵı��,����
		





/*********************************************************/
// ��Ƭ���ڲ�EEPROM��ʹ��
/*********************************************************/
void ISP_Disable()
{
	ISP_CONTR = 0;
	ISP_ADDRH = 0;
	ISP_ADDRL = 0;
}


/*********************************************************/
// �ӵ�Ƭ���ڲ�EEPROM��һ���ֽڣ���0x2000��ַ��ʼ
/*********************************************************/
unsigned char EEPROM_Read(unsigned int add)
{
	ISP_DATA  = 0x00;
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x01;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	// ��STC89C51ϵ����˵��ÿ��Ҫд��0x46����д��0xB9,ISP/IAP�Ż���Ч
	ISP_TRIG  = 0x46;	   
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
	return (ISP_DATA);
}


/*********************************************************/
// ����Ƭ���ڲ�EEPROMдһ���ֽڣ���0x2000��ַ��ʼ
/*********************************************************/
void EEPROM_Write(unsigned int add,unsigned char ch)
{
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x02;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	ISP_DATA  = ch;
	ISP_TRIG  = 0x46;
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
}


/*********************************************************/
// ������Ƭ���ڲ�EEPROM��һ������
// д8�����������һ���ĵ�ַ���������������д��ǰҪ�Ȳ���
/*********************************************************/
void Sector_Erase(unsigned int add)	  
{
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x03;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	ISP_TRIG  = 0x46;
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
}



/*********************************************************/
// ���뼶����ʱ������time��Ҫ��ʱ�ĺ�����
/*********************************************************/
void DelayMs(uint time)
{
	uint i,j;
	for(i=0;i<time;i++)
		for(j=0;j<112;j++);
}


/*********************************************************/
// 1602Һ��д�������cmd����Ҫд�������
/*********************************************************/
void LcdWriteCmd(uchar cmd)
{ 
	LcdRs_P = 0;
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=cmd;
	DelayMs(2);
	LcdEn_P = 1;    
	DelayMs(2);
	LcdEn_P = 0;	
}


/*********************************************************/
// 1602Һ��д���ݺ�����dat����Ҫд�������
/*********************************************************/
void LcdWriteData(uchar dat)
{
	LcdRs_P = 1; 
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=dat;
	DelayMs(2);
	LcdEn_P = 1;    
	DelayMs(2);
	LcdEn_P = 0;
}


/*********************************************************/
// Һ����궨λ����
/*********************************************************/
void LcdGotoXY(uchar line,uchar column)
{
	// ��һ��
	if(line==0)        
		LcdWriteCmd(0x80+column); 
	// �ڶ���
	if(line==1)        
		LcdWriteCmd(0x80+0x40+column); 
}



/*********************************************************/
// Һ������ַ�������
/*********************************************************/
void LcdPrintStr(uchar *str)
{
	while(*str!='\0')
			LcdWriteData(*str++);
}


/*********************************************************/
// Һ���������
/*********************************************************/
void LcdPrintNum(uint num)
{
	LcdWriteData(num/100+0x30);				// ��λ
	LcdWriteData(num%100/10+0x30);		// ʮλ
	LcdWriteData(num%10+0x30);				// ��λ
}


/*********************************************************/
// 1602Һ�����ܳ�ʼ��
/*********************************************************/
void LcdInit()
{
	LcdWriteCmd(0x38);        // 16*2��ʾ��5*7����8λ���ݿ�
	LcdWriteCmd(0x0C);        // ����ʾ������ʾ���
	LcdWriteCmd(0x06);        // ��ַ��1����д�����ݺ�������
	LcdWriteCmd(0x01);        // ����
}



/*********************************************************/
// 1602Һ����ʾ���ݳ�ʼ��
/*********************************************************/
void LcdShowInit()
{
	LcdGotoXY(0,0);	    							// ��λ����0�е�0��
	LcdPrintStr("      U         ");	// ��0����ʾ��      U         ��
	LcdGotoXY(1,0);	    							// ��λ����1�е�0��
	LcdPrintStr(" L    D    R    ");	// ��1����ʾ�� L    D    R    ��
}


/*********************************************************/
// ���㴫����1�������ľ���
/*********************************************************/
float GetDistance1(void)
{
	float ss;					// ���ڼ�¼��õľ���

	TH0=0;
	TL0=0;

	Trig1_P=1;				// ��������ģ��1һ����ʼ����
	DelayMs(1);
	Trig1_P=0;

	while(!Echo1_P);	// �ȴ�������ģ��1�ķ�������
	TR0=1;						// ������ʱ������ʼ��ʱ
	while(Echo1_P);		// �ȴ�������ģ��1�ķ����������
	TR0=0;						// ֹͣ��ʱ����ֹͣ��ʱ

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// ����cm=��ʱ��us * �ٶ�cm/us��/2
	return ss;
}


/*********************************************************/
// ���㴫����2�������ľ���
/*********************************************************/
float GetDistance2(void)
{
	float ss;					// ���ڼ�¼��õľ���

	TH0=0;
	TL0=0;

	Trig2_P=1;				// ��������ģ��2һ����ʼ����
	DelayMs(1);
	Trig2_P=0;

	while(!Echo2_P);	// �ȴ�������ģ��2�ķ�������
	TR0=1;						// ������ʱ������ʼ��ʱ
	while(Echo2_P);		// �ȴ�������ģ��2�ķ����������
	TR0=0;						// ֹͣ��ʱ����ֹͣ��ʱ

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// ����cm=��ʱ��us * �ٶ�cm/us��/2
	return ss;
}


/*********************************************************/
// ���㴫����3�������ľ���
/*********************************************************/
float GetDistance3(void)
{
	float ss;					// ���ڼ�¼��õľ���

	TH0=0;
	TL0=0;

	Trig3_P=1;				// ��������ģ��3һ����ʼ����
	DelayMs(1);
	Trig3_P=0;

	while(!Echo3_P);	// �ȴ�������ģ��3�ķ�������
	TR0=1;						// ������ʱ������ʼ��ʱ
	while(Echo3_P);		// �ȴ�������ģ��3�ķ����������
	TR0=0;						// ֹͣ��ʱ����ֹͣ��ʱ

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// ����cm=��ʱ��us * �ٶ�cm/us��/2
	return ss;
}


/*********************************************************/
// ���㴫����4�������ľ���
/*********************************************************/
float GetDistance4(void)
{
	float ss;					// ���ڼ�¼��õľ���

	TH0=0;
	TL0=0;

	Trig4_P=1;				// ��������ģ��4һ����ʼ����
	DelayMs(1);
	Trig4_P=0;

	while(!Echo4_P);	// �ȴ�������ģ��4�ķ�������
	TR0=1;						// ������ʱ������ʼ��ʱ
	while(Echo4_P);		// �ȴ�������ģ��4�ķ����������
	TR0=0;						// ֹͣ��ʱ����ֹͣ��ʱ

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// ����ms=��ʱ��us * �ٶ�cm/us��/2
	return ss;
}


/*********************************************************/
// ����ɨ��
/*********************************************************/
void KeyScanf()
{
	if(KeySet_P==0)					// �ж��Ƿ��а�������
	{
		LcdGotoXY(0,0);	    							// ��궨λ
		LcdPrintStr("   Alarm Set    ");	// ��0����ʾ��   Alarm Set    ��
		LcdGotoXY(1,0);										// ��궨λ
		LcdPrintStr("  alarm=   cm   ");	// ��1����ʾ��  alarm=   cm   ��
		LcdGotoXY(1,8);										// ��궨λ
		LcdPrintNum(gAlarm);							// ��ʾ��ǰ�ı���ֵ

		DelayMs(10);											// �����������µĶ���
		while(!KeySet_P);									// �ȴ������ͷ�
		DelayMs(10);											// ���������ɿ��Ķ���		

		while(1)
		{		
			/* ����ֵ���Ĵ��� */
			if(KeyDown_P==0)					
			{
				if(gAlarm>2)						// ����ֵ����2���ܼ�1
					gAlarm--;							// ����ֵ��1
				LcdGotoXY(1,8);					// ��궨λ
				LcdPrintNum(gAlarm);		// ˢ���޸ĺ�ı���ֵ
				DelayMs(300);						// ��ʱ
			}

			/* ����ֵ�ӵĴ��� */
			if(KeyUp_P==0)					
			{
				if(gAlarm<400)					// ����ֵС��400���ܼ�1
					gAlarm++;							// ����ֵ��1
				LcdGotoXY(1,8);					// ��궨λ
				LcdPrintNum(gAlarm);		// ˢ���޸ĺ�ı���ֵ
				DelayMs(300);						// ��ʱ
			}
			
			/* �˳�����ֵ���� */
			if(KeySet_P==0)					
			{
				break;									// �˳�whileѭ��
			}	
		}
		
		LcdShowInit();							// Һ���ָ���������������
		DelayMs(10);		  					// �����������µĶ���
		while(!KeySet_P);						// �ȴ������ͷ�
		DelayMs(10);		  					// ���������ɿ��Ķ���		

		Sector_Erase(0x2000);				// ���汨������
		EEPROM_Write(0x2000,gAlarm/100);
		EEPROM_Write(0x2001,gAlarm%100);		
	}	
}


/*********************************************************/
// ������1�����ж�
/*********************************************************/
void AlarmJudge1(float ss)
{
	if(ss<gAlarm)		// LED���ж�
	{
		Led1_P=0;
	}
	else
	{
		Led1_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// �������ж�
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// ������2�����ж�
/*********************************************************/
void AlarmJudge2(float ss)
{
	if(ss<gAlarm)		// LED���ж�
	{
		Led2_P=0;
	}
	else
	{
		Led2_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// �������ж�
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// ������3�����ж�
/*********************************************************/
void AlarmJudge3(float ss)
{
	if(ss<gAlarm)		// LED���ж�
	{
		Led3_P=0;
	}
	else
	{
		Led3_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// �������ж�
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// ������4�����ж�
/*********************************************************/
void AlarmJudge4(float ss)
{
	if(ss<gAlarm)		// LED���ж�
	{
		Led4_P=0;
	}
	else
	{
		Led4_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// �������ж�
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// ����ֵ��ʼ��
/*********************************************************/
void AlarmInit()
{
	gAlarm=EEPROM_Read(0x2000)*100+EEPROM_Read(0x2001) * 10;		// ��EEPROM��ȡ����ֵ

	if((gAlarm==0)||(gAlarm>400))			// �����ȡ���ı���ֵ�쳣������0�����400����Ϊ�쳣��
	{
		gAlarm=25;											// ���¸�ֵ����ֵΪ25
	}
}

char temp_table[20]  ;
code int distance[2][8] = { 
{30,50,100,150,200,250,300,800},				
{27,45,90,136,181,226,272,726}};

uint Calibration_Data(uint _val)
{
	char flag = 0xff;
	if(_val >= distance[1][0] && _val < distance[1][1]){
		flag = 0;
	}else if(_val >= distance[1][1] && _val < distance[1][2]){
		flag = 1;
	}else if(_val >= distance[1][2] && _val < distance[1][3]){
		flag = 2;
	}else if(_val >= distance[1][3] && _val < distance[1][4]){
		flag = 3;
	}else if(_val >= distance[1][4] && _val < distance[1][5]){
		flag = 4;
	}else if(_val >= distance[1][5] && _val < distance[1][6]){
		flag = 5;
	}else if(_val >= distance[1][6] && _val <= distance[1][7]){
		flag = 6;
	}
	
	return  (distance[0][flag + 1] - distance[0][flag]) / \
			(distance[1][flag + 1] - distance[1][flag]) * \
			(_val - distance[1][flag]) + distance[0][flag];
	
	
}
/*********************************************************/
// ������
/*********************************************************/
void main()
{
	uchar i;									// ѭ������
	float dist;								// ����������
	
	LcdInit();								// Һ�����ܳ�ʼ��
	LcdShowInit();						// Һ����ʾ���ݳ�ʼ��
	AlarmInit();							// ����ֵ��ʼ��
	
	TMOD |= 0x01;							// ѡ��ʱ��0������ȷ���ǹ�����ʽ1��Ϊ�˳�����ģ����������ʱ�õģ�
	UartInit();
	Trig1_P=0;								// ��ʼ����������Ϊ�͵�ƽ
	Trig2_P=0;
	Trig3_P=0;
	Trig4_P=0;

	while(1)
	{
		sprintf(temp_table, "temp:%04.1f\n",Temp_val);
		Send_String(temp_table);
		sprintf(temp_table, "voice:%04.3f\n",CLAC_SPEED(Audio_speed));
		Send_String(temp_table);
		/*������1*/
		LcdGotoXY(0,6);	    		// ��궨λ
		dist = Calibration_Data(GetDistance1() * 10);
		LcdPrintNum(dist);			// ��ʾ������1�������ľ���
		AlarmJudge1(dist);			// �жϴ�����1�Ĳ��������Ƿ���Ҫ����
		

		/*��ʱ��ɨ�谴��*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*������2*/
		LcdGotoXY(1,13);	    	// ��궨λ
		dist = Calibration_Data(GetDistance2() * 10);
		LcdPrintNum(dist);			// ��ʾ������2�������ľ���
		AlarmJudge2(dist);			// �жϴ�����2�Ĳ��������Ƿ���Ҫ����
		
		/*��ʱ��ɨ�谴��*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*������3*/
		dist = Calibration_Data(GetDistance3() * 10);
		LcdGotoXY(1,7);	    		// ��궨λ
		LcdPrintNum(dist);			// ��ʾ������3�������ľ���
		AlarmJudge3(dist);			// �жϴ�����3�Ĳ��������Ƿ���Ҫ����
		
		/*��ʱ��ɨ�谴��*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*������4*/
		dist = Calibration_Data(GetDistance4() * 10);
		LcdGotoXY(1,1);	    		// ��궨λ
		LcdPrintNum(dist);			// ��ʾ������4�������ľ���
		AlarmJudge4(dist);			// �жϴ�����4�Ĳ��������Ƿ���Ҫ����
		
		/*��ʱ��ɨ�谴��*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
	}
}