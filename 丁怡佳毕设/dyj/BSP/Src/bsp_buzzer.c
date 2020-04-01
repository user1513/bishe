#include "bsp_buzzer.h"


typedef struct tagBuzzer_T
{
    /* data */
    uint16_t usPeriod;          /*���������η�תʱ��*/
    uint16_t usTotalPeriod;     /*����������Ӧʱ��*/
    uint16_t usBuzzerState;     /*������״̬*/
    uint16_t usBuzrPerTmp;      /*��������ʱ����*/
    uint16_t usBuzrTotalPerTmp;      /*��������ʱ����*/
}Buzzer_T;

Buzzer_T tBuzzer = {0, 0, 0, 0};     /*����������ṹ��*/

/*��������ʼ��*/

void bspBuzzerInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOFʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED0��LED1��ӦIO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOA,GPIO_Pin_5);//GPIOF9,F10���øߣ�����

}

/*����������*/

void bspBuzzerFlash(void)
{
    BEEP = (BEEP) ? 0 : 1;
}

/*��������������*/
/*���Ƶ�� 500Hz*/
/*������� 2ms*/

void bspBuzzerFreq(uint16_t  _usPeriod,uint16_t _usTotalPeriod)

{
    tBuzzer.usPeriod = _usPeriod;

    tBuzzer.usTotalPeriod = _usTotalPeriod;
}

void bspBeepHandler(void)
{
    if(tBuzzer.usBuzzerState)                       /*������ʹ��*/
    {
        tBuzzer.usBuzrPerTmp++;                      /*��������ת���ڼ�����*/

        if( tBuzzer.usBuzrPerTmp> tBuzzer.usPeriod)      
        {
            tBuzzer.usBuzrPerTmp = 0;

            bspBuzzerFlash();
        }
    
        if(tBuzzer.usTotalPeriod != 0xffff)         /*usTotalPeriod����Ϊ0XFFFF�������������ֹͣ*/
        {
            tBuzzer.usBuzrTotalPerTmp++;               /*�������ܷ�ת���ڼ�����*/

            if(tBuzzer.usBuzrTotalPerTmp > tBuzzer.usTotalPeriod)       
            {
                tBuzzer.usBuzzerState = 0;              /*��������־λ����*/

                BEEP = 0;                               /*������ʧ��*/

                //EventStopA(1);
            }  
        }      
    }
}

/*����������ʹ��*/

void bspBeepEnable(void)
{
    tBuzzer.usBuzzerState = 1;

    tBuzzer.usBuzrTotalPerTmp = 0;
}

/*����������ʧ��*/

void bspBeepDisable(void)
{
    tBuzzer.usBuzzerState = 0;              /*��������־λ����*/
            
    BEEP = 0;                               /*������ʧ��*/
}

