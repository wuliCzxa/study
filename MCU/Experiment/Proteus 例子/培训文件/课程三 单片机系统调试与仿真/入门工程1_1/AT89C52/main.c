#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

#define led_out P2	//定时P2 名为 LED输出

sbit button=P1^0;
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
////////主函数////////////////////
void main(void)
 { 
   uchar i=0;
//   led_out=0x01;	//寄存器操作
   while (1)
   {
	if(button==1)
	{
	    led_out=~(0x01<<i);
	    i++;
	    i&=0x07;
	    delay1ms(500); 
	}
	else
	{
	   led_out=0xaa;
	   delay1ms(500);
	   led_out=0x55;
	   delay1ms(500);
	}
    }
 }