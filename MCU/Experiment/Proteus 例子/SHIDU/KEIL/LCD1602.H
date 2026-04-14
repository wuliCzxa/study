#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "reg52.h"
#include "delay.h"
#define uchar unsigned char //宏定义
#define uint unsigned int
sbit LCDRS = P2^3; //管脚定义，1602液晶RS控制端;
sbit LCDWR = P2^4; //管脚定义，1602液晶W/R控制端;
sbit LCDEN = P2^5; //管脚定义，1602液晶EN使能端;
sbit RED =P2^0;
sbit GRE =P2^1;
uchar code table[]={'0','1','2','3','4','5','6','7','8','9'};
uchar code zifuid[]="IdHum:  %";
uchar code zifuod[]="OdHum:  %";
uchar code zifuop[]="Open";
uchar code zifucl[]="Close";
uchar code setmode[]="Set Mode:";
uchar code set[]="Set:  %";
uchar code set_chuang[]="Sta:   ";
uchar code set_chuang1[]="S:   ";
/*******指令码*******/
void write_com(uchar com)
{
 LCDRS=0;
 LCDEN=0;
 delay(2);
 LCDEN=1;
 P0=com;
 delay(2);
 LCDEN=0; 
}
/*******数据码*******/
void write_data(uchar date)
{
 LCDRS=1;
 LCDEN=0;
 delay(2);
 LCDEN=1;
 P0=date;
 delay(2);
 LCDEN=0;
}
/*********************/
/********1602液晶显示初始化*******/
void LCD_Init() 
{
 LCDEN=0; //使能低电平
 LCDWR=0; 
 write_com(0x38); //显示模式设置，设置16*2显示，5*7点阵，8位数据接口
 delay(1);
 write_com(0x0c); //显示开关及光标设置
 delay(1);
 write_com(0x06);
 delay(1);
 write_com(0x01); //清屏
 delay(1);
}
///////////////////////字符串////////////////////////////////////////
void string(uchar ad,uchar *s)
{
write_com(ad);
while(*s>0)
	{
	write_data(*s++);
//	delay(1);
	}
}

/**********1602液晶显示******/
void LCD_Display()
{
//液晶显示第一行:;
 write_com(0x0c);
string(0x80,zifuid);

 write_com(0x86);
 write_data(table[(Hum_Z1/10)]);
 write_data(table[(Hum_Z1%10)]); 
if(moshi1==0)
{
string(0x8a,moshi);
 write_com(0x8d);
 write_data(table[(data_Humcha/10)]);
 write_data(table[(data_Humcha%10)]);
	if((Hum_Z-Hum_Z1>=50)&&(Hum_Z-Hum_Z1>=data_Humcha))
	{
		jiaodu=-90;
		string(0xca,set_chuang1);
		write_com(0xcc);
		write_data(0x2d);
		write_data(table[(90/10)]);
		write_data(table[(90%10)]);
	}
	else
	{	
		jiaodu=0;
		string(0xca,set_chuang1);
		write_com(0xcc);
		if(jiaodu<0)
		{
			write_data(0x2d);
			jiaodu1=0-jiaodu;
			write_data(table[(jiaodu1/10)]);
			write_data(table[(jiaodu1%10)]);
		}
		else
		{
			write_data(0x2b);
			write_data(table[(jiaodu/10)]);
			write_data(table[(jiaodu%10)]);
		}
	}
}
else
{
	string(0x8a,moshi0);
	write_com(0x8d);
	string(0xca,set_chuang1);
	write_com(0xcc);
	if(jiaodu<0)
	{
		write_data(0x2d);
		jiaodu1=0-jiaodu;
		write_data(table[(jiaodu1/10)]);
		write_data(table[(jiaodu1%10)]);
	}
	else
	{
		write_data(0x2b);
		write_data(table[(jiaodu/10)]);
		write_data(table[(jiaodu%10)]);
	}
}
//液晶第二行:湿度显示;
string(0xC0,zifuod);
 write_com(0xc6);
 write_data(table[(Hum_Z/10)]);//delay(1); //输出湿度的十位
 write_data(table[(Hum_Z%10)]);//delay(1); //输出湿度的个位

}

///////////模式设置
void LCD_Displaysetmode()
{
	string(0x83,setmode);
	write_com(0xc7);
	if(moshi1==0)
	{write_data(moshi[0]);
	write_data(moshi[1]);}
	else
	{write_data(moshi0[0]);
	write_data(moshi0[1]);}
}
//////////阈值设置
void LCD_Displayset()
{
	string(0x81,set);
	write_com(0x85);
	write_data(table[(data_Humcha/10)]);
	write_data(table[(data_Humcha%10)]);
}
//////////////窗角度///////////////////////////
void LCD_Displaysta()
{
	string(0x81,set_chuang);
	write_com(0x85);
	if(jiaodu<0)
	{
		write_data(0x2d);
		jiaodu1=0-jiaodu;
		write_data(table[(jiaodu1/10)]);
		write_data(table[(jiaodu1%10)]);
	}
	else
	{
		write_data(0x2b);
		write_data(table[(jiaodu/10)]);
		write_data(table[(jiaodu%10)]);
	}
}

#endif