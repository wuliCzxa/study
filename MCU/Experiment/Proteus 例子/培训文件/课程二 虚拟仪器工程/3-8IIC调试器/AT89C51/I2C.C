#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char 
#define uint unsigned int
#define out P2
sbit scl=P1^1;
sbit sda=P1^0;
sbit key1=P3^2;
sbit key2=P3^3;
uchar data mem[4]_at_ 0x55;	//发送缓冲区的首地址
uchar mem[4]={0x41,0x42,0x43,0x44}; 
uchar data rec_mem[4] _at_ 0x60 ;//接收缓冲区的首地址
void start(void);//开始信号
void stop(void); //停止信号
void sack(void); //发送应答信号
bit rack(void);	 //接收应答信号
void ackn(void); //发送无应答信号
void send_byte(uchar); //发送一个字节
uchar rec_byte(void);  //接收一个字节
void write(void); //写一组数据
void read(void);  //读一组数据
void delay3us(void); //延时3us 

void main(void)
{
EA=1;EX0=1;EX1=1;
while(1);
}

void ext0()interrupt 0
{
write();
}

void ext1()interrupt 2
{
read();
}

void read(void)
{
uchar i;
bit f;
start();			//开始信号
send_byte(0xa0);	//发从机的地址
f=rack();			//接收应答
if(!f){
start();			//开始信号
send_byte(0xa0);
f=rack();
	send_byte(0x00);//设置要读取从器件的片内地址
	f=rack();
	if(!f){
		  start();		   //开始信号
		  send_byte(0xa1);		
		  f=rack();
		  if(!f){
		  		for(i=0;i<3;i++)
		  	 		{rec_mem[i]=rec_byte();
		   	  		sack();
		   	  		}
		  		rec_mem[3]=rec_byte();ackn();
		  		}
		  }							
	  }
stop();out=rec_mem[3];while(!key2);
}

void write(void)
{
uchar i;
bit f;
start();
send_byte(0xa0);
f=rack();
if(!f){
	   send_byte(0x00);
	   f=rack();
	   if(!f){
		  		for(i=0;i<4;i++)
		  	 		{send_byte(mem[i]);
					 f=rack();
					 if(f)break;
		   	  		}
	   	      }
	   }
stop();out=0xaa;while(!key1);
}

/*开始信号*/
void start(void)
{
scl=1;
sda=1;
delay3us();
sda=0;
delay3us();
scl=0;
}
/*停止信号*/
void stop(void)
{
scl=0;
sda=0;
delay3us();
scl=1;
delay3us();
sda=1;
delay3us();
}
/*接收一个应答位*/
bit rack(void)
{
bit flag;
scl=1;
delay3us();
flag=sda;
scl=0;
return(flag);
}
/*发送一个接收应答位*/
void sack(void)
{
sda=0;
delay3us();
scl=1;
delay3us();
scl=0;
delay3us();
sda=1;
delay3us();
}
/*发送一个非接收接收应答位*/
void ackn(void)
{
sda=1;
delay3us();
scl=1;
delay3us();
scl=0;
delay3us();
}
/*接收一个字节*/
uchar rec_byte(void)
{
uchar i,temp;
for(i=0;i<8;i++)
	{
	temp<<=1;
	scl=1;
	delay3us();
	temp|=sda;
	scl=0;
	delay3us();
	}
return(temp);
}
/*发送一个字节*/
void send_byte(uchar temp)
{
uchar i;
scl=0;
for(i=0;i<8;i++)
	{
	sda=(bit)(temp&0x80);
	scl=1;
	delay3us();
	scl=0;
	temp<<=1;
	}
sda=1;
}
/*延时3US*/
void delay3us(void)
{
_nop_();_nop_();_nop_();
}