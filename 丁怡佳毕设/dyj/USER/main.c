#include "bsp.h"

uint8_t g_ucStateFlag = 0xff;		/*全局状态标志位*/

uint8_t g_uctStr[17];				/*sprintf专用数组*/

extern uint8_t g_ucTable[];

int main(void)
{ 
	bspInit();

//	EventStartA(1);

	bspBeepEnable();
	FS0 = 0;
	while(1)
	{
		if(!(g_ucStateFlag & LEDTIMEOUTFLAG))//LED超时标志位被清零
		{
			g_ucStateFlag |= LEDTIMEOUTFLAG;

			bspLedFlash();
			//EventStopA(0);
		}

		if(!(g_ucStateFlag & DHT22TIMEOUTFLAG))//DHT22超时标志位被清零
		{
			g_ucStateFlag |= DHT22TIMEOUTFLAG;
			
			double DHT22_Tab[2];
			FS0 = !FS0;
			bspBeepEnable();
			TIM_Cmd(TIM3,DISABLE); //失能定时器3
			bsp_sgp30_test();
			printf("TVOC:%d,CO2:%d\n",(g_ucTable[0]<<8) + g_ucTable[1],(g_ucTable[3]<<8) + g_ucTable[4]);
			
			sprintf((char*)g_uctStr, "CO2:%05d ppb",(g_ucTable[3]<<8) + g_ucTable[4]);

			OLED_ShowString(0, 4, g_uctStr, 16,0);
			
			sprintf((char*)g_uctStr, "TVOC:%05d ppb",(g_ucTable[0]<<8) + g_ucTable[1]);

			OLED_ShowString(0, 6, g_uctStr, 16,0);
			
			if(DHTxx_Get_Data(DHT22_Tab) == 0)
			{
				printf("HUM:%lf,TEMP:%lf\n",DHT22_Tab[0],DHT22_Tab[1]);

				sprintf((char*)g_uctStr, "HUM:%.1lf %%",DHT22_Tab[0]);

				OLED_ShowString(0, 0, g_uctStr, 16,1);

				sprintf((char*)g_uctStr, "TEMP:%.1lf ℃",DHT22_Tab[1]);

				OLED_ShowString(0, 2, g_uctStr, 16,0);
			}
			TIM_Cmd(TIM3,ENABLE); //使能定时器3
		}

		if(!(g_ucStateFlag & OLEDTIMEOUTFLAG))//OLE超时更新标志位被清零
		{
			g_ucStateFlag |= OLEDTIMEOUTFLAG;

			//EventStopB(0);
		}
	}
}




