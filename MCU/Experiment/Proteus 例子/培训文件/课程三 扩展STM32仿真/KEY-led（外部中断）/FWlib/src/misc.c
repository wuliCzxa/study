/**
  ******************************************************************************
  * @file    misc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the miscellaneous firmware functions (add-on
  *          to CMSIS functions).
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "misc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup MISC 
  * @brief MISC driver modules
  * @{
  */

/** @defgroup MISC_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MISC_Private_Defines
  * @{
  */

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
/**
  * @}
  */

/** @defgroup MISC_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_Functions
  * @{
  */

/**
  * @brief  配置优先级分组：先占优先级和子优先级。
  * @param  NVIC_PriorityGroup：指定优先级分组比特长度。 
  *   该参数可以是以下值之一：
  * @arg NVIC_PriorityGroup_0：抢占优先权的0位
		* 4位子优先
  * @arg NVIC_PriorityGroup_1：抢占优先权1位
  * 3位子优先
  * @arg NVIC_PriorityGroup_2：2位用于占先优先
   * 2位子优先
  * @arg NVIC_PriorityGroup_3：抢占优先权的3位
  * 1位子优先
   * @arg NVIC_PriorityGroup_4：抢占优先权的4位
     子优先级为0位
  * @retval None
  */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/**
  * @brief  根据NVIC_InitStruct中指定的参数初始化NVIC外围设备。
  * @param  NVIC_InitStruct：指向NVIC_InitTypeDef结构的指针，该结构包含指定NVIC外围设备的配置信息。
  * @retval None
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)//中断使能执行if内代码
  {
    /* 计算相应的IRQ优先级 --------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;//中断分组 SCB_AIRC寄存器10-8位控制分组（5组，10——8位读数约大分组越前）最高--最低0,1,2,3,4
    tmppre = (0x4 - tmppriority);//                       4,   3,    2,   1，  0，倒tmppriority顺序
    tmpsub = tmpsub >> tmppriority;//tmpsub结果为这五个数1111,0111,0011,0001,0000
		//在中断分组中的优先级确定由NVIC_IPR 0-14 (中断优先级控制寄存器组确定
    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;//抢占优先级写入
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;//响应优先级写入
    tmppriority = tmppriority << 0x04 ;//NVIC_IPR只用高4位
        
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority; //NVIC_IPR 0-14 (中断优先级控制寄存器组)；中断优先级写入
    
    /* 启用选定的IRQ频道 --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] = //右移5位选择ISER（0或者1）//中断使用60个中断
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);//将NVIC->ISER中断使能寄存器对应位写1使能
  }
  else
  {
    /* 禁用选定的IRQ频道 -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);//NVIC->ICER中断清除寄存器写1关闭中断
  }
}

/**
  * @brief  Sets the vector table location and Offset.
  * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
  *   This parameter can be one of the following values:
  *     @arg NVIC_VectTab_RAM
  *     @arg NVIC_VectTab_FLASH
  * @param  Offset: Vector Table base offset field. This value must be a multiple 
  *         of 0x200.
  * @retval None
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* Check the parameters */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/**
  * @brief  Selects the condition for the system to enter low power mode.
  * @param  LowPowerMode: Specifies the new mode for the system to enter low power mode.
  *   This parameter can be one of the following values:
  *     @arg NVIC_LP_SEVONPEND
  *     @arg NVIC_LP_SLEEPDEEP
  *     @arg NVIC_LP_SLEEPONEXIT
  * @param  NewState: new state of LP condition. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_NVIC_LP(LowPowerMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
}

/**
  * @brief  Configures the SysTick clock source.
  * @param  SysTick_CLKSource: specifies the SysTick clock source.
  *   This parameter can be one of the following values:
  *     @arg SysTick_CLKSource_HCLK_Div8: AHB clock divided by 8 selected as SysTick clock source.
  *     @arg SysTick_CLKSource_HCLK: AHB clock selected as SysTick clock source.
  * @retval None
  */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
