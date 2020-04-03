#include "inc.h"

static void ds1302_write_byte(uchar addr, uchar d) ;
static uchar ds1302_read_byte(uchar addr);

//��ʼʱ�䶨��
xdata uchar time_buf[8] = {0x20,0x20,0x02,0x26,0x21,0x55,0x40,0x03};//��ʼʱ��
xdata uchar readtime[15];//��ǰʱ��
uchar sec_buf=0;  //�뻺��


//DS1302��ʼ������
void ds1302_init(void) 
{
	DS1302_RST=0;			//DS1302_RST���õ�
	DS1302_SCK=0;			//DS1302_SCK���õ�
}
//��DS1302д��һ�ֽ�����
static void ds1302_write_byte(uchar addr, uchar d) 
{
	uchar i;
	DS1302_RST=1;					//����DS1302����	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;   //���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DS1302_IO=1;
			}
		else {
			DS1302_IO=0;
			}
		DS1302_SCK=1;      //����ʱ��
		DS1302_SCK=0;
		addr = addr >> 1;
		}	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			DS1302_IO=1;
			}
		else {
			DS1302_IO=0;
			}
		DS1302_SCK=1;    //����ʱ��
		DS1302_SCK=0;
		d = d >> 1;
		}
	DS1302_RST=0;		//ֹͣDS1302����
}

//��DS1302����һ�ֽ�����
static uchar ds1302_read_byte(uchar addr) {

	uchar i,temp;	
	DS1302_RST=1;					//����DS1302����
	//д��Ŀ���ַ��addr
	addr = addr | 0x01;    //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DS1302_IO=1;
			}
		else {
			DS1302_IO=0;
			}
		DS1302_SCK=1;
		DS1302_SCK=0;
		addr = addr >> 1;
		}	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (DS1302_IO) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		DS1302_SCK=1;
		DS1302_SCK=0;
		}	
	DS1302_RST=0;					//ֹͣDS1302����
	return temp;
}
//��DS302д��ʱ������
void ds1302_write_time(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//�ر�д���� 
	ds1302_write_byte(ds1302_sec_add,0x80);				//��ͣʱ�� 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //������ 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		//�� 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	//�� 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		//�� 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		//ʱ 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//��
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);		//��
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//�� 
	ds1302_write_byte(ds1302_control_add,0x80);			//��д����     
}
//��DS302����ʱ������
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//�� 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//�� 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//�� 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//ʱ 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//�� 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//�룬������ĵ�7λ�����ⳬ��59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//�� 	
}


uint8_t ds1302ShowTime(void)
{
	uchar sec_flag = 0; //���־λ
	ds1302_read_time();  //��ȡʱ�� 
	readtime[0]=(time_buf[0]>>4);   //�������ǧλ
	readtime[1]=(time_buf[0]&0x0F); //��������λ 
	readtime[2]=(time_buf[1]>>4);   //�������ʮλ
	readtime[3]=(time_buf[1]&0x0F); //��������λ 

	readtime[4]=(time_buf[2]>>4);   //�������ʮλ
	readtime[5]=(time_buf[2]&0x0F); //������¸�λ 

	readtime[6]=(time_buf[3]>>4);   //�������ʮλ
	readtime[7]=(time_buf[3]&0x0F); //������ո�λ 

	readtime[8]=(time_buf[4]>>4);   //�����Сʱʮλ
	readtime[9]=(time_buf[4]&0x0F); //�����Сʱ��λ 
	readtime[10]=(time_buf[5]>>4);   //���������ʮλ
	readtime[11]=(time_buf[5]&0x0F); //��������Ӹ�λ
	readtime[12]=(time_buf[6]>>4);   //���������ʮλ
	readtime[13]=(time_buf[6]&0x0F); //��������Ӹ�λ 
	readtime[14]=time_buf[7]; //��������Ӹ�λ 
	if(readtime[13]!=sec_buf)
	{
		sec_flag = 1;
		sec_buf=readtime[13];
	}
	return sec_flag;	
}

uchar* GetTime(void)
{
	return readtime;
}
   



