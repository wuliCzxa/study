#include <reg52.h>

#define uchar unsigned char
#define uint unsigned int

// 共阴数码管段码表（0-F）
uchar code smgduan_cc[] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71
};

// 共阳数码管段码表（0-F），共阴取反
uchar code smgduan_ca[] = {
    ~0x3F, ~0x06, ~0x5B, ~0x4F,
    ~0x66, ~0x6D, ~0x7D, ~0x07,
    ~0x7F, ~0x6F, ~0x77, ~0x7C,
    ~0x39, ~0x5E, ~0x79, ~0x71
};

// 延时函数
void delay_ms(uint ms) {
    uint i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 120; j++);
}

// 显示共阴数码管（低电平选通，P1.0 高位，P1.1 低位）
void display_cc(uchar hex) {
    uchar high = hex >> 4;
    uchar low  = hex & 0x0F;

    // 高位显示（P1.0 = 0）
    P1 |= 0x03;                // 清除低两位，关闭显示
    P0 = smgduan_cc[high];
    P1 &= ~0x01;               // P1.0 = 0 选通
    delay_ms(2);
    P1 |= 0x01;                // P1.0 = 1 关闭

    // 低位显示（P1.1 = 0）
    P1 |= 0x03;
    P0 = smgduan_cc[low];
    P1 &= ~0x02;               // P1.1 = 0 选通
    delay_ms(2);
    P1 |= 0x02;                // P1.1 = 1 关闭
}

// 显示共阳数码管（高电平选通，P1.2 高位，P1.3 低位）
void display_ca(uchar hex) {
    uchar high = hex >> 4;
    uchar low  = hex & 0x0F;

    // 高位显示（P1.2 = 1）
    P1 &= 0xF3;                // 清除 P1.2、P1.3
    P0 = smgduan_ca[high];
    P1 |= 0x04;                // P1.2 = 1 选通
    delay_ms(2);
    P1 &= ~0x04;               // P1.2 = 0 关闭

    // 低位显示（P1.3 = 1）
    P1 &= 0xF3;
    P0 = smgduan_ca[low];
    P1 |= 0x08;                // P1.3 = 1 选通
    delay_ms(2);
    P1 &= ~0x08;               // P1.3 = 0 关闭
}

// 主函数：共阳递增，共阴递减
void main() {
    uchar num_ca = 0x00;   // 共阳：从 00 递增
    uchar num_cc = 0xFF;   // 共阴：从 FF 递减
    uint i;

    while(1) {
        for(i = 0; i < 100; i++) {
            display_ca(num_ca);  // 显示共阳数码管
            display_cc(num_cc);  // 显示共阴数码管
        }

        num_ca++;  // 共阳递增
        num_cc--;  // 共阴递减
    }
}
