#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

uchar code tableYI[10] = 
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//共阴
uchar code tableYA[10] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98};//共阳

uchar count=0;
uchar count1=0;
/********************************************************************
* 名称 : Time0_Init()
* 功能 : 定时器的初始化，1ms  12M晶振 即初值加1只要1us。
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Time0_Init()
{
	TMOD = 0x01;	//方式1，16位定时功能
	TH0  = 0x3c;	//初值  50ms定时
	TL0  = 0xb0;
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
	 P0=tableYA[count/10];
	 P2=0X01;
	 delayms(3);
	 P2=0X00;
	 P0=tableYA[count%10];
	 P2=0X02;
	 delayms(3);
	 P2=0X00;
      } 
 }
 /////////////定时器中断函数0///////////////////////////
void Time0_Int() interrupt 1
 {
    static uchar a=20;
     TH0  = 0x3c;	//初值  50ms定时
     TL0  = 0xb0;
    a--;
    if(a==0)
    {
       a=20;
      count++;
       if(count>59)
       {
	  count=0;
	  count1++;
	  if(count1>59)
	     count1=0;
	     
       }
    }
  }

