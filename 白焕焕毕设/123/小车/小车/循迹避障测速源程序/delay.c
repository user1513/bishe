
#include "std_for_wym.h"

void DelayMs(unsigned int z)	  //1MS����ʱ����
{
   unsigned int x;
   for(;z>0;z--)
     for(x=110;x>0;x--);
}/*
void Delay(unsigned char z)
{
    while(z--);
}*/
