/*************************************************  
*文件名称： main.c 
*作 者：    
*版 本：    V1.00
*说 明：    步进电机控制实验
*修改记录：  
--------------------------------------------------
*Descriptoon: 控制步进电机实现正转、反转

*接线说明：P00-K1，P01-K2，P20-SA，P21-SB，P22-SC
           P23-SD                    
**************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define out  P2
/***********端口定义******************************/
sbit pos=P0^0;
sbit neg=P0^1;
void delayms(uint);
uchar code turn[]={0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x03};

/***********主函数********************************/
void main(void)
{
	uchar i;
	out=0x03;
	while(1)
	{
		if(!pos)			//正转
		{
			i = i < 8 ? i+1 : 0;
			out=turn[i];
			delayms(50);
		}
		else if(!neg)		//反转
		{
			i = i > 0 ? i-1 : 7;	
			out=turn[i];
			delayms(50);
		}	
	}
}
/***********延时函数******************************/
void delayms(uint j)
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