/*******************************************
文件名:16and16dotled
MCU :AT89S52
功能:测试代码 
编写:广州风标 zw
日期:2011年01月28日 
说明： 
接线方式：74ls595 P1.0-SHC   P1.1-STC  p1.2-DS 
		  74ls138 P2.0-138A1  P2.1-138B1 P2.2-138C1 P2.3-138EN1 P2.4-138A2  P2.5-138B2 P2.6-138C2   P2.7-138EN2
其他： 
********************************************/

#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int

uchar code dischar[128]={ 
/*--  文字:  广  --*/
/*--  华文彩云12;  此字体下对应的点阵为：宽x高=16x17   --*/
0xFF,0xFF,0x7F,0xF8,0x07,0x07,0xF3,0x7F,0xFB,0x7F,0x1B,0x00,0xDB,0xFF,0xDB,0xFF,
0xDB,0xFF,0xDB,0xFF,0xDB,0xFF,0xDB,0xFF,0xDD,0xFF,0xED,0xFF,0xED,0xFF,0xF1,0xFF,

/*--  文字:  州  --*/
/*--  华文彩云12;  此字体下对应的点阵为：宽x高=16x17   --*/
0xFF,0xFF,0x43,0x84,0xBB,0xBB,0xBB,0xBB,0xA3,0xBB,0x5D,0xB7,0xDD,0xB6,0xDD,0xAE,
0xDD,0xAE,0xDD,0xAE,0x21,0xB3,0xBB,0xBB,0xBB,0xBB,0x9D,0xBB,0xAD,0xBB,0x63,0x84,

/*--  文字:  风  --*/
/*--  华文彩云12;  此字体下对应的点阵为：宽x高=16x17   --*/
0xFF,0xFF,0x03,0xE0,0xFB,0xDF,0xFB,0xDF,0x0B,0xD8,0x6B,0xD6,0xEB,0xD7,0xDB,0xDB,
0xDB,0xDB,0x9B,0xDB,0xCB,0x13,0xED,0x77,0x6D,0x76,0x8D,0x79,0xED,0xB7,0xF1,0xCF,

/*--  文字:  标  --*/
/*--  华文彩云12;  此字体下对应的点阵为：宽x高=16x17   --*/
0xFF,0xFF,0x03,0x80,0xBB,0xBF,0xB9,0xBF,0x7E,0x80,0x7E,0x7F,0x99,0x7F,0x6D,0x5F,
0xCD,0xA4,0x1E,0xB5,0xBE,0x75,0xDD,0x6C,0xDB,0x6C,0x3B,0x8F,0x3B,0xEF,0xC3,0xE0,

};

/***********端口定义*****************/
sbit LS138_A1 =P2^0;
sbit LS138_B1 =P2^1;
sbit LS138_C1 =P2^2;
sbit LS138_EN1=P2^3;

sbit LS138_A2 =P2^4;
sbit LS138_B2 =P2^5;
sbit LS138_C2 =P2^6;

sbit LS138_EN2=P2^7;

sbit LS595_SHC=P1^0;    //SHcp   锁存时钟-----74HC595 
sbit LS595_STC=P1^1;    //STcp   移位时钟-----74HC595 
sbit LS595_DS =P1^2;    //DSI    数据---------74HC595 
  

/**********nms延时子程序*************/
void Delay_Nms(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<20;j++)
			;
}



/**********发送一字节数据*************/
void SendOneByte(uchar content)
{  
   uchar i; 
    
    for(i=0;i<8;i++) 
    { 
        LS595_DS=(bit)(content & 0x80);      //判断输出数据 
        LS595_SHC=0;        			     //初始化移位时钟 
        content<<=1; 						 //更新数据 
        LS595_SHC=1; 
    }    
}

/***********主函数********************/
void main(void) 
{ 
    uchar i;
	uchar j;
	uchar k;
	uchar	ycode; 
	LS138_EN1=0;		 //不使能138译码器  
	LS138_EN2=0;		 //不使能138译码器  
	while(1) 
    {   for(j=0;j<4;j++)
		{
			for(k=0;k<50;k++)
			{
				for(i=0;i<16;i++) 
        		{ 
					LS595_STC=0; 						
            		SendOneByte(dischar[i*2+j*32+1]); //译码显示 
					SendOneByte(dischar[i*2+j*32]);   //译码显示 			
					LS595_STC=1;	      //数据已准备好
					if(i<8)
					{
						//ycode=i;
						P2=i; 		      //开启扫描的行数据 
						LS138_EN1=1;	  //打开前8行输出
						//LS138_EN2=0;	  //关闭后8行输出
					}	 
					else 
					{
			 			ycode=i-8;
						P2=ycode<<4; 	  //开启扫描的行数据 
						//LS138_EN1=0;	  //关闭前8行输出
						LS138_EN2=1;	  //打开后8行输出
					}		 
            		Delay_Nms(4);
					LS138_EN1=0;	      //关闭行输出
					LS138_EN2=0;	      //关闭行输出 		    
            	}
			} 
		}
		
    }                
}

