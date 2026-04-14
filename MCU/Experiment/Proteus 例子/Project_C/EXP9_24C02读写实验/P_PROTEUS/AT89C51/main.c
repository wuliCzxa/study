/*******************************************************************************  
文件名称： main.c 
作 者：    
版 本：    V1.00
说 明：    24C02读写实验 
修改记录：  
-------------------------------------------------------------------------------- 
* 功能描述:                                                                    
*          程序每重新运行一次                                                  
*          就向24C02的特定地址读出一字节数据，                                 
*          然后把该字节数据显示出来，然后对该字节数据加一后，                  
*          重新写入该地址
*-------------------------------------------------------------------------------
*接线说明：P10-SDA，P11-SCK，P00~P07——SA~SH，P20~P22——C0~C2				                                       
*******************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int
#define rom_add 0x0a	  //定义特定地址读取
#define P_COM P2		  //位控制输出接口
#define P_SEG P0		  //段码控制输出接口

/*******端口定义***************************************************************/
sbit SCK=P1^1;
sbit SDA=P1^0;

unsigned char data dis_digit;
unsigned char code dis_code[11]={0xc0,0xf9,0xa4,0xb0,	// 0, 1, 2, 3
								 0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};// 4, 5, 6, 7, 8, 9, off 
unsigned char data dis_buf[3];	  //定义显示位数 2位
unsigned char data dis_cnt;

uchar rom_dat=0; //写入数据初始化为0
/********函数申明**************************************************************/
void 	start(void);		//开始信号
void 	stop(void); 		//停止信号
bit  	rack(void);	 		//接收应答信号
void 	ackn(void); 		//发送无应答信号
void 	send_byte(uchar); 	//发送一个字节
uchar 	rec_byte(void);  	//接收一个字节
void 	write(void); 		//写一组数据




/********nms延时子程序*********************************************************/
void Delay_Nms(uint n)
{
	uint i,j;
	for(i=0;i<n;i++)
		for(j=0;j<125;j++)
			;
}

/********发送开始信号**********************************************************/
void start(void)
{
	SCK=1;
	SDA=1;
	_nop_();
	SDA=0;
	_nop_();
	SCK=0;
	_nop_();
}

/********发送停止信号*********************************************************/
void stop(void)
{
	SCK=0;
	SDA=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=1;
	_nop_();
}

/********接收一个应答位*******************************************************/
bit rack(void)
{
	bit flag;
	SCK=1;
	_nop_();
	flag=SDA;
	SCK=0;
	return(flag);
}

/********发送一个非接收接收应答位*********************************************/
void ackn(void)
{
	SDA=1;
	_nop_();
	SCK=1;
	_nop_();
	SCK=0;
	_nop_();
}

/********接收一个字节*********************************************************/
uchar rec_byte(void)
{
	uchar i,temp;
	for(i=0;i<8;i++)
	{
		temp<<=1;
		SCK=1;
		_nop_();
		temp|=SDA;
		SCK=0;
		_nop_();
	}
return(temp);
}

/********发送一个字节*********************************************************/
void send_byte(uchar temp)
{
	uchar i;
	SCK=0;
	for(i=0;i<8;i++)
	{
		SDA=(bit)(temp&0x80);
		SCK=1;
		_nop_();
		SCK=0;
		temp<<=1;
	}
	SDA=1;
}
/********读函数**************************************************************/
void read(void)
{
	bit f;
	start();			//开始信号
	send_byte(0xa0);	//发送读命令
	f=rack();			//接收应答
	if(!f)
	{		
		send_byte(rom_add);//设置要读取从器件的片内地址
		f=rack();
		if(!f)
		{
			start();		   //开始信号
			send_byte(0xa1);	//发送读命令	
			f=rack();
			if(!f)
			{
				rom_dat=rec_byte();//读出数据
		   	  	ackn();
		   	 }		  	
	    }							
	}
	stop();
}
/*******写函数***************************************************************/
void write(void)
{
	bit f;
	start();
	send_byte(0xa0);			 //发送读命令
	f=rack();
	if(!f)
	{
		send_byte(rom_add);		 //发送器件地址
		f=rack();
		if(!f)
		{
		 	send_byte(rom_dat);	 //发送数据
			f=rack();
		 }
	} 
	stop();
}
/********主函数***************************************************************/
void main(void)
{
	P_COM = 0xff;  //点亮所有数码管
	P_SEG = 0x00;
	TMOD = 0x01;   //初始化定时器0
	TH0  = 0xFC;
	TL0  = 0x17;
	IE 	 = 0x82;   //开中断
	Delay_Nms(20);
	read();		   //从EEPROM中读取数据
	dis_buf[0]=dis_code[rom_dat/100];	//数据暂存到数据缓冲器
	dis_buf[1]=dis_code[(rom_dat%100)/10];
	dis_buf[2]=dis_code[rom_dat%10];
	rom_dat+=1;	   //计数器加1
	Delay_Nms(20);
	write();	   //再次把数据写回到EEPROM中
	TR0 = 1;	   //启动定时器定时
	while(1)
	{;}
}
/********定时器0中断处理函数**************************************************/
void timer0() interrupt 1
// 定时器0中断服务程序, 用于数码管的动态扫描
// dis_cnt --- 扫描计数
// dis_buf   --- 显于缓冲区基地址	
{
	TH0 = 0xFA;
	TL0 = 0x17;
	P_COM = 0x00;							// 先关闭所有数码管

	switch(dis_cnt)
	{
		case	0:P_COM=0x01;P_SEG=dis_buf[0];dis_cnt++;break;
		case	1:P_COM=0x02;P_SEG=dis_buf[1];dis_cnt++;break; 
		case	2:P_COM=0x04;P_SEG=dis_buf[2];dis_cnt=0;break; //扫描最末位时 清零位扫描计数
		default	 :break;	
	}
}













