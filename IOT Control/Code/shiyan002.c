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
    for(i = 0; i < ms; i++)
        for(j = 0; j < 114; j++);
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

// I2C发送应答信号
void I2C_Ack(uchar ack) {
    SDA = ack;
    I2C_Delay();
    SCL = 1; I2C_Delay();
    SCL = 0; I2C_Delay();
}

// I2C发送一个字节
void I2C_SendByte(uchar dat) {
    uchar i;
    for(i = 0; i < 8; i++) {
        SDA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        I2C_Delay();
        SCL = 1; I2C_Delay();
        SCL = 0; I2C_Delay();
    }
    SDA = 1;  // 释放总线以接收ACK
    SCL = 1; I2C_Delay();  // 模拟接收ACK过程
    SCL = 0; I2C_Delay();
}

// I2C接收一个字节
uchar I2C_RecvByte() {
    uchar i, dat = 0;
    SDA = 1;  // 释放数据线
    for(i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1; I2C_Delay();
        if(SDA) dat |= 0x01;
        SCL = 0; I2C_Delay();
    }
    return dat;
}

// 写数据到AT24C02
void AT24C02_Write(uchar addr, uchar dat) {
    I2C_Start();
    I2C_SendByte(0xA0);  // 写控制字
    I2C_SendByte(addr);  // 写地址
    I2C_SendByte(dat);   // 写数据
    I2C_Stop();
    DelayMs(5);          // 写操作后延时等待
}

// 从AT24C02读取数据
uchar AT24C02_Read(uchar addr) {
    uchar dat;
    I2C_Start();
    I2C_SendByte(0xA0);  // 写控制字
    I2C_SendByte(addr);  // 指定地址
    I2C_Start();         // 重启信号
    I2C_SendByte(0xA1);  // 读控制字
    dat = I2C_RecvByte();
    I2C_Ack(1);          // 发送NACK
    I2C_Stop();
    return dat;
}

// 数码管动态显示函数
void Display(uchar num) {
    static uchar pos = 0;  // 用于轮流显示十位/个位
    uchar high = num >> 4;  // 高4位
    uchar low  = num & 0x0F;  // 低4位

    // 清除显示
    P0 = 0xFF;
    DIG0 = 0;
    DIG1 = 0;

    if(pos == 0) {
        // 显示十位
        P0 = SEG[high];
        DIG1 = 1;
        pos = 1;
    } else {
        // 显示个位
        P0 = SEG[low];
        DIG0 = 1;
        pos = 0;
    }
}

// 主函数
void main() {
    uchar recv_data;
    uint counter = 0;

    P0 = 0xFF;  // 初始化数码管段选
    P1 = 0x00;  // 初始化位选（DIG0/DIG1）

    AT24C02_Write(0x07, 0x58);  // 向地址0x07写入数据（例如：0x5C）

    while(1) {
        if(++counter >= 500) {
            counter = 0;
            recv_data = AT24C02_Read(0x07);  // 读取地址0x07的数据
        }

        Display(recv_data);
        DelayMs(1);  // 刷新间隔
    }
}
