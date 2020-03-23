#include "bsp_lcd.h"

/*******************************************************************************
* 函 数 名         : Delay1ms
* 函数功能		   : 延时函数，延时1ms
* 输    入         : c
* 输    出         : 无
* 说    名         : 该函数是在12MHZ晶振下，12分频单片机的延时。
*******************************************************************************/

void Delay1ms(uchar z)		//@11.0592MHz
{
	unsigned char i, j,k;
	for(k=0;k<z;k++)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Delay100us()		//@11.0592MHz
{
	unsigned char i;

//	_nop_();
//	i = 43;
//	while (--i);
	

	_nop_();//实际延时50us//仿真时候用
	i = 100;
	while (--i);
}


/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : com
* 输    出         : 无
*******************************************************************************/
#ifndef 	LCD1602_4PINS	 //当没有定义这个LCD1602_4PINS时
void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;     //使能
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	
	LCD1602_DATAPINS = com;     //放入命令
	//Delay1ms(1);		//等待数据稳定
	Delay100us();
	LCD1602_E = 1;	          //写入时序
	Delay100us();	  //保持时间
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;	 //使能清零
	LCD1602_RS = 0;	 //选择写入命令
	LCD1602_RW = 0;	 //选择写入

	LCD1602_DATAPINS = com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	 //写入时序
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;

//	Delay1ms(1);
	LCD1602_DATAPINS = com << 4; //发送低四位
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	 //写入时序
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/		   
#ifndef 	LCD1602_4PINS		   
void LcdWriteData(uchar dat)			//写入数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;   //写入时序
	Delay100us();   //保持时间
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)			//写入数据
{
	LCD1602_E = 0;	  //使能清零
	LCD1602_RS = 1;	  //选择写入数据
	LCD1602_RW = 0;	  //选择写入

	LCD1602_DATAPINS = dat;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	  //写入时序
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; //写入低四位
	//Delay1ms(1);
	Delay100us();
	LCD1602_E = 1;	  //写入时序
	Delay100us();
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* 函 数 名       : LcdInit()
* 函数功能		 : 初始化LCD屏
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/		   
#ifndef		LCD1602_4PINS
void LcdInit()						  //LCD初始化子程序
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}
#else
void LcdInit()						  //LCD初始化子程序
{
	LcdWriteCom(0x32);	 //将8位总线转为4位总线
	LcdWriteCom(0x28);	 //在四位线下的初始化
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}
#endif

void LcdShowStr(char cAddr,char *pStr)
{
	while(*pStr != '\0')
	{
		LcdWriteCom(cAddr);
		cAddr++;
		LcdWriteData(*pStr);
		pStr++;
	}
}

void LcdShowStrToLength(char cAddr,char *pStr,char length)
{
	while(length--)
	{
		LcdWriteCom(cAddr);
		cAddr++;
		LcdWriteData(*pStr);
		pStr++;
	}
}


