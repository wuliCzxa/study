/*******************************************************************************  
文件名称： main.c 
作 者：    zw sangel
版 本：    V1.00
说 明：    DS1302时钟实验 
修改记录：  
*******************************************************************************/
/*******************************************************************************    
* 功能描述:时钟显示                                                              	  
*          		                                      
*接线说明：P00~P07-DB0~DB7，P20-RS，P21-RW，P22-EN,P10-RST,P11-SCLK,P12-IO
*******************************************************************************/

#include <reg52.h>
//#include "absacc.h"
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define out P0
/////////////////////////////////////
//#define LCD_WC XBYTE[0X7FFC]
//#define LCD_WD XBYTE[0X7FFD]
//#define LCD_RC XBYTE[0X7FFE]
void check_busy(void);
void write_command(uchar com);
void write_data(uchar dat);
void LCD_initial(void);
void string(uchar ad,uchar *s);
void lcd_test(void);
void delay(uint);
/***********端口定义**********************************************************/
sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;

sbit rst=P1^0;
sbit sclk=P1^1;
sbit io=P1^2;

sbit bit7=ACC^7;
sbit bit0=ACC^0;

void input(uchar);
uchar output(void);
void wr1302(uchar,uchar);
uchar re1302(uchar);
void set1302(uchar *p);
void get1302(uchar curtime[]) ;
void display(void);
/***********************时钟初始值*********************************************/
uchar code clock[7]={0x50,0x59,0x23,0x31,0x07,0x07,0x11};
uchar time[8];
/***********************主函数*************************************************/
void main(void)
{
LCD_initial();	     //LCD1602初始化
set1302(clock);	     //设置时钟
while(1)
	{
	get1302(time);	 //获取时钟值
	display();
	}				 //显示
}
///////////////////////////////////////////
/*八位数据写入函数*/
void input(uchar date)
{
uchar i;
ACC=date;
for(i=8; i>0; i--)
	{
	io=bit0;
	sclk=1;
	sclk=0;
	ACC=ACC>>1;
	}
}
/*八位数据读出函数*/
uchar output(void)
{
uchar i;
for(i=8; i>0; i--)
	{
	ACC=ACC>>1;
	bit7=io;
	sclk=1;
	sclk=0;	 	
	}
	return(ACC);
}
/*写寄存器函数*/
void wr1302(uchar add,uchar date)
  {
  rst=0;
  sclk=0;
  rst=1;
  input(add);
  input(date);
  sclk=1;
  rst=0;
  }
/*读寄存器函数*/
uchar re1302(uchar add)
{
  uchar date;
  rst=0;
  sclk=0;
  rst=1;
  input(add);
  date=output();
  sclk=1;
  rst=0;
  return(date);
  }
 /*设置时间初值函数*/
void set1302(uchar *p)
  {
 uchar i;
 uchar add=0x80;
 wr1302(0x8e,0x00);
 for(i=7;i>0;i--)
	 {
	 wr1302(add,*p);
	 p++;
	 add+=2;
     }
 wr1302(0x8e,0x00);
  }
/*读当前时间值函数*/
 void get1302(uchar curtime[]) 
{
    uchar i;
    uchar add=0x81;
    for (i=0;i<8;i++)
    	{
        curtime[i]=re1302(add);
        add+=2;	 
    	}
	
}
///////////////////////////////////////////
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
		en=0;
		rs=0;	
		rw=1;
		en=1;
		dt=out;
	}while(dt&0x80);
	en=0;
}
//写控制指令
void write_command(uchar com)
{
check_busy();
en=0;
rs=0;
rw=0;
out=com;
en=1;
_nop_();
en=0;
delay(10);
}
//写数据指令
void write_data(uchar dat)
{
check_busy();
en=0;
rs=1;
rw=0;
out=dat;
en=1;
_nop_();
en=0;
delay(10);	
}
//液晶屏初始化
void LCD_initial(void)
{
	delay(1);
	write_command(0x30);
	write_command(0x38);//8位总线,双行显示，5X7的点阵字符
	write_command(0x0C);//开整体显示,光标关，无黑块
//	write_command(0x01);//清屏
	write_command(0x06);//光标右移
	write_command(0x01);
	delay(10);
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

void display(void)
{
	write_command(0x84);//写年
	write_data(time[6]/16+0x30);
	write_data(time[6]%16+0x30);

	write_data(0x2D); //写“-”
	write_command(0x87);//写月
	write_data(time[4]/16+0x30);
	write_data(time[4]%16+0x30);

	write_data(0x2D); //写“-”	
	write_command(0x8a);//写日
	write_data(time[3]/16+0x30);
	write_data(time[3]%16+0x30);

	write_command(0xc1);//写星期
	write_data(time[5]%16+0x30);
	
	write_command(0xc4);//写时
	write_data(time[2]/16+0x30);
	write_data(time[2]%16+0x30);
	write_data(0x2D); //写“-”
	write_command(0xc7);//写分
	write_data(time[1]/16+0x30);
	write_data(time[1]%16+0x30);
	write_data(0x2D); //写“-”
	write_command(0xca);//写秒
	write_data(time[0]/16+0x30);
	write_data(time[0]%16+0x30);				
}