/*************************************************  
*文件名称： main.c 
*作 者：    
*版 本：    V1.00
*说 明：    红外遥控器解码实验
*修改记录：  
--------------------------------------------------
*Descriptoon: 
*接线说明：P34-红外接收,P25-RS,P26-RW,P27-EN,
           P00~P07-DB0~DB7                 
**************************************************/                         

#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int
#define c(x)(x*110592/120000) //晶振11.0592

uchar Lcd_Buf[4];             //显示数据缓冲区         
unsigned char Ir_Buf[4];      //用于保存解码结果
sbit Ir_Pin=P3^4;             //红外线数据接口
sbit rs=P2^0;   	          //LCD数据命令端
sbit rw=P2^1;		          //读写接口
sbit lcde=P2^2;		          //使能接口
uchar code table[]=" WWW.WINDWAY.CN ";//LCD显示数据
uchar code table1[]="Wait for IR in..";
uchar code table2[]="  IR DATA...";
		   
	   unsigned char code Led_Tab[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xf8,
	0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};   //共阳极数码显示码0-F
unsigned char code Led_Sel[]={0xf,0xf,0xb,0x7}; //数码管位选，屏蔽前两位
unsigned char Led_Buf[4]; //显示缓冲区
char Led_Index;           //定义位选         
unsigned char Ir_Buf[4];  //用于保存解码结果

sbit LCD_E = P2^7;
//========1MS延时====================================
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=113;y>0;y--);
}
//======LCD写命令=====
void write_com(uchar com)
{
	rs=0;
	lcde=0;
	P0=com;
	lcde=1;
	delay(1);//延时1MS跳过忙信号
	lcde=0;
	delay(1);
}
//======LCD写数据=====================================
void write_date(uchar date)
{
	rs=1;
	lcde=0;
	P0=date;
	lcde=1;
	delay(1);
	lcde=0;
	delay(1);
}
//=====初始化函数=====================================
void init()
{	
	uchar num;
	rw=0;		//允许LCD写
	lcde=0;
	write_com(0x38);//显示模式设置							  
	write_com(0x0c);//显示开及光标设置
	write_com(0x06);//显示光标移动设置
	write_com(0x01);//清屏
	write_com(0x80);//显示首地址
	for(num=0;num<15;num++)//显示第一行
	{
		write_date(table[num]);
		delay(1);
	}
	write_com(0x80+0x40);//显示第二行
        for(num=0;num<16;num++)
	{
		write_date(table1[num]);
		delay(1);
	}
	TMOD=0x10;		//设置定时器0、1工作模式为1
}
//=====显示解码========================================
void write_ir(uchar date)
{
	uchar tmp, num,ff;	  

	if(ff==0)
	{
		ff++;
		write_com(0x01);
		write_com(0x80);
		for(num=0;num<12;num++)
		{
			write_date(table2[num]);
			delay(1);
		}	 	
	}
	
	write_com(0xC0+6);//定位在第二行第六位
	tmp = date>>4;			// 高四位	 
	write_date(tmp > 9 ? 'A'+tmp-10 : '0'+tmp);
	write_com(0xC0+7);//定位第二行第七位	   
	tmp = date&0x0F;			// 低四位
	write_date(tmp > 9 ? 'A'+tmp-10 : '0'+tmp);	 

}
//======================================================
//解码取回低电平
unsigned int Ir_Get_Low()
{
	TL1=0;
	TH1=0;
	TR1=1;
	while(!Ir_Pin && (TH1&0x80)==0);   //等待低电平到来              
	TR1=0;           
	return TH1*256+TL1;//提取低电平时间间隔时长
}
//======================================================
//解码取回高电平========================================
unsigned int Ir_Get_High()
{
	TL1=0;
	TH1=0;
	TR1=1;
	while(Ir_Pin && (TH1&0x80)==0);//等待高电平到来
	TR1=0;
	return TH1*256+TL1;//提取高电平时间间隔时长
}
//======================================================
//======================================================

void main()
{
	unsigned int temp;
	char i,j;
	init();	//初始化
//=====用do做循环，等待遥控按键=========================
	do{
restart:
		while(Ir_Pin) ;//等待低电平

		temp=Ir_Get_Low();//把低电平附给变量
		if(temp<c(8500) || temp>c(9500)) continue;//引导脉冲低电平等待9000us
		temp=Ir_Get_High();//把高电平附给变量
		if(temp<c(4000) || temp>c(5000)) continue;//引导脉冲高电平等待4500us
		//连续读出32个字节
		for(i=0;i<4;i++) //4个字节
		{
			for(j=0;j<8;j++) //每个字节8位
			{
				temp=Ir_Get_Low();
				if(temp<c(200) || temp>c(800)) goto restart;//等待560us跳过低电平
				temp=Ir_Get_High();
				if(temp<c(200) || temp>c(2000)) goto restart;//等待1000us跳过高电平
				Ir_Buf[i]>>=1;//把电平移进数组
				if(temp>c(1120)) Ir_Buf[i]|=0x80;//若为高电平再填1
			}
		}  
		write_ir(Ir_Buf[2]);//调用显示	
	}	
	while(1);//循环
}