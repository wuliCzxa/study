/**
  ******************************************************************************
  * @file    stm32f10x_exti.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the EXTI firmware functions.
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
#include "stm32f10x_exti.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup EXTI 
  * @brief EXTI driver modules
  * @{
  */

/** @defgroup EXTI_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Defines
  * @{
  */

#define EXTI_LINENONE    ((uint32_t)0x00000)  /* No interrupt selected */

/**
  * @}
  */

/** @defgroup EXTI_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void EXTI_DeInit(void)
{
  EXTI->IMR = 0x00000000;
  EXTI->EMR = 0x00000000;
  EXTI->RTSR = 0x00000000; 
  EXTI->FTSR = 0x00000000; 
  EXTI->PR = 0x000FFFFF;
}

/**
  * @brief  根据指定初始化EXTI外围设备
  *         EXTI_InitStruct中的参数。
  * @param  EXTI_InitStruct：指向EXTI_InitTypeDef结构的指针
  *         包含EXTI外设的配置信息。
  * @retval None
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;//EXTI起始地址
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* 清除EXTI配置 */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    tmp += EXTI_InitStruct->EXTI_Mode;//EXTI地址加上00（外部中断寄存器地址），04事件中断寄存器地址

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;//写中断屏蔽寄存器（EXTI_IMR ）或事件屏蔽寄存器（EXTI_EMR ）对应位（根据EXTI_Mode决定）

    /* 清除上升下降沿配置 */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* 选择所选外部中断的触发器 */
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)//电平改变（EXTI_Trigger_Rising_Falling）,RTSR(上升沿），FTSR(下降沿)对应位写1
    {
      /* 上升下降的边缘 */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;//
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {//只有一种模式-上升沿触发或者下降沿触发
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;//RTSR(上升沿）或者 FTSR(下降沿)对应位写1
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* 禁用选定的外部线路 */
   *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line; //单独禁用选定为位
  }
}

/**
  * @brief  用它的重置值填充每个EXTI_InitStruct成员。
  * @param  EXTI_InitStruct：指向将被初始化的EXTI_InitTypeDef结构的指针
  * @retval None
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LINENONE;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/**
  * @brief  产生一个软件中断。
  * @param  EXTI_Line：指定要启用或禁用的EXTI行。 该参数可以是EXTI_Linex的任意组合，其中x可以是（0..19）。
  * @retval None
  */
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/**
  * @brief  检查指定的EXTI行标志是否被设置。
  * @param  EXTI_Line：指定要检查的EXTI行标志。 这个参数可以是：
  *     @arg EXTI_Linex：外部中断线x其中x（0..19）
  * @retval TEXTI线的新状态（SET或RESET）。
  */
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  if ((EXTI->PR & EXTI_Line) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清除EXTI等待标志。
  * @param  EXTI_Line: 指定要清除的EXTI行标志。
  *   该参数可以是EXTI_Linex的任意组合，其中x可以是（0..19）。
  * @retval None
  */
void EXTI_ClearFlag(uint32_t EXTI_Line)
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;  //写1清除挂起寄存器（EXTI_PR ）
}

/**
  * @brief  检查指定的EXTI行是否被声明。
  * @param  EXTI_Line：指定要检查的EXTI行。
  *   这个参数可以是：
  *     @arg EXTI_Linex：外部中断线x其中x（0..19）
  * @retval EXTI_Line的新状态（SET或RESET）。
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* Check the parameters */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;
  if (((EXTI->PR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))//外部中断开启后等待中断位为1说明中断产生
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清除EXTI的行挂起位。
  * @param  EXTI_Line：指定要清除的EXTI行。
  *   该参数可以是EXTI_Linex的任意组合，其中x可以是（0..19）。
  * @retval None
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
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
