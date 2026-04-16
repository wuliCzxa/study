#include<reg52.h>

/*
    16x16 点阵"好"字滚动显示

    按键分配（接地触发，内部上拉）：
        S1  P3.1  启/停
        S2  P3.2  左右切换（按一下换方向，同时切到水平模式）
        S3  P3.3  加速
        S4  P3.4  减速
        S5  P3.6  上下切换（按一下换方向，同时切到垂直模式）

    注意：P3.5 是 T1 的外部时钟脚
*/


/* ——————————— 74HC595 三根控制线 ——————————— */
sbit shcp = P1^2;   // 移位时钟，每个上升沿吃进一位
sbit stcp = P1^1;   // 锁存时钟，上升沿才把移位寄存器的值推到输出端
sbit ds   = P1^0;   // 串行数据，要移哪位就在这里给高低电平


/* ——————————— 五个按键 ——————————— */
sbit s1 = P3^1;
sbit s2 = P3^2;
sbit s3 = P3^3;
sbit s4 = P3^4;
sbit s5 = P3^6;     // 不是 P3^5


/* ——————————— 全局变量 ——————————— */
unsigned char fx;           // 左右方向标志：0 向左，1 向右
unsigned char fy;           // 上下方向标志：0 向上，1 向下
unsigned char scroll_mode;  // 当前模式：0 水平，1 垂直
unsigned char alt;          // 定时器里的计数器，凑够 speed 次才走一格
unsigned int  net;          // 水平滚动时字模表的读取起点
unsigned char vrow;         // 垂直滚动时"虚拟画布"的观察位置，0~47
unsigned char speed = 5;    // 速度档，1 最快 12 最慢，数值越小越快
unsigned char zi    = 1;    // 显示几个字（不算前后的空白帧）
unsigned int  zong;         // 水平方向一共要走多少字节，main 里算好


/*
    字模表 tab（向左滚用）
    结构：32 字节全 0（开场空白）+ 32 字节"好"的点阵 + 32 字节全 0（收场空白）
    每两个字节是一列的 16 行 LED 状态，bit15 对应最顶行，bit0 对应最底行
    net 从 0 往上走，每走 2 字节就相当于画面往左平移一列

    字模是从取模软件直接抄过来的，顺序别动
*/
unsigned char code tab[]={
    // 开头留一个字符宽度的空白，让字能平滑滚进来
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // "好" 字点阵，16 列 × 每列 2 字节
    0x08,0x02, 0x08,0x44, 0x0F,0xA8, 0xF8,0x10,
    0x08,0x68, 0x0F,0x86, 0x00,0x00, 0x01,0x00,
    0x41,0x02, 0x41,0x01, 0x47,0xFE, 0x49,0x00,
    0x51,0x00, 0x61,0x00, 0x01,0x00, 0x00,0x00, /*"好"*/

    // 结尾同样留空白，不然字会突然消失，不好看
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*
    字模表 tab1（向右滚用）
    只有一个字，正序和逆序点阵一样，
    向右的效果靠 ISR 里把 net 倒着减来实现，表本身不用改
*/
unsigned char code tab1[]={
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    0x08,0x02, 0x08,0x44, 0x0F,0xA8, 0xF8,0x10,
    0x08,0x68, 0x0F,0x86, 0x00,0x00, 0x01,0x00,
    0x41,0x02, 0x41,0x01, 0x47,0xFE, 0x49,0x00,
    0x51,0x00, 0x61,0x00, 0x01,0x00, 0x00,0x00, /*"好"*/

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


/*
    软件延时，z 大概对应 z 毫秒（11.0592MHz 晶振下）
    精度无所谓，主要给按键消抖和扫描间隔用
*/
void delay(unsigned int z)
{
    unsigned char x;
    for(; z > 0; z--)
        for(x = 110; x > 0; x--);
}

/*
    往 74HC595 送 16 位数据（两片级联）
    dat1 先送，dat2 后送，每次移位时钟给个上升沿
    全部移完再给锁存时钟，数据才会出现在输出引脚上

    注意：shcp 置 1 后要立刻清 0，不能只拉高不拉低，
    595 靠的是"上升沿"触发，高电平持续没用
*/

void writedata(unsigned char dat1, unsigned char dat2)
{
    unsigned char i;
    for(i = 0; i < 8; i++) { dat1 <<= 1; ds = CY; shcp = 1; shcp = 0; }
    for(i = 0; i < 8; i++) { dat2 <<= 1; ds = CY; shcp = 1; shcp = 0; }
    stcp = 0; stcp = 1; stcp = 0;  // 锁存：低→高→低，产生上升沿
}

/*
    垂直滚动的核心，这里是整个程序最难理解的地方，多看几遍

    背景：
        P2 选的是"列"，595 输出的是这一列里哪些"行"亮。
        所以垂直滚动不能像水平那样挪偏移量，
        得把每列本来的 16 位数据整体上移或下移若干位。

    模型：
        想象一张高 48 行的虚拟纸条：
            第  0~15 行：空白（字还没出现）
            第 16~31 行：真正的"好"字
            第 32~47 行：空白（字已经过去了）
        vrow 是一扇 16 行高的"观察窗口"从纸条顶端往下数的位置。
        vrow=0  → 窗口对着最上面的空白，屏幕全灭
        vrow=16 → 窗口正好对着字，完整显示
        vrow=32 → 窗口对着最下面的空白，屏幕又全灭

    计算：
        窗口右移（vrow 变大）→ 字相对往上跑 → 视觉上字向上滚动
        字在虚拟行 16~31，窗口顶端在 vrow：
        ① vrow < 16：窗口上半在空白，下半开始看到字的顶部
                      字只露出底部 vrow 行 → 数据右移 (16-vrow) 位
        ② vrow = 16：完整显示，直接用原始数据
        ③ vrow > 16：字的顶部已经移出窗口，只剩底部
                      数据左移 (vrow-16) 位，高位自动补 0 就是空白
*/

unsigned int compute_display(unsigned int char_val, unsigned char v)
{
    if(v == 0 || v > 31) return 0;   // 窗口完全在空白区，直接返回全灭

    if(v <= 16)
        return char_val >> (16 - v); // 字从底部往上爬进来
    else
        return char_val << (v - 16); // 字从顶部往上飘出去
}

void main()
{
    char i;
    int  aa;   // 水平扫描时跳过字模表用的小偏移，每列加 2，到 30 就归零
    unsigned int col_val, disp_val;

    // 定时器 0，16 位模式，12MHz 晶振，定时 10ms
    // 10ms 中断一次，攒够 speed 次再走一格，所以 speed 越小越快
    TMOD = 0x01;
    TH0  = (65536 - 10000) / 256;
    TL0  = (65536 - 10000) % 256;
    ET0  = 1;
    EA   = 1;
    TR0  = 1;

    zong = (unsigned int)(zi + 1) * 32;  // 1 个字 → 2 段 × 32 字节 = 64

    // 开机默认向左走，从头开始，垂直也从头
    scroll_mode = 0;
    fx = 0;  fy = 0;
    net = 0; vrow = 0;
    aa  = 0;

    while(1)
    {
        // S1：一键启停，用取反定时器运行位来实现
        if(s1 == 0)
        {
            delay(30);                          // 等按键稳定，去抖
            if(s1 == 0) { TR0 = ~TR0; while(s1 == 0); }
        }

        // S2：切换左右，同时把模式拉回水平
        // fx 用异或翻转，比 if-else 少写几行
        if(s2 == 0)
        {
            delay(30);
            if(s2 == 0)
            {
                scroll_mode = 0;
                fx ^= 1;
                aa  = 0;
                net = (fx == 0) ? 0 : zong;   // 向左从头，向右从尾
                while(s2 == 0);
            }
        }

        // S5：切换上下，同时把模式切到垂直
        // vrow 的起点也跟着方向走，不然切换时画面会闪
        if(s5 == 0)
        {
            delay(30);
            if(s5 == 0)
            {
                scroll_mode = 1;
                fy ^= 1;
                vrow = (fy == 0) ? 0 : 47;    // 向上从 0 开始，向下从 47 开始
                while(s5 == 0);
            }
        }

        // S3 加速 / S4 减速
        // speed 有上下限，别让它跑飞或者停死
        if(s3 == 0)
        {
            delay(3);
            if(s3 == 0) { if(speed > 1)  speed--; }
            while(!s3);
        }
        if(s4 == 0)
        {
            delay(3);
            if(s4 == 0) { if(speed < 12) speed++; }
            while(!s4);
        }

        // 主显示循环，P2 每次选一列，595 送这列的亮行数据
        // 每列点亮后要消隐（送全 0），不然切换到下一列时会有拖影
        if(scroll_mode == 0)
        {
            // 水平模式：aa 从 0 跳到 30，配合 net 的偏移一起定位字模
            for(i = 0; i < 16; i++)
            {
                P2 = i;
                if(fx == 0) writedata(tab [net + aa], tab [net + aa + 1]);
                else        writedata(tab1[net + aa], tab1[net + aa + 1]);
                delay(3);
                writedata(0, 0);   // 消隐，防残影
                aa += 2;
                if(aa > 30) aa = 0;
            }
        }
        else
        {
            // 垂直模式：不靠偏移，靠位移
            // 字符数据固定在 tab[32] 开始，按列取出来再交给 compute_display 算
            for(i = 0; i < 16; i++)
            {
                // 第 i 列的原始点阵数据，两字节拼成 16 位
                col_val  = (unsigned int)tab[32 + (unsigned char)(i * 2)]     << 8;
                col_val |= (unsigned int)tab[32 + (unsigned char)(i * 2) + 1];

                // 按当前 vrow 算出这列实际该送的数据
                disp_val = compute_display(col_val, vrow);

                P2 = i;
                writedata((unsigned char)(disp_val >> 8), (unsigned char)(disp_val));
                delay(3);
                writedata(0, 0);
            }
        }
    }
}

/*
    定时器 0 中断，每 10ms 进来一次
    不是每次都移动，要等 alt 攒够 speed 次才走一格，
    speed 越大等越久，看起来就越慢

    水平：net 每次动 2 字节（= 移动一列）
    垂直：vrow 每次动 1 行（= 移动一个像素行）
    两个方向的循环边界都是 zong 或 47，超出就绕回来
*/

void timer0() interrupt 1
{
    TH0 = (65536 - 10000) / 256;
    TL0 = (65536 - 10000) % 256;

    if(++alt < speed) return;   
    alt = 0;

    if(scroll_mode == 0)
    {
        if(fx == 0)
        {
            net += 2;
            if(net > zong) net = 0;
        }
        else
        {
            if(net < 2) net = zong;
            else        net -= 2;
        }
    }
    else
    {
        if(fy == 0)
        {
            vrow++;
            if(vrow > 47) vrow = 0;    // 走完一圈回起点，无缝循环
        }
        else
        {
            if(vrow == 0) vrow = 47;   // 同上，反向绕回
            else          vrow--;
        }
    }
}