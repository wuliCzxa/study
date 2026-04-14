#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

// I2C引脚定义
sbit SDA = P2^1;  // 数据线
sbit SCL = P2^0;  // 时钟线

// 数码管位选控制（共阳数码管高电平有效）
sbit DIG1 = P1^1;  // 十位
sbit DIG0 = P1^0;  // 个位

// 共阳数码管段码表（0-F）
uchar code SEG[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,  // 0-4
    0x92, 0x82, 0xF8, 0x80, 0x90,  // 5-9
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E  // A-F
};

// 微秒级延时
void I2C_Delay() {
    _nop_(); _nop_(); _nop_(); _nop_();
}

// 毫秒级延时
void DelayMs(uint ms) {
    uint i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<114; j++);
}

// I2C起始信号
void I2C_Start() {
    SDA = 1; I2C_Delay();
    SCL = 1; I2C_Delay();
    SDA = 0; I2C_Delay();
    SCL = 0; I2C_Delay();
}

// I2C停止信号
void I2C_Stop() {
    SDA = 0; I2C_Delay();
    SCL = 1; I2C_Delay();
    SDA = 1; I2C_Delay();
}

// I2C发送应答
void I2C_Ack(uchar ack) {
    SDA = ack;
    I2C_Delay();
    SCL = 1; I2C_Delay();
    SCL = 0; I2C_Delay();
}

// I2C发送单字节
void I2C_SendByte(uchar dat) {
    uchar i;
    for(i=0; i<8; i++) {
        SDA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        I2C_Delay();
        SCL = 1; I2C_Delay();
        SCL = 0; I2C_Delay();
    }
    I2C_Ack(1);  // 等待从机应答
}

// I2C接收单字节
uchar I2C_RecvByte() {
    uchar i, dat = 0;
    SDA = 1;  // 释放数据线
    for(i=0; i<8; i++) {
        SCL = 1; I2C_Delay();
        dat <<= 1;
        if(SDA) dat |= 0x01;
        SCL = 0; I2C_Delay();
    }
    return dat;
}

// AT24C02写数据
void AT24C02_Write(uchar addr, uchar dat) {
    I2C_Start();
    I2C_SendByte(0xA0);  // 器件地址+写
    I2C_SendByte(addr);   // 存储地址
    I2C_SendByte(dat);    // 写入数据
    I2C_Stop();
    DelayMs(5);  // 必须的写入等待
}

// AT24C02读数据
uchar AT24C02_Read(uchar addr) {
    uchar dat;
    I2C_Start();
    I2C_SendByte(0xA0);  // 器件地址+写
    I2C_SendByte(addr);   // 指定地址
    I2C_Start();
    I2C_SendByte(0xA1);   // 器件地址+读
    dat = I2C_RecvByte();
    I2C_Ack(1);           // 发送NACK
    I2C_Stop();
    return dat;
}
/*

// 数码管动态显示
void Display(uchar num) {
    static uchar pos = 0;  // 动态扫描位置
    uchar shi = num >> 4;  // 十位
    uchar ge = num & 0x0F; // 个位
    
    // 先关闭所有显示
    P0 = 0xFF;
    DIG0 = 0;
    DIG1 = 0;
    
    if(pos == 0) {
        // 显示十位
        P0 = SEG[shi];
        DIG1 = 1;
        pos = 1;
    } else {
        // 显示个位
        P0 = SEG[ge];
        DIG0 = 1;
        pos = 0;
    }
}
*/

// 数码管动态显示（显示两个独立的数字x和y）
void Display(uchar y, uchar x) {
    static uchar pos = 0;  // 动态扫描位置

    // 先关闭所有显示
    P0 = 0xFF;
    DIG0 = 0;
    DIG1 = 0;

    if(pos == 0) {
        // 显示高位（y）
        P0 = SEG[y];
        DIG1 = 1;
        pos = 1;
    } else {
        // 显示低位（x）
        P0 = SEG[x];
        DIG0 = 1;
        pos = 0;
    }
}

/*
void main() {
    uchar recv_data;
    uint counter = 0;
    
    P0 = 0xFF;
    P1 = 0x00;
    
    AT24C02_Write(0x07, 0x58);
    
    while(1) {
        // 每隔一段时间读取一次EEPROM
        if(++counter >= 500) {  // 约每500次显示刷新读取一次
            counter = 0;
            recv_data = AT24C02_Read(0x07);
        }
        
        Display(recv_data);
        DelayMs(1);  // 整体显示刷新控制
    }
}
*/

void main() {
    uchar recv_data;
    uchar x, y;
    uint counter = 0;

    P0 = 0xFF;
    P1 = 0x00;

    AT24C02_Write(0x07, 0x58);  // 示例写入值0x58

    while(1) {
        // 每隔一段时间读取一次EEPROM
        if(++counter >= 500) {
            counter = 0;
            recv_data = AT24C02_Read(0x07);
            x = recv_data % 16;  // 个位
            y = recv_data / 16;  // 十位
        }

        Display(y, x);  // 显示两个数
        DelayMs(1);     // 整体显示刷新控制
    }
}
