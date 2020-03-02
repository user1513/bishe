#include <reg52.h>
#include <intrins.h>
#include "math.h"
#include "BSP_DS18B20.h"
#include "bsp_usart.h"
#include "stdio.h"
#define uchar unsigned char	// 以后unsigned char就可以用uchar代替
#define uint  unsigned int	// 以后unsigned int 就可以用uint 代替
#define CLAC_SPEED(X)  		X / 10000.0
float 	Audio_speed = 0.0f; 			//voice sound
float 	Temp_val = 0.0f; 			//voice sound
sfr ISP_DATA  = 0xe2;			// 数据寄存器
sfr ISP_ADDRH = 0xe3;			// 地址寄存器高八位
sfr ISP_ADDRL = 0xe4;			// 地址寄存器低八位
sfr ISP_CMD   = 0xe5;			// 命令寄存器
sfr ISP_TRIG  = 0xe6;			// 命令触发寄存器
sfr ISP_CONTR = 0xe7;			// 命令寄存器

sbit LcdRs_P   = P1^1;		// 1602液晶的RS管脚       
sbit LcdRw_P   = P1^2;		// 1602液晶的RW管脚 
sbit LcdEn_P   = P1^3;		// 1602液晶的EN管脚
sbit Trig1_P   = P2^5;		// 超声波模块1的Trig管脚
sbit Echo1_P   = P2^6;		// 超声波模块1的Echo管脚
sbit Trig2_P   = P3^5;		// 超声波模块2的Trig管脚
sbit Echo2_P   = P3^6;		// 超声波模块2的Echo管脚
sbit Trig3_P   = P3^2;		// 超声波模块3的Trig管脚
sbit Echo3_P   = P3^3;		// 超声波模块3的Echo管脚
sbit Trig4_P   = P1^4;		// 超声波模块4的Trig管脚
sbit Echo4_P   = P1^5;		// 超声波模块4的Echo管脚
sbit KeySet_P  = P2^2;		// 设置按键的管脚
sbit KeyDown_P = P2^1;		// 减按键的管脚
sbit KeyUp_P   = P2^0;		// 加按键的管脚
sbit Buzzer_P  = P2^3;		// 蜂鸣器的管脚
sbit Led1_P    = P2^4;		// 传感器1报警灯
sbit Led2_P    = P3^4;		// 传感器2报警灯
sbit Led3_P    = P1^6;		// 传感器3报警灯
sbit Led4_P    = P1^0;		// 传感器4报警灯

uint gAlarm;							// 报警距离变量


//温度与声速
//方法一公式法
//V=331×根号(1+T/273)(m/S)
//T：是摄氏温度；V：在T℃时的音速(声速)
float datapros(int temp) 	 
{
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp = temp-1;
		temp = ~temp;
		tp = - temp*0.0625;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		//如果温度是正的那么，那么正数的原码就是补码它本身
		tp = temp*0.0625;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	return tp;
}

float Sound_Speed_func()
{
	Temp_val = datapros(Ds18b20ReadTemp());
	Audio_speed = 331 * sqrt(1 + (Temp_val) / 273.0);
	return Audio_speed;
}
//方法二查表法
//没有找到合适的表格,待续
		





/*********************************************************/
// 单片机内部EEPROM不使能
/*********************************************************/
void ISP_Disable()
{
	ISP_CONTR = 0;
	ISP_ADDRH = 0;
	ISP_ADDRL = 0;
}


/*********************************************************/
// 从单片机内部EEPROM读一个字节，从0x2000地址开始
/*********************************************************/
unsigned char EEPROM_Read(unsigned int add)
{
	ISP_DATA  = 0x00;
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x01;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	// 对STC89C51系列来说，每次要写入0x46，再写入0xB9,ISP/IAP才会生效
	ISP_TRIG  = 0x46;	   
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
	return (ISP_DATA);
}


/*********************************************************/
// 往单片机内部EEPROM写一个字节，从0x2000地址开始
/*********************************************************/
void EEPROM_Write(unsigned int add,unsigned char ch)
{
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x02;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	ISP_DATA  = ch;
	ISP_TRIG  = 0x46;
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
}


/*********************************************************/
// 擦除单片机内部EEPROM的一个扇区
// 写8个扇区中随便一个的地址，便擦除该扇区，写入前要先擦除
/*********************************************************/
void Sector_Erase(unsigned int add)	  
{
	ISP_CONTR = 0x83;
	ISP_CMD   = 0x03;
	ISP_ADDRH = (unsigned char)(add>>8);
	ISP_ADDRL = (unsigned char)(add&0xff);
	ISP_TRIG  = 0x46;
	ISP_TRIG  = 0xB9;
	_nop_();
	ISP_Disable();
}



/*********************************************************/
// 毫秒级的延时函数，time是要延时的毫秒数
/*********************************************************/
void DelayMs(uint time)
{
	uint i,j;
	for(i=0;i<time;i++)
		for(j=0;j<112;j++);
}


/*********************************************************/
// 1602液晶写命令函数，cmd就是要写入的命令
/*********************************************************/
void LcdWriteCmd(uchar cmd)
{ 
	LcdRs_P = 0;
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=cmd;
	DelayMs(2);
	LcdEn_P = 1;    
	DelayMs(2);
	LcdEn_P = 0;	
}


/*********************************************************/
// 1602液晶写数据函数，dat就是要写入的数据
/*********************************************************/
void LcdWriteData(uchar dat)
{
	LcdRs_P = 1; 
	LcdRw_P = 0;
	LcdEn_P = 0;
	P0=dat;
	DelayMs(2);
	LcdEn_P = 1;    
	DelayMs(2);
	LcdEn_P = 0;
}


/*********************************************************/
// 液晶光标定位函数
/*********************************************************/
void LcdGotoXY(uchar line,uchar column)
{
	// 第一行
	if(line==0)        
		LcdWriteCmd(0x80+column); 
	// 第二行
	if(line==1)        
		LcdWriteCmd(0x80+0x40+column); 
}



/*********************************************************/
// 液晶输出字符串函数
/*********************************************************/
void LcdPrintStr(uchar *str)
{
	while(*str!='\0')
			LcdWriteData(*str++);
}


/*********************************************************/
// 液晶输出数字
/*********************************************************/
void LcdPrintNum(uint num)
{
	LcdWriteData(num/100+0x30);				// 百位
	LcdWriteData(num%100/10+0x30);		// 十位
	LcdWriteData(num%10+0x30);				// 个位
}


/*********************************************************/
// 1602液晶功能初始化
/*********************************************************/
void LcdInit()
{
	LcdWriteCmd(0x38);        // 16*2显示，5*7点阵，8位数据口
	LcdWriteCmd(0x0C);        // 开显示，不显示光标
	LcdWriteCmd(0x06);        // 地址加1，当写入数据后光标右移
	LcdWriteCmd(0x01);        // 清屏
}



/*********************************************************/
// 1602液晶显示内容初始化
/*********************************************************/
void LcdShowInit()
{
	LcdGotoXY(0,0);	    							// 定位到第0行第0列
	LcdPrintStr("      U         ");	// 第0行显示“      U         ”
	LcdGotoXY(1,0);	    							// 定位到第1行第0列
	LcdPrintStr(" L    D    R    ");	// 第1行显示“ L    D    R    ”
}


/*********************************************************/
// 计算传感器1测量到的距离
/*********************************************************/
float GetDistance1(void)
{
	float ss;					// 用于记录测得的距离

	TH0=0;
	TL0=0;

	Trig1_P=1;				// 给超声波模块1一个开始脉冲
	DelayMs(1);
	Trig1_P=0;

	while(!Echo1_P);	// 等待超声波模块1的返回脉冲
	TR0=1;						// 启动定时器，开始计时
	while(Echo1_P);		// 等待超声波模块1的返回脉冲结束
	TR0=0;						// 停止定时器，停止计时

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// 距离cm=（时间us * 速度cm/us）/2
	return ss;
}


/*********************************************************/
// 计算传感器2测量到的距离
/*********************************************************/
float GetDistance2(void)
{
	float ss;					// 用于记录测得的距离

	TH0=0;
	TL0=0;

	Trig2_P=1;				// 给超声波模块2一个开始脉冲
	DelayMs(1);
	Trig2_P=0;

	while(!Echo2_P);	// 等待超声波模块2的返回脉冲
	TR0=1;						// 启动定时器，开始计时
	while(Echo2_P);		// 等待超声波模块2的返回脉冲结束
	TR0=0;						// 停止定时器，停止计时

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// 距离cm=（时间us * 速度cm/us）/2
	return ss;
}


/*********************************************************/
// 计算传感器3测量到的距离
/*********************************************************/
float GetDistance3(void)
{
	float ss;					// 用于记录测得的距离

	TH0=0;
	TL0=0;

	Trig3_P=1;				// 给超声波模块3一个开始脉冲
	DelayMs(1);
	Trig3_P=0;

	while(!Echo3_P);	// 等待超声波模块3的返回脉冲
	TR0=1;						// 启动定时器，开始计时
	while(Echo3_P);		// 等待超声波模块3的返回脉冲结束
	TR0=0;						// 停止定时器，停止计时

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// 距离cm=（时间us * 速度cm/us）/2
	return ss;
}


/*********************************************************/
// 计算传感器4测量到的距离
/*********************************************************/
float GetDistance4(void)
{
	float ss;					// 用于记录测得的距离

	TH0=0;
	TL0=0;

	Trig4_P=1;				// 给超声波模块4一个开始脉冲
	DelayMs(1);
	Trig4_P=0;

	while(!Echo4_P);	// 等待超声波模块4的返回脉冲
	TR0=1;						// 启动定时器，开始计时
	while(Echo4_P);		// 等待超声波模块4的返回脉冲结束
	TR0=0;						// 停止定时器，停止计时

	ss=((TH0*256+TL0)*CLAC_SPEED(Sound_Speed_func()))/2;		// 距离ms=（时间us * 速度cm/us）/2
	return ss;
}


/*********************************************************/
// 按键扫描
/*********************************************************/
void KeyScanf()
{
	if(KeySet_P==0)					// 判断是否有按键按下
	{
		LcdGotoXY(0,0);	    							// 光标定位
		LcdPrintStr("   Alarm Set    ");	// 第0行显示“   Alarm Set    ”
		LcdGotoXY(1,0);										// 光标定位
		LcdPrintStr("  alarm=   cm   ");	// 第1行显示“  alarm=   cm   ”
		LcdGotoXY(1,8);										// 光标定位
		LcdPrintNum(gAlarm);							// 显示当前的报警值

		DelayMs(10);											// 消除按键按下的抖动
		while(!KeySet_P);									// 等待按键释放
		DelayMs(10);											// 消除按键松开的抖动		

		while(1)
		{		
			/* 报警值减的处理 */
			if(KeyDown_P==0)					
			{
				if(gAlarm>2)						// 报警值大于2才能减1
					gAlarm--;							// 报警值减1
				LcdGotoXY(1,8);					// 光标定位
				LcdPrintNum(gAlarm);		// 刷新修改后的报警值
				DelayMs(300);						// 延时
			}

			/* 报警值加的处理 */
			if(KeyUp_P==0)					
			{
				if(gAlarm<400)					// 报警值小于400才能加1
					gAlarm++;							// 报警值加1
				LcdGotoXY(1,8);					// 光标定位
				LcdPrintNum(gAlarm);		// 刷新修改后的报警值
				DelayMs(300);						// 延时
			}
			
			/* 退出报警值设置 */
			if(KeySet_P==0)					
			{
				break;									// 退出while循环
			}	
		}
		
		LcdShowInit();							// 液晶恢复测量到测量界面
		DelayMs(10);		  					// 消除按键按下的抖动
		while(!KeySet_P);						// 等待按键释放
		DelayMs(10);		  					// 消除按键松开的抖动		

		Sector_Erase(0x2000);				// 保存报警距离
		EEPROM_Write(0x2000,gAlarm/100);
		EEPROM_Write(0x2001,gAlarm%100);		
	}	
}


/*********************************************************/
// 传感器1报警判断
/*********************************************************/
void AlarmJudge1(float ss)
{
	if(ss<gAlarm)		// LED灯判断
	{
		Led1_P=0;
	}
	else
	{
		Led1_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// 蜂鸣器判断
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// 传感器2报警判断
/*********************************************************/
void AlarmJudge2(float ss)
{
	if(ss<gAlarm)		// LED灯判断
	{
		Led2_P=0;
	}
	else
	{
		Led2_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// 蜂鸣器判断
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// 传感器3报警判断
/*********************************************************/
void AlarmJudge3(float ss)
{
	if(ss<gAlarm)		// LED灯判断
	{
		Led3_P=0;
	}
	else
	{
		Led3_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// 蜂鸣器判断
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// 传感器4报警判断
/*********************************************************/
void AlarmJudge4(float ss)
{
	if(ss<gAlarm)		// LED灯判断
	{
		Led4_P=0;
	}
	else
	{
		Led4_P=1;
	}
	
	if((Led1_P==0)||(Led2_P==0)||(Led3_P==0)||(Led4_P==0))	// 蜂鸣器判断
	{
		Buzzer_P=0;
	}
	else
	{
		Buzzer_P=1;
	}
}


/*********************************************************/
// 报警值初始化
/*********************************************************/
void AlarmInit()
{
	gAlarm=EEPROM_Read(0x2000)*100+EEPROM_Read(0x2001) * 10;		// 从EEPROM读取报警值

	if((gAlarm==0)||(gAlarm>400))			// 如果读取到的报警值异常（等于0或大于400则认为异常）
	{
		gAlarm=25;											// 重新赋值报警值为25
	}
}

char temp_table[20]  ;
code int distance[2][8] = { 
{30,50,100,150,200,250,300,800},				
{27,45,90,136,181,226,272,726}};

uint Calibration_Data(uint _val)
{
	char flag = 0xff;
	if(_val >= distance[1][0] && _val < distance[1][1]){
		flag = 0;
	}else if(_val >= distance[1][1] && _val < distance[1][2]){
		flag = 1;
	}else if(_val >= distance[1][2] && _val < distance[1][3]){
		flag = 2;
	}else if(_val >= distance[1][3] && _val < distance[1][4]){
		flag = 3;
	}else if(_val >= distance[1][4] && _val < distance[1][5]){
		flag = 4;
	}else if(_val >= distance[1][5] && _val < distance[1][6]){
		flag = 5;
	}else if(_val >= distance[1][6] && _val <= distance[1][7]){
		flag = 6;
	}
	
	return  (distance[0][flag + 1] - distance[0][flag]) / \
			(distance[1][flag + 1] - distance[1][flag]) * \
			(_val - distance[1][flag]) + distance[0][flag];
	
	
}
/*********************************************************/
// 主函数
/*********************************************************/
void main()
{
	uchar i;									// 循环变量
	float dist;								// 保存测量结果
	
	LcdInit();								// 液晶功能初始化
	LcdShowInit();						// 液晶显示内容初始化
	AlarmInit();							// 报警值初始化
	
	TMOD |= 0x01;							// 选择定时器0，并且确定是工作方式1（为了超声波模块测量距离计时用的）
	UartInit();
	Trig1_P=0;								// 初始化触发引脚为低电平
	Trig2_P=0;
	Trig3_P=0;
	Trig4_P=0;

	while(1)
	{
		sprintf(temp_table, "temp:%04.1f\n",Temp_val);
		Send_String(temp_table);
		sprintf(temp_table, "voice:%04.3f\n",CLAC_SPEED(Audio_speed));
		Send_String(temp_table);
		/*传感器1*/
		LcdGotoXY(0,6);	    		// 光标定位
		dist = Calibration_Data(GetDistance1() * 10);
		LcdPrintNum(dist);			// 显示传感器1测量到的距离
		AlarmJudge1(dist);			// 判断传感器1的测量距离是否需要报警
		

		/*延时并扫描按键*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*传感器2*/
		LcdGotoXY(1,13);	    	// 光标定位
		dist = Calibration_Data(GetDistance2() * 10);
		LcdPrintNum(dist);			// 显示传感器2测量到的距离
		AlarmJudge2(dist);			// 判断传感器2的测量距离是否需要报警
		
		/*延时并扫描按键*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*传感器3*/
		dist = Calibration_Data(GetDistance3() * 10);
		LcdGotoXY(1,7);	    		// 光标定位
		LcdPrintNum(dist);			// 显示传感器3测量到的距离
		AlarmJudge3(dist);			// 判断传感器3的测量距离是否需要报警
		
		/*延时并扫描按键*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
		
		/*传感器4*/
		dist = Calibration_Data(GetDistance4() * 10);
		LcdGotoXY(1,1);	    		// 光标定位
		LcdPrintNum(dist);			// 显示传感器4测量到的距离
		AlarmJudge4(dist);			// 判断传感器4的测量距离是否需要报警
		
		/*延时并扫描按键*/
		for(i=0;i<15;i++)
		{
			KeyScanf();
			DelayMs(10);
		}
	}
}