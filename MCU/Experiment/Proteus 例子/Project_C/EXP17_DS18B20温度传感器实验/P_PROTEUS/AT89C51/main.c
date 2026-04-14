/***********************************************************  
文件名称： main.c 
作 者：    zw sangel
版 本：    V1.00
说 明：    DS18B20温度传感器实验 
修改记录：  
------------------------------------------------------------
*功能描述: 实时测量周边环境的温度，并提供数码管显示温度值                                                              				                               
*-----------------------------------------------------------                 					                                       
*接线说明：P00~P07-SA~SH，P10~P11-C0~C1,P37-DQ                 					                                       
*                                                                                                                                                            
************************************************************/
#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int 

uchar code seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x01};


sbit DQ=P3^7;	//定义io口
void delay5(uchar);
void init_ds18b20(void);
uchar readbyte(void);
void writebyte(uchar);
uchar retemp(void);
/*********主函数********************************************/
void main(void)
{
uchar i,temp;
delay5(1000);
temp=retemp(); //读取温度值
while(1)
	{
	 		 
		for(i=0;i<50;i++)
		{
	 	P0 = seg[temp/10];	// 输出十位
		P1 = 0x01;
		delay5(1000);		//延时5ms
		if(i==0)			//扫描50次 采样一次
		temp=retemp();
        P0=0xff;

		P0 = seg[temp%10];	// 输出个位
		P1 = 0x02;	 
		delay5(1000);		//延时5ms

        P0=0xff;
		}
			  
	}

}

 /*--------------精确延时5us子程序---------*/	
void delay5(uchar n)
{
	 do
	 {
	 _nop_();
	 _nop_();
	 _nop_();
	 n--;
	 }
	 while(n);
}
/*--------------初始化函数--------------------*/
void init_ds18b20(void)
{
	 uchar x=0; 
	 DQ =0;    
	 delay5(100); 
	 DQ =1;    
	 delay5(5);
	 delay5(20);
}
/*--------------读取一字节函数----------------*/
uchar readbyte(void)
{
	uchar i=0;
	uchar date=0;
	for (i=8;i>0;i--)
	 {
		  DQ =0;
		  delay5(1);
		  DQ =1;	//15微秒内拉释放总线
		  date>>=1;
		  if(DQ)
		  date|=0x80;
		  delay5(11);
	 }
 	return(date);
}
/*--------------写一字节函数------------------*/
void writebyte(uchar dat)
{
 uchar i=0;
 for(i=8;i>0;i--)
	 {
	  DQ =0;
	  DQ =dat&0x01;    //写"1" 在15微秒内拉低
	  delay5(12);	   //写"0" 拉低60微秒
	  DQ = 1;	   
	  dat>>=1;
	  delay5(5);
	  }
}
/*--------------读取温度函数------------------*/
uchar retemp(void)
{
	uchar a,b,tt;
	uint t;
	init_ds18b20();		//初始化
	writebyte(0xCC); 	//写命令
	writebyte(0x44);
	init_ds18b20();
	writebyte(0xCC); 
	writebyte(0xBE); 
	a=readbyte();		//读出低位
	b=readbyte();		//读出高位
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;		//转换为摄氏温度值
	return(tt);
}