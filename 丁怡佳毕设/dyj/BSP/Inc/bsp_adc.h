#ifndef __BSP_ADC_H
#define __BSP_ADC_H	
#include "sys.h" 

 							   
void bsp_adc_init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
#endif 















