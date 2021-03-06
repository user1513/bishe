#include "bsp_led.h" 


//初始化PC13为输出口.并使能时钟		    
//LED IO初始化
void bspLedInit(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//LED0和LED1对应IO口
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0);//GPIOF9,F10设置高，灯灭

}


void bspLedFlash(void)
{
  static uint8_t flag = 0;
  LED0 = flag;
  flag = !flag;
}






