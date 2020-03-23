#include "bsp_405x.h"
//test

const float Electronic_dw[3] = {1, 4.48, 45};//����ÿ����λ�ķŴ���,������1->3��λ

struct _Es_Range
{
	/* data */
	float Range_min;		//��λ��Сֵ
	float Range_max;		//��λ���ֵ
};

xdata struct _Es_Range Es_Range[3] ={ 
{1, 5},				  //���õ�һ����λ�ķ�Χ
{0.1, 1},			  //���õڶ�����λ�ķ�Χ
{0, 0.1}};			   //���õ�������λ�ķ�Χ

static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw);//�Զ���ȡ���ʵ�λ

static double ES_Val[2] = 0 ;//��ȡ�ĵ�ѹֵ
static double ES_Table_Val[4] = 0 ;//��ȡ�ĵ�ѹֵ
 
double* Get_ES_Table_Val(void)	//�ⲿ���û�ȡ��ѹֵ
{
	return(ES_Table_Val);
}


static double Get_ES_Val(void)	//�ڲ����û�ȡ��ѹֵ
{
	return(*ES_Val);
}

static double Get_ES_Val2(void)	//�ڲ����û�ȡ��ѹֵ
{
	return(*(ES_Val + 1));
}

static void Set_ES_Val1(double temp)	//�ڲ���������У���ѹֵ
{
	*ES_Val = temp;
}

static void Set_ES_Val2(double temp)	//�ڲ���������δУ���ѹֵ
{
	*(ES_Val + 1) = temp;
}

uint8_t Electronic_scale_scan(uint8_t _scan,float val)
{
	uint8_t uReturn = 0;
	static uint8_t State_Machine = 1;	//״̬��
	static uint8_t last_scan = 0;		//��¼��һ�����뵲λ
	static uint8_t last_dw = 1;			//��¼��һ�ηŴ�λ(��ʼ��Ĭ��Ϊ1)
	uint8_t temp = 0;
	if(_scan != last_scan){				//������ĵ��ӳӱ������һ�β�ͬʱ����״̬����1
		last_scan = _scan;
		//���ôӵ�һ����ʼ
		State_Machine = 1;
		last_dw = 1;
	}



	switch(State_Machine)
	{
		case 1:
			switch(last_scan)							//�򿪶�Ӧ���뿪��
			{
				case 0:INPUT_ONE_ENALBE 		break;
				case 1:INPUT_TWO_ENALBE 		break;
				case 2:INPUT_THREE_ENALBE 		break;
				case 3:INPUT_FOUR_ENALBE 		break;
				case 4:INPUT_CLEAR_ENALBE 		break;
				default :INPUT_CLEAR_ENALBE	break;
			}
			OP_dw1_ENALBE;
			State_Machine++;
		break;
		case 2:											//�������ݵĻ�ȡ
			temp = Electronic_Auto_Mode(val, last_dw);
			
			if(last_dw != temp){
				last_dw = temp;
				switch(last_dw)
				{
					case 1:OP_dw1_ENALBE  		break;
					case 2:OP_dw2_ENALBE  		break;
					case 3:OP_dw3_ENALBE  		break;
					default:break;
				}
				State_Machine = 5;
			}
			else{
				Set_ES_Val2(val / Electronic_dw[last_dw-1]);
				State_Machine++;
				INPUT_CLEAR_ENALBE
			}
			break;
		case 3:State_Machine++; break;
		case 4:						//��ȡУ������
			switch (last_dw)
			{
				case 1:
				if((Get_ES_Val2() - (val / Electronic_dw[last_dw - 1])) >= 4.971917)
					Set_ES_Val1(((4.95-5)/(4.971917 - 4.978022)*(Get_ES_Val2() - (val / Electronic_dw[last_dw - 1]) - 4.971917))+4.95) ;
				else			
					Set_ES_Val1(0.991612*(Get_ES_Val2() - (val / Electronic_dw[last_dw - 1]))+0.0193707) ;
				break;
				case 2:Set_ES_Val1(1.02098*(Get_ES_Val2() - (val / Electronic_dw[last_dw - 1]))+0.00819215) ;break;
				case 3:Set_ES_Val1(1.02078*(Get_ES_Val2() - (val / Electronic_dw[last_dw - 1]))+0.000493174) ;
					  if(Get_ES_Val() <= 0.001) Set_ES_Val1(0);break;
				default:break;
			}
			ES_Table_Val[_scan] = Get_ES_Val();
			uReturn = 1;
//			switch(last_scan)							//�򿪶�Ӧ���뿪��
//			{
//				case 0:INPUT_ONE_ENALBE 		break;
//				case 1:INPUT_TWO_ENALBE 		break;
//				case 2:INPUT_THREE_ENALBE 		break;
//				case 3:INPUT_FOUR_ENALBE 		break;
//				case 4:INPUT_CLEAR_ENALBE 		break;
//				default :INPUT_CLEAR_ENALBE	break;
//			}
//			State_Machine = 5;//���»�ȡ����
			break;
		case 5:State_Machine = 2; break;	
		default: break;
	}
	return uReturn;
}



static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw)
{
	uint8_t state = 0;
	float temp = val / Electronic_dw[dw - 1];//����洢��0��ʼ
		if(temp > Es_Range[0].Range_min)
			state = 1;
		else if(temp <= Es_Range[1].Range_max && temp > Es_Range[1].Range_min)
			state = 2;
		else if(temp >= Es_Range[2].Range_min)
			state = 3;
	return state;
}





// static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw)
// {
// 	uint8_t state = 0;
// 	float temp = val / Electronic_dw[dw];
// 	switch(dw)
// 	{
// 		case 1: if(temp > Es_Range[0].Rangle_min)
// 					state = 1;
// 				else if(temp <= Es_Range[1].Rangle_max && temp > Es_Range[1].Rangle_min)
// 					state = 2;
// 				else if(temp > 0)
// 					state = 3;
// 		break;
// 		case 2:
// 				if(temp <= Es_Range[1].Rangle_max && temp > Es_Range[1].Rangle_min)
// 					state = 2;
// 				else if(temp > 0)
// 					state = 3;
// 		break;
// 		case 3:
// 				if(temp <= Es_Range[2].Rangle_max && temp > Es_Range[2].Rangle_min)
// 					state = 3;
// 	}
// 	return state;
// }
