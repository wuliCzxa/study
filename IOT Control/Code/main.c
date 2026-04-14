#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int

sbit SCL = P1^6;
sbit SDA = P1^7;
sbit DIG1 = P1^0;
sbit DIG2 = P1^1;

// 共阳极数码管编码表
uchar code segTable_CA_Dot[] = {
		/* 
		// 共阴极编码表 无小数点
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
		*/
    0xC1, // 0.
    0xF9, // 1.
    0xA5, // 2.
    0xB1, // 3.
    0x99, // 4.
    0x93, // 5.
    0x83, // 6.
    0xF9, // 7.
    0x81, // 8.
    0x91, // 9.
    0x89, // A.
    0x83, // b.
    0xC7, // C.
    0xA1, // d.
    0x87, // E.
    0x8F  // F.
};

// 延时函数
void delay(uint t) {
    uint i, j;
    for (i = 0; i < t; i++)
        for (j = 0; j < 123; j++);
}

// I2C 开始
void I2C_Start() {
    SDA = 1; delay(1);
    SCL = 1; delay(1);
    SDA = 0; delay(1);
    SCL = 0;
}

// I2C 停止
void I2C_Stop() {
    SDA = 0; delay(1);
    SCL = 1; delay(1);
    SDA = 1;
}

// 发送一个字节
void I2C_WriteByte(uchar dat) {
    uchar i;
    for (i = 0; i < 8; i++) {
        SDA = dat & 0x80;
        dat <<= 1;
        SCL = 1; delay(1);
        SCL = 0; delay(1);
    }
    SDA = 1; // 等待 ACK
    SCL = 1; delay(1);
    SCL = 0;
}

// 读取一个字节
uchar I2C_ReadByte() {
    uchar i, dat = 0;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1; delay(1);
        if (SDA) dat |= 0x01;
        SCL = 0; delay(1);
    }
    return dat;
}

// 应答
void I2C_Ack(bit ack) {
    SDA = ack;
    SCL = 1; delay(1);
    SCL = 0;
    SDA = 1;
}

// 写 EEPROM
void EEPROM_Write(uchar addr, uchar dat) {
    I2C_Start();
    I2C_WriteByte(0xA0);  // 写控制字
    I2C_WriteByte(addr);
    I2C_WriteByte(dat);
    I2C_Stop();
    delay(10);  // 写入完成延时
}

// 读 EEPROM
uchar EEPROM_Read(uchar addr) {
    uchar dat;
    I2C_Start();
    I2C_WriteByte(0xA0);
    I2C_WriteByte(addr);
    I2C_Start();
    I2C_WriteByte(0xA1);
    dat = I2C_ReadByte();
    I2C_Ack(1);
    I2C_Stop();
    return dat;
}

// 使用共阳极编码表（已含小数点的编码）
void displayHex(uchar val) {
    uchar high = val >> 4;     // 取高 4 位（十六进制高位）
    uchar low = val & 0x0F;    // 取低 4 位（十六进制低位）

    // 显示高位
    DIG2 = 1;                  // 选择高位数码管
    P0 = segTable_CA_Dot[high]; // 直接使用共阳极编码（不再取反）
    DIG1 = 0;
    delay(5);                  // 延时保持显示

    // 显示低位
    DIG1 = 1;                  // 选择低位数码管
    P0 = segTable_CA_Dot[low];  // 直接使用共阳极编码
    DIG2 = 0;
    delay(5);                  // 延时保持显示
}

void main() {
    uchar value;

    EEPROM_Write(7, 0x58);     // 向地址7写入十六进制 58
    delay(10);                 // 写完等一下
    value = EEPROM_Read(7);    // 从地址7读取

    while (1) {
        displayHex(value);     // 显示读取值
    }
}
