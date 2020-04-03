#include "bsp_sgp30.h"
#include "bsp.h"

/*定义bsp_swi2c.h内的Swi2c_T结构体变量*/

Swi2c_T t_sgp30_swi2c ={
	(unsigned long *)&SGP30_I2C_SCL,
	(unsigned long *)&SGP30_I2C_SDA_OUT,
	(unsigned long *)&SGP30_I2C_SDA_IN,
	bsp_sgp30_in,
	bsp_sgp30_out
};

/*定义crc.h内的CRC_8结构体变量*/

CRC_8 g_sgp30_crc8 = {0x31, 0xff, 0x00, FALSE, FALSE}; 


static void bsp_sgp30_out(void);

static void bsp_sgp30_in(void);

/*sgp30设置读写状态头部函数*/
static void bsp_sgp30_header(uint8_t _ucWR);
/*发送sgp30 2字节命令*/
static void bsp_sgp30_comm(uint16_t _usComm);
/*发送sgp30 数据*/
static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length);
/*读取sgp30 数据*/
static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length);

static void bsp_sgp30_sensor_init(void);

void bsp_sgp30_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	//GPIOA3,4初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4);//GPIOA3,4置高

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

/*sgp30设置读写状态头部函数*/

static void bsp_sgp30_header(uint8_t _ucWR)

{
	swi2cStart(&t_sgp30_swi2c);											/*I2C开始信号*/

	swi2cSendByte(&t_sgp30_swi2c, (SGP30_ADDRESS << 1) | (_ucWR & (0x01)));/*发送sgp30头部信息*/
 
	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
}

/*发送sgp30 2字节命令*/

static void bsp_sgp30_comm(uint16_t _usComm)

{
	swi2cSendByte(&t_sgp30_swi2c, _usComm >> 8);						/*发送sgp30命令高八位信息*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
 
 	swi2cSendByte(&t_sgp30_swi2c, (uint8_t)_usComm);					/*发送sgp30命令低八位信息*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
}

/*发送sgp30 数据*/

static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		swi2cSendByte(&t_sgp30_swi2c, _ucTable[i]);							/*发送sgp30命令高八位信息*/

		swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
	}

}

/*读取sgp30 数据*/

static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		_ucTable[i] = swi2cReceiveByte(&t_sgp30_swi2c);						/*读取sgp30的信息*/

		swi2cAck(&t_sgp30_swi2c, i < (length - 1) ? 1 : 0);						/*master发送ack*/
	}
								
}

/*sgp30传感器状态初始化*/

static void bsp_sgp30_sensor_init(void)
{
	bsp_sgp30_header(0);

	bsp_sgp30_comm(INIT_AIR_QUALITY);

	swi2cStop(&t_sgp30_swi2c);
}

uint8_t g_ucTable[6] = {0};


/******************************
**获取sgp30 MEASURE_AIR_QUALITY 的6位数据 
**6位数据数据格式
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
	
	delay_ms(100);		/*需要一点时间的等待*/
	
	bsp_sgp30_header(1);

	bsp_sgp30_read_data(g_ucTable, 6);
	
	swi2cStop(&t_sgp30_swi2c);

	/*检测crc检验值*/

	if((crc8(g_ucTable, 2, &g_sgp30_crc8) != g_ucTable[2]) ||
	   (crc8(g_ucTable + 3, 2, &g_sgp30_crc8) != g_ucTable[5]))
	{
		/*将数组清零*/
		memset(g_ucTable, 0, 6);
	}
}

/*启动片上湿度补偿*/
void bsp_sgp30_humidity_compensation(void)
{

}



