#include "inc.h"

typedef struct _Dishe
{
	char* DisheName;
	float price;
}Dishes;

typedef struct _info
{
	float weight;
	float price;
	float money;
	int Num;
}Info;

xdata Dishes temp[] ={
{"qingcai",2.2},
{"dabaicai",2.8},
{"xiguan",4.9},
{"xiangjiao",71.3},
{"xihongshi",88.2},
{"niunan",99},
{"jinju",19},
{"niunan",22},
{"qiezi",33},
{"huasheng",55},
{"niunai",88},};

Info Dishes_Info = {0,2.2,0,0};


void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 2;
	j = 103;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




void lcd_display(void);
char xdata str[35] = 0;
void main(void)
{
	char key = 0;
	uint8_t flag = 0;
	
	lcd_display();
	Timer0Init();
	UartInit();
	while(1)
	{
		key = Get_KeyPad();
		Dishes_Info.weight = GetTcl1543();
		sprintf(str, "电压值:%f, key:%c\n",Dishes_Info.weight * 5 /4095,key);
		Send_String(str);
		if(flag != 2)
		{
			sprintf(str, "WE:%01.3f PR:%02.1f",Dishes_Info.weight * 5 /4095,Dishes_Info.price);
			LcdShowStr(0x80,str);
			Dishes_Info.money = Dishes_Info.weight * 5 /4095* Dishes_Info.price;
			sprintf(str, "MONEY:%06.2fNo:%d", Dishes_Info.money, (int)Dishes_Info.Num);
			LcdShowStr(0x80 + 0x40,str);
			switch(key)
			{
				case 'S': ++ flag ; flag %= 2; uClear_KeyPad();break;
				case 0xff:break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': if(flag == 1)
							{
								flag = 2;
								LcdShowStr(0x80,"                ");
								LcdShowStr(0x80+ 0x40,"                ");
							}break;
				default:break;
			}			
		}
		if(flag == 2)
		{
			if(key == 'S')
			{
				flag = 0;
				uClear_KeyPad();
			}
			else
			{
				Dishes_Info.price = temp[key-'0'].price;
				Dishes_Info.Num = key - '0';
				sprintf(str, "菜名:%s, 单价:%f\n",temp[key-'0'].DisheName, Dishes_Info.price);
				Send_String(str);
				
				sprintf(str, "name:%s",temp[key-'0'].DisheName);
				LcdShowStr(0x80,str);
				
				sprintf(str, "price:%2.1f      ",Dishes_Info.price);	
				LcdShowStr(0x80 + 0x40,str);	
			}
			
		}
		
		Delay200ms();
		
		
		
	}
}

void lcd_display(void)
{
	LcdInit();	
	LcdShowStr(0x80,"name: XXX");
	LcdShowStr(0x80 + 0x40,"Electronic scale");
	Delay200ms();
	Delay200ms();
	Delay200ms();
	Delay200ms();
	LcdShowStr(0x80,"WE:0.000 PR:00.0");
	LcdShowStr(0x80 + 0x40,"MONEY:000.0 No:0");	
}

































