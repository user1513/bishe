/******************************************************************************
文 件 名   : lcd12864.c

@file lcd12864.c
@brief lcd12864驱动(无字库驱动)

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "main.h"

#ifdef LCD12864_DEV_DRIVER

/*----------------------------------------------*
 * 宏定义                                 *
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

///LCD12864指令
#define LCDSTARTROW     0xC0///<设置LCD12864起始行指令
#define LCDPAGE         0xB8///<设置LCD12864页指令
#define LCDLINE         0x40///<设置LCD12864列指令
/*----------------------------------------------*
 * 枚举定义                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构体定义                              *
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
 * 外部函数原型说明                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                       *
 *----------------------------------------------*/
///下列 ASCII 都为 宋体12;  此字体下对应的点阵为：宽x高=8x16
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


///汉字显示点阵, 此字体下对应的点阵为：宽x高=16x16
static chinese_lcd12864_t code chinese_code_12864[] = {
  {{0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8,0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00,
    0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00},"单"},
  {{0x00,0x00,0x00,0xFE,0x20,0x20,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x20,0x00,0x00,
    0x00,0x80,0x60,0x1F,0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00},"片"},
  {{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,
    0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},"机"},
  {{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
    0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},"温"},
  {{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
    0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},"度"},
};
/*---------------------------------------------------------------------------*/
/**
*@brief 检测lcd12864是否忙
*
*
*@param none
*
*@return true 忙 false 空闲
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
  LCD12864_EN = 1;///<LCD12864_EN的下降沿，然后LCD12864_EN持续拉高读才有效
  state = LCD12864_PORT;
  LCD12864_EN = 0;

  busy = (bool)(state>>7);
	return busy; 
}
/*---------------------------------------------------------------------------*/
/**
*@brief 向lcd12864写入数据
*
*
*@param ucData 要写入的数据值
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
*@brief 向lcd12864写入命令
*
*
*@param ucCMD 命令值
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
*@brief lcd12864初始化
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
  
	lcd12864_write_cmd(0x38);///<8位形式，两行字符
	lcd12864_write_cmd(0x0F);///<开显示
	lcd12864_write_cmd(0x01);///<清屏
	lcd12864_write_cmd(0x06);///<画面不动，光标右移
	lcd12864_write_cmd(LCDSTARTROW);///<设置起始行
}
/*---------------------------------------------------------------------------*/
/**
*@brief lcd12864显示点阵数据
*
*
*@param ucPage  范围: 0 ~ 6
*@param ucLine  范围: 0 ~ 127 - 8 (119)
*@param ucWidth 字符宽度 8 / 16
*@param ucaRow 点阵数据指针
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
*@brief 从点阵数据中显示一个汉字
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 127 - 16 (111)
*@param ucaChinMap 汉字点阵数据
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
*@brief 从点阵数据中显示一个字符
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 127 - 8 (119)
*@param ucaCharMap 字符点阵数据
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
*@brief 显示一个字符
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 127 - 8 (119)
*@param ch 要显示的字符
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
*@brief 从chinese_code_12864表中查找汉字对于的点阵
*
*
*@param ptr 汉字字符串指针
*
*@return 该汉字对于的点阵数组指针
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
*@brief 显示一个汉字
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 112
*@param str 汉字字符串指针
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
*@brief 在lcd12864上显示一行汉字
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 112
*@param str 汉字字符串指针
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
*@brief 在lcd12864上显示ASCII字符串
*
*
*@param ucPage 范围: 0 ~ 6
*@param uucLine 范围: 0 ~ 119
*@param str 要显示的字符串指针
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
*@brief 在lcd12864上绘制图片
*
*
*@param ucPage 范围: 0 ~ 6
*@param ucLine 范围: 0 ~ 119
*@param ucWidth 图像宽度
*@param ucHigh 图像高度
*@param ucaGraph 图像点阵数据
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
*@brief 设置lcd12864在那一页显示
*
*
*@param page 范围: 0 ~ 6
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
*@brief 设置lcd12864在那一列显示
*
*
*@param column 范围: 0 ~ 127
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
*@brief 选择左半屏还是右半屏写入命令
*
*
*@param chip_select 0/1
*@param dsp_data 写入的命令值
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
*@brief 读取左半屏/右半屏数据
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
  LCD12864_EN = 1;/* E的下降沿，然后E持续拉高读才有效 */
  dsp_data = LCD12864_PORT;
  LCD12864_EN = 0;
  
  return dsp_data;
}
/*---------------------------------------------------------------------------*/
/**
*@brief 清屏
*
*
*@param clear_color 1--黑色,其他值--本色
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
    color = 0xFF;///<设置清屏颜色
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
*@brief 在lcd12864上画点
*
*
*@param X 0 ~ 127
*@param Y 0 ~ 63
*@param Type 参考 lcd12864_graphic_e
*
*@return 
* 
*
*@note 
*
*/
void lcd12864_draw_point(uint8_t X, uint8_t Y, lcd12864_graphic_e Type) 
{ 									                                                           
   uint8_t DX = (Y >> 3);///<计算出属于哪个字节
   uint8_t BX = Y - (DX << 3);///<计算出属于字节哪一位
   uint8_t TempData = 0; 
   uint8_t chip_select;

   if (X > 63) { 
    chip_select = 2;
    X -= 64; 
   }else{ 
    chip_select = 1; 
   } 

   lcd12864_set_display_page(DX);///<设行地址
   lcd12864_set_display_column(X);///<设列地址
    
   TempData = lcd12864_read_data(chip_select);///<读出所画点所在字节的内容
    
   switch (Type)///<对该字节进行相应操作
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
    
   lcd12864_set_display_page(DX);///<设置行地址
   lcd12864_set_display_column(X);///<设置列地址
   lcd12864_write_data_from_select(chip_select, TempData);///<把修改后的字节送回LCD，达到画点目的
} 
/*---------------------------------------------------------------------------*/
/**
*@brief 在lcd12864上画线
*
*
*@param x1 起点坐标 x
*@param y1 起点坐标 y
*@param x2 终点坐标 x
*@param y2 终点坐标 y
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
*@brief 在横屏中央画水平线
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
*@brief 在lcd12864上画直虚线
*
*
*@param x1 起点坐标 x
*@param y1 起点坐标 y
*@param x2 终点坐标 x
*@param y2 终点坐标 y
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
  
  if(y1 == y2){ /* 画水平虚线 */
    if(x1 < x2){
      for(i = x1; i < x2; i += 2){
        lcd12864_draw_point(i,y1,LCD12864_GRAPHIC_DRAW);
      }
    }else{
      for(i = x2; i < x1; i += 2){
        lcd12864_draw_point(i,y1,LCD12864_GRAPHIC_DRAW);
      }
    }
  }else if(x1 == x2){/* 画垂直虚线 */
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
*@brief 在lcd12864上画正弦曲线
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
*@brief 在lcd12864上画圆
*
*
*@param x0 圆心坐标
*@param y0 圆心坐标
*@param r 圆半径
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
    lcd12864_draw_line(xn,yn,xi,yi);///<在一系列零散的点上，两点两点之间连线，从而得到一条曲线
    xn = xi;
    yn = yi;
  }
}
/*---------------------------------------------------------------------------*/
/**
*@brief 在lcd12864上画矩形/正方形
*
*
*@param x0 左上角坐标 x
*@param y0 左上角坐标 y
*@param x1 右下角坐标 x
*@param y1 右下角坐标 y
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
*@brief 在lcd12864上画三角形
*
*
*@param x1 三角形顶点坐标
*@param y1 三角形顶点坐标
*@param x2 三角形顶点坐标
*@param y2 三角形顶点坐标
*@param x3 三角形顶点坐标
*@param y3 三角形顶点坐标
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

