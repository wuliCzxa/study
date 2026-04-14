/************************************************************************ 
文件名称： main.c 
作 者：    
版 本：    V1.00
说 明：    PWM蜂鸣器实验 
修改记录：  
-------------------------------------------------------------------------   
*功能描述: 蜂鸣器演奏歌曲
* 
-------------------------------------------------------------------------
*接线说明：P00——BUZ
*                                                           
*************************************************************************/
#include<absacc.h> //头文件
#include<reg52.h> 
#include<intrins.h> 
#define uint unsigned int 
#define uchar unsigned char 
sbit BUZZER=P0^0;//发声音口 
uchar code T0_H[16]={0xFF,0xFB,0xFB,0xFC,0xFC,0xFC,0xFD,0xFD,0xFD,0xFD,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE}; //定时器延时高位
uchar code T0_L[16]={0xF1,0x4,0x90,0xC,0x44,0xAC,0x9,0x34,0x82,0xC8,0x06,0x22,0x56,0x85,0x9A,0xC1}; 	//定时器延时低位
uchar code music[32]={8,0,8,9,8,0xB,0xA,0,8,0,8,9,8,0xC,0xB,0,8,0,8,0xF,0xD,0xB,0xA,9,0xE,0,0xE,0xD,0xB,0xC,0xB,0x0}; 	//音律选择
uchar code music_L[32]={2,1,1,4,4,4,4,4,2,1,1,4,4,4,4,4,2,1,1,4,4,4,4,4,2,1,1,4,4,4,4,4}; 	 //延时长度
uchar a,b; 
void delay_music(uint v) //延时
{ 
	while(v!=0) 
	v--; 
} 
void delay_music1(uint j)//延时 
{ 
	while(j!=0)
	{
		j--;
		delay_music(200);
	} 
} 
void delay_music2(uint j)//延时
{ 
	while(j!=0)
	{
		j--;
		delay_music1(80);
	} 
} 
/************************主函数****************************************/
void main() 
{//uint i; 
	uchar k,n,m; 
	delay_music(100); 
	TMOD=0x11; 	 //初始化定时器
	TH0=0xfc; 
	TL0=0x66; 
	EA=1; 		 //开中断
	ET0=0; 
	TR0=0; 
	ET1=0; 
	TR1=0; 
	while(1) 
	{ 
		ET1=1; 	 //打开定时器
		TR1=1; 	 //启动计数
		for(k=0;k<32;k++)  //循环32次
		{ 
			n=music[k]; 
			m=music_L[k]; 
			a=T0_H[n]; 
			b=T0_L[n]; 
			delay_music2(m); 
		} 
	} 
}  
/**********************定时计数器T1中断处理****************************/
time1 () interrupt 3 using 3    //定时器中断
{ 
	TH1=a; 						//初始化定时器计数器
	TL1=b; 
	BUZZER=!BUZZER; 		    //蜂鸣器取反
} 
