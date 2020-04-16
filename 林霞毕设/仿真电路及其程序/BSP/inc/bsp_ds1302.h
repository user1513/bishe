#ifndef _BSP_DS1302_H
#define _BSP_DS1302_H

#include "inc.h"
//DS1302���Ŷ���,�ɸ���ʵ����������޸Ķ˿ڶ���
sbit DS1302_RST=P1^0;
sbit DS1302_IO=P1^2;
sbit DS1302_SCK=P1^1;
//DS1302��ַ����
#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

/*Ĭ��ʹ��  ds1302_write_time()һ��ֻʹ��һ��
 ds1302_init(); 		//DS1302��ʼ��
 ds1302_write_time(); 	//д���ʼֵ
*/
uchar* GetTime(void);
void ds1302_init(void);

void ds1302_write_time(void) ;
//��DS302����ʱ������
void ds1302_read_time(void) ;
uint8_t ds1302ShowTime(void);
#endif
