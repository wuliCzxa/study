
#line 1 "..\main.c" /0
  
#line 1 "..\..\..\..\..\..\..\..\KEIL\C51\INC\ATMEL\REG51.H" /0






 
 
 
 
 
 
 sfr P0   = 0x80;
 sfr P1   = 0x90;
 sfr P2   = 0xA0;
 sfr P3   = 0xB0;
 sfr PSW  = 0xD0;
 sfr ACC  = 0xE0;
 sfr B    = 0xF0;
 sfr SP   = 0x81;
 sfr DPL  = 0x82;
 sfr DPH  = 0x83;
 sfr PCON = 0x87;
 sfr TCON = 0x88;
 sfr TMOD = 0x89;
 sfr TL0  = 0x8A;
 sfr TL1  = 0x8B;
 sfr TH0  = 0x8C;
 sfr TH1  = 0x8D;
 sfr IE   = 0xA8;
 sfr IP   = 0xB8;
 sfr SCON = 0x98;
 sfr SBUF = 0x99;
 
 
 
 
 sbit CY   = 0xD7;
 sbit AC   = 0xD6;
 sbit F0   = 0xD5;
 sbit RS1  = 0xD4;
 sbit RS0  = 0xD3;
 sbit OV   = 0xD2;
 sbit P    = 0xD0;
 
 
 sbit TF1  = 0x8F;
 sbit TR1  = 0x8E;
 sbit TF0  = 0x8D;
 sbit TR0  = 0x8C;
 sbit IE1  = 0x8B;
 sbit IT1  = 0x8A;
 sbit IE0  = 0x89;
 sbit IT0  = 0x88;
 
 
 sbit EA   = 0xAF;
 sbit ES   = 0xAC;
 sbit ET1  = 0xAB;
 sbit EX1  = 0xAA;
 sbit ET0  = 0xA9;
 sbit EX0  = 0xA8;
 
 
 sbit PS   = 0xBC;
 sbit PT1  = 0xBB;
 sbit PX1  = 0xBA;
 sbit PT0  = 0xB9;
 sbit PX0  = 0xB8;
 
 
 sbit RD   = 0xB7;
 sbit WR   = 0xB6;
 sbit T1   = 0xB5;
 sbit T0   = 0xB4;
 sbit INT1 = 0xB3;
 sbit INT0 = 0xB2;
 sbit TXD  = 0xB1;
 sbit RXD  = 0xB0;
 
 
 sbit SM0  = 0x9F;
 sbit SM1  = 0x9E;
 sbit SM2  = 0x9D;
 sbit REN  = 0x9C;
 sbit TB8  = 0x9B;
 sbit RB8  = 0x9A;
 sbit TI   = 0x99;
 sbit RI   = 0x98;
 
 
#line 1 "..\main.c" /0
 
  
#line 1 "..\..\..\..\..\..\..\..\KEIL\C51\INC\INTRINS.H" /0






 
 
 
 
 
 extern void          _nop_     (void);
 extern bit           _testbit_ (bit);
 extern unsigned char _cror_    (unsigned char, unsigned char);
 extern unsigned int  _iror_    (unsigned int,  unsigned char);
 extern unsigned long _lror_    (unsigned long, unsigned char);
 extern unsigned char _crol_    (unsigned char, unsigned char);
 extern unsigned int  _irol_    (unsigned int,  unsigned char);
 extern unsigned long _lrol_    (unsigned long, unsigned char);
 extern unsigned char _chkfloat_(float);
 extern void          _push_    (unsigned char _sfr);
 extern void          _pop_     (unsigned char _sfr);
 
 
 
#line 2 "..\main.c" /0
 
 
 
 
 sbit KEY1=P1^0;
 sbit KEY2=P1^1;
 sbit KEY3=P1^2;
 sbit KEY4=P1^3;
 sbit SUOC=P1^4;
 
  unsigned char code tableYI[10] = 
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
  unsigned char code tableYA[10] = 
 {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98}; 
 
  unsigned char count=15;
  unsigned char count1=0;





 
 void Time0_Init()
 {
 TMOD = 0x01;	 
 TH0  = 0x3c;	 
 TL0  = 0xb0;
 ET0 = 1;
 EA = 1;		
 TR0 = 1;		
 }





 
 void Outside_Init0(void)
 {
 EX0 = 1;   
 
 IT0 = 1;   
 EA = 1;	   
 }
 
 void delayms(unsigned int j)		   
 {
  unsigned char i;
 for(;j>0;j--)
 {
 i=250;
 while(--i);
 i=249;
 while(--i);
 }
 }
 
 void main(void)
 { 
 SUOC=1;
 Time0_Init();
 Outside_Init0();
 PX0=1;
 while (1)
 {
 P0=tableYA[count/10];
 P2=0X01;
 delayms(3);
 P2=0X00;
 P0=tableYA[count%10];
 P2=0X02;
 delayms(3);
 P2=0X00;
 } 
 }
 
 void Outside_Int0() interrupt 0
 {
 EA=0;
 SUOC=0;
 _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
 SUOC=1;
 if(KEY1==0)
 count=1;
 else if(KEY2==0)
 count=2;
 else if(KEY3==0)
 count=3;
 else
 count=4;
 
 }
 
 void Time0_Int() interrupt 1
 {
 static unsigned char a=20;
 TH0  = 0x3c;	 
 TL0  = 0xb0;
 a--;
 if(a==0)
 {
 a=20;
 count--;
 if(count==0)
 {
 EA=0;	     
 }
 }
 }
 
