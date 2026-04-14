#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

uchar code tableYI[10] = 
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//共阴
uchar code tableYA[10] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98};//共阳

sbit led=P2^0;		//位定义
sbit KEY1 = P3^4;
uint i=0;
/*******************************************************
* 名称 : Time0_Init()
* 功能 : 定时器的初始化。
* 输入 : 无
* 输出 : 无
*********************************************************/
void Time0_Init()
{
	TMOD = 0x04;	//方式0，13位计数功能
	TH0  = 0xFF;	
	TL0  = 0x1F;
	ET0 = 1;
	EA = 1;		
	TR0 = 1;		
}
///////////////延时函数/////////////////////////
void delayms(uint j)		  //1ms延时
{
   uchar i;
   for(;j>0;j--)
  {
      i=250;
      while(--i);
      i=249;
      while(--i);
   }
}
//////////////主函数//////////////////////////
void main(void)
 { 
   Time0_Init();
   while (1)
   {
	P0 = tableYA[i % 10];	  //取 i 的个位
	delayms(1);
    } 
 }
 /////////////定时器中断函数0///////////////////////////
void Time0_Int() interrupt 1
 {
      TH0  = 0xFF;	
      TL0  = 0x1F;
      if(KEY1 == 0)
      {
	delayms(2);
	while(1)
	{
	    if(KEY1 == 1)
	    {
		delayms(2);
		if(KEY1 == 1)
		{
		     i++;
		     break;
		}	
	    }
	 }
      }
      led=~led;   
  }

