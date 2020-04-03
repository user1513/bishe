#ifndef _BSP_SHT11_H
#define _BSP_SHT11_H

#include "stdlib.h"


#define SHT11_DATA     P1_4
#define SHT11_SCK     P1_3


enum {TEMP,HUMI};      //TEMP=0,HUMI=1


#define noACK 0             //�����ж��Ƿ����ͨѶ
#define ACK   1             //�������ݴ���
                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

/****************���庯��****************/
void s_connectionreset(void);          //���Ӹ�λ����
void vAutoGetSht11(char isprintf);    //isprintf �����ӡ
float fGetHumi(void);
float fGetTemp(void);


#endif


