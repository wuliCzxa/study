/************************************************ 
文件名称： main.c 
作 者：    
版 本：    V1.00
说 明：    HS1011湿度传感器实验
修改记录：  
**************************************************/
/************************************************
*    Descriptoon:                                *
*      通过测量频率转换成湿度，通过两位数码管显示*
*                                                *
**************************************************/
//计数器引脚 P34 接 频率输出
//数码管段码   P20~P27 接 SA~SH
//数码管位选   P10~P11 接 C0~C1

#include <reg52.h>                    // SFR declarations
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int


uchar code seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x01};

void Sys_Init(void);

bit  Disflag=0;

uint fre = 0;  //HS1101频率输出	  

void main(void)
{
	uint j;
	uchar temp0;
    uchar  temp1; 
    uchar hum; 
	
    Sys_Init();
	TR0=1;								// 启动计数器0
	TR1=1;								// 启动定时器1
	IE=0x88;							// 打开定时中断1和总中断
    
	while(1)
	{	
		//Disflag=1;
        if(Disflag)	
        {
            Disflag=0;	 
			
			   //fre=6064;
            if(( 5623<= fre)&&( fre<=6852) )//相对湿度在有效范围内（0％－－100%） 
            { 
                if(( 6734<fre)&&( fre<= 6852) ) 
                { temp0 = 0; temp1 =(6852 -fre)*10/118; } 
                if( (6618<fre)&&( fre<= 6734) ) 
                { temp0 = 1; temp1 =(6734 -fre)*10/116; } 
                if( (6503<fre)&&( fre<= 6618 ) ) 
                { temp0 = 2; temp1 =(6618 -fre)*10/115; } 
                if( (6388<fre)&&( fre<= 6503 ) ) 
                { temp0 = 3; temp1 =(6503 -fre)*10/115; } 
                if( (6271<fre)&&( fre<= 6388 ) ) 
                { temp0 = 4; temp1 =(6388 -fre)*10/117; } 
                if( (6152<fre)&&( fre<= 6271 ) ) 
                { temp0 = 5; temp1 =(6271 -fre)*10/119; } 
                if( (6029<fre)&&( fre<= 6152 ) ) 
                { temp0 = 6; temp1 =(6152 -fre)*10/123; } 
                if( (5901<fre)&&( fre<= 6029 ) ) 
                { temp0 = 7;temp1 =(6029 -fre)*10/128; } 
                if( (5766<fre)&&( fre<= 5901 )) 
                { temp0 = 8; temp1 =(5901 -fre)*10/135; } 
                if( (5623<fre)&&(fre<= 5766)) 
                { temp0 = 9; temp1 =(5766 -fre)*10/143; } 
            } 
            else 
            { 
                temp0 = 0; temp1 = 0; 
            } 
        }
        hum=temp0*10+temp1;

          					 // 输出十位
		P0 = seg[hum/10];	
		P1 = 0x01;
		for(j=0;j<1000;j++);
        P0=0xff;

		P0 = seg[hum%10];			 // 输出个位
		P1 = 0x02;	 
		for(j=0;j<1000;j++);
        P0=0xff;	  

        
	}		   
}
 

/****************************************************************************
* 名称： timer1() 
* 功能： 定时器1，每50000us中断一次。
* 入口参数：
****************************************************************************/
void timer1() interrupt 3 
{
	static char j = 0;
	TH1=0x4C;						   // 重设定时器值，50ms @ 11.0592MHz XTAL
	TL1=0x00;
	if(++j == 20)					   // 50ms * 20 = 1S
	{			  
		j = 0;
		fre = (TH0 << 8) | TL0;		   // 1S内的计数值即为1秒内的输入频率
		TH0 = 0;					   // 清零计数
		TL0 = 0;
        Disflag=1;	 
	}
}


void Sys_Init(void)
{   
	TMOD=0x15;              // 定时器0工作于计数方式，工作方式1，16位计数
	                        // 定时器1工作于定时方式，工作方式1，16位定时
	TH0=0;					// 清零计数器
	TL0=0;
	TH1=0x4C;				// 12M晶振工作下，定时50ms
	TL1=0x00;
}

