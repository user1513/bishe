
#include "std_for_wym.h"

sbit  RX = P3^3;
sbit  TX = P3^4;                                                      
static bit flag = 0;

//触发一个高电平时间
static void  StartModule(void) 		         //启动模块
{
	TX=1;			                     //启动一次模块
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	TX=0;
}

/*******************************************************************************
* Function Name  : Conut
* Description    : 取出定时器的值，并通过计算得到距离，返回距离到调用函数
* Input          : None
* Output         : None
* Return         : 计算得到的距离
* Attention		 : 当距离大于5m时，返回3个8，表示超出量程
*******************************************************************************/
static int Conut(void)
{
	uint S,time;
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;

	S=(time*1.7)/100;     //算出来是CM
	if((S>=500)||flag==1) //超出测量范围 , 亲测可以达到6M多一点点，不过
	{                     //跳动的很厉厉害，现改为5M
		flag = 0;
		S = 888;
	}
	return S;
}

/*******************************************************************************
* Function Name  : GetDis
* Description    : 发出一个10us的高电平脉冲，得到超声波测出的距离
* Input          : None
* Output         : None
* Return         : S 测得的距离，Uint型变量，比如返回124，则表示为1.24M
* Attention		 : 调用的函数有：StartModule，Conut
*******************************************************************************/
int GetDis(void)
{
	StartModule();      //给一个高电平触发脉冲
	while(!RX);		    //当RX为零时等待
	TR0=1;			    //开启计数
	while(RX);			//当RX为1计数并等待
	TR0=0;	
	return Conut();		//计算
}

//定时器初始化
static void Timer0Init(void)
{
	TMOD &= 0xf0;
	TMOD |= 0x01;	   //设T0为方式1，
	TH0=0;
	TL0=0;          
	ET0=1;             //允许T0中断
	EA=1;			   //开启总中断	
}
/*******************************************************************************
* Function Name  : CsbInit
* Description    : 超声波初始化函数
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 调用的函数有：Timer0Init ,只调用了一个定时器初始化
*******************************************************************************/
void CsbInit(void)
{
	Timer0Init();
}

/************************** 定时器0中断服务函数 *******************************/
void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
	flag=1;							 //中断溢出标志
}

