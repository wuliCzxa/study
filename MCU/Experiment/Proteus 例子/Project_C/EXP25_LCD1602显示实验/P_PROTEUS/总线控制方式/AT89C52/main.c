/*******************************************************************************  
文件名称： main.c 
作 者：    zw sangel
版 本：    V1.00
说 明：    LCD1602总线控制方式 
修改记录：  
*******************************************************************************/
/*******************************************************************************    
* 功能描述:                                                              	  *
*          程序运行后显示                                                 	  *
*          第一行：WINDWAY                                   			      *
*          第二行：A GOOD NEWS			                                      *
*
*接线说明：P00~P07-DB0~DB7,P22-IO, 
*******************************************************************************/
#include<reg52.h>
#include<absacc.h>

#define uchar unsigned char
#define uint unsigned int

#define busy 0x80 
#define RSaddr 0x1000
#define RWaddr 0x2000
#define CSaddr 0x0400


#define WC 0xcfff
#define WD 0xdfff
#define RC 0xefff

unsigned char code homepage[] = {"www.ispdown.com"};
unsigned char code email[] = {"ispdown@21cn.com"};

void delay5ms(void)
{
	unsigned int TempCyc = 5552;
 	while(TempCyc--);
}

void delay400ms(void)
{
 	unsigned char TempCycA = 5;
 	unsigned int TempCycB;
 	while(TempCycA--)
 	{
   		TempCycB=7269;
   		while(TempCycB--);
	};
}

void CheckBusy()
{
    uchar temp;
 	//do 
    //{
   		temp=RC;
		delay5ms();
 	//}while(temp&0x80);
}

//写控制指令
void write_command(uchar cmd)
{
   	CheckBusy();
   	XBYTE[WC]=cmd;
	delay5ms();
}

//写数据指令
void write_data(uchar dat)
{
    CheckBusy();
	XBYTE[WD]=dat;
	delay5ms();
}

void LcdInit()
{
	write_command(0x30);
	write_command(0x38);
	write_command(0x0C);
	write_command(0x01);//清屏
	write_command(0x06);
} 

//输出字符串
void string(uchar ad,uchar *s)
{
write_command(ad);
while(*s>0)
	{
	write_data(*s++);
	delay400ms();
	}
}

void main(void)
{

	LcdInit();
  	delay400ms();
 	string(0x84,"WINDWAY");
	string(0xC2,"A GOOD NEWS");
	delay400ms();
	write_command(0x01);//清屏
	delay400ms();		
}

