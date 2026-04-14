#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

#define led_out P2	//定时P2 名为 LED输出

sbit button=P1^0;
uint count=0;
uchar a,b;
///////////////延时函数/////////////////////////
void delay1ms(uint j)		  //1ms延时
{
uchar i;
for(   ;j>0;j--)
  	{
	 i=250;
   	 while(--i);
   	 i=249;
   	 while(--i);
   	}
}
/********************************************************************
* 名称 : Outside_Init0()
* 功能 : 外部中断0 的初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Outside_Init0(void)
{
	EX0 = 1;  //开外部中断0
	 EX1=1;
//	IT0 = 0;  //电平触发 
	IT0 = 1;  //边沿触发
	IT1=1;
	EA = 1;	  //开总中断
}
/********************************************************************
* 名称 : Time0_Init()
* 功能 : 定时器的初始化，1ms  12M晶振 即初值加1只要1us。
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Time0_Init()
{
	TMOD = 0x00;	//方式0，13位定时功能      
//	TMOD = 0x04;	//方式0，13位计数功能
	TH0  = 0xE0;	//高八低5位
	TL0  = 0x18;
     
	ET0 = 1;
	EA = 1;		
	TR0 = 1;		
}
////////主函数////////////////////
void main(void)
 { 
   Outside_Init0();
    Time0_Init();
    IP=02; //中断嵌套中优先级设置，设置的1位高于设置0的位。
   while (1)
   {;
    }
 }
  /////////////外部中断函数0///////////////////////////
 void Outside_Int0(void) interrupt 2
 {

   for(a=0;a<8;a++)
   {
      led_out=~(0x01<<a);
      delay1ms(1000); 
      }
    
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
       led_out=0x01<<b;
       b++;
       if(b>7)b=0;
      
    }
  }