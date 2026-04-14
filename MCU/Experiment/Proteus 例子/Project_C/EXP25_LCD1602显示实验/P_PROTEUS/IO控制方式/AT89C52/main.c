/*******************************************************************************  
文件名称： main.c 
作 者：    zw sangel
版 本：    V1.00
说 明：    LCD1602IO控制方式 
修改记录：  
*******************************************************************************/
/*******************************************************************************    
* 功能描述:                                                              	  *
*          程序运行后显示                                                 	  *
*          第一行：WINDWAY                                   			      *
*          第二行：A GOOD NEWS			                                      *

*接线说明：P00~P07-DB0~DB7，P20-RS，P21-RW，P22-EN
*******************************************************************************/
#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define out P0
/***********端口定义**********************************************************/
sbit rs=P2^0;
sbit rw=P2^1;
sbit e=P2^2;
/***********函数申明**********************************************************/
void check_busy(void);
void write_command(uchar com);
void write_data(uchar dat);
void LCD_initial(void);
void string(uchar ad ,uchar *s);
void lcd_test(void);
void delay(uint);

/***********主程序*************************************************************/
void main(void)
{
LCD_initial();			     //LCD1602 初始化
while(1)
	{
	string(0x84,"WINDWAY");	 //显示字符串
	string(0xC2,"A GOOD NEWS");
	delay(100); 
	write_command(0x01);     //清屏
	delay(100);			
	}
}
//1ms延时程序
void delay(uint j)
{
uchar i=250;
for(;j>0;j--)
	{
	while(--i);
	i=249;
	while(--i);
	i=250;
	}
}
//查忙程序
void check_busy(void)
{
uchar dt;
do
{
dt=0xff;
e=0;
rs=0;	
rw=1;
e=1;
dt=out;
}while(dt&0x80);
e=0;
}
//写控制指令
void write_command(uchar com)
{
check_busy();
e=0;
rs=0;
rw=0;
out=com;
e=1;
_nop_();
e=0;
delay(1);
}
//写数据指令
void write_data(uchar dat)
{
check_busy();
e=0;
rs=1;
rw=0;
out=dat;
e=1;
_nop_();
e=0;
delay(1);	
}
//液晶屏初始化
void LCD_initial(void)
{
	write_command(0x38);//8位总线,双行显示，5X7的点阵字符
	write_command(0x0C);//开整体显示,光标关，无黑块
	write_command(0x06);//光标右移
	write_command(0x01);//清屏
	delay(1);
}
//输出字符串
void string(uchar ad,uchar *s)
{
write_command(ad);
while(*s>0)
	{
	write_data(*s++);
	delay(100);
	}
}