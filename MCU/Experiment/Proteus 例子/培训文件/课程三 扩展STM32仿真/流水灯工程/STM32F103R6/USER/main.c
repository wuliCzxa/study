
#include "stm32f10x.h"
#include "led.h"

//#define    MI_ERR    (-2)

void Delay(__IO u32 nCount);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{	
	unsigned int change=0x01;
	unsigned char i=0;
	/* LED 端口初始化 */
  	LED_GPIO_Config();


	while (1)
	{
		GPIOC->ODR =~change;
		change=0x01<<i;
		i++;
		i&=0x07;
		
		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		Delay(0x0FFFEF);
//		LED0( ON );			  // 亮
//		Delay(0x0FFFEF);
//		LED0( OFF );		  // 灭
//		
//		LED1( ON );
//		Delay(0x0FFFEF);
//		LED1( OFF );
//		
//		LED2( ON );
//		Delay(0x0FFFEF);
//		LED2( OFF );      

//		LED3( ON );
//		Delay(0x0FFFEF);
//		LED3( OFF );      

//		LED4( ON );
//		Delay(0x0FFFEF);
//		LED4( OFF );      

//		LED5( ON );
//		Delay(0x0FFFEF);
//		LED5( OFF );      

//		LED6( ON );
//		Delay(0x0FFFEF);
//		LED6( OFF );      

//		LED7( ON );
//		Delay(0x0FFFEF);
//		LED7( OFF );      
	}
}

void Delay(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
} 


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
