/*************************************************************
*
*
*
*
 *************************************************************/

#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

//#define led_out P2	//定时P2 名为 LED输出

sbit led=P2^0;		//位定义
sbit button=P2^1;


void main(void)
 { 
   //led=0;		//位操作
 //  led_out=0xfe;	//寄存器操作
   while (1)
   {
	if(button==0)
	  led=0; 
	else
	   led=1;
    }
      ;
 }