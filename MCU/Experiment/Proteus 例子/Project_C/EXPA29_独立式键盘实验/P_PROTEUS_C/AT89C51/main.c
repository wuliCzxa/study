#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
#define in  P3
#define out  P1

void main(void)
{
uchar i;
bit m;
while(1)
	{
	 for(i=0;i<8;i++)
	 	{
		m=(in>>i)&0x01;
		if(m==0)out=_crol_(0xfe,i);
		}						
  		
	}

}
