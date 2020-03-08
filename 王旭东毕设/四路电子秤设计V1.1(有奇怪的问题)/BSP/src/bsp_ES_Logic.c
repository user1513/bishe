#include "bsp_405x.h"
//test

const float Electronic_dw[3] = {0.989, 4.1, 43};//����ÿ����λ�ķŴ���,������1->3��λ

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

static float ES_Val[2] = 0 ;//��ȡ�ĵ�ѹֵ

float Get_ES_Val(void)	//�ⲿ���û�ȡ��ѹֵ
{
	return(*ES_Val);
	//return(*(ES_Val + 1));
}

static float Get_ES_Val2(void)	//�ⲿ���û�ȡ��ѹֵ
{
	return(*(ES_Val + 1));
}

static void Set_ES_Val1(float temp)	//�ڲ���������У���ѹֵ
{
	*ES_Val = temp;
}

static void Set_ES_Val2(float temp)	//�ڲ���������δУ���ѹֵ
{
	*(ES_Val + 1) = temp;
}

uint8_t Electronic_scale_scan(uint8_t _scan,float val)
{
	static uint8_t State_Machine = 1;	//״̬��
	static uint8_t last_scan = 0;		//��¼��һ�����뵲λ
	static uint8_t last_dw = 1;			//��¼��һ�ηŴ�λ(��ʼ��Ĭ��Ϊ1)
	uint8_t temp = 0;
	if(_scan != last_scan){				//������ĵ��ӳӱ������һ�β�ͬʱ����״̬����1
		last_scan = _scan;
		//���ôӵ�һ����ʼ
		State_Machine = 1;
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
			Set_ES_Val1(Get_ES_Val2() - (val / Electronic_dw[last_dw - 1]));
			switch(last_scan)							//�򿪶�Ӧ���뿪��
			{
				case 0:INPUT_ONE_ENALBE 		break;
				case 1:INPUT_TWO_ENALBE 		break;
				case 2:INPUT_THREE_ENALBE 		break;
				case 3:INPUT_FOUR_ENALBE 		break;
				case 4:INPUT_CLEAR_ENALBE 		break;
				default :INPUT_CLEAR_ENALBE	break;
			}
			State_Machine = 5;//���»�ȡ����
			break;
		case 5:State_Machine = 2; break;	
		default: break;
	}
	return 0;
}



static uint8_t  Electronic_Auto_Mode(float val,uint8_t dw)
{
	uint8_t state = 0;
	float temp = val / Electronic_dw[dw - 1];//����洢��0��ʼ
		if(temp > Es_Range[0].Range_min)
			state = 1;
		else if(temp <= Es_Range[1].Range_max && temp > Es_Range[1].Range_min)
			state = 2;
		else if(temp <= Es_Range[2].Range_max && temp >= Es_Range[2].Range_min)
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
