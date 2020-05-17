
#line 1 "source\lcd1602.c" /0






 
 


 
  
#line 1 "SOURCE\MAIN.H" /0
 
 
 
 
 
#line 6 "SOURCE\MAIN.H" /1
 
 
 
 
#line 10 "SOURCE\MAIN.H" /0
 
  
#line 1 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STDIO.H" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
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
 
 
 
#line 11 "SOURCE\MAIN.H" /0
 
  
#line 1 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STRING.H" /0






 
 
 
 
 
 
#line 13 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STRING.H" /1
  
 
 
#line 16 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STRING.H" /0
 
 
#line 18 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STRING.H" /1
  
 
#line 20 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\STRING.H" /0
 
 #pragma SAVE
 #pragma REGPARMS
 extern char *strcat (char *s1, char *s2);
 extern char *strncat (char *s1, char *s2, int n);
 
 extern char strcmp (char *s1, char *s2);
 extern char strncmp (char *s1, char *s2, int n);
 
 extern char *strcpy (char *s1, char *s2);
 extern char *strncpy (char *s1, char *s2, int n);
 
 extern int strlen (char *);
 
 extern char *strchr (const char *s, char c);
 extern int strpos (const char *s, char c);
 extern char *strrchr (const char *s, char c);
 extern int strrpos (const char *s, char c);
 
 extern int strspn (char *s, char *set);
 extern int strcspn (char *s, char *set);
 extern char *strpbrk (char *s, char *set);
 extern char *strrpbrk (char *s, char *set);
 extern char *strstr  (char *s, char *sub);
 extern char *strtok  (char *str, const char *set);
 
 extern char memcmp (void *s1, void *s2, int n);
 extern void *memcpy (void *s1, void *s2, int n);
 extern void *memchr (void *s, char val, int n);
 extern void *memccpy (void *s1, void *s2, char val, int n);
 extern void *memmove (void *s1, void *s2, int n);
 extern void *memset  (void *s, char val, int n);
 #pragma RESTORE
 
 
#line 12 "SOURCE\MAIN.H" /0
 
  
#line 1 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\INTRINS.H" /0






 
 
 
 
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 
#line 13 "SOURCE\MAIN.H" /0
 
  
#line 1 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\MATH.H" /0






 
 
 
 
 
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
 
 #pragma RESTORE
 
 
#line 14 "SOURCE\MAIN.H" /0
 
  
#line 1 "D:\PROGRAM FILES (X86)\KEIL C51\C51\INC\REG52.H" /0






 
 
 
 
 
 
 sfr P0    = 0x80;
 sfr P1    = 0x90;
 sfr P2    = 0xA0;
 sfr P3    = 0xB0;
 sfr PSW   = 0xD0;
 sfr ACC   = 0xE0;
 sfr B     = 0xF0;
 sfr SP    = 0x81;
 sfr DPL   = 0x82;
 sfr DPH   = 0x83;
 sfr PCON  = 0x87;
 sfr TCON  = 0x88;
 sfr TMOD  = 0x89;
 sfr TL0   = 0x8A;
 sfr TL1   = 0x8B;
 sfr TH0   = 0x8C;
 sfr TH1   = 0x8D;
 sfr IE    = 0xA8;
 sfr IP    = 0xB8;
 sfr SCON  = 0x98;
 sfr SBUF  = 0x99;
 
 
 sfr T2CON  = 0xC8;
 sfr RCAP2L = 0xCA;
 sfr RCAP2H = 0xCB;
 sfr TL2    = 0xCC;
 sfr TH2    = 0xCD;
 
 
 
 
 sbit CY    = PSW^7;
 sbit AC    = PSW^6;
 sbit F0    = PSW^5;
 sbit RS1   = PSW^4;
 sbit RS0   = PSW^3;
 sbit OV    = PSW^2;
 sbit P     = PSW^0;  
 
 
 sbit TF1   = TCON^7;
 sbit TR1   = TCON^6;
 sbit TF0   = TCON^5;
 sbit TR0   = TCON^4;
 sbit IE1   = TCON^3;
 sbit IT1   = TCON^2;
 sbit IE0   = TCON^1;
 sbit IT0   = TCON^0;
 
 
 sbit EA    = IE^7;
 sbit ET2   = IE^5;  
 sbit ES    = IE^4;
 sbit ET1   = IE^3;
 sbit EX1   = IE^2;
 sbit ET0   = IE^1;
 sbit EX0   = IE^0;
 
 
 sbit PT2   = IP^5;
 sbit PS    = IP^4;
 sbit PT1   = IP^3;
 sbit PX1   = IP^2;
 sbit PT0   = IP^1;
 sbit PX0   = IP^0;
 
 
 sbit RD    = P3^7;
 sbit WR    = P3^6;
 sbit T1    = P3^5;
 sbit T0    = P3^4;
 sbit INT1  = P3^3;
 sbit INT0  = P3^2;
 sbit TXD   = P3^1;
 sbit RXD   = P3^0;
 
 
 sbit SM0   = SCON^7;
 sbit SM1   = SCON^6;
 sbit SM2   = SCON^5;
 sbit REN   = SCON^4;
 sbit TB8   = SCON^3;
 sbit RB8   = SCON^2;
 sbit TI    = SCON^1;
 sbit RI    = SCON^0;
 
 
 sbit T2EX  = P1^1;  
 sbit T2    = P1^0;  
 
 
 sbit TF2    = T2CON^7;
 sbit EXF2   = T2CON^6;
 sbit RCLK   = T2CON^5;
 sbit TCLK   = T2CON^4;
 sbit EXEN2  = T2CON^3;
 sbit TR2    = T2CON^2;
 sbit C_T2   = T2CON^1;
 sbit CP_RL2 = T2CON^0;
 
 
#line 15 "SOURCE\MAIN.H" /0
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
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
 BUTTON_PRESS_DOWN = 0,
 BUTTON_PRESS_CLICK,
 BUTTON_PRESS_SHORT_START,
 BUTTON_PRESS_SHORT_UP,
 BUTTON_PRESS_LONG_START,
 BUTTON_PRESS_LONG_UP,
 BUTTON_PRESS_LONG_HOLD,
 BUTTON_PRESS_LONG_HOLD_UP,
 BUTTON_PRESS_MAX,
 BUTTON_PRESS_NONE,
 }button_event_t;
 
 





 
 
 
















































 
 extern code char * button_event_debug_string[];
 uint8_t get_key_value(uint8_t key_num);
 
 
 
 
 void lcd1602_init(void);
 void lcd1602_display_char(    uint8_t      x, uint8_t y, uint8_t ch );
 void lcd1602_display_string(    uint8_t       x, uint8_t y, uint8_t * str );
 void lcd1602_clear_display(void);
 
 
 
#line 176 "SOURCE\MAIN.H" /1
 
 
 
 
#line 180 "SOURCE\MAIN.H" /0
 
 
 
 
 
 
 
#line 12 "source\lcd1602.c" /0
 
 
 
 


 
 
 
 
#line 22 "source\lcd1602.c" /1
  
 
#line 24 "source\lcd1602.c" /0
 
 
 
 
 sbit LCD1602_PIN_RS = P2^6;
 sbit LCD1602_PIN_RW = P2^5;
 sbit LCD1602_PIN_EN = P2^7;
 


 
 


 
 


 
 


 
 


 
 


 
 
 











 
 static void lcd1602_delay_1us(void)
 {
 _nop_();
 }
 
 











 
 void lcd1602_delay_1ms(void)
 {
 unsigned char i, j;
 
 _nop_();
 i = 2;
 j = 199;
 do
 {
 while (--j);
 } while (--i);
 }
 
 











 
 static uint8_t lcd1602_read_state(void)
 {
 uint8_t state;
 
 
 LCD1602_PIN_RS = 0;
 LCD1602_PIN_RW = 1;
 LCD1602_PIN_EN = 1;
 lcd1602_delay_1us();
 state = P0;
 LCD1602_PIN_EN = 0;
 lcd1602_delay_1us();
 
 return state;
 }
 











 
 static void lcd1602_busy_wait(void)
 {
 uint16_t timeout;
 
 timeout  = 0xffff;
 while((lcd1602_read_state() & 0x80) == 0x80){
 timeout--;
 if(timeout == 0){
  ;
 break;
 }
 }
 lcd1602_delay_1us();
 }
 











 
 static void lcd1602_write_data(uint8_t dat)
 {
 
 lcd1602_busy_wait();
 LCD1602_PIN_RS = 1;
 LCD1602_PIN_RW = 0;
 LCD1602_PIN_EN = 0;
  P0 = dat;
 LCD1602_PIN_EN = 1;
 lcd1602_delay_1us();
 LCD1602_PIN_EN = 0;	
 }
 











 
 static void lcd1602_write_command(uint8_t cmd)
 {
 
 lcd1602_busy_wait();
 LCD1602_PIN_RS = 0;
 LCD1602_PIN_RW = 0;
 LCD1602_PIN_EN = 0;
  P0 = cmd;
 LCD1602_PIN_EN = 1;
 lcd1602_delay_1us();
 LCD1602_PIN_EN = 0;	
 }
 











 
 void lcd1602_init(void)
 {
 lcd1602_write_command(0x38);  
 lcd1602_delay_1ms();	
 lcd1602_write_command(0x01);  
 lcd1602_delay_1ms();	
 lcd1602_write_command(0x06);  
 lcd1602_delay_1ms();	
 lcd1602_write_command(0x0c);  
 lcd1602_delay_1ms();
 }
 













 
 void lcd1602_display_char(    uint8_t      x, uint8_t y, uint8_t ch )
 {
 if(x > 15 || y > 1){
  ;
 return;
 }
 if(y == 0){
 lcd1602_write_command(x | 0x80); 
 }else if(y == 1){
 lcd1602_write_command(x | 0x80 | 0x40); 
 }
 lcd1602_write_data( ch );
 }
 













 
 void lcd1602_display_string(    uint8_t       x, uint8_t y, uint8_t * str )
 {
 while(*str != '\0'){
 lcd1602_display_char(x, y, *str);  
 str++;   
 x++;     
 if(x > 15){
  ;
 }
 }
 }
 











 
 void lcd1602_clear_display(void)
 {
 lcd1602_write_command(0x01);
 lcd1602_delay_1ms();
 }
 
 
 
 
 
