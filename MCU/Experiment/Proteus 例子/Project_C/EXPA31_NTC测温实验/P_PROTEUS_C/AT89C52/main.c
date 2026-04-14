#include<reg52.h>
#include<absacc.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int


unsigned char code segbit[]={0xc0,0xf9,0xa4,0xb0,	// 0, 1, 2, 3
								 0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off 
unsigned char code combit[]={0xf1,0xf2,0xf4,0xf8};




#define ADC0809 XBYTE[0x7fff]    /* 定义ADC0809 端口地址 */
#define Smg_Seg P1
#define Smg_Com P2
sbit EOC=P3^0;


void TimeInitial();
void Delay(unsigned int i);


void Delay(unsigned int i)
{
  unsigned int j;
  for(;i>0;i--)
  {
  for(j=0;j<125;j++)
  {;}
  }
}
 

void main()
{
	uchar  temp,loopdat1;
	uint   voldata;
	uint   tempedata;
	uchar dispbuf[4];
	while(1)
	{
		ADC0809=0x0f;		
		do
		{;}
		while(~EOC);  //转换是否完成		
		//delayms(1);
		temp = ADC0809; //读出转换结果
		voldata = temp*1.0/255*500;
		if((voldata>=0)&&(voldata<=500))
		{
		 	if((voldata>440)&&(voldata<=500))
			{
				tempedata=260+(500-voldata)/6;
			}
			if((voldata>400)&&(voldata<=440))
			{
				tempedata=270+(440-voldata)/4;
			}
			if((voldata>360)&&(voldata<=400))
			{
				tempedata=280+(400-voldata)/4;
			}
			if((voldata>330)&&(voldata<=360))
			{
				tempedata=290+(360-voldata)/3;
			}
			if((voldata>300)&&(voldata<=330))
			{
				tempedata=300+(330-voldata)/3;
			}
			if((voldata>280)&&(voldata<=300))
			{
				tempedata=310+(280-voldata)/2;
			}
			if((voldata>260)&&(voldata<=280))
			{
				tempedata=320+(280-voldata)/2;
			}
			if((voldata>240)&&(voldata<=260))
			{
				tempedata=330+(260-voldata)/2;
			}
		}
		else
		{
			tempedata = 0;
		}
		dispbuf[3] = tempedata%10;
		dispbuf[2] = tempedata/10%10;
		dispbuf[1] = tempedata/100%10;
		dispbuf[0] = tempedata/1000;
		for(loopdat1=0;loopdat1<4;loopdat1++)
		{
			
			Smg_Seg = segbit[dispbuf[loopdat1]];
			if( loopdat1 == 2 )
			{
				Smg_Seg &= 0x7f;
			}
			Smg_Com = combit[loopdat1];
			Delay(4);
			Smg_Com = 0xf0;
		}
	}
}




