#include "reg51.h"
#define uchar unsigned char
#define uint unsigned int
#define outk P1
#define out1 P0
#define out2 P2

uchar code seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
uchar key,send,rec;

uchar scan(void);
void  delayms(uint);

void main(void)
{
TMOD=0X20; //设置定时器1工作在方式2
TH1=0XF2;
TL1=0XF2;
TR1=1;
SCON=0X50; //串口工作在方式1，REN=1允许接收数据
PCON=0X00; //波特率不加倍
EA=1;
ES=1;
EX1=1;
EX0=1;
while(1)
	{
	outk=0x07;
	out1=seg[send];
	out2=seg[rec];
	}
}

uchar scan(void)
{
uchar k=10,m,n,in;
delayms(10);
outk=0x07;
if((outk&0x07)!=0x07)
	{
	for(m=0;m<3;m++)
		{
	 	outk=~(0x01<<(m+3));
	 	for(n=0;n<3;n++)
			{
		 	in=outk;
		 	in=in>>n;
	 	 	if((in&0x01)==0)
				{
			 	if((in&0x01)==0){k=n+m*3;break;}
				}
			}
		if(k!=10)break;
		}
	}
return(k);
}

void delayms(uint j)
{
uchar i;
for(;j>0;j--)
  	{i=250;
   	 while(--i);
   	 i=249;
   	 while(--i);
   	}
}

void ext0()interrupt 0
{
EX0=0;
key=scan();
if(key!=10)send=key+1;
EX0=1;
}

void ext1()interrupt 2
{
ES=0;;
SBUF=send;
while(!TI);
TI=0;ES=1;
}

void com()interrupt 4
{
RI=0;
rec=SBUF;
}
