#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

uchar j=0;

unsigned char code tabP0[]={
 0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00,
 0x00,0x08,0x14,0x22,0x41,0x22,0x14,0x08,
 0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81,
 0x00,0x3C,0x24,0x24,0xFF,0x24,0x24,0x3C
};

unsigned char code tabP2[]=
{0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};

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
/********************************************************************
* 名称 : Outside_Init0()
* 功能 : 外部中断0 的初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Outside_Init0(void)
{
	EX0 = 1;  //开外部中断0
//	IT0 = 0;  //电平触发 
	IT0 = 1;  //边沿触发
	EA = 1;	  //开总中断
}

//////////////主函数//////////////////////////
void main()
{
	uchar i;
   	Outside_Init0();
	while(1)
	{
		for(i=0;i<8;i++)
		{
			P0=0;
			P2=0XFF;
			P2=tabP2[i];
			P0=tabP0[8*j+i];
			delayms(2);
		}		
	}
}
 /////////////外部中断函数0///////////////////////////
 void Outside_Int0(void) interrupt 0
 {
    EA=0;
    j++;
    if(j>3)j=0;
    EA=1;
  }
