/**
  ************************************* Copyright ****************************** 
  *
  *                 (C) Copyright 2019,ZERO,China, NANJING.
  *                            All Rights Reserved
  *                              
  *                     
  *                     https://user1513.github.io/Codes/
  *    
  * FileName   : timer.c   
  * Version    : v1.0		
  * Author     : ZERO			
  * Date       : 2019-04-11         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */
#include "bsp_timer.h"


void Timer0Init(void)				//��ʱ����ʼ��  10����@12.000MHz
{
    TMOD &= 0xF0;					//���ö�ʱ��ģʽ
    
	TMOD |= 0x01;					//���ö�ʱ��ģʽ
    
	TL0 = 0x00;						//���ö�ʱ��ֵ
	
	TH0 = 0x4C;						//���ö�ʱ��ֵ
    
	TF0 = 0;						//���TF0��־
    
	TR0 = 1;						//��ʱ��0��ʼ��ʱ
	
    IE |=0X82;    					//EA , ET0 �жϴ�
}



void tim0() interrupt 1
{
	static uint8_t timeout = 0;
    TL0 = 0x00;																							//���ö�ʱ��ֵ
    TH0 = 0x4C;																							//���ö�ʱ��ֵ
	
	if(timeout++ == 10)
	{
		timeout = 0;
		uGetKeyPadAction();
		Electronic_scale_scan(1,(float)read2543(0) / 819.0 );
	}
	
}

