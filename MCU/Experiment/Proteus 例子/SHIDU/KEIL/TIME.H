#ifndef __TIME_H__
#define __TIME_H__


void Time0_Init()
{
	TMOD = 0x21;	//方式1，16位定时功能      
	TH0  = 0x3c;	//高八低5位
	TL0  = 0xb0;
     
	ET0 = 1;
	EA = 1;		
	TR0 = 1;		
}


#endif