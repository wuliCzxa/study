/************************************************************************ 
文件名称： main.c 
作 者：    
版 本：    V1.00
说 明：    RS485通信实验 
修改记录：  
-------------------------------------------------------------------------   
*功能描述: 通过RS485端口不停地发送数据a——z
*  		   用示波器观察RS485端口A、B信号线上的波形
           或者用RS485接收器接收发送的字符。
-------------------------------------------------------------------------
* 接线说明：P30-RO,P31-DI,P32-DIR,CA或CB接示波器
*                                                           
*************************************************************************/
#include <reg52.h>
#include <string.h>

#define uchar unsigned char
#define uint unsigned int
char code SST516[3] _at_ 0x003b;

#define Seg  P1
sbit A485_DIR1 = P3^2;    	// 驱动器使能，1发送 
sbit B485_DIR2 = P3^3;    	// 驱动器使能，0接收
sbit Com1 = P2^0;

unsigned char code segbit[]={0xc0,0xf9,0xa4,0xb0,	// 0, 1, 2, 3
								 0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off

uchar RecData(void);
void SendByte(uchar Sdata);    	// 该函数发送一帧数据中的一个字节，由send_data()函数调用
void delayms(uint);

void main()
{
	uchar	i;
	A485_DIR1 = 1;     //	驱动器使能，1发送
    B485_DIR2 = 0;    	// 驱动器使能，0接收
	Com1 = 1;
/* 系统初始化 */
	SCON=0x50;
    TMOD=0x20;
	PCON=0x00;
    TH1 =0xfd;
	TL1 =0xfd;  //预置初值，设波特率为9600
	TR1 =1;	
   	ES  =1;
	EA  =1;		//开中断


/* 主程序流程 */
	while(1)    // 主循环
	{
		for(i=0;i<10;i++)
		{
			SendByte(segbit[i]);			
			delayms(1000);
		}
		//Seg = segbit[1];			   
	} 
}



/********************************************************* 
**函数名称：INT_UART_Rev()
**函数功能：串口接收中断函数 
*********************************************************/
void INT_UART_Rev() interrupt 4
{
	if(RI)
	{ 
		RI=0;
		Seg = SBUF;
	}
}

/* 该函数发送一个数据字节，若该字节为0xdb，则发送0xdbdd，若该字节为0xc0则，发送0xdbdc */
void SendByte(uchar Sdata)
{
    //M_DIR = 1;    // 置发送允许，接收禁止
	SBUF = Sdata;
    while(!TI);
    TI = 0;
}

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