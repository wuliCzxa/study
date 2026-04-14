#include "reg52.h"
#define uchar unsigned char
#define uint unsigned int
#define outk P1
#define out P0
#define con P2

uchar code seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
uchar key_buffer[8]={0x88,0xc0,0xc0,0xbf,0xbf,0x83,0xc0,0xc0};

void seg_scan(void);
uchar scan(void);
void  delayms(uint);
volatile uchar s_seg=0; 
bit ok;
uchar key,send,rec;

void main(void)
{
TMOD=0X21; //设置定时器1工作在方式2,定时器0工作在方式0
TH1=0XFD;//波特率9600
TL1=0XFD;
TR1=1; //打开定时器1
TH0=(65536-3000)/256;//计时3ms
TL0=(65536-3000)%256;
TR0=1;//打开定时器0
SCON=0X50; //串口工作在方式1，REN=1允许接收数据
PCON=0X00; //波特率不加倍
EA=1;	//开总中断、外中断1（边沿触发）、外中断0（电平触发）、定时器0中断，开串行中断
EX1=1;
IT1=1;
EX0=1;
IT0=1;
ET0=1;	 
ES=1;

while(1)
	{
	outk=0xf0;
	if(ok){ok=0;seg_scan();}
	}
}

uchar scan(void)	  //按键扫描
{
uchar k=16,m,n,in;
delayms(10);
outk=0xf0;		//扫描所有行
if((outk&0xf0)!=0xf0)
	{
	for(m=0;m<4;m++)
		{
	 	outk=~(0x01<<m);
	 	for(n=0;n<4;n++)
			{
		 	in=outk;
		 	in=in>>(4+n);
	 		if((in&0x01)==0)
				{
			 	delayms(10);
			 	if((in&0x01)==0){k=n+m*4;break;}
				}
			}
	 	if(k!=16){break;}
		}
	}
return(k);
}

void delayms(uint j)  //ms延时
{
uchar i;
for(;j>0;j--)
  	{i=250;
   	 while(--i);
   	 i=249;
   	 while(--i);
   	}
}

void seg_scan(void)	  //数码管扫描
{
	out=key_buffer[s_seg];
	con=(1<<s_seg);
	s_seg++;
	if(s_seg==8)s_seg=0;
}

void ext0()interrupt 0	 //矩阵键盘扫描中断
{
EX0=0;
key=scan();
if(key!=16)send=key,key_buffer[1]=seg[key/10],key_buffer[2]=seg[key%10];


ES=0;
SBUF=send;
while(!TI);
TI=0;
ES=1;
EX0=1;


}

void timer0(void)interrupt 1  //定时3ms，扫描数码管
{
ok=1;
TH0=(65536-3000)/256;
TL0=(65536-3000)%256;
out=0xff;
}

/*void ext1()interrupt 2	//按单按键后开始通信
{
EX1=0;
ES=0;
SBUF=send;
while(!TI);
TI=0;
ES=1;
EX1=1;
}*/

void com()interrupt 4	//接收中断
{
RI=0;
rec=SBUF;
key_buffer[6]=seg[rec/10],key_buffer[7]=seg[rec%10]; 
}
