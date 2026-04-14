
#include "exti.h"

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置嵌套向量中断控制器NVIC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 中断分组设置，选用中断分组1,1位抢占，3位响应 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断分组设置0，1,2,3,4组（配置抢占位和响应位）
  
  /* 配置P[A|B|C|D|E]的5-9为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;  //          
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级由分组确定最大值
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级--由分组确定最大值
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);//中断优先级设置
}

/*
 * 函数名：EXTI_PB5_Config
 * 描述  ：配置 PB5 为线中断口，并设置中断优先级
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void EXTI_PB5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 配置extiline（PB5）时钟和AFIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
												
	/* 配置NVIC分两步--第一步中断分组；第二步中断优先级设置*/
	NVIC_Configuration();

	/* EXTI行gpio配置 */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*EXTI模式配置 */
	//选通对应的外部中断口与引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); //GPIO的部中断配置寄存器 x(AFIO_EXTICRx)对应位置写入值
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line4; //外部中断位（可选0-19）
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//中断模式（外部或者事件中断两者选一）
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断（三种模式选，在stm32f10x_exti.h中定义了上升，下降，变化三种模式的相对地址）
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能位
  EXTI_Init(&EXTI_InitStructure); //外部中断初始化
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

