

/*
*************************************************************************
*产品名称：超声波避障寻迹小车
*硬件平台：焊好的模块
*显示模块：液晶1602
*输入模块：暂无
*实现的功能：寻迹小车加避障功能  ,  当前面有障碍物，则小车向右转
*操作描述：上电即可
*接口说明：----------------------------------------- 
*当前的缺陷：  
*待改进的地方：暂无
*遇到的问题：—————————————————————————————— 
*开发平台：KELL_MDK4.7

*备    注： s = t * v;   v = s / t; 
*
*第一次修改时间：2014.4.11 
*第一次修改内容：增加有前面距离小于20CM就右转的功能,测试的现象是刚好右转了180度
*
*第二次修改时间：
*第二次修改内容：
*************************************************************************
*/

#include "std_for_wym.h"
#include "eepom52.h"//
#define L 3
#define R 4

/***************************
              直径6.5cm
			  周长：3.14*6.5 = 20.4cm	 小车走一圈就是 20.4cm
				 // 2.55 = 20.4 / 8 = 2.55 算出每一个脉冲的轮子走了多少距离
*****************************/

// long lSpeedCount = 0;
// int iSpeedCount = 0;
// float fSpeed = 0;

/********************寻迹模块IO口定义*********************************/
sbit xun_ll = P0^4;	  //从左到右第1个
sbit xun_l  = P0^3;	  //从左到右第2个
sbit xun_z  = P0^2;	  //从左到右第3个
sbit xun_r  = P0^1;	  //从左到右第4个
sbit xun_rr = P0^0;	  //从左到右第5个

/********************LN298电机驱动IO口定义*********************************/
sbit qu_ll = P2^0;	   //左边电机控制IN1
sbit qu_zl = P2^1;	   //左边电机控制IN2
sbit qu_zr = P2^2;	   //右边电机控制IN1
sbit qu_rr = P2^3;	   //右边电机控制IN2



/***********************小车前进函数************************/
void go()
{
	qu_ll = 1;  
	qu_zl = 0; 
	qu_zr = 0; 
	qu_rr = 1;    	
}

// /***********************小车后退函数************************/
// void back()
// {
// 	qu_ll = 0;  
// 	qu_zl = 1; 
// 	qu_zr = 1; 
// 	qu_rr = 0;    	
// }

/***********************小车左转函数 只有一个轮子动************************/
void left()
{
	qu_ll = 0;  
	qu_zl = 0; 
	qu_zr = 0; 
	qu_rr = 1;    	
}

/***********************小车左转函数 左边轮子后退 右边轮子前进************************/
void left_s()
{
	qu_ll = 0;  
	qu_zl = 1; 
	qu_zr = 0; 
	qu_rr = 1;    	
}

/***********************小车停下函数************************/
void stop()
{
	qu_ll = 0;  
	qu_zl = 0; 
	qu_zr = 0; 
	qu_rr = 0;
}

/***********************小车右转函数 只有一个轮子动************************/
void right()
{
	qu_ll = 1;  
	qu_zl = 0; 
	qu_zr = 0; 
	qu_rr = 0;    	
}

/***********************小车右转函数 左边轮子前进 右边轮子后退************************/
void right_s()
{
	qu_ll = 1;  
	qu_zl = 0; 
	qu_zr = 1; 
	qu_rr = 0;	
}


void che_90_180_break(uchar dat)
{	
	uchar i_z = 0;
	while(1)			  //循环等待中间寻迹传感器到黑线上
	{	
	    if(xun_z == 0)	  //如果中间那寻迹传感器到了黑线上，要让小车停下来前进了
		{
			i_z++;	      //消去干扰
			if(i_z >= 10) //10次之后就确定寻迹模块的中间传感器到黑线上了
			{
				if(dat == L)   //如果是左转90度就让右转的方法制动
					right_s();	
				else 
					left_s();  //如果是右转90度就让左转的方法制动
				DelayMs(50);
				go();		 
				break;		   //break退出while(1)这个死循环
			}
		}
		else	   //没有在黑钱上就给i_z变量清零
		{
			i_z = 0;
		}	
	}
}

/***********************小车左转90度************************/
void left_s_90_while()		//小车向左转90度
{
	go();					//前进一小会让小车转90度时刚好让黑线在小车的中间				  
	DelayMs(120);
	left_s();				//左转90度注意这个延时不能太长  只要能让寻迹模块中间的传感离开黑线就好
	DelayMs(180);
	che_90_180_break(L);	
}

/***********************小车向右转90度************************/
void right_s_90_while()	    //
{
	go();					//前进一小会让小车转90度时刚好让黑线在小车的中间				  
	DelayMs(120);
	right_s();				//左转90度注意这个延时不能太长  只要能让寻迹模块中间的传感离开黑线就好
	DelayMs(180);
	che_90_180_break(R);	
}




// 白线为 1   黑线为 0
void xunnji()
{
	if((xun_ll==1) && (xun_l==1) && (xun_z==0) && (xun_r==1) && (xun_rr==1))
	{
		go();	  //小车前进
	}		
	if((xun_ll==0) && (xun_l==0) && (xun_z==0) && (xun_r==0) && (xun_rr==0)) 
	{	
		stop();					
	}
	
	if(((xun_ll==1) && (xun_l==1) && (xun_z==0) && (xun_r==0) && (xun_rr==1)) ||
	   ((xun_ll==1) && (xun_l==1) && (xun_z==1) && (xun_r==0) && (xun_rr==1)) ||
	   ((xun_ll==1) && (xun_l==1) && (xun_z==1) && (xun_r==0) && (xun_rr==0)) ||
	   ((xun_ll==1) && (xun_l==1) && (xun_z==1) && (xun_r==1) && (xun_rr==0)))	   
	{
		right();
	}
	if(((xun_ll==1) && (xun_l==0) && (xun_z==0) && (xun_r==1) && (xun_rr==1)) ||
	   ((xun_ll==1) && (xun_l==0) && (xun_z==1) && (xun_r==1) && (xun_rr==1)) ||
	   ((xun_ll==0) && (xun_l==0) && (xun_z==1) && (xun_r==1) && (xun_rr==1)) ||
	   ((xun_ll==0) && (xun_l==1) && (xun_z==1) && (xun_r==1) && (xun_rr==1)))	   
	{
		left();
	}
	if((xun_ll==1) && (xun_z==0) && (xun_r==0) && (xun_rr==0))	   
	{		
		go();								
		DelayMs(1);
		if((xun_ll==1) && (xun_z==0) && (xun_r==0) && (xun_rr==0))	 
		{	
			right_s_90_while();
		}
	}	
	if((xun_ll==0) && (xun_l==0)  && (xun_z==0) && (xun_rr==1)) 	   
	{
		go();
		DelayMs(1);
		if((xun_ll==0) && (xun_l==0)  && (xun_z==0) && (xun_rr==1)) 	 
		{
			left_s_90_while();
		}
	}
} 
/************************************************
** 函数名称 ： void int0_init(void)
** 函数功能 ： 外部中断0初始化
************************************************/
void int0_init(void)
{
	IT0=1;	    //下降沿触发
	EX0=1;		//中断标志位置1
	EA=1;		//打开总中断
}
uchar count_v=0,v=0;
void main(void)
{
	uchar speed_v[16]={"V=00cm/sL=000.0m"};
	uint S;
	int Dis = 50;						
	uchar i=0;
	InitTimer();	//定时器初始化
	Init_1602();    //液晶初始化
	CsbInit();		//超声波初始化
	int0_init();
	while(1)
	{
		xunnji();	 //循迹函数
		if(time_1000ms)
		{	
			time_1000ms = 0 ;
			v=count_v*2.55;
			count_v=0;
			S+=v;

			xunnji();	 //循迹函数

			Dis = GetDis();	  //读出距离
			Lcd1602Printf(2,3,"Dis = %4.2f",Dis/100.0);

			xunnji();	 //循迹函数

			if(Dis < 24)	  //壁障距离
			{
				right_s();	  //右转
				DelayMs(500);
			}

			xunnji();	 //循迹函数

			speed_v[2]=v/10+0x30;
			speed_v[3]=v%10+0x30;
			xunnji();
			speed_v[10]=S/10000+0x30;
			speed_v[11]=S/1000%10+0x30;
			speed_v[12]=S/100%10+0x30;
			speed_v[14]=S/10%10+0x30;

			xunnji();	 //循迹函数

			Lcd1602Printf(1,0,speed_v);

			xunnji();	 //循迹函数
		}	
	}
}
/************************************************
** 函数名称 ： void int0() interrupt 0
** 函数功能 ： 外部0中断函数
************************************************/
void int0() interrupt 0
{	
	count_v++;	
}





