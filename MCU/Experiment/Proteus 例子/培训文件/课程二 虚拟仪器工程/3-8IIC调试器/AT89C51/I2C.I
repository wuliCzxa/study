
#line 1 "..\I2C.C" /0
  
#line 1 "C:\KEIL\C51\INC\ATMEL\REG51.H" /0






 
 
 
 
 
 
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
 
 
#line 1 "..\I2C.C" /0
 
  
#line 1 "C:\KEIL\C51\INC\INTRINS.H" /0






 
 
 
 
 
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
 
 
 
#line 2 "..\I2C.C" /0
 
 
 
 
 sbit scl=P1^1;
 sbit sda=P1^0;
 sbit key1=P3^2;
 sbit key2=P3^3;
  unsigned char data mem[4]_at_ 0x55;	 
  unsigned char mem[4]={0x41,0x42,0x43,0x44}; 
  unsigned char data rec_mem[4] _at_ 0x60 ; 
 void start(void); 
 void stop(void);  
 void sack(void);  
 bit rack(void);	  
 void ackn(void);  
 void send_byte(unsigned char);  
  unsigned char rec_byte(void);   
 void write(void);  
 void read(void);   
 void delay3us(void);  
 
 void main(void)
 {
 EA=1;EX0=1;EX1=1;
 while(1);
 }
 
 void ext0()interrupt 0
 {
 write();
 }
 
 void ext1()interrupt 2
 {
 read();
 }
 
 void read(void)
 {
  unsigned char i;
 bit f;
 start();			 
 send_byte(0xa0);	 
 f=rack();			 
 if(!f){
 start();			 
 send_byte(0xa0);
 f=rack();
 send_byte(0x00); 
 f=rack();
 if(!f){
 start();		    
 send_byte(0xa1);		
 f=rack();
 if(!f){
 for(i=0;i<3;i++)
 {rec_mem[i]=rec_byte();
 sack();
 }
 rec_mem[3]=rec_byte();ackn();
 }
 }							
 }
 stop();P2=rec_mem[3];while(!key2);
 }
 
 void write(void)
 {
  unsigned char i;
 bit f;
 start();
 send_byte(0xa0);
 f=rack();
 if(!f){
 send_byte(0x00);
 f=rack();
 if(!f){
 for(i=0;i<4;i++)
 {send_byte(mem[i]);
 f=rack();
 if(f)break;
 }
 }
 }
 stop();P2=0xaa;while(!key1);
 }
 
 
 void start(void)
 {
 scl=1;
 sda=1;
 delay3us();
 sda=0;
 delay3us();
 scl=0;
 }
 
 void stop(void)
 {
 scl=0;
 sda=0;
 delay3us();
 scl=1;
 delay3us();
 sda=1;
 delay3us();
 }
 
 bit rack(void)
 {
 bit flag;
 scl=1;
 delay3us();
 flag=sda;
 scl=0;
 return(flag);
 }
 
 void sack(void)
 {
 sda=0;
 delay3us();
 scl=1;
 delay3us();
 scl=0;
 delay3us();
 sda=1;
 delay3us();
 }
 
 void ackn(void)
 {
 sda=1;
 delay3us();
 scl=1;
 delay3us();
 scl=0;
 delay3us();
 }
 
  unsigned char rec_byte(void)
 {
  unsigned char i,temp;
 for(i=0;i<8;i++)
 {
 temp<<=1;
 scl=1;
 delay3us();
 temp|=sda;
 scl=0;
 delay3us();
 }
 return(temp);
 }
 
 void send_byte(unsigned char temp)
 {
  unsigned char i;
 scl=0;
 for(i=0;i<8;i++)
 {
 sda=(bit)(temp&0x80);
 scl=1;
 delay3us();
 scl=0;
 temp<<=1;
 }
 sda=1;
 }
 
 void delay3us(void)
 {
 _nop_();_nop_();_nop_();
 }
