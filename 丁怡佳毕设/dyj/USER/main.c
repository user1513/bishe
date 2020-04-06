#include "bsp.h"

uint8_t g_ucStateFlag = 0xff;		/*全局状态标志位*/

uint8_t g_uctStr[17];				/*sprintf专用数组*/

extern uint8_t g_ucaTable[];

#define	PM2_5_VALUE_MAX 	150		/*宏定义PM2.5报警阈值*/
#define TVOC_VALUE_MAX 		442		/*宏定义TVOC报警阈值*/
#define CO2_VALUE_MAX 		2000	/*宏定义CO2报警阈值*/


/*
24小时PM2.5平均值标准值：单位ug/m3
优：0~50    
良：50~100    
轻度污染：100~150    
中度污染：150~200    
重度污染：200~300    
严重污染：大于300及以上    
*/

/*
TVOC参数
100 ppb = 0.1 ppm = 0.12 mg/m3.
国家标准室内0.5mg/m3 = 833 * 0.5 = 441.5ppb
*/

/*
CO2参数
400～1000ppm：空气清新，呼吸顺畅。
>10,00ppm：感觉空气浑浊，并开始觉得昏昏欲睡。
>20,00ppm：感觉头痛、嗜睡、呆滞、注意力无法集中、心跳加速、轻度恶心。
>50,00ppm：可能导致严重缺氧，造成永久性脑损伤、昏迷、甚至死亡。
*/
double DHT22_Tab[2];	/*定义dht22数据接收数组[0]:hum [1]:temp*/

int main(void)
{ 
	int state_switch = 0;		/*显示切换*/

	double dPm25_val = 0.0;		/*用于存放pm2.5的变量*/
	bspInit();

	OLED_show_chinese_length(0, 0, 0, 12);/*显示医院室内空气监测系统设计*/

	OLED_show_chinese_length(0, 6, 12, 6);/*显示作者:xxx*/

	bspBeepEnable();		/*测试器件是否失效*/
	FS0 = 1;				/*测试器件是否失效*/
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
//	EventStartA(1);
//	OLED_Clear();			/*OLED清屏*/
	bspBeepDisable();		/*测试器件是否失效*/
	FS0 = 0;				/*测试器件是否失效*/
	
	while(1)
	{
		
		if(!(g_ucStateFlag & LEDTIMEOUTFLAG))//LED超时标志位被清零
		{
			g_ucStateFlag |= LEDTIMEOUTFLAG;

			bspLedFlash();
			
			//printf("GetAbsolutehumidity:%f\n", GetAbsolutehumidity(t, h));
			//EventStopA(0);
		}

		if(!(g_ucStateFlag & DHT22TIMEOUTFLAG))//DHT22超时标志位被清零
		{
			g_ucStateFlag |= DHT22TIMEOUTFLAG;
			
			//FS0 = !FS0;
			//bspBeepEnable();
			TIM_Cmd(TIM3,DISABLE); //失能定时器3
				
			if(DHTxx_Get_Data(DHT22_Tab) == 0)
			{
				printf("HUM:%0.1lf,TEMP:%0.1lf\n",DHT22_Tab[0],DHT22_Tab[1]);
				
				/*设置sgp30绝对湿度值*/
				
				bsp_sgp30_humidity_compensation(DHT22_Tab[1], DHT22_Tab[0]);
			}
			TIM_Cmd(TIM3,ENABLE); //使能定时器3
		}

		if(!(g_ucStateFlag & OLEDTIMEOUTFLAG))//OLED超时更新标志位被清零
		{
			g_ucStateFlag |= OLEDTIMEOUTFLAG;

			TIM_Cmd(TIM3,DISABLE); //使能定时器3

			if(state_switch++ < 4)/*4*500ms 切换时间*/
			{
				OLED_Clear_static(1, 0, 7);		/*清屏*/

				sprintf((char*)g_uctStr, "HUM:%4.1f %%    ",DHT22_Tab[0]);

				OLED_ShowString(0, 0, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "TEMP:%4.1f ℃",DHT22_Tab[1]);

				OLED_ShowString(0, 2, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "PM2.5:%4.1fug/m3",dPm25_val);

				OLED_ShowString(0, 4, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "max:%d ug/m3",PM2_5_VALUE_MAX);

				OLED_ShowString(0, 6, g_uctStr, 16,0);

			}
			else 
			{
				OLED_Clear_static(2, 0, 7); 	/*清屏*/

				sprintf((char*)g_uctStr, "CO2:%05d ppb",(g_ucaTable[0]<<8) + g_ucaTable[1]);

				OLED_ShowString(0, 0, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "CO2_max:%d ppb",CO2_VALUE_MAX);
				
				OLED_ShowString(0, 2, g_uctStr, 16,0);
				
				sprintf((char*)g_uctStr, "TVOC:%05d ppb",(g_ucaTable[3]<<8) + g_ucaTable[4]);

				OLED_ShowString(0, 4, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "TVOC_max:%d ppb",TVOC_VALUE_MAX);
				
				OLED_ShowString(0, 6, g_uctStr, 16,0);
			}

			TIM_Cmd(TIM3,ENABLE); //使能定时器3
			
			if(state_switch == 8)
			{
				state_switch = 0;
			}

			/*对比报警参数*/

			if((CO2_VALUE_MAX < ((g_ucaTable[0]<<8) + g_ucaTable[1])) ||
			   (TVOC_VALUE_MAX < ((g_ucaTable[3]<<8) + g_ucaTable[4])) ||
			   (PM2_5_VALUE_MAX < dPm25_val))
			{
				bspBeepEnable();		/*蜂鸣器报警风扇打开*/
				FS0 = 1;				
			}
			else
			{
				bspBeepDisable();		/*蜂鸣器关闭风扇关闭*/
				FS0 = 0;				
			}

			//EventStopB(0);
		}

		if(!(g_ucStateFlag & SGP30TIMEOUTFLAG))//SGP30超时更新标志位被清零
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




