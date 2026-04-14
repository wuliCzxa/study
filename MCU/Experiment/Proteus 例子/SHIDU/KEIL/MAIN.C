#include "reg52.h"
#define uchar unsigned char //宏定义
#define uint unsigned int
uchar data_byte;
uchar data_byte1;
uchar data_Humcha=15; //湿度差阈值
uchar Tem_Z,Tem_X,Hum_Z,Hum_X,Check;
uchar Tem_Z1,Tem_X1,Hum_Z1,Hum_X1,Check1;
char jiaodu=0;
char jiaodu2=0;
char jiaodu3=0;
char jiaodu4=0;
uchar jiaodu1;
uint count=0;
uint count2=0;
uchar count1=0;
uchar com_shuju[5]={0,0,1,5,0};
uchar code turn[]={0xF3,0xF2,0xF6,0xF4,0xFc,0xF8,0xF9,0xF1};
uchar buffer[10];
uchar i1,i2,i3;
uchar moshi[6]="AT:  %";
uchar moshi0[6]="M";
uchar moshi1=0;

uchar caidan=0;//菜单页面

#include "delay.h"
#include "DHT11.H"
#include "LCD1602.H"
#include "com.h"
#include "IT.h"
//#include "chuang.h"
#include "TIME.h"


sbit IT0_JIAN=P3^2;
sbit IT1_JIA=P3^3;
sbit cai=P3^4;
sbit mot=P2^2;

 
//主函数
void main()
{
	i1=0;
	i3=0;
	i2=0;
	caidan=0;
	LCD_Init();
	Outside_Init();
	Time0_Init();
	Com_Init();
	IP=0X07;
 while(1) 
 {
	 	
	if(cai==0)
	{
		while(!cai);
		caidan++;
		if(caidan>3)caidan=0;
		 write_com(0x01); //清屏
	}
	
	if(caidan==0)
	{
		Rec_DHT();
		LCD_Display(); 
		if(count>40)
		{
			count=0;
			fasong();
		}
	      ///////窗户电机//////////
		if(jiaodu<jiaodu3)
		{
			if(i3==1)
			{	
				jiaodu2=jiaodu;
				i2=jiaodu3-jiaodu;
				i3=0;
			}
			if(i2!=0)
			{
				i2--;
				jiaodu3--;
				i1 = i1 > 0 ? i1-1 : 7;	
				P1=turn[i1];	
			}
			if(jiaodu2!=jiaodu)
				i3=1;
		}
		if(jiaodu>jiaodu3)
		{
			if(i3==1)
			{	
				jiaodu2=jiaodu;
				i2=jiaodu-jiaodu3;
				i3=0;
			}
			if(i2!=0)
			{
				i2--;	
				jiaodu3++;
				i1 = i1 <7 ? i1+1 : 0;	
				P1=turn[i1];	
			}
			if(jiaodu2!=jiaodu)
				i3=1;
		}
	}
	if(caidan==1)
	{
		LCD_Displaysetmode();
	}
	
	if(caidan==2)
	{
		LCD_Displayset();
	}
	if(caidan==3)
	{
		LCD_Displaysta();
	}
	
	if(Hum_Z-Hum_Z1<data_Humcha)
	{RED=1;GRE=0;}
	if((Hum_Z-Hum_Z1>=50)&&(Hum_Z-Hum_Z1>=data_Humcha))
	{RED=0;GRE=1;}
	if((Hum_Z-Hum_Z1<50)&&(Hum_Z-Hum_Z1>=data_Humcha))
	{RED=1;GRE=1;}
 }
}


 /////////////外部中断函数0///////////////////////////
 void Outside_Int0(void) interrupt 0
 {
    EA=0;
	while(!IT0_JIAN);
	 if(caidan==1)
	 {
		moshi1=0;
	 }
	 if(caidan==2)
	 {
		data_Humcha--;
		 if(data_Humcha<=1)data_Humcha=1;
	 }
	 if(caidan==3)
	 {
		 jiaodu--;
		 if(jiaodu<=-90)jiaodu=-90;
	 }
	 
    EA=1;
  }
  ///////////外部中断函数1/////////////////////
 void Outside_Int1(void) interrupt 2
 {
    EA=0;
	while(!IT1_JIA);
	 if(caidan==1)
	 {
		moshi1=1;
	 }
	 if(caidan==2)
	 {
		data_Humcha++;
		 if(data_Humcha>=99)data_Humcha=99;
	 }
	 if(caidan==3)
	 {
		 jiaodu++;
		 if(jiaodu>=90)jiaodu=90;
	 }
    EA=1;
  }
   /////////////定时器中断函数0///////////////////////////
void Time0_Int() interrupt 1
 {
    TH0  = 0x3c;
    TL0  = 0xb0;
    count++;
	 
  }
  

