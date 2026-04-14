#include <reg51.h>              // 包含51系列单片机的特殊功能寄存器定义
#include <intrins.h>            // 包含内部函数定义，如_nop_()

// 基本数据类型定义，增强可读性
typedef unsigned char  U8;      // 无符号8位整型
typedef signed   char  S8;      // 有符号8位整型
typedef unsigned int   U16;     // 无符号16位整型
typedef signed   int   S16;     // 有符号16位整型
typedef unsigned long  U32;     // 无符号32位整型
typedef signed   long  S32;     // 有符号32位整型
typedef float          F32;     // 单精度浮点数
typedef double         F64;     // 双精度浮点数

#define uchar unsigned char
#define uint unsigned int
#define Data_0_time 4          // 读取DHT数据时，低电平时间参考值

//----------------------------------------------
// IO口定义
//----------------------------------------------
sbit P2_0 = P2^0;               // 数据传输引脚连接到P2.0

//----------------------------------------------
// 全局变量定义
//----------------------------------------------
U8 U8FLAG, k;
U8 U8count, U8temp;
U8 U8T_data_H, U8T_data_L, U8RH_data_H, U8RH_data_L, U8checkdata;
U8 U8T_data_H_temp, U8T_data_L_temp, U8RH_data_H_temp, U8RH_data_L_temp, U8checkdata_temp;
U8 U8comdata;
U8 outdata[5];                // 串口发送数据缓存
U8 indata[5];                 // 串口接收数据缓存
U8 count, count_r = 0;        // 串口数据发送/接收计数器
U8 str[5] = {"RS232"};         // 串口初始提示信息
U16 U16temp1, U16temp2;

// 串口发送函数
SendData(U8 *a)
{
    outdata[0] = a[0]; 
    outdata[1] = a[1];
    outdata[2] = a[2];
    outdata[3] = a[3];
    outdata[4] = a[4];
    count = 1;
    SBUF = outdata[0];       // 启动发送第一个字节
}

// 毫秒级延时函数
void Delay(U16 j)
{
    U8 i;
    for(; j > 0; j--)
        for(i = 0; i < 27; i++);
}

// 10微秒延时函数
void Delay_10us(void)
{
    U8 i;
    i--; i--; i--; i--; i--; i--;
}

// 读取一个字节数据函数（从DHT传感器）
void COM(void)
{
    U8 i;
    for(i = 0; i < 8; i++)
    {
        U8FLAG = 2;
        while((!P2_0) && U8FLAG++); // 等待低电平结束
        Delay_10us(); Delay_10us(); Delay_10us();

        U8temp = 0;
        if(P2_0) U8temp = 1;       // 判断数据位是0还是1

        U8FLAG = 2;
        while((P2_0) && U8FLAG++); // 等待高电平结束
        if(U8FLAG == 1) break;     // 超时则跳出

        U8comdata <<= 1;
        U8comdata |= U8temp;
    }
}

// 读取温湿度主函数（驱动DHT）
void RH(void)
{
    P2_0 = 0;
    Delay(180);                   // 主机拉低18ms启动信号
    P2_0 = 1;
    Delay_10us(); Delay_10us(); Delay_10us(); Delay_10us();
    P2_0 = 1;

    if(!P2_0)                      // 等待从机响应
    {
        U8FLAG = 2;
        while((!P2_0) && U8FLAG++); // 等待从机发出80us低电平
        U8FLAG = 2;
        while((P2_0) && U8FLAG++);  // 等待从机发出80us高电平

        COM(); U8RH_data_H_temp = U8comdata;
        COM(); U8RH_data_L_temp = U8comdata;
        COM(); U8T_data_H_temp  = U8comdata;
        COM(); U8T_data_L_temp  = U8comdata;
        COM(); U8checkdata_temp = U8comdata;
        P2_0 = 1;

        // 校验和
        U8temp = (U8T_data_H_temp + U8T_data_L_temp + U8RH_data_H_temp + U8RH_data_L_temp);
        if(U8temp == U8checkdata_temp)
        {
            U8RH_data_H = U8RH_data_H_temp;
            U8RH_data_L = U8RH_data_L_temp;
            U8T_data_H = U8T_data_H_temp;
            U8T_data_L = U8T_data_L_temp;
            U8checkdata = U8checkdata_temp;
        }
    }
}

// 共阴数码管段码表（0~F）
uchar code smgduan_cc[] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71
};

// 共阳数码管段码表（0~F），共阴取反
uchar code smgduan_ca[] = {
    ~0x3F, ~0x06, ~0x5B, ~0x4F,
    ~0x66, ~0x6D, ~0x7D, ~0x07,
    ~0x7F, ~0x6F, ~0x77, ~0x7C,
    ~0x39, ~0x5E, ~0x79, ~0x71
};

// 延时函数，单位ms
void delay_ms(uint ms) {
    uint i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);
}

// 显示共阴数码管（2位，带小数点）
void display_cc(uchar hex, bit dot_on) {
    uchar high = hex >> 4;
    uchar low  = hex & 0x0F;

    P1 |= 0x03;
    P0 = smgduan_cc[high] | (dot_on ? 0x80 : 0x00);
    P1 &= ~0x01;
    delay_ms(2);
    P1 |= 0x01;

    P1 |= 0x03;
    P0 = smgduan_cc[low] | (dot_on ? 0x80 : 0x00);
    P1 &= ~0x02;
    delay_ms(2);
    P1 |= 0x02;
}

// 显示共阳数码管（2位，带小数点）
void display_ca(uchar hex, bit dot_on) {
    uchar high = hex >> 4;
    uchar low  = hex & 0x0F;

    P1 &= 0xF3;
    P0 = smgduan_ca[high] & (dot_on ? ~0x80 : 0xFF);
    P1 |= 0x04;
    delay_ms(2);
    P1 &= ~0x04;

    P1 &= 0xF3;
    P0 = smgduan_ca[low] & (dot_on ? ~0x80 : 0xFF);
    P1 |= 0x08;
    delay_ms(2);
    P1 &= ~0x08;
}

// 主函数
void main()
{
    U8 i, j;
    uchar num_ca = 0, num_cc = 0;
    uchar prev_num_ca = 0xFF, prev_num_cc = 0xFF;

    // 串口初始化: 11.0592MHz, 9600bps
    TMOD = 0x20;
    TH1 = 253;
    TL1 = 253;
    TR1 = 1;
    SCON = 0x50;
    ES = 1;
    EA = 1;
    TI = 0;
    RI = 0;

    SendData(str);   // 发送初始化提示信息
    Delay(1);

    while(1)
    {
        RH(); // 获取温湿度数据

        str[0] = U8RH_data_H;
        str[1] = U8RH_data_L;
        str[2] = U8T_data_H;
        str[3] = U8T_data_L;
        str[4] = U8checkdata;

        SendData(str); // 通过串口发送数据

        num_ca = str[0];
        num_cc = str[2];

        if (num_ca != prev_num_ca || num_cc != prev_num_cc)
        {
            prev_num_ca = num_ca;
            prev_num_cc = num_cc;
        }

        // 显示数码管内容 2 秒
        for(i = 0; i < 200; i++)
        {
            display_ca(num_ca, 0);
            display_cc(num_cc, 0);
        }

        // 小数点闪烁效果
        for(j = 0; j < 10; j++)
        {
            display_ca(num_ca, 1);
            display_cc(num_cc, 1);
        }
    }
}

// 串口中断服务函数（发送+接收）
void RSINTR() interrupt 4 using 2
{
    U8 InPut3;

    if(TI == 1)
    {
        TI = 0;
        if(count != 5)
        {
            SBUF = outdata[count];
            count++;
        }
    }

    if(RI == 1)
    {
        InPut3 = SBUF;
        indata[count_r] = InPut3;
        count_r++;
        RI = 0;

        if (count_r == 5)
        {
            count_r = 0;
            str[0] = indata[0];
            str[1] = indata[1];
            str[2] = indata[2];
            str[3] = indata[3];
            str[4] = indata[4];
            P0 = 0; // 清屏指令或提示用
        }
    }
}