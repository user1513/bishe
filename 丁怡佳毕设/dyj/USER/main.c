#include "bsp.h"

uint8_t g_ucStateFlag = 0xff;		/*ȫ��״̬��־λ*/

uint8_t g_uctStr[17];				/*sprintfר������*/

extern uint8_t g_ucaTable[];

#define	PM2_5_VALUE_MAX 	150		/*�궨��PM2.5������ֵ*/
#define TVOC_VALUE_MAX 		442		/*�궨��TVOC������ֵ*/
#define CO2_VALUE_MAX 		2000	/*�궨��CO2������ֵ*/


/*
24СʱPM2.5ƽ��ֵ��׼ֵ����λug/m3
�ţ�0~50    
����50~100    
�����Ⱦ��100~150    
�ж���Ⱦ��150~200    
�ض���Ⱦ��200~300    
������Ⱦ������300������    
*/

/*
TVOC����
100 ppb = 0.1 ppm = 0.12 mg/m3.
���ұ�׼����0.5mg/m3 = 833 * 0.5 = 441.5ppb
*/

/*
CO2����
400��1000ppm���������£�����˳����
>10,00ppm���о��������ǣ�����ʼ���û����˯��
>20,00ppm���о�ͷʹ����˯�����͡�ע�����޷����С��������١���ȶ��ġ�
>50,00ppm�����ܵ�������ȱ������������������ˡ����ԡ�����������
*/
double DHT22_Tab[2];	/*����dht22���ݽ�������[0]:hum [1]:temp*/

int main(void)
{ 
	int state_switch = 0;		/*��ʾ�л�*/

	double dPm25_val = 0.0;		/*���ڴ��pm2.5�ı���*/
	bspInit();

	OLED_show_chinese_length(0, 0, 0, 12);/*��ʾҽԺ���ڿ������ϵͳ���*/

	OLED_show_chinese_length(0, 6, 12, 6);/*��ʾ����:xxx*/

	bspBeepEnable();		/*���������Ƿ�ʧЧ*/
	FS0 = 1;				/*���������Ƿ�ʧЧ*/
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
//	EventStartA(1);
//	OLED_Clear();			/*OLED����*/
	bspBeepDisable();		/*���������Ƿ�ʧЧ*/
	FS0 = 0;				/*���������Ƿ�ʧЧ*/
	
	while(1)
	{
		
		if(!(g_ucStateFlag & LEDTIMEOUTFLAG))//LED��ʱ��־λ������
		{
			g_ucStateFlag |= LEDTIMEOUTFLAG;

			bspLedFlash();
			
			//printf("GetAbsolutehumidity:%f\n", GetAbsolutehumidity(t, h));
			//EventStopA(0);
		}

		if(!(g_ucStateFlag & DHT22TIMEOUTFLAG))//DHT22��ʱ��־λ������
		{
			g_ucStateFlag |= DHT22TIMEOUTFLAG;
			
			//FS0 = !FS0;
			//bspBeepEnable();
			TIM_Cmd(TIM3,DISABLE); //ʧ�ܶ�ʱ��3
				
			if(DHTxx_Get_Data(DHT22_Tab) == 0)
			{
				printf("HUM:%0.1lf,TEMP:%0.1lf\n",DHT22_Tab[0],DHT22_Tab[1]);
				
				/*����sgp30����ʪ��ֵ*/
				
				bsp_sgp30_humidity_compensation(DHT22_Tab[1], DHT22_Tab[0]);
			}
			TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
		}

		if(!(g_ucStateFlag & OLEDTIMEOUTFLAG))//OLED��ʱ���±�־λ������
		{
			g_ucStateFlag |= OLEDTIMEOUTFLAG;

			TIM_Cmd(TIM3,DISABLE); //ʹ�ܶ�ʱ��3

			if(state_switch++ < 4)/*4*500ms �л�ʱ��*/
			{
				OLED_Clear_static(1, 0, 7);		/*����*/

				sprintf((char*)g_uctStr, "HUM:%4.1f %%    ",DHT22_Tab[0]);

				OLED_ShowString(0, 0, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "TEMP:%4.1f ��",DHT22_Tab[1]);

				OLED_ShowString(0, 2, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "PM2.5:%4.1fug/m3",dPm25_val);

				OLED_ShowString(0, 4, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "max:%d ug/m3",PM2_5_VALUE_MAX);

				OLED_ShowString(0, 6, g_uctStr, 16,0);

			}
			else 
			{
				OLED_Clear_static(2, 0, 7); 	/*����*/

				sprintf((char*)g_uctStr, "CO2:%05d ppb",(g_ucaTable[0]<<8) + g_ucaTable[1]);

				OLED_ShowString(0, 0, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "CO2_max:%d ppb",CO2_VALUE_MAX);
				
				OLED_ShowString(0, 2, g_uctStr, 16,0);
				
				sprintf((char*)g_uctStr, "TVOC:%05d ppb",(g_ucaTable[3]<<8) + g_ucaTable[4]);

				OLED_ShowString(0, 4, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "TVOC_max:%d ppb",TVOC_VALUE_MAX);
				
				OLED_ShowString(0, 6, g_uctStr, 16,0);
			}

			TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
			
			if(state_switch == 8)
			{
				state_switch = 0;
			}

			/*�Աȱ�������*/

			if((CO2_VALUE_MAX < ((g_ucaTable[0]<<8) + g_ucaTable[1])) ||
			   (TVOC_VALUE_MAX < ((g_ucaTable[3]<<8) + g_ucaTable[4])) ||
			   (PM2_5_VALUE_MAX < dPm25_val))
			{
				bspBeepEnable();		/*�������������ȴ�*/
				FS0 = 1;				
			}
			else
			{
				bspBeepDisable();		/*�������رշ��ȹر�*/
				FS0 = 0;				
			}

			//EventStopB(0);
		}

		if(!(g_ucStateFlag & SGP30TIMEOUTFLAG))//SGP30��ʱ���±�־λ������
		{
			g_ucStateFlag |= SGP30TIMEOUTFLAG;

			bsp_sgp30_test();

			printf("TVOC:%d,CO2:%d\n",(g_ucaTable[3]<<8) + g_ucaTable[4],(g_ucaTable[0]<<8) + g_ucaTable[1]);

			dPm25_val = Get_Adc(ADC_Channel_0) * 0.16113;
			printf("ADC:%.1f\n", dPm25_val);
			//EventStopB(0);
		}
		
	}
}




