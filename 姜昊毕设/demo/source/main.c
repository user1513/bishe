/******************************************************************************
�� �� ��   : main.c

@file main.c
@brief �������ļ�
******************************************************************************/


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "main.h"

/*----------------------------------------------*
 * �궨��                                 *
 *----------------------------------------------*/

#define BUTTON_NUM_PERIOD             ( 0 )///<ѡ����ܻ��Ǽ�������
#define BUTTON_NUM_SWITCH_CHANNEL     ( 1 )///<ѡ��ͨ������

sbit SWITCH_CHANNEL_LOW = P2^0;///<ͨ��ѡ���л�
sbit SWITCH_CHANNEL_HIGH = P2^1;///<ͨ��ѡ���л�

#define WAVE_LENGH        ( 10 )///<����
#define SWITHC_VALUE      ( 50000 )///<����/����ת������
/*----------------------------------------------*
 * ö�ٶ���                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ṹ�嶨��                              *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
static bool is_test_freq_period = true;///<�Ƿ����, true---���� false---����
static uint16_t freq_value;///<���ڱ���Ƶ��ֵ
static uint8_t display_buf[16];///<��ʾbuf
static uint16_t cnt = 0;///<���ڼ���Ƶ��

static bool is_convert_busy = false;
static uint8_t channel = 0;



/*����ʹ��*/
void uart_init()
{
	SCON=0x40;
	PCON = 0X00;				//�����ʲ��ӱ�
	REN=0;						//���ͽ�������
	T2MOD = 0X01;
	T2CON = 0X30;			   	//����T2λ������ʱ��
	RCAP2H = 0xff;			   	//65536-11059200/32��9600
	RCAP2L = 0xDC;			  	//��ʼ������ֵ 11.0592Mhz 9600������
	TH2 = 0xff;				  	//��ʼ������ֵ 11.0592Mhz 9600������
	TL2 = 0xDC;				  
	SM0=0;					  	//����ģʽ8Ϊ����Ϊ��1λֹͣλ
	SM1=1;					  	//����ģʽ8Ϊ����Ϊ��1λֹͣλ
	TR2=1;					  	//��ʱ��2��ʼ����
	TI=0;					  	//����жϱ�־λ
}



void send_char(unsigned char dat)		//����һ���ֽ�����
{
	SBUF=dat;					//��������
	while(!TI);				    //�ȴ������Ƿ����
	TI=0;
}

void send_num(unsigned int num)		//����һ���ֽ�����
{
	int i;
	unsigned char tab[2] = {0};

	tab[0] = (num & 0xff00) >> 8;	   //���͹������ݣ�����10000���ģ�������λ��������λ��ǰ����λ�ں�
	tab[1] = (num & 0x00ff);
		 
	for(i=0;i<2;i++)			//���������ݷ��ͳ�ȥ
	{
		send_char(tab[i]);
	}
}

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/

/*---------------------------------------------------------------------------*/
/**
*@brief ͨ���л�
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
*@brief ��ʾ�̶��ַ���
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
*@brief ����timer1�����ڼ���ģʽ
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
  TMOD &= 0x0F;///<���ö�ʱ��ģʽ
  TMOD |= 0x50;	///<���ö�ʱ������
  TL1 = 0;///<���ö�ʱ��ֵ
  TH1 = 0;///<���ö�ʱ��ֵ
  TF1 = 0;///<���TF1��־
  TR1 = 1;///<��ʱ��1��ʼ��ʱ
}
/*---------------------------------------------------------------------------*/
//	cnt = 0;
//	        is_test_freq_period = !is_test_freq_period;
//	        if(is_test_freq_period){

/*---------------------------------------------------------------------------*/
/**
*@brief ��ʾƵ��ֵ
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
  
  if(is_test_freq_period){		/*���ܷ�*/
    v = (uint32_t)freq_value * WAVE_LENGH  * 3600;
    temp = v / 2000000.0;
    sprintf(display_buf, "%.1fKm/h    ", temp);
    lcd12864_show_string(ch * 2, 8*(sizeof("Ch0:") - 1), display_buf);
  }else{			/*��Ƶ��*/
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
*@brief �������
*
*
*@param none
*
*@return
* 
*
*@note ������ں���main
*
*/
void main(void)
{
  uint16_t limit;

  limit = SWITHC_VALUE;
  limit /= 1000;
  limit *= 20;
  lcd12864_init();
  timer0_mode2_init();///<��ʱ��ģʽ2��ʼ��
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
  static uint8_t ticks = 0;///<����1�������
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
  }else if((is_test_freq_period == false) && (cnt == 100)){///<����0.2��,��Ƶ��ֵ
    freq_value = (uint16_t)(TH1 << 8) | TL1;
    is_convert_busy = true;
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ���ڽ����жϴ���
*
*
*@param ch ���ڽ��յ����ַ�
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
*@brief ���ڷ����жϴ���
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

