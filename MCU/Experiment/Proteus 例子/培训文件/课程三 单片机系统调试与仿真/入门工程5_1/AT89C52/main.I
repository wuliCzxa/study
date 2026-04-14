
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
 
  
#line 1 "..\..\..\..\..\..\..\..\KEIL\C51\INC\STDIO.H" /0






 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 #pragma SAVE
 #pragma REGPARMS
 extern char _getkey (void);
 extern char getchar (void);
 extern char ungetchar (char);
 extern char putchar (char);
 extern int printf   (const char *, ...);
 extern int sprintf  (char *, const char *, ...);
 extern int vprintf  (const char *, char *);
 extern int vsprintf (char *, const char *, char *);
 extern char *gets (char *, int n);
 extern int scanf (const char *, ...);
 extern int sscanf (char *, const char *, ...);
 extern int puts (const char *);
 
 #pragma RESTORE
 
 
 
#line 2 "..\main.c" /0
 
 
 
 
 
 
  unsigned char code tableYI[10] = 
 {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
  unsigned char code tableYA[10] = 
 {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x83,0xf8,0x80,0x98}; 
  unsigned char code buffer1[] =
 {"Welcome to Proteus software"};	 
  unsigned char code buffer2[] =
 {0,1,2,3,4,5,6,7,8,9};		 
  unsigned char buffer3;
 
 
 void Com_Init(void)
 {
 TMOD = 0x20;	 
 TH1 = 0xFd;
 TR1 = 1;
 SCON = 0x50;	 
 SBUF=0;
 ES = 1;		 
 EA = 1;		 
 }
 
 void Com_SendBye(unsigned char dat)
 {
 SBUF=dat;
 while(!TI);	 
 TI=0;		 
 }
 
 void Com_SendStr(unsigned char *dat)
 {	
 while(1)
 {
 if(*dat == '\0')	 
 break;
 else 
 {
 SBUF=*dat;
 while(!TI);        
 TI = 0;		 
 dat++;
 }
 }
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
  unsigned char i=0;
 Com_Init();
 
 
 Com_SendStr(buffer1);
 Com_SendBye(0x0d);	 
 for(;i<10;i++)
 {
 Com_SendBye(buffer2[i]+0x30);	 
 }
 
 
 while (1)
 {
  P0=tableYA[buffer3];
 }
 }
 
 
 void Com_Int(void) interrupt 4
 {
 
 if(RI == 1)   		    
 {
 buffer3 = SBUF - 0x30;  
 RI = 0;  
 }
 }
 
 
 
