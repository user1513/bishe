
#line 1 "source\timer0.c" /0






 
 


 
  
#line 1 "source\main.h" /0
 
 
 
 
 
#line 6 "source\main.h" /1
 
 
 
 
#line 10 "source\main.h" /0
 
  
#line 1 "F:\Keil_v5\C51\Inc\stdio.h" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
 
 
 
#line 11 "source\main.h" /0
 
  
#line 1 "F:\Keil_v5\C51\Inc\string.h" /0







 
 
 
 
 
 
#line 14 "F:\Keil_v5\C51\Inc\string.h" /1
  
 
 
#line 17 "F:\Keil_v5\C51\Inc\string.h" /0
 
 
#line 19 "F:\Keil_v5\C51\Inc\string.h" /1
  
 
#line 21 "F:\Keil_v5\C51\Inc\string.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 extern char  *strcat  (char *s1, const char *s2);
 extern char  *strncat (char *s1, const char *s2, size_t n);
 
 extern char   strcmp  (const char *s1, const char *s2);
 extern char   strncmp (const char *s1, const char *s2, size_t n);
 
 extern char  *strcpy  (char *s1, const char *s2);
 extern char  *strncpy (char *s1, const char *s2, size_t n);
 
 extern size_t strlen  (const char *);
 
 extern char  *strchr  (const char *s, char c);
 extern int    strpos  (const char *s, char c);
 extern char  *strrchr (const char *s, char c);
 extern int    strrpos (const char *s, char c);
 
 extern size_t strspn  (const char *s, const char *set);
 extern size_t strcspn (const char *s, const char *set);
 extern char  *strpbrk (const char *s, const char *set);
 extern char  *strrpbrk(const char *s, const char *set);
 extern char  *strstr  (const char *s, const char *sub);
 extern char  *strtok  (char *str, const char *set);
 
 extern char   memcmp  (const void *s1, const void *s2, size_t n);
 extern void  *memcpy  (void *s1, const void *s2, size_t n);
 extern void  *memchr  (const void *s, char val, size_t n);
 extern void  *memccpy (void *s1, const void *s2, char val, size_t n);
 extern void  *memmove (void *s1, const void *s2, size_t n);
 extern void  *memset  (void *s, char val, size_t n);
 #pragma RESTORE
 
 
#line 12 "source\main.h" /0
 
  
#line 1 "F:\Keil_v5\C51\Inc\intrins.h" /0






 
 
 
 
 
 #pragma SAVE
 
 
#line 15 "F:\Keil_v5\C51\Inc\intrins.h" /1
 
 
 
#line 18 "F:\Keil_v5\C51\Inc\intrins.h" /0
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 
#line 29 "F:\Keil_v5\C51\Inc\intrins.h" /1
 
 
 
#line 32 "F:\Keil_v5\C51\Inc\intrins.h" /0
 
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 #pragma RESTORE
 
 
 
#line 13 "source\main.h" /0
 
  
#line 1 "F:\Keil_v5\C51\Inc\math.h" /0







 
 
 
 
 
 
#line 14 "F:\Keil_v5\C51\Inc\math.h" /1
 
  
 
 
 
  
 
 
 
 
 
 
 
 
#line 28 "F:\Keil_v5\C51\Inc\math.h" /0
 
 #pragma SAVE
 #pragma REGPARMS
 
 extern char  cabs  (char  val);
 extern int    abs  (int   val);
 extern long  labs  (long  val);
 
 
 extern float fabs  (float val);
 extern float sqrt  (float val);
 extern float exp   (float val);
 extern float log   (float val);
 extern float log10 (float val);
 extern float sin   (float val);
 extern float cos   (float val);
 extern float tan   (float val);
 extern float asin  (float val);
 extern float acos  (float val);
 extern float atan  (float val);
 extern float sinh  (float val);
 extern float cosh  (float val);
 extern float tanh  (float val);
 extern float atan2 (float y, float x);
 
 extern float ceil  (float val);
 extern float floor (float val);
 extern float modf  (float val, float *n);
 extern float fmod  (float x, float y);
 extern float pow   (float x, float y);
 
 
#line 60 "F:\Keil_v5\C51\Inc\math.h" /1
 
 
 
#line 63 "F:\Keil_v5\C51\Inc\math.h" /0
 
 #pragma RESTORE
 
 
#line 14 "source\main.h" /0
 
  
#line 1 "F:\Keil_v5\C51\Inc\Atmel\REGX52.H" /0






 
 
 
 
 


 
 sfr P0      = 0x80;
 sfr SP      = 0x81;
 sfr DPL     = 0x82;
 sfr DPH     = 0x83;
 sfr PCON    = 0x87;
 sfr TCON    = 0x88;
 sfr TMOD    = 0x89;
 sfr TL0     = 0x8A;
 sfr TL1     = 0x8B;
 sfr TH0     = 0x8C;
 sfr TH1     = 0x8D;
 sfr P1      = 0x90;
 sfr SCON    = 0x98;
 sfr SBUF    = 0x99;
 sfr P2      = 0xA0;
 sfr IE      = 0xA8;
 sfr P3      = 0xB0;
 sfr IP      = 0xB8;
 sfr T2CON   = 0xC8;
 sfr T2MOD   = 0xC9;
 sfr RCAP2L  = 0xCA;
 sfr RCAP2H  = 0xCB;
 sfr TL2     = 0xCC;
 sfr TH2     = 0xCD;
 sfr PSW     = 0xD0;
 sfr ACC     = 0xE0;
 sfr B       = 0xF0;
 


 
 sbit P0_0 = 0x80;
 sbit P0_1 = 0x81;
 sbit P0_2 = 0x82;
 sbit P0_3 = 0x83;
 sbit P0_4 = 0x84;
 sbit P0_5 = 0x85;
 sbit P0_6 = 0x86;
 sbit P0_7 = 0x87;
 


 
 
 
 
 
 
 
 
 
 


 
 sbit IT0  = 0x88;
 sbit IE0  = 0x89;
 sbit IT1  = 0x8A;
 sbit IE1  = 0x8B;
 sbit TR0  = 0x8C;
 sbit TF0  = 0x8D;
 sbit TR1  = 0x8E;
 sbit TF1  = 0x8F;
 


 
 
 
 
 
 
 
 
 
 
 
 
 


 
 sbit P1_0 = 0x90;
 sbit P1_1 = 0x91;
 sbit P1_2 = 0x92;
 sbit P1_3 = 0x93;
 sbit P1_4 = 0x94;
 sbit P1_5 = 0x95;
 sbit P1_6 = 0x96;
 sbit P1_7 = 0x97;
 
 sbit T2   = 0x90;        
 sbit T2EX = 0x91;        
 


 
 sbit RI   = 0x98;
 sbit TI   = 0x99;
 sbit RB8  = 0x9A;
 sbit TB8  = 0x9B;
 sbit REN  = 0x9C;
 sbit SM2  = 0x9D;
 sbit SM1  = 0x9E;
 sbit SM0  = 0x9F;
 


 
 sbit P2_0 = 0xA0;
 sbit P2_1 = 0xA1;
 sbit P2_2 = 0xA2;
 sbit P2_3 = 0xA3;
 sbit P2_4 = 0xA4;
 sbit P2_5 = 0xA5;
 sbit P2_6 = 0xA6;
 sbit P2_7 = 0xA7;
 


 
 sbit EX0  = 0xA8;        
 sbit ET0  = 0xA9;        
 sbit EX1  = 0xAA;        
 sbit ET1  = 0xAB;        
 sbit ES   = 0xAC;        
 sbit ET2  = 0xAD;        
 
 sbit EA   = 0xAF;        
 


 
 sbit P3_0 = 0xB0;
 sbit P3_1 = 0xB1;
 sbit P3_2 = 0xB2;
 sbit P3_3 = 0xB3;
 sbit P3_4 = 0xB4;
 sbit P3_5 = 0xB5;
 sbit P3_6 = 0xB6;
 sbit P3_7 = 0xB7;
 
 sbit RXD  = 0xB0;        
 sbit TXD  = 0xB1;        
 sbit INT0 = 0xB2;        
 sbit INT1 = 0xB3;        
 sbit T0   = 0xB4;        
 sbit T1   = 0xB5;        
 sbit WR   = 0xB6;        
 sbit RD   = 0xB7;        
 


 
 sbit PX0  = 0xB8;
 sbit PT0  = 0xB9;
 sbit PX1  = 0xBA;
 sbit PT1  = 0xBB;
 sbit PS   = 0xBC;
 sbit PT2  = 0xBD;
 


 
 sbit CP_RL2= 0xC8;       
 sbit C_T2 = 0xC9;        
 sbit TR2  = 0xCA;        
 sbit EXEN2= 0xCB;        
 sbit TCLK = 0xCC;        
 sbit RCLK = 0xCD;        
 sbit EXF2 = 0xCE;        
 sbit TF2  = 0xCF;        
 


 
 
 
 


 
 sbit P    = 0xD0;
 sbit F1   = 0xD1;
 sbit OV   = 0xD2;
 sbit RS0  = 0xD3;
 sbit RS1  = 0xD4;
 sbit F0   = 0xD5;
 sbit AC   = 0xD6;
 sbit CY   = 0xD7;
 



 
 
 
 
 
 
 
 
 
 
 
 
 
#line 15 "source\main.h" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned char uint8_t;
 typedef char int8_t;
 
 typedef unsigned int uint16_t;
 typedef int int16_t;
 
 typedef unsigned long int uint32_t;
 typedef long int int32_t;
 
 typedef bit bool;
 
 typedef uint16_t clock_time_t;
 
 
 
 
 
 
 
 
 
 
 
 
 extern volatile clock_time_t clock_ticks;
 
 
 


 
 


 
 




 
 






 
 void timer0_mode1_init(void);
 void timer0_irq(void);
 void timer0_mode2_init(void);
 void HAL_Delay(uint32_t Delay);
 
 
 
 typedef enum{
 LCD12864_GRAPHIC_CLEAR = 0x01, 
 LCD12864_GRAPHIC_NOT = 0x02, 
 LCD12864_GRAPHIC_DRAW = 0X03 
 }lcd12864_graphic_e;
 
 void lcd12864_init( void );
 void lcd12864_show_one_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str);
 void lcd12864_show_line_chinese(uint8_t ucPage,uint8_t ucLine, const uint8_t * str);
 void lcd12864_show_one_char(uint8_t ucPage,uint8_t ucLine,uint8_t ch);
 void lcd12864_show_string(uint8_t ucPage,uint8_t ucLine, uint8_t * str);
 void lcd12864_show_graph(uint8_t ucPage,uint8_t ucLine,uint8_t ucWidth,uint8_t ucHigh,uint8_t * ucaGraph);
 
 
 void lcd12864_clear_display(uint8_t clear_color);
 void lcd12864_draw_point(uint8_t X, uint8_t Y, lcd12864_graphic_e Type);
 void lcd12864_draw_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2);
 void lcd12864_draw_level(void);
 void lcd12864_draw_sine_wave(void);
 void lcd12864_draw_circle(uint8_t x0,uint8_t y0,uint8_t r);
 void lcd12864_draw_retic(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1);
 void lcd12864_draw_trigle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t x3,uint8_t y3);
 void lcd12864_draw_dotted_line(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2);
 
 
 
 
#line 114 "source\main.h" /1
 
 
 
 
#line 118 "source\main.h" /0
 
 
 
 
 
 
 
#line 12 "source\timer0.c" /0
 
 
 
 


 
 
 
 
 
#line 23 "source\timer0.c" /1
 
  
  
 
  
  
 
  
  
 
  
  
 
#line 36 "source\timer0.c" /0
 
 
 


 
 


 
 


 
 


 
 


 
 
 volatile clock_time_t clock_ticks = 0;


 
 
 
 
 
 
#line 68 "source\timer0.c" /1











 
 
 
 
 
 
 
 
 
 
 
 
 
#line 92 "source\timer0.c" /0











 
 void timer0_mode2_init(void)
 {
 TMOD &= 0xF0;		 
 TMOD |= 0x02;		 
 TL0 = 0x48;		   
 TH0 = 0x48;		   
 TF0 = 0;		     
 TR0 = 1;		     
 ET0 = 1;         
 }
 
 











 
 void HAL_Delay(uint32_t Delay)
 {
 clock_time_t tickstart = clock_ticks;
 clock_time_t wait = Delay;
 
 
 if (wait < 0xFFFFU){
 wait += 1u;
 }
 
 while ((clock_ticks - tickstart) < wait){
 }
 }
 











 
 void timer0() interrupt 1
 {
 
#line 157 "source\timer0.c" /1
 
 
 
 
 
#line 162 "source\timer0.c" /0
 timer0_irq(); 
 
 }
 
 
 
