#include "bsp_sgp30.h"
#include "bsp.h"

/*����bsp_swi2c.h�ڵ�Swi2c_T�ṹ�����*/

Swi2c_T t_sgp30_swi2c ={
	(unsigned long *)&SGP30_I2C_SCL,
	(unsigned long *)&SGP30_I2C_SDA_OUT,
	(unsigned long *)&SGP30_I2C_SDA_IN,
	bsp_sgp30_in,
	bsp_sgp30_out
};

/*����crc.h�ڵ�CRC_8�ṹ�����*/

CRC_8 g_sgp30_crc8 = {0x31, 0xff, 0x00, FALSE, FALSE}; 


static void bsp_sgp30_out(void);

static void bsp_sgp30_in(void);

/*sgp30���ö�д״̬ͷ������*/
static void bsp_sgp30_header(uint8_t _ucWR);
/*����sgp30 2�ֽ�����*/
static void bsp_sgp30_comm(uint16_t _usComm);
/*����sgp30 ����*/
static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length);
/*��ȡsgp30 ����*/
static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length);

static void bsp_sgp30_sensor_init(void);

void bsp_sgp30_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	//GPIOA3,4��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4);//GPIOA3,4�ø�

	bsp_sgp30_sensor_init();
}


static void bsp_sgp30_out(void)
{
    SGP30_IO_OUT()
}

static void bsp_sgp30_in(void)
{
    SGP30_IO_IN()
}

/*sgp30���ö�д״̬ͷ������*/

static void bsp_sgp30_header(uint8_t _ucWR)

{
	swi2cStart(&t_sgp30_swi2c);											/*I2C��ʼ�ź�*/

	swi2cSendByte(&t_sgp30_swi2c, (SGP30_ADDRESS << 1) | (_ucWR & (0x01)));/*����sgp30ͷ����Ϣ*/
 
	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*�ȴ�sgp30��ack*/
}

/*����sgp30 2�ֽ�����*/

static void bsp_sgp30_comm(uint16_t _usComm)

{
	swi2cSendByte(&t_sgp30_swi2c, _usComm >> 8);						/*����sgp30����߰�λ��Ϣ*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*�ȴ�sgp30��ack*/
 
 	swi2cSendByte(&t_sgp30_swi2c, (uint8_t)_usComm);					/*����sgp30����Ͱ�λ��Ϣ*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*�ȴ�sgp30��ack*/
}

/*����sgp30 ����*/

static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		swi2cSendByte(&t_sgp30_swi2c, _ucTable[i]);							/*����sgp30����߰�λ��Ϣ*/

		swi2cWaitAck(&t_sgp30_swi2c, 1);									/*�ȴ�sgp30��ack*/
	}

}

/*��ȡsgp30 ����*/

static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		_ucTable[i] = swi2cReceiveByte(&t_sgp30_swi2c);						/*��ȡsgp30����Ϣ*/

		swi2cAck(&t_sgp30_swi2c, i < (length - 1) ? 1 : 0);						/*master����ack*/
	}
								
}

/*sgp30������״̬��ʼ��*/

static void bsp_sgp30_sensor_init(void)
{
	bsp_sgp30_header(0);

	bsp_sgp30_comm(INIT_AIR_QUALITY);

	swi2cStop(&t_sgp30_swi2c);
}

uint8_t g_ucTable[6] = {0};


/******************************
**��ȡsgp30 MEASURE_AIR_QUALITY ��6λ���� 
**6λ�������ݸ�ʽ
** xx xx xx xx xx xx
** co2
*********crc
************tvoc
*****************crc		
*******************************/

void bsp_sgp30_test(void)
{
    bsp_sgp30_header(0);

	bsp_sgp30_comm(MEASURE_AIR_QUALITY);
	
	swi2cStop(&t_sgp30_swi2c);
	
	delay_ms(100);		/*��Ҫһ��ʱ��ĵȴ�*/
	
	bsp_sgp30_header(1);

	bsp_sgp30_read_data(g_ucTable, 6);
	
	swi2cStop(&t_sgp30_swi2c);

	/*���crc����ֵ*/

	if((crc8(g_ucTable, 2, &g_sgp30_crc8) != g_ucTable[2]) ||
	   (crc8(g_ucTable + 3, 2, &g_sgp30_crc8) != g_ucTable[5]))
	{
		/*����������*/
		memset(g_ucTable, 0, 6);
	}
}

/*����Ƭ��ʪ�Ȳ���*/
void bsp_sgp30_humidity_compensation(void)
{

}



