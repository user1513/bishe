#include "bsp_led.h" 


//��ʼ��PC13Ϊ�����.��ʹ��ʱ��		    
//LED IO��ʼ��
void bspLedInit(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOFʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//LED0��LED1��ӦIO��
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0);//GPIOF9,F10���øߣ�����

}


void bspLedFlash(void)
{
  static uint8_t flag = 0;
  LED0 = flag;
  flag = !flag;
}






