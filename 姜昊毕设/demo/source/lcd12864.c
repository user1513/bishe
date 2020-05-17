/******************************************************************************
�� �� ��   : lcd12864.c

@file lcd12864.c
@brief lcd12864����(���ֿ�����)

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "main.h"

#ifdef LCD12864_DEV_DRIVER

/*----------------------------------------------*
 * �궨��                                 *
 *----------------------------------------------*/
#define DEBUG   ( 0 )

#if DEBUG
#define log(X)    print_string(X)
#else
#define log(X)
#endif


sbit LCD12864_EN  = P2^7;
sbit LCD12864_RW  = P2^5;
sbit LCD12864_DI  = P2^6;
sbit LCD12864_CS1 = P3^2;
sbit LCD12864_CS2 = P3^3;
#define LCD12864_PORT   P0

///LCD12864ָ��
#define LCDSTARTROW     0xC0///<����LCD12864��ʼ��ָ��
#define LCDPAGE         0xB8///<����LCD12864ҳָ��
#define LCDLINE         0x40///<����LCD12864��ָ��
/*----------------------------------------------*
 * ö�ٶ���                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ṹ�嶨��                              *
 *----------------------------------------------*/
typedef struct{
  uint8_t ascii_display_code[16];
  uint8_t ch;
}ascii_lcd12864_t;

typedef struct{
  uint8_t chinese_display_code[32];
  uint8_t str[2];
}chinese_lcd12864_t;
/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                       *
 *----------------------------------------------*/
///���� ASCII ��Ϊ ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=8x16
static ascii_lcd12864_t code ascii_code_12864[] = {
  {{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20}, 'A'},
  {{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20}, 'a'},
  {{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00}, 'B'},
  {{0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00}, 'b'},
  {{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00}, 'C'},
  {{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00}, 'c'},
  {{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00}, 'D'},
  {{0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20}, 'd'},
  {{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00}, 'E'},
  {{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00}, 'e'},
  {{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00}, 'F'},
  {{0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00}, 'f'},
  {{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00}, 'G'},
  {{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00}, 'g'},
  {{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20}, 'H'},
  {{0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20}, 'h'},
  {{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00}, 'I'},
  {{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00}, 'i'},
  {{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00}, 'J'},
  {{0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00}, 'j'},
  {{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00}, 'K'},
  {{0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00}, 'k'},
  {{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00}, 'L'},
  {{0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00}, 'l'},
  {{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00}, 'M'},
  {{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F}, 'm'},
  {{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00}, 'N'},
  {{0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20}, 'n'},
  {{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00}, 'O'},
  {{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00}, 'o'},
  {{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00}, 'P'},
  {{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00}, 'p'},
  {{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00}, 'Q'},
  {{0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80}, 'q'},
  {{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20}, 'R'},
  {{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00}, 'r'},
  {{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00}, 'S'},
  {{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00}, 's'},
  {{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00}, 'T'},
  {{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00}, 't'},
  {{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00}, 'U'},
  {{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20}, 'u'},
  {{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00}, 'V'},
  {{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00}, 'v'},
  {{0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00}, 'W'},
  {{0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00}, 'w'},
  {{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20}, 'X'},
  {{0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00}, 'x'},
  {{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00}, 'Y'},
  {{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00}, 'y'},
  {{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00}, 'Z'},
  {{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00}, 'z'},
  {{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00}, '0'},
  {{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00}, '1'},
  {{0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00}, '2'},
  {{0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00}, '3'},
  {{0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00}, '4'},
  {{0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00}, '5'},
  {{0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00}, '6'},
  {{0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00}, '7'},
  {{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00}, '8'},
  {{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00}, '9'},
  {{0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00}, '!'},
  {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00}, '.'},
  {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01}, '-'},
  {{0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00}, ':'},
  {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, ' '},
  {{0x00,0x00,0x00,0x00,0xC0,0x38,0x04,0x00,0x00,0x60,0x18,0x07,0x00,0x00,0x00,0x00}, '/'}};


///������ʾ����, �������¶�Ӧ�ĵ���Ϊ����x��=16x16
static chinese_lcd12864_t code chinese_code_12864[] = {
  {{0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8,0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00,
    0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00},"��"},
  {{0x00,0x00,0x00,0xFE,0x20,0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x00,0x00,
    0x00,0x80,0x60,0x1F,0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00},"Ƭ"},
  {{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,
    0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},"��"},
  {{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
    0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},"��"},
  {{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
    0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},"��"},
};
/*---------------------------------------------------------------------------*/
/**
*@brief ���lcd12864�Ƿ�æ
*
*
*@param none
*
*@return true æ false ����
* 
*
*@note 
*
*/
static bool lcd12864_is_busy(void)
{
  uint8_t state = 0;
  bool busy;
  
	LCD12864_PORT = 0xFF;
	LCD12864_RW = 1;
	LCD12864_DI = 0;
	LCD12864_EN = 1;
	LCD12864_EN = 0;
  LCD12864_EN = 1;///<LCD12864_EN���½��أ�Ȼ��LCD12864_EN�������߶�����Ч
  state = LCD12864_PORT;
  LCD12864_EN = 0;

  busy = (bool)(state>>7);
	return busy; 
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864д������
*
*
*@param ucData Ҫд�������ֵ
*
*@return
* 
*
*@note 
*
*/
static void lcd12864_write_data(uint8_t ucData)
{
  uint16_t timeout;

  timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_write_data err\r\n");
      break;
    }
  }
	LCD12864_PORT = 0xFF;

	LCD12864_RW = 0;
	LCD12864_DI = 1;
	LCD12864_PORT = ucData;
	LCD12864_EN = 1;
	LCD12864_EN = 0;
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864д������
*
*
*@param ucCMD ����ֵ
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_write_cmd(uint8_t ucCMD)
{
	uint16_t timeout;

  timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_write_cmd err\r\n");
      break;
    }
  }
	LCD12864_PORT = 0xFF;

	LCD12864_RW = 0;
	LCD12864_DI = 0;
	LCD12864_PORT = ucCMD;
	LCD12864_EN = 1;
	LCD12864_EN = 0;
}
/*---------------------------------------------------------------------------*/
/**
*@brief lcd12864��ʼ��
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
void lcd12864_init( void )
{
	LCD12864_CS1 = 1;
	LCD12864_CS2 = 1;
  
	lcd12864_write_cmd(0x38);///<8λ��ʽ�������ַ�
	lcd12864_write_cmd(0x0F);///<����ʾ
	lcd12864_write_cmd(0x01);///<����
	lcd12864_write_cmd(0x06);///<���治�����������
	lcd12864_write_cmd(LCDSTARTROW);///<������ʼ��
}
/*---------------------------------------------------------------------------*/
/**
*@brief lcd12864��ʾ��������
*
*
*@param ucPage  ��Χ: 0 ~ 6
*@param ucLine  ��Χ: 0 ~ 127 - 8 (119)
*@param ucWidth �ַ���� 8 / 16
*@param ucaRow ��������ָ��
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_show_custom_row(uint8_t ucPage,uint8_t ucLine,uint8_t ucWidth,uint8_t *ucaRow)
{
	uint8_t ucCount;
  
	if(ucLine < 64){
  	LCD12864_CS1=1;
  	LCD12864_CS2=0;
  	lcd12864_write_cmd(LCDPAGE + ucPage);
  	lcd12864_write_cmd(LCDLINE + ucLine);
  	if( (ucLine + ucWidth) < 64 ){
		  for(ucCount = 0; ucCount < ucWidth; ucCount++){
			  lcd12864_write_data(*(ucaRow + ucCount));
		  }
		}
  	else{
  		for(ucCount = 0; ucCount < (64 - ucLine); ucCount++){
  			lcd12864_write_data(*(ucaRow+ucCount));
      }

  		LCD12864_CS1 = 0;
  		LCD12864_CS2 = 1;
  		lcd12864_write_cmd(LCDPAGE + ucPage);
  		lcd12864_write_cmd(LCDLINE);
  		for(ucCount = 64-ucLine; ucCount < ucWidth; ucCount++){
  			lcd12864_write_data(*(ucaRow+ucCount));
  		}
    }
	}
	else{
  	LCD12864_CS1 = 0;
  	LCD12864_CS2 = 1;
  	lcd12864_write_cmd(LCDPAGE+ucPage);
  	lcd12864_write_cmd(LCDLINE+ucLine-64);
  	for(ucCount = 0; ucCount < ucWidth; ucCount++){
  		lcd12864_write_data(*(ucaRow+ucCount));
  	}
	}
}
/*---------------------------------------------------------------------------*/
/**
*@brief �ӵ�����������ʾһ������
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 127 - 16 (111)
*@param ucaChinMap ���ֵ�������
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_show_one_chin(uint8_t ucPage,uint8_t ucLine,uint8_t *ucaChinMap)
{
  if(ucaChinMap == NULL)
    return;
	lcd12864_show_custom_row(ucPage, ucLine,16, ucaChinMap);
	lcd12864_show_custom_row(ucPage+1, ucLine,16, ucaChinMap + 16);
}
/*---------------------------------------------------------------------------*/
/**
*@brief �ӵ�����������ʾһ���ַ�
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 127 - 8 (119)
*@param ucaCharMap �ַ���������
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_show_one_ch(uint8_t ucPage,uint8_t ucLine,uint8_t *ucaCharMap)
{
  if(ucaCharMap == NULL){
    return;
  }
	lcd12864_show_custom_row(ucPage, ucLine, 8, ucaCharMap);
	lcd12864_show_custom_row(ucPage+1, ucLine, 8, ucaCharMap + 8);
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��ʾһ���ַ�
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 127 - 8 (119)
*@param ch Ҫ��ʾ���ַ�
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_show_one_char(uint8_t ucPage,uint8_t ucLine,uint8_t ch)
{
  uint8_t i;
  
  for(i = 0; i < (sizeof(ascii_code_12864) / sizeof(ascii_lcd12864_t)); i++){
    if(ascii_code_12864[i].ch == ch){
      lcd12864_show_one_ch(ucPage, ucLine, ascii_code_12864[i].ascii_display_code);
      break;
    }
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��chinese_code_12864���в��Һ��ֶ��ڵĵ���
*
*
*@param ptr �����ַ���ָ��
*
*@return �ú��ֶ��ڵĵ�������ָ��
* 
*
*@note 
*
*/
const uint8_t * find_chinese_table_code(const uint8_t * ptr)
{
  uint8_t i, j;

  if(ptr == NULL){
    return NULL;
  }

  j = 0xff;  
  for(i = 0; i < (sizeof(chinese_code_12864) / sizeof(chinese_lcd12864_t)); i++){
    if(memcmp(chinese_code_12864[i].str, ptr, 2) == 0){
      j = i;
      break;
    }
  }

  if(j == 0xff){
    return NULL;
  }
  
  return (const uint8_t *)chinese_code_12864[j].chinese_display_code;
}

/*---------------------------------------------------------------------------*/
/**
*@brief ��ʾһ������
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 112
*@param str �����ַ���ָ��
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_show_one_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str)
{
  const uint8_t * ptr = NULL;

  ptr = find_chinese_table_code(str);
  lcd12864_show_one_chin(ucPage, ucLine, ptr);
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864����ʾһ�к���
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 112
*@param str �����ַ���ָ��
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_show_line_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str)
{
  const uint8_t * p = str;

  if(p == NULL){
    return;
  }
  
  if(ucPage + 2 > 8){
    return;    
  }

  while(*p != '\0'){
    lcd12864_show_one_chinese(ucPage, ucLine, p);
    p += 2;
    ucLine += 16;
    if(ucLine > 128){
      return;
    }
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864����ʾASCII�ַ���
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param uucLine ��Χ: 0 ~ 119
*@param str Ҫ��ʾ���ַ���ָ��
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_show_string(uint8_t ucPage,uint8_t ucLine, uint8_t * str)
{
  uint8_t * p = str;

  if(p == NULL){
    return;
  }
  
  if(ucPage + 2 > 8){
    return;    
  }
  while(*p != '\0'){
    if(ucLine + 8 > 128){
      ucLine = 0;
      ucPage += 2;
      if(ucPage + 2 > 8){
        break;
      }
    }
    lcd12864_show_one_char(ucPage, ucLine, *p);
    ucLine += 8;
    p++;
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ���ͼƬ
*
*
*@param ucPage ��Χ: 0 ~ 6
*@param ucLine ��Χ: 0 ~ 119
*@param ucWidth ͼ����
*@param ucHigh ͼ��߶�
*@param ucaGraph ͼ���������
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_show_graph(uint8_t ucPage,uint8_t ucLine,uint8_t ucWidth,uint8_t ucHigh,uint8_t * ucaGraph)
{
	uint8_t ucCount;

  if(ucaGraph == NULL){
    return;
  }
	for(ucCount = 0; ucCount < ucHigh; ucCount++){
		lcd12864_show_custom_row(ucPage + ucCount, ucLine, ucWidth, ucaGraph + ucCount*ucWidth);
	}
}
/*---------------------------------------------------------------------------*/
/**
*@brief ����lcd12864����һҳ��ʾ
*
*
*@param page ��Χ: 0 ~ 6
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_set_display_page(uint8_t page)
{	  
  uint16_t timeout;
  
  LCD12864_CS1 = 1;
  LCD12864_CS2 = 0;
  timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_set_display_page err1\r\n");
      break;
    }
  }

  LCD12864_PORT = 0xFF;
  LCD12864_RW = 0;
  LCD12864_DI = 0;
  LCD12864_PORT = page + 0xB8;
  LCD12864_EN = 1;
  LCD12864_EN = 0;

  LCD12864_CS1 = 0;
  LCD12864_CS2 = 1;
  
  timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_set_display_page err2\r\n");
      break;
    }
  }
  
  LCD12864_PORT = 0xFF;
  LCD12864_RW = 0;
  LCD12864_DI = 0;
  LCD12864_PORT = page + 0xB8;
  LCD12864_EN = 1;
  LCD12864_EN = 0;
}
/*---------------------------------------------------------------------------*/
/**
*@brief ����lcd12864����һ����ʾ
*
*
*@param column ��Χ: 0 ~ 127
*
*@return 
* 
*
*@note 
*
*/
static void lcd12864_set_display_column(uint8_t column)
{
  uint16_t timeout;
  
	LCD12864_CS1 = 1;
	LCD12864_CS2 = 0;
	timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_set_display_column err1\r\n");
      break;
    }
  }
	LCD12864_PORT = 0xFF;
	LCD12864_RW = 0;
	LCD12864_DI = 0;
	LCD12864_PORT = column + 0x40;
	LCD12864_EN = 1;
	LCD12864_EN = 0;

	LCD12864_CS1 = 0;
	LCD12864_CS2 = 1;
	timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_set_display_column err2\r\n");
      break;
    }
  }
	LCD12864_PORT = 0xFF;
	LCD12864_RW = 0;
	LCD12864_DI = 0;
	LCD12864_PORT = column + 0x40;
	LCD12864_EN = 1;
	LCD12864_EN = 0;
}
/*---------------------------------------------------------------------------*/
/**
*@brief ѡ������������Ұ���д������
*
*
*@param chip_select 0/1
*@param dsp_data д�������ֵ
*
*@return static
* 
*
*@note 
*
*/
static void lcd12864_write_data_from_select(uint8_t chip_select, uint8_t dsp_data)
{
  uint16_t timeout;
  
	timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_write_data_from_select err\r\n");
      break;
    }
  }
  
	LCD12864_PORT = 0xFF;
	if(chip_select==1){
		LCD12864_CS1 = 1;
		LCD12864_CS2 = 0;
  }else{
    LCD12864_CS1 = 0;
    LCD12864_CS2 = 1;
  }
	LCD12864_RW = 0;
	LCD12864_DI = 1;
	
	LCD12864_PORT = dsp_data;
	LCD12864_EN = 1;
	LCD12864_EN = 0;
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��ȡ�����/�Ұ�������
*
*
*@param chip_select 0 / 1
*
*@return 
* 
*
*@note 
*
*/
static uint8_t lcd12864_read_data(uint8_t chip_select)
{
  uint16_t timeout;
  uint8_t dsp_data;
  
  timeout = 0;
	while(lcd12864_is_busy()){
    timeout++;
    if(timeout > 0xff){
      log("lcd12864_read_data err\r\n");
      break;
    }
  }
  LCD12864_PORT = 0xFF;
  if(chip_select == 1){
    LCD12864_CS2 = 0;
    LCD12864_CS1 = 1;
  }
  else{
    LCD12864_CS2 = 1;
    LCD12864_CS1 = 0;
  }
  LCD12864_RW = 1;
  LCD12864_DI = 1;
  LCD12864_EN = 1;
  LCD12864_EN = 0;
  LCD12864_EN = 1;/* E���½��أ�Ȼ��E�������߶�����Ч */
  dsp_data = LCD12864_PORT;
  LCD12864_EN = 0;
  
  return dsp_data;
}
/*---------------------------------------------------------------------------*/
/**
*@brief ����
*
*
*@param clear_color 1--��ɫ,����ֵ--��ɫ
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_clear_display(uint8_t clear_color)
{
	uint8_t page,column, color;
  
  if(clear_color == 1){
    color = 0xFF;///<����������ɫ
  }else{
    color = 0x00;
  } 
  
	for(page = 0; page < 8; page++){
	  lcd12864_set_display_page(page);
	  lcd12864_set_display_column(0);
	  for(column = 0; column < 64; column++){	
      lcd12864_write_data_from_select(1, color);
      lcd12864_write_data_from_select(2, color);
		}
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ���
*
*
*@param X 0 ~ 127
*@param Y 0 ~ 63
*@param Type �ο� lcd12864_graphic_e
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_point(uint8_t X, uint8_t Y, lcd12864_graphic_e Type) 
{ 									                                                           
   uint8_t DX = (Y >> 3);///<����������ĸ��ֽ�
   uint8_t BX = Y - (DX << 3);///<����������ֽ���һλ
   uint8_t TempData = 0; 
   uint8_t chip_select;

   if (X > 63) { 
    chip_select = 2;
    X -= 64; 
   }else{ 
    chip_select = 1; 
   } 

   lcd12864_set_display_page(DX);///<���е�ַ
   lcd12864_set_display_column(X);///<���е�ַ
    
   TempData = lcd12864_read_data(chip_select);///<���������������ֽڵ�����
    
   switch (Type)///<�Ը��ֽڽ�����Ӧ����
   { 
    case LCD12864_GRAPHIC_CLEAR:
      TempData &= ~(1<<BX); 
    break;
    
    case LCD12864_GRAPHIC_NOT:
      TempData ^= (1 << BX); 
    break;
    
    case LCD12864_GRAPHIC_DRAW: 
      TempData |= (1 << BX);  
    break;
    
    default:				
    break;
   } 
    
   lcd12864_set_display_page(DX);///<�����е�ַ
   lcd12864_set_display_column(X);///<�����е�ַ
   lcd12864_write_data_from_select(chip_select, TempData);///<���޸ĺ���ֽ��ͻ�LCD���ﵽ����Ŀ��
} 
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ���
*
*
*@param x1 ������� x
*@param y1 ������� y
*@param x2 �յ����� x
*@param y2 �յ����� y
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2)
{                           
  uint8_t x,y; 
  uint8_t d_x,d_y;     	
  char err = 0;
  uint8_t temp = 0;

  if(y2 < y1){
    x = x1;
    y = y1;
    x1 = x2;
    y1 = y2;
    x2 = x;
    y2 = y;
  }
  d_y = y2-y1;
  if(d_y == 0){
    if(x1 > x2){
      x = x1;
      x1 = x2;
      x2 = x;
    }
    for(x = x1; x <= x2; x++){
      lcd12864_draw_point(x,y1,LCD12864_GRAPHIC_DRAW);
    } 
  }else{
    if(x2 >= x1){
      temp = 1;
      d_x = x2-x1;
    }else{
      d_x = x1-x2;
    } 
    x = x1;
    y = y1;
    lcd12864_draw_point(x,y,LCD12864_GRAPHIC_DRAW);
    if(temp && (d_y <= d_x)){
      while(x != x2){
        if(err < 0){
          x = x+1;
          err = err+(y2-y);
        }else{
          x = x+1;
          y = y+1;
          err = err+(y2-y)-(x2-x);
        }
        lcd12864_draw_point(x,y,LCD12864_GRAPHIC_DRAW);
      }
    }else if(temp && (d_y > d_x)){
      while(y != y2){
        d_x = x2-x;
        d_y = y2-y; 
        if(err < 0){
          x = x+1;
          y = y+1;
          err = err+d_y-d_x;
        }else{
          y = y+1;
          err = err-d_x;
        }
        lcd12864_draw_point(x,y,LCD12864_GRAPHIC_DRAW);
      }
    }else if(!temp && (d_y<=d_x)){
      while(x != x2){
        d_x = x-x2;
        d_y = y2-y; 
        if(err < 0){
          x = x-1;
          err = err+d_y;
        }else{
          x = x-1;
          y = y+1;
          err = err+d_y-d_x;
        }
        lcd12864_draw_point(x,y,LCD12864_GRAPHIC_DRAW);
      }
    } else if(!temp &&(d_y>d_x)){
      while(y != y2){
        d_x = x-x2;
        d_y = y2-y; 
        if(err<0){
          x = x-1;
          y = y+1;
          err = err+d_y-d_x;
        }else{
          y = y+1;
          err = err-d_x;
        }
        lcd12864_draw_point(x,y,LCD12864_GRAPHIC_DRAW);
      }
    }
  }   
}                                  
/*---------------------------------------------------------------------------*/
/**
*@brief �ں������뻭ˮƽ��
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
void lcd12864_draw_level(void)
{
  uint8_t i;
  
  for(i = 0; i < 127; i++){
    lcd12864_draw_point(i,32,LCD12864_GRAPHIC_DRAW);
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ�ֱ����
*
*
*@param x1 ������� x
*@param y1 ������� y
*@param x2 �յ����� x
*@param y2 �յ����� y
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_dotted_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2)
{
  uint8_t i;
  
  if(y1 == y2){ /* ��ˮƽ���� */
    if(x1 < x2){
      for(i = x1; i < x2; i += 2){
        lcd12864_draw_point(i,y1,LCD12864_GRAPHIC_DRAW);
      }
    }else{
      for(i = x2; i < x1; i += 2){
        lcd12864_draw_point(i,y1,LCD12864_GRAPHIC_DRAW);
      }
    }
  }else if(x1 == x2){/* ����ֱ���� */
    if(y1 < y2){
      for(i = y1; i < y2; i += 2){
        lcd12864_draw_point(x1,i,LCD12864_GRAPHIC_DRAW);
      }
    }else{
      for(i = y2; i < y1; i += 2){
        lcd12864_draw_point(x1,i,LCD12864_GRAPHIC_DRAW);
      }
    }
  }
}

/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ���������
*
*
*@param void
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_sine_wave(void)
{
	uint8_t xn = 0;
	uint8_t yn = 32;
	uint8_t i;
	uint8_t xi,yi;
  for(i = 0; i < 127; i++){
		xi = i;
		yi = (sin(i*0.1)*32) + 32;
		lcd12864_draw_line(xn,yn,xi,yi);      
		xn = i;
		yn = yi;
  }  
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ�Բ
*
*
*@param x0 Բ������
*@param y0 Բ������
*@param r Բ�뾶
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_circle(uint8_t x0,uint8_t y0,uint8_t r)
{
  uint8_t xn,yn,xi,yi;
  unsigned int j;

  xn = cos(0)*r + x0;
  yn = sin(0)*r + y0;

  for(j = 0; j < 630; j++){
    xi = (cos(j*0.01)*r)+x0;
    yi = (sin(j*0.01)*r)+y0;
    lcd12864_draw_line(xn,yn,xi,yi);///<��һϵ����ɢ�ĵ��ϣ���������֮�����ߣ��Ӷ��õ�һ������
    xn = xi;
    yn = yi;
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ�����/������
*
*
*@param x0 ���Ͻ����� x
*@param y0 ���Ͻ����� y
*@param x1 ���½����� x
*@param y1 ���½����� y
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_retic(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1)
{
	uint8_t i;
  
	for(i = 0; i <= x1-x0; i++){  
    lcd12864_draw_point(x0 + i, y0, LCD12864_GRAPHIC_DRAW);  
    lcd12864_draw_point(x0 + i, y1, LCD12864_GRAPHIC_DRAW);  
  }
  
	for(i = 0; i <= y1-y0; i++){  
    lcd12864_draw_point(x0,y0+i,LCD12864_GRAPHIC_DRAW);  
    lcd12864_draw_point(x1,y0+i,LCD12864_GRAPHIC_DRAW);  
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief ��lcd12864�ϻ�������
*
*
*@param x1 �����ζ�������
*@param y1 �����ζ�������
*@param x2 �����ζ�������
*@param y2 �����ζ�������
*@param x3 �����ζ�������
*@param y3 �����ζ�������
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_trigle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3)
{
	lcd12864_draw_line(x1,y1,x2,y2);
	lcd12864_draw_line(x2,y2,x3,y3);
	lcd12864_draw_line(x1,y1,x3,y3);
}
/*---------------------------------------------------------------------------*/
#endif//LCD12864_DEV_DRIVER

