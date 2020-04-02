#ifndef _BSP_SWI2C_H
#define _BSP_SWI2C_H

#include "bsp.h"

typedef void (*I2C_FUNC)(void);


typedef struct tagSwi2c_T
{
    /* data */
    unsigned long* ulpScl;              /*����ָ��scl��ַ*/

    unsigned long* ulpSdaOut;              /*����ָ��sda��ַ*/

    unsigned long* ulpSdaIn;              /*����ָ��sda��ַ*/

    I2C_FUNC    xfpI2cinput;                        /*�Զ��庯����ָ��,����ָ��sdaΪ����ģʽ*/

    I2C_FUNC    xfpI2coutput;                       /*�Զ��庯����ָ��,����ָ��sdaΪ���ģʽ*/

}Swi2c_T;

void swi2cStart(Swi2c_T * _tSwi2c);                 /*���I2C��ʼ�ź�*/

void swi2cStop(Swi2c_T * _tSwi2c);                  /*���I2C��ֹ�ź�*/

uint8_t swi2cReceiveByte(Swi2c_T * _tSwi2c);           /*���I2C�Ķ�ȡһ���ֽ�*/

void swi2cSendByte(Swi2c_T * _tSwi2c, uint8_t _ucData);              /*���I2C�ķ���һ���ֽ�*/

uint8_t swi2cWaitAck(Swi2c_T * _tSwi2c, uint8_t ucState); /*���I2C�ĵȴ�slave�� ackӦ���ź�*/

void swi2cAck(Swi2c_T * _tSwi2c, uint8_t ucState);      /*���I2C--master�ķ���һ��ackӦ��*/

#endif




