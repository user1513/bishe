#include "bsp_405x.h"

static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw);

float ES_Val = 0 ;
float Get_ES_Val(void)
{
	return(ES_Val);
}
uint8_t laststate = 1;
char xdata xstr[10] = 0;
uint8_t Electronic_scale_scan(uint8_t _scan,float val)
{
	
	static uint8_t State_Machine = 1;
	uint8_t temp = 0;
	switch(State_Machine)
	{
		case 1:
			switch(_scan)
			{
				case 1:INPUT_ONE_ENALBE 		break;
				case 2:INPUT_TWO_ENALBE 		break;
				case 3:INPUT_THREE_ENALBE 		break;
				case 4:INPUT_FOUR_ENALBE 		break;
				case 5:INPUT_CLEAR_ENALBE 		break;
				default :INPUT_CLEAR_ENALBE	break;
			}
			State_Machine++;
		break;
		case 2:
			laststate = Electronic_Auto_Mode(val, 1);
			switch(laststate)
				{
					case 1:OP_1_ENALBE 		break;
					case 2:OP_5_ENALBE 		break;
					case 3:OP_25_ENALBE 	break;
					default:break;
				}
//			State_Machine++;	
				State_Machine = 4;
//				sprintf(xstr, "ls10:%d",(int)laststate);
//				Send_String(xstr);
			break;
			/*
			temp = Electronic_Auto_Mode(val, laststate);
			if(temp != laststate)
			{
				laststate = temp;
				switch(laststate)
				{
					case 1:OP_1_ENALBE 		break;
					case 2:OP_5_ENALBE 		break;
					case 3:OP_25_ENALBE 	break;
					default:break;
				}
				State_Machine++;
				break;
			}		
			*/	
//		case 3:	State_Machine++;break;	
		case 4:	State_Machine++;break;			
		case 5:
//			if(Electronic_Auto_Mode(val,laststate) != laststate)
//				State_Machine = 2;
//			else 
			{
				State_Machine = 1;
//				sprintf(xstr, "ls20:%d",(int)laststate);
//				Send_String(xstr);
				switch(laststate)
				{
					case 1: val /= 1.0;		break;
					case 2: val /= 4.5; 	break;
					case 3:	 val/= 25.0; 	break;
					default:val = 0;break;
				}
				ES_Val = val;
				OP_1_ENALBE
			}
				
		break;
	}
	return 0;
}

static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw)
{
	uint8_t state = 0;
	switch(dw)
	{
		case 1: if(val >= 1)
					state = 1;
				else if(val < 1 && val >= 0.2)
					state = 2;
				else if(val > 0)
					state = 3;
		break;
		case 2:
				if(val >= 1)
					state = 2;
				else 
					state = 3;
		break;
		case 3:
			if(val < 5)
					state = 3;
			break;
	}
	return state;
}
