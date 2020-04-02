#include "bsp_swi2c.h"

void swi2cStart(Swi2c_T * _tSwi2c)                 /*���I2C��ʼ�ź�*/

{
    _tSwi2c->xfpI2coutput();/*����sdaΪ���ģʽ*/

    *(_tSwi2c->ulpSdaOut) = 1;/*��sda�ø�*/

    *(_tSwi2c->ulpScl) = 1;/*��scl�ø�*/

    delay_us(5);

    *(_tSwi2c->ulpSdaOut) = 0;/*��sda�õ�*/

    delay_us(5);
    
    *(_tSwi2c->ulpScl) = 0;/*��scl�õ�*/
	
	delay_us(3);

}

void swi2cStop(Swi2c_T * _tSwi2c)                  /*���I2C��ֹ�ź�*/

{

   

     _tSwi2c->xfpI2coutput();/*����sdaΪ���ģʽ*/

     *(_tSwi2c->ulpSdaOut) = 0;/*��sda�ø�*/

	delay_us(1);

    *(_tSwi2c->ulpScl) = 1;/*��scl�ø�*/

    delay_us(1);
    
    *(_tSwi2c->ulpSdaOut) = 1;/*��sda�ø�*/	

    

    //*(_tSwi2c->ulpSdaOut) = 0;/*��sda�õ�*/

   // delay_us(5);

  



}

uint8_t swi2cWaitAck(Swi2c_T * _tSwi2c, uint8_t ucState)  /*���I2C��ackӦ���ź�*/

{
    int cTimeout = 50;  //���峬ʱʱ��

    _tSwi2c->xfpI2cinput();

    *(_tSwi2c->ulpScl) = 1;/*��scl�ø�*/
    
    if(ucState)     /*ackӦ��ʹ��*/
    {
        while (*(_tSwi2c->ulpSdaIn))
        {
            delay_us(10);

            if((cTimeout--) < 0)
            {
                return 1;/*��ȡackʧ��*/
            }
        }
    }

     delay_us(5);
              
     *(_tSwi2c->ulpScl) = 0;/*��scl�ø�*/

     delay_us(5);

    return 0;/*��ȡack�ɹ�*/
}


void swi2cAck(Swi2c_T * _tSwi2c, uint8_t ucState)      /*���I2C--master�ķ���һ��ackӦ��*/

{
    _tSwi2c->xfpI2coutput(); /*����Ϊ���ģʽ*/

    if(ucState)
        *(_tSwi2c->ulpSdaOut) = 0;

    delay_us(3);
	*(_tSwi2c->ulpScl) = 1; /*scl�ø�*/
	
	delay_us(2);
    *(_tSwi2c->ulpScl) = 0; /*scl�ø�*/
    delay_us(1);
    *(_tSwi2c->ulpSdaOut) = 1;
    delay_us(3);

}


uint8_t swi2cReceiveByte(Swi2c_T * _tSwi2c)           /*���I2C�Ķ�ȡһ���ֽ�*/

{
    uint8_t ucData = 0;

    _tSwi2c->xfpI2cinput(); /*����Ϊ����ģʽ*/

    

    for (int i = 0; i < 8; i++)
    {
        /* code */
        *(_tSwi2c->ulpScl) = 1; /*scl�ø�*/

        delay_us(2);

        ucData |=  *(_tSwi2c->ulpSdaIn);

        ucData <<= (i < 7 ? 1 : 0);

        delay_us(3);
    
        *(_tSwi2c->ulpScl) = 0;/*scl�õ�*/

        delay_us(5);

//        *(_tSwi2c->ulpScl) = 1; /*scl�ø�*/
    }
	return ucData;
}

void swi2cSendByte(Swi2c_T * _tSwi2c, uint8_t _ucData)              /*���I2C�ķ���һ���ֽ�*/

{
    _tSwi2c->xfpI2coutput(); /*����Ϊ���ģʽ*/

    for(int i = 7; i >= 0; i--)
    {
        *(_tSwi2c->ulpSdaOut) = (0x01) &(_ucData >> i);

        delay_us(2);

        *(_tSwi2c->ulpScl) = 1; /*scl�ø�*/

        delay_us(5);

        *(_tSwi2c->ulpScl) = 0; /*scl�ø�*/

        delay_us(3);
    }
}




