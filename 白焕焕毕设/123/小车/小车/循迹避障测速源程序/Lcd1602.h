
#ifndef __LCD1602_H__
#define __LCD1602_H__

sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^7;

#define RS_H       RS = 1
#define RS_L       RS = 0

#define RW_H       RW = 1
#define RW_L       RW = 0

#define EN_H       EN = 1
#define EN_L       EN = 0

#define READ_DATA  P1


void Init_1602(void);
void LcdWriteStr(unsigned char hang,unsigned char add,char*s);
//void LcdWriteChar(unsigned char hang,unsigned char add,char Ch);
void Lcd1602Printf( unsigned char x, unsigned char y, unsigned char *p,...);


#endif




