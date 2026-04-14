
#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

sbit led=P2^0;		//位定义

uint count=0;
/********************************************************************
* 名称 : Time0_Init()
* 功能 : 定时器的初始化，1ms  12M晶振 即初值加1只要1us。
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Time0_Init()
{
	TMOD = 0x00;	//方式1，16位定时功能      
//	TMOD = 0x04;	//方式0，13位计数功能
	TH0  = 0xE0;	//高八低5位
	TL0  = 0x18;
     
	ET0 = 1;
	EA = 1;		
	TR0 = 1;		
}
//////////////主函数//////////////////////////
void main(void)
 { 
   Time0_Init();
   while (1);  
 }
 /////////////定时器中断函数0///////////////////////////
void Time0_Int() interrupt 1
 {
    TH0  = 0xE0;
    TL0  = 0x18;
    count++;
    if(count==1000)
    {
       count=0;
	led=~led;
    }
  }

