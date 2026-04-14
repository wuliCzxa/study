#ifndef __COM_H__
#define __COM_H__
#include "delay.h"
#include "DHT11.H"
#include "LCD1602.H"

uchar code shuju1[]="Id Humidity：";
uchar code shuju2[]="Od Humidity：";
uchar code shuju3[]="Sta：";
uchar code shuju4[]="Warning";
uchar code shuju5[]="Mode:";

//////////////串口初始化//////////////////////
void Com_Init(void)
{
	//TMOD = 0x20;	//定时器1 方式2，八位自动重载功能00100000		
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

void fasong()
 {
	Com_SendStr(shuju1);
	Com_SendBye(Hum_Z1/10+0x30);
	Com_SendBye(Hum_Z1%10+0x30);
	Com_SendBye(0x0d);
	 
	Com_SendStr(shuju2);
	Com_SendBye(Hum_Z/10+0x30);
	Com_SendBye(Hum_Z%10+0x30);
	Com_SendBye(0x0d);
	 
	 Com_SendStr(shuju3);
	 if(moshi1==0)
	 {
		 if((Hum_Z-Hum_Z1>=50)&&(Hum_Z-Hum_Z1>=data_Humcha))
		{
			Com_SendStr(zifucl);
			Com_SendBye(0x28);
			if(jiaodu3<0)
			{
				Com_SendBye(0x2d);
				Com_SendBye((-jiaodu3)/10+0x30);
				Com_SendBye((-jiaodu3)%10+0x30);
			}
			else
			{
				Com_SendBye(jiaodu3/10+0x30);
				Com_SendBye(jiaodu3%10+0x30);
			}
			Com_SendBye(0x29);
		}
		else
		{
			Com_SendStr(zifuop);
			Com_SendBye(0x28);
			if(jiaodu3<0)
			{
				Com_SendBye(0x2d);
				Com_SendBye((-jiaodu3)/10+0x30);
				Com_SendBye((-jiaodu3)%10+0x30);
			}
			else
			{
				Com_SendBye(jiaodu3/10+0x30);
				Com_SendBye(jiaodu3%10+0x30);
			}
			Com_SendBye(0x29);
		}

	 }
	 else
	 {
		if(jiaodu==-90)
		{
		
			Com_SendStr(zifucl);
			Com_SendBye(0x28);
			if(jiaodu3<0)
			{
				Com_SendBye(0x2d);
				Com_SendBye((-jiaodu3)/10+0x30);
				Com_SendBye((-jiaodu3)%10+0x30);
			}
			Com_SendBye(0x29);
		}
		else
		{
			Com_SendStr(zifuop);
			Com_SendBye(0x28);
			if(jiaodu3<0)
			{
				Com_SendBye(0x2d);
				Com_SendBye((-jiaodu3)/10+0x30);
				Com_SendBye((-jiaodu3)%10+0x30);
			}
			else
			{
				Com_SendBye(jiaodu3/10+0x30);
				Com_SendBye(jiaodu3%10+0x30);
			}
			Com_SendBye(0x29);
		}
		

	 }
	 Com_SendBye(0x0d);
	 
	 
     Com_SendBye(0x0d);
 }


#endif