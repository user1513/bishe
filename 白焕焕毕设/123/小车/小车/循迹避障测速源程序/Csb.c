
#include "std_for_wym.h"

sbit  RX = P3^3;
sbit  TX = P3^4;                                                      
static bit flag = 0;

//����һ���ߵ�ƽʱ��
static void  StartModule(void) 		         //����ģ��
{
	TX=1;			                     //����һ��ģ��
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	_nop_();_nop_(); _nop_(); _nop_(); _nop_(); _nop_();_nop_(); 
	TX=0;
}

/*******************************************************************************
* Function Name  : Conut
* Description    : ȡ����ʱ����ֵ����ͨ������õ����룬���ؾ��뵽���ú���
* Input          : None
* Output         : None
* Return         : ����õ��ľ���
* Attention		 : ���������5mʱ������3��8����ʾ��������
*******************************************************************************/
static int Conut(void)
{
	uint S,time;
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;

	S=(time*1.7)/100;     //�������CM
	if((S>=500)||flag==1) //����������Χ , �ײ���Դﵽ6M��һ��㣬����
	{                     //�����ĺ����������ָ�Ϊ5M
		flag = 0;
		S = 888;
	}
	return S;
}

/*******************************************************************************
* Function Name  : GetDis
* Description    : ����һ��10us�ĸߵ�ƽ���壬�õ�����������ľ���
* Input          : None
* Output         : None
* Return         : S ��õľ��룬Uint�ͱ��������緵��124�����ʾΪ1.24M
* Attention		 : ���õĺ����У�StartModule��Conut
*******************************************************************************/
int GetDis(void)
{
	StartModule();      //��һ���ߵ�ƽ��������
	while(!RX);		    //��RXΪ��ʱ�ȴ�
	TR0=1;			    //��������
	while(RX);			//��RXΪ1�������ȴ�
	TR0=0;	
	return Conut();		//����
}

//��ʱ����ʼ��
static void Timer0Init(void)
{
	TMOD &= 0xf0;
	TMOD |= 0x01;	   //��T0Ϊ��ʽ1��
	TH0=0;
	TL0=0;          
	ET0=1;             //����T0�ж�
	EA=1;			   //�������ж�	
}
/*******************************************************************************
* Function Name  : CsbInit
* Description    : ��������ʼ������
* Input          : None
* Output         : None
* Return         : None
* Attention		 : ���õĺ����У�Timer0Init ,ֻ������һ����ʱ����ʼ��
*******************************************************************************/
void CsbInit(void)
{
	Timer0Init();
}

/************************** ��ʱ��0�жϷ����� *******************************/
void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
{
	flag=1;							 //�ж������־
}

