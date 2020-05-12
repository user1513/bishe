#ifndef UART_H_
#define UART_H_
			
void uart_init();
void send_char(uchar dat);
void send_string(uchar *da);
void send_num(uint *num);

#endif