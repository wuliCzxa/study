#ifndef __DHT11_H__
#define __DHT11_H__

#include "reg52.h"
#include "delay.h"
#define uchar unsigned char //宏定义
#define uint unsigned int
sbit DHTIO = P2^7; //管脚定义，DHT11总线输入输出;
sbit DHTIO1 = P2^6; //管脚定义，DHT11总线输入输出;

/********主机发出开始信号*******/
void DHT11_Start() //请结合时序图分析
{
 DHTIO=1;
 DHTIO1=1;
 delay1();
 DHTIO=0;
 DHTIO1=0;
 delay(20); //保持电平时间大于18ms
 DHTIO=1;
 DHTIO1=1;
 delay1(); //延迟等待20-40us
 delay1();
 delay1();
 delay1();
 delay1();
}

/********DHT11接收一个wai字节数据*******/
uchar receive_data()
{
 uchar i,temp,count;
 data_byte=0;
 for(i=0;i<8;i++)
 {
 count=2;
 while((!DHTIO)&&count++);//等待50us低电平结束
 temp=0;
 delay1();delay1();delay1();delay1();
 if(DHTIO==1) 
 {
 temp=1;
 count=2;
 while((DHTIO)&&count++);
 } 
 if(count==1) break;
 data_byte<<=1; 
 data_byte|=temp;
 }
 return data_byte;
}

/********DHT11接收一个nei字节数据*******/
uchar receive_data1()
{
 uchar i,temp,count;
 data_byte1=0;
 for(i=0;i<8;i++)
 {
 count=2;
 while((!DHTIO1)&&count++);//等待50us低电平结束
 temp=0;
 delay1();delay1();delay1();delay1();
 if(DHTIO1==1) 
 {
 temp=1;
 count=2;
 while((DHTIO1)&&count++);
 } 
 if(count==1) break;
 data_byte1<<=1; 
 data_byte1|=temp;
 }
 return data_byte1;
}

/********DHT11接收数据*******/
void Rec_DHT()
{
 uchar flag;
/////////////////////////////WAI/////////////////////////////////////////////////////////////////
 DHT11_Start();//主机发出开始信号
 DHTIO=1; //拉高等待DHT11响应
 if(!DHTIO) //从机发出响应信号
 {
 flag = 2;
 while((!DHTIO) && flag++); 
//判断从机是否发出 80us 的低电平响应信号是否结束
 flag = 2;
 while(DHTIO && flag++); 
//判断从机是否发出 80us 的高电平，若发出则开始采集数据
 Tem_Z = receive_data(); //采集湿度整数部分
 Tem_X = receive_data(); //采集湿度小数部分
 Hum_Z = receive_data(); //采集温度整数部分
 Hum_X = receive_data(); //采集温度小数部分
 Check = receive_data(); //采集校验位
 DHTIO = 1;
 }
/////////////////////////////NEI////////////////////////////////////////////////////////////////////////////////
 DHT11_Start();//主机发出开始信号
 DHTIO1=1; //拉高等待DHT11响应
 if(!DHTIO1) //从机发出响应信号
 {
 flag = 2;
 while((!DHTIO1) && flag++); 
//判断从机是否发出 80us 的低电平响应信号是否结束
 flag = 2;
 while(DHTIO1 && flag++); 
//判断从机是否发出 80us 的高电平，若发出则开始采集数据
 Tem_Z1 = receive_data1(); //采集湿度整数部分
 Tem_X1 = receive_data1(); //采集湿度小数部分
 Hum_Z1 = receive_data1(); //采集温度整数部分
 Hum_X1 = receive_data1(); //采集温度小数部分
 Check1 = receive_data1(); //采集校验位
 DHTIO1 = 1;
 }
}

#endif