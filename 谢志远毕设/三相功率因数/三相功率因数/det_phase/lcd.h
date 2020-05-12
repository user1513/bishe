#ifndef LCD_H_
#define LCD_H_

#define uint unsigned int
#define ulong unsigned long int
#define uchar unsigned char
sbit lcdrs=P0^0;
sbit lcdrw=P0^1;
sbit lcden=P0^2;

sbit addr_b=P1^2;
sbit addr_a=P1^3;

sfr T2MOD  = 0x0C9;

void delay(uint z);
void write_com(uchar com);
void write_data(uchar dat);
void lcd_init();
//void display_string(uchar num, uchar *string);
void display_res(uint pos, uint d_tab) ;


#endif