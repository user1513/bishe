
#include "std_for_wym.h"


//液晶写命令
void LCDWriteCom(unsigned char com)
{
	RS_L;
	RW_L;
	READ_DATA = com;
	EN_H;
	DelayMs(2);
	EN_L;
}
//液晶写数据
void LCDWriteData(unsigned char dat)
{
	RS_H;
	RW_L;
	READ_DATA = dat;
	EN_H;
	DelayMs(2);
	EN_L;
}
/*******************************************************************************
**函数名称：LCD_Write_str()
**函数功能：在LCD上写入一串字符
**输 入 口：hang : 要写入的行，add 要写入列   *s要写入的指针数组
**输 出 口：无
**返 回 值：无
**备    注：strlen()是引用库函数string.h  ,可以求出数组的长度
**日    期：2014.2.22
*******************************************************************************/
void LcdWriteStr(unsigned char hang,unsigned char add,char*s)
{
    unsigned char i;
    unsigned char length = 0;
	if(hang==1)   
		LCDWriteCom(0x80+add);
	else
		LCDWriteCom(0x80+0x40+add);
    length = strlen(s);
    for(i=0;i<length;i++)
      LCDWriteData(*s++);          //指针送完数据后自加一
}
/*void LcdWriteChar(unsigned char hang,unsigned char add,char Ch)
{
	if(hang == 1)LCDWriteCom(0x80+add);
	else LCDWriteCom(0x80+0x40+add);
	LCDWriteData(Ch);
}*/

//能像printf一样使用
void Lcd1602Printf( unsigned char x, unsigned char y, unsigned char *p,...)
{
	char idata LcdBuf[17];
  va_list ap;
  va_start(ap, p);
	vsprintf(LcdBuf,p,ap);
	va_end(ap);
  LcdWriteStr(x,y,LcdBuf);
}

void Init_1602(void)
{
	 LCDWriteCom(0x38);	  //设置16*2显示，5*7点阵，8位数据接口
	 LCDWriteCom(0x0c);	  //开显示，不显示光标
	 LCDWriteCom(0x06);	  //地址加1，当写入数据的时候光标右移
	 LCDWriteCom(0x01);	  // 清屏
	 DelayMs(2);
	 //Lcd1602Printf(1,0," Lcd1602 Normal ");    //测试液晶1602显示用  Normal:正常
}









