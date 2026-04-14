#ifndef __DELAY_H__
#define __DELAY_H__

/*********ÑÓÊ±º¯Êý********/
void delay(uchar ms)
{ 
 uchar i ;
 while(ms--)
 {
 for(i=0;i<250;i++);
 }
}
/***ÑÓÊ±10us***/
void delay1()
{
 uchar i;
 i--;i--;i--;
 i--;i--;i--;
}



#endif