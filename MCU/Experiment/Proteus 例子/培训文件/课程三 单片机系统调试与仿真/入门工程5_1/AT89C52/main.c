#include <reg51.h>
#include <stdio.h>
#define uchar unsigned char
#define uint unsigned int

#define led_out P0

uchar code tableYI[10] = 
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//共阴
uchar code tableYA[10] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98};//共阳
uchar code buffer1[] =
{"Welcome to Proteus software"};	//字符串
uchar code buffer2[] =
{0,1,2,3,4,5,6,7,8,9};		//数组
uchar buffer3;

///////////////串口初始化//////////////////////
void Com_Init(void)
{
     TMOD = 0x20;	//定时器1 方式2，八位自动重载功能00100000		
     TH1 = 0xFd;
     TR1 = 1;
     SCON = 0x50;	//设置串口工作方式1 ，允许接收01010000	
     SBUF=0;
     ES = 1;		//开串口中断
     EA = 1;		//开总中断
}
/////////////发送一个字符数据//////////////////////////
void Com_SendBye(uchar dat)
{
      SBUF=dat;
      while(!TI);	//如果发送完毕，硬件会置位TI
      TI=0;		//TI清零
}
/////////////发送字符串//////////////////////////
 void Com_SendStr(uchar *dat)
 {	
    while(1)
    {
      if(*dat == '\0')	//在每个字符串的最后，会有一个'\0'
	break;
      else 
      {
	 SBUF=*dat;
	 while(!TI);       //如果发送完毕，硬件会置位TI
	 TI = 0;		//TI清零
	 dat++;
      }
   }
 }
///////////////延时函数/////////////////////////
void delayms(uint j)		  //1ms延时
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
//////////////主函数//////////////////////////
void main(void)
 { 
   uchar i=0;
   Com_Init();
    
    
   Com_SendStr(buffer1);
   Com_SendBye(0x0d);	//0x0d相当于字符的“\r”   —————— 回车键
   for(;i<10;i++)
   {
      Com_SendBye(buffer2[i]+0x30);	//加上0x30对应ascii表0-9
   }
   
   
   while (1)
   {
      led_out=tableYA[buffer3];
    }
 }
 
 //////////////////串口中断函数//////////////////////////
 void Com_Int(void) interrupt 4
{
//	static uchar i = 7;    //定义为静态变量，当重新进入这个子函数时 i 的值不会发生改变
	if(RI == 1)   		   //当硬件接收到一个数据时，RI会置位
	{
		buffer3 = SBUF - 0x30; //这里减去0x30是因为从电脑中发送过来的数据是ASCII码。
		RI = 0;  
	}
}



