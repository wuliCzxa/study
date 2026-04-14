/************************************************************************ 
文件名称: main.c 
作者:    
版本:    V1.01
说明:    模数转换实验 
修改记录: 由LED显示模数转换的结果，改变成由四位数码管显示电压值  
-------------------------------------------------------------------------   
* 功能描述: 采集电位器的模拟电压值，转换成数字量
* 通过四位共阳数码管显示
-------------------------------------------------------------------------
* 接线说明:数码管段码P10~P17--SA~SH,数码管位选P20~P23--C0~C3
*          ADC0809：0809DB0~DB7--P00~P07 , 0809CS--P27，0809EOC--P30
*                   0809WR--P36,0809RD--P37,0809A,B,C通道选择--GND  
*                   0809CLK--ALE(注：在核心板上),0809IN0--POT（电位器输出）                                        
*************************************************************************/
#include<reg52.h>
#include<absacc.h>

#define uchar unsigned char
#define uint unsigned int
//char code SST516[3] _at_ 0x003b;

unsigned char code segbit[]={0xc0,0xf9,0xa4,0xb0,	// 0, 1, 2, 3
								 0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off 
unsigned char code combit[]={0xf1,0xf2,0xf4,0xf8};




#define ADC0809 XBYTE[0x7fff]    /* 定义ADC0809 端口地址 */
#define Smg_Seg P1
#define Smg_Com P2
sbit EOC=P3^0;


void TimeInitial();
void Delay(unsigned int i);


void Delay(unsigned int i)
{
  unsigned int j;
  for(;i>0;i--)
  {
  for(j=0;j<125;j++)
  {;}
  }
}
 

void main()
{
	uchar  temp,loopdat1;
	uint   voldata;
	uchar dispbuf[4];
	while(1)
	{
		ADC0809=0x0f;		
		do
		{;}
		while(~EOC);  //转换是否完成		
		//delayms(1);
		temp = ADC0809; //读出转换结果
		voldata = temp*1.0/255*500;
		dispbuf[3] = voldata%10;
		dispbuf[2] = voldata/10%10;
		dispbuf[1] = voldata/100%10;
		dispbuf[0] = voldata/1000;
		for(loopdat1=0;loopdat1<4;loopdat1++)
		{
			
			Smg_Seg = segbit[dispbuf[loopdat1]];
			if( loopdat1 == 1 )
			{
				Smg_Seg &= 0x7f;
			}
			Smg_Com = combit[loopdat1];
			Delay(4);
			Smg_Com = 0xf0;
		}
	}
}




