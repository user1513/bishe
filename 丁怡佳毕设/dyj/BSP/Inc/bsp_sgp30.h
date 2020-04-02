#ifndef _BSP_SGP30_H
#define _BSP_SGP30_H

#include "bsp.h"

//sgp30I2C��ַ
#define SGP30_ADDRESS 0x58 

#define INIT_AIR_QUALITY        0X2003          /*��ʼ��SGP30*/
#define MEASURE_AIR_QUALITY     0X2008          /*SGP30��ʼ����*/
#define GET_BASELINE            0X2015          /*��ȡSGP30��׼��*/
#define SET_BASELINE            0X201E          /*����SGP30��׼��*/
#define SET_HUMIDITY            0X2061          /*���þ���ʪ��*/
//#define MEASURE_TEST            0X2032          /*����Ƭ���Բ���*/
#define GET_FEATURE_SET_VERSION 0X202F          /*��ȡ���ܼ��汾*/
#define MEASURE_RAW_SIGNALS     0X2050          /*������ԭʼ�ź�*/

//IO�����ź궨��
#define SGP30_I2C_SCL       PAout(3) 
#define SGP30_I2C_SDA_IN    PAin(4)
#define SGP30_I2C_SDA_OUT   PAout(4)


//IO��������
#define SGP30_IO_IN()  {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=0<<4*2;}	//PA4����ģʽ
#define SGP30_IO_OUT() {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=1<<4*2;} 	//PA4���ģʽ
/*��������*/

void bsp_sgp30_io_init(void);

void bsp_sgp30_test(void);

void bsp_sgp30_out(void);

void bsp_sgp30_in(void);

#endif

