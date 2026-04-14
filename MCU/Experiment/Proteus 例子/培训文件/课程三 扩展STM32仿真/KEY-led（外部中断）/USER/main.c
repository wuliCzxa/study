

#include "stm32f10x.h" 
#include "led.h"
#include "exti.h"

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{	
	/* config the led */
	LED_GPIO_Config();
	LED1( OFF );
	
	/* exti line config */
	EXTI_PB5_Config(); 
	
	/* wait interrupt */
	while(1)                            
	{
	}
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
