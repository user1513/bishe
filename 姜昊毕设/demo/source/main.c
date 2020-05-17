/******************************************************************************
文 件 名   : main.c

@file main.c
@brief 主函数文件
******************************************************************************/


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "main.h"

/*----------------------------------------------*
 * 宏定义                                 *
 *----------------------------------------------*/

#define BUTTON_NUM_PERIOD             ( 0 )///<选择测周还是计数按键
#define BUTTON_NUM_SWITCH_CHANNEL     ( 1 )///<选择通道按键

sbit SWITCH_CHANNEL_LOW = P2^0;///<通道选择切换
sbit SWITCH_CHANNEL_HIGH = P2^1;///<通道选择切换

#define WAVE_LENGH        ( 10 )///<波长
#define SWITHC_VALUE      ( 50000 )///<测周/计数转换门限
/*----------------------------------------------*
 * 枚举定义                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构体定义                              *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
static bool is_test_freq_period = true;///<是否测周, true---测周 false---计数
static uint16_t freq_value;///<用于保存频率值
static uint8_t display_buf[16];///<显示buf
static uint16_t cnt = 0;///<用于计算频率

static bool is_convert_busy = false;
static uint8_t channel = 0;



/*串口使用*/
void uart_init()
{
	SCON=0x40;
	PCON = 0X00;				//波特率不加倍
	REN=0;						//发送接收是能
	T2MOD = 0X01;
	T2CON = 0X30;			   	//设置T2位波特率时钟
	RCAP2H = 0xff;			   	//65536-11059200/32×9600
	RCAP2L = 0xDC;			  	//初始化计数值 11.0592Mhz 9600波特率
	TH2 = 0xff;				  	//初始化计数值 11.0592Mhz 9600波特率
	TL2 = 0xDC;				  
	SM0=0;					  	//串口模式8为数据为，1位停止位
	SM1=1;					  	//串口模式8为数据为，1位停止位
	TR2=1;					  	//定时器2开始计数
	TI=0;					  	//清除中断标志位
}



void send_char(unsigned char dat)		//发送一个字节数据
{
	SBUF=dat;					//发送数据
	while(!TI);				    //等待发送是否完成
	TI=0;
}

void send_num(unsigned int num)		//发送一个字节数据
{
	int i;
	unsigned char tab[2] = {0};

	tab[0] = (num & 0xff00) >> 8;	   //发送功率数据，扩大10000倍的，方便上位机处理，高位在前，低位在后
	tab[1] = (num & 0x00ff);
		 
	for(i=0;i<2;i++)			//将所有数据发送出去
	{
		send_char(tab[i]);
	}
}

/*----------------------------------------------*
 * 常量定义                                       *
 *----------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
*@brief 通道切换
*
*
*@param none
*
*@return 
* 
*
*@note 
*
*/
static void channel_switch(void)
{
  cnt = 0;
  switch(channel){
    case 0:
      SWITCH_CHANNEL_LOW = 0;
      SWITCH_CHANNEL_HIGH = 0;
      //lcd1602_display_string(sizeof("Ch:") - 1, 0, "0");
      break;
    case 1:
      SWITCH_CHANNEL_LOW = 1;
      SWITCH_CHANNEL_HIGH = 0;
      //lcd1602_display_string(sizeof("Ch:") - 1, 0, "1");
      break;
    case 2:
      SWITCH_CHANNEL_LOW = 0;
      SWITCH_CHANNEL_HIGH = 1;
      //lcd1602_display_string(sizeof("Ch:") - 1, 0, "2");
      break;
    case 3:
      SWITCH_CHANNEL_LOW = 1;
      SWITCH_CHANNEL_HIGH = 1;
      //lcd1602_display_string(sizeof("Ch:") - 1, 0, "3");
      break;
  }
  
}
/*---------------------------------------------------------------------------*/
/**
*@brief 显示固定字符串
*
*
*@param none
*
*@return 
* 
*
*@note 
*
*/
static void display_fix_string(void)
{
  lcd12864_show_string(0, 0, "Ch0:");
  lcd12864_show_string(2, 0, "Ch1:");
  lcd12864_show_string(4, 0, "Ch2:");
  lcd12864_show_string(6, 0, "Ch3:");
  //lcd12864_show_string(0, 120, "P");
}
/*---------------------------------------------------------------------------*/
/**
*@brief 设置timer1工作在计数模式
*
*
*@param none
*
*@return 
* 
*
*@note 
*
*/
static void timer1_cnt_init(void)
{
  TMOD &= 0x0F;///<设置定时器模式
  TMOD |= 0x50;	///<设置定时器计数
  TL1 = 0;///<设置定时初值
  TH1 = 0;///<设置定时初值
  TF1 = 0;///<清除TF1标志
  TR1 = 1;///<定时器1开始计时
}
/*---------------------------------------------------------------------------*/
//	cnt = 0;
//	        is_test_freq_period = !is_test_freq_period;
//	        if(is_test_freq_period){

/*---------------------------------------------------------------------------*/
/**
*@brief 显示频率值
*
*
*@param none
*
*@return 
* 
*
*@note 
*
*/
unsigned int uart_str[4] = {0}; 

static void display_freq(uint8_t ch)
{
  float temp;
  uint32_t v;
  
  if(is_test_freq_period){		/*测周法*/
    v = (uint32_t)freq_value * WAVE_LENGH  * 3600;
    temp = v / 2000000.0;
    sprintf(display_buf, "%.1fKm/h    ", temp);
    lcd12864_show_string(ch * 2, 8*(sizeof("Ch0:") - 1), display_buf);
  }else{			/*测频法*/
    temp = freq_value / 20.0;
    temp = temp * WAVE_LENGH * 1.8;
    sprintf(display_buf, "%.1fKm/h   ", temp);
    lcd12864_show_string(ch * 2, 8*(sizeof("Ch0:") - 1), display_buf);
    HAL_Delay(1000);
  }
	uart_str[ch] = (temp + 0.05) * 10;
	send_num(0x55AA);
	for(v = 0; v < 4; v++)
	{
		send_num(uart_str[v]);
	}  
}



/*---------------------------------------------------------------------------*/
/**
*@brief 程序入口
*
*
*@param none
*
*@return
* 
*
*@note 程序入口函数main
*
*/
void main(void)
{
  uint16_t limit;

  limit = SWITHC_VALUE;
  limit /= 1000;
  limit *= 20;
  lcd12864_init();
  timer0_mode2_init();///<定时器模式2初始化
  display_fix_string();
  channel_switch();
	uart_init();
  __enable_irq();
  
  timer1_cnt_init();
  while(1){
	  //send_num(0x55AA);
	  //send_num(0xaa55);
    if(is_convert_busy){
      display_freq(channel);

      if(is_test_freq_period && freq_value > SWITHC_VALUE){
        is_test_freq_period = false;
      }else if((is_test_freq_period == false) && freq_value < limit){
        is_test_freq_period = true;
      }else{
        channel++;
        if(channel > 3){
          channel = 0;
        }
      }
      channel_switch();
      is_convert_busy = false;
      TH1 = 0;
  	  TL1 = 0;
    }
  }
}

/*---------------------------------------------------------------------------*/
void timer0_irq(void)
{
  static uint8_t ticks = 0;///<用于1毫秒计数
  ++ticks;
  ++cnt;
  if(ticks == 5){
    ticks = 0;
    ++clock_ticks;
  }

  if(is_convert_busy){
    return;
  }
  if(is_test_freq_period && (cnt == 5000)){
    freq_value = (uint16_t)(TH1 << 8) | TL1;
    is_convert_busy = true;
  }else if((is_test_freq_period == false) && (cnt == 100)){///<计数0.2秒,求频率值
    freq_value = (uint16_t)(TH1 << 8) | TL1;
    is_convert_busy = true;
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief 串口接收中断处理
*
*
*@param ch 串口接收到的字符
*
*@return 
* 
*
*@note 
*
*/
void uart_rx_irq(uint8_t ch)
{
  ch = ch;
    //uart_put_char(ch);
}
/*---------------------------------------------------------------------------*/
/**
*@brief 串口发送中断处理
*
*
*@param none
*
*@return 
* 
*
*@note 
*
*/
void uart_tx_irq(void)
{

}
/*---------------------------------------------------------------------------*/

