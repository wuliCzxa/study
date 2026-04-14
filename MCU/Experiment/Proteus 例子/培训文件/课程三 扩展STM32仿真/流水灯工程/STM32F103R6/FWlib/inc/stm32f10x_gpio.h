/**
  ******************************************************************************
  * @file    stm32f10x_gpio.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the GPIO 
  *          firmware library.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_GPIO_H
#define __STM32F10x_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/** @defgroup GPIO_Exported_Types
  * @{
  */

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))
                                     
/** 
  * @brief    输出最大频率选择
  */

typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/** 
  * @brief  配置模式枚举
  */

typedef enum //如下值主要由于在GPIO_Init函数中确定的
{ GPIO_Mode_AIN = 0x0,//模拟输入                输入/出位  模式有效位
  GPIO_Mode_IN_FLOATING = 0x04, //悬空输入 000    0          01         00
  GPIO_Mode_IPD = 0x28, //        下拉输入 001    0          10         00
  GPIO_Mode_IPU = 0x48, //        上拉输入 010    0          10         00
  GPIO_Mode_Out_OD = 0x14, //     开漏输出 000    1          01         00
  GPIO_Mode_Out_PP = 0x10, //     推挽输出 000    1          00         00
  GPIO_Mode_AF_OD = 0x1C,  //     开漏复用 000    1          11         00
  GPIO_Mode_AF_PP = 0x18 //       推挽复用 000    1          10         00
}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))

/** 
  * @brief  GPIO Init结构定义  
  */

typedef struct
{
  uint16_t GPIO_Pin;             /*!< 指定要配置的GPIO引脚。
									此参数可以是@ref GPIO_pins_define的任何值 */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< 指定选定引脚的速度。
                                       该参数可以是@ref GPIOSpeed_TypeDef的值（枚举变量有3个速度可选）*/

  GPIOMode_TypeDef GPIO_Mode;    /*!< 指定选定引脚的工作模式。
                                       该参数可以是@ref GPIOMode_TypeDef的值（枚举变量）*/
}GPIO_InitTypeDef;


/** 
  * @brief  Bit_SET and Bit_RESET enumeration  
  */

typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
  * @}
  */

/** @defgroup GPIO_Exported_Constants
  * @{
  */

/** @defgroup GPIO_pins_define 
  * @{
  */

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))

/**
  * @}
  */

/** @defgroup GPIO_Remap_define 
  * @{
  */

#define GPIO_Remap_SPI1             ((uint32_t)0x0000 0001)  /*!<  SPI1备用功能映射*/
#define GPIO_Remap_I2C1             ((uint32_t)0x0000 0002)  /*!<  I2C1备用功能映射*/
#define GPIO_Remap_USART1           ((uint32_t)0x0000 0004)  /*!<  USART1备用功能映射*/
#define GPIO_Remap_USART2           ((uint32_t)0x0000 0008)  /*!<  USART2替代功能映射*/
#define GPIO_PartialRemap_USART3    ((uint32_t)0x0014 0010)  /*!<  USART3部分替代功能映射*/
#define GPIO_FullRemap_USART3       ((uint32_t)0x0014 0030)  /*!<  USART3完全替代功能映射*/
#define GPIO_PartialRemap_TIM1      ((uint32_t)0x0016 0040)  /*!<  TIM1部分替代功能映射*/
#define GPIO_FullRemap_TIM1         ((uint32_t)0x0016 00C0)  /*!<  TIM1完整备用功能映射*/
#define GPIO_PartialRemap1_TIM2     ((uint32_t)0x0018 0100)  /*!<  TIM2部分1替代功能映射*/
#define GPIO_PartialRemap2_TIM2     ((uint32_t)0x0018 0200)  /*!< TIM2 Partial2备用功能映射 */
#define GPIO_FullRemap_TIM2         ((uint32_t)0x0018 0300)  /*!< TIM2完全替代功能映射 */
#define GPIO_PartialRemap_TIM3      ((uint32_t)0x001A 0800)  /*!< TIM3部分交替功能映射 */
#define GPIO_FullRemap_TIM3         ((uint32_t)0x001A 0C00)  /*!< TIM3完全替代功能映射 */
#define GPIO_Remap_TIM4             ((uint32_t)0x0000 1000)  /*!< TIM4备用功能映射 */
#define GPIO_Remap1_CAN1            ((uint32_t)0x001D 4000)  /*!< CAN1交替功能映射*/
#define GPIO_Remap2_CAN1            ((uint32_t)0x001D 6000)  /*!< CAN1交替功能映射 */
#define GPIO_Remap_PD01             ((uint32_t)0x0000 8000)  /*!< PD01备用功能映射*/
#define GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x0020 0001)  /*!< LSI连接到TIM5通道4输入捕捉进行校准 */
#define GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x0020 0002)  /*!< ADC1外部触发器注入转换重新映射 */
#define GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x0020 0004)  /*!< ADC1外部触发器定期转换重新映射 */
#define GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x0020 0008)  /*!< ADC2外部触发器注入转换重新映射 */
#define GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x0020 0010)  /*!< ADC2外部触发器定期转换重新映射 */
#define GPIO_Remap_ETH              ((uint32_t)0x0020 0020)  /*!< 以太网重新映射 (仅限CL设备) */
#define GPIO_Remap_CAN2             ((uint32_t)0x0020 0040)  /*!< CAN2重新映射 (仅限CL设备) */
#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x0030 0100)  /*!< 完全SWJ(JTAG-DP + SW-DP)但没有JNTRST */
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x0030 0200)  /*!< 关闭JTAG-DP，启用SW-DP */
#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x0030 0400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) 关闭JTAG-DP，关闭SW-DP*/
#define GPIO_Remap_SPI3             ((uint32_t)0x0020 1100)  /*!< SPI3 / I2S3交替功能映射 (仅限CL设备) */
#define GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x0020 2000)  /*!<以太网PTP输出或USB OTG SOF（起始帧）连接到TIM2内部触发器1进行校准
                                                                 (仅限CL设备) */
#define GPIO_Remap_PTP_PPS          ((uint32_t)0x0020 4000)  /*!< PB05上的以太网MAC PPS_PTS输出 (仅限CL设备) */

#define GPIO_Remap_TIM15            ((uint32_t)0x8000 0001)  /*!< TIM15备用功能映射（仅限Value Line设备） */
#define GPIO_Remap_TIM16            ((uint32_t)0x8000 0002)  /*!<TIM16备用功能映射（仅限Value Line设备） */
#define GPIO_Remap_TIM17            ((uint32_t)0x8000 0004)  /*!< TIM17备用功能映射 (仅限Value Line设备) */
#define GPIO_Remap_CEC              ((uint32_t)0x8000 0008)  /*!< CEC交替函数映射 (仅限Value Line设备s) */
#define GPIO_Remap_TIM1_DMA         ((uint32_t)0x8000 0010)  /*!< TIM1 DMA请求映射（仅限Value Line设备） */

#define GPIO_Remap_TIM9             ((uint32_t)0x8000 0020)  /*!< TIM9备用功能映射 (仅限XL设备) */
#define GPIO_Remap_TIM10            ((uint32_t)0x8000 0040)  /*!< TIM10替代功能映射 (仅限XL设备s) */
#define GPIO_Remap_TIM11            ((uint32_t)0x8000 0080)  /*!< TIM11替代功能映射 (仅限XL设备) */
#define GPIO_Remap_TIM13            ((uint32_t)0x8000 0100)  /*!< TIM13备用功能映射 (仅限HD-XL设备) */
#define GPIO_Remap_TIM14            ((uint32_t)0x8000 0200)  /*!< TIM14备用功能映射 (仅限HD-XL设备) */
#define GPIO_Remap_FSMC_NADV        ((uint32_t)0x8000 0400)  /*!< FSMC_NADV替代功能映射 (仅限HD-XL设备s) */

#define GPIO_Remap_TIM67_DAC_DMA    ((uint32_t)0x8000 0800)  /*!< TIM6 / TIM7和DAC DMA请求重新映射 (仅限HD-Value Line设备) */
#define GPIO_Remap_TIM12            ((uint32_t)0x8000 1000)  /*!< TIM12替代功能映射 (仅限HD-Value Line设备) */
#define GPIO_Remap_MISC             ((uint32_t)0x8000 2000)  /*!< 杂项重映射 (DMA2通道5位置和DAC触发重映射， 
                                                                 仅限HD-Value Line设备) */                                                       

#define IS_GPIO_REMAP(REMAP) (((REMAP) == GPIO_Remap_SPI1) || ((REMAP) == GPIO_Remap_I2C1) || \
                              ((REMAP) == GPIO_Remap_USART1) || ((REMAP) == GPIO_Remap_USART2) || \
                              ((REMAP) == GPIO_PartialRemap_USART3) || ((REMAP) == GPIO_FullRemap_USART3) || \
                              ((REMAP) == GPIO_PartialRemap_TIM1) || ((REMAP) == GPIO_FullRemap_TIM1) || \
                              ((REMAP) == GPIO_PartialRemap1_TIM2) || ((REMAP) == GPIO_PartialRemap2_TIM2) || \
                              ((REMAP) == GPIO_FullRemap_TIM2) || ((REMAP) == GPIO_PartialRemap_TIM3) || \
                              ((REMAP) == GPIO_FullRemap_TIM3) || ((REMAP) == GPIO_Remap_TIM4) || \
                              ((REMAP) == GPIO_Remap1_CAN1) || ((REMAP) == GPIO_Remap2_CAN1) || \
                              ((REMAP) == GPIO_Remap_PD01) || ((REMAP) == GPIO_Remap_TIM5CH4_LSI) || \
                              ((REMAP) == GPIO_Remap_ADC1_ETRGINJ) ||((REMAP) == GPIO_Remap_ADC1_ETRGREG) || \
                              ((REMAP) == GPIO_Remap_ADC2_ETRGINJ) ||((REMAP) == GPIO_Remap_ADC2_ETRGREG) || \
                              ((REMAP) == GPIO_Remap_ETH) ||((REMAP) == GPIO_Remap_CAN2) || \
                              ((REMAP) == GPIO_Remap_SWJ_NoJTRST) || ((REMAP) == GPIO_Remap_SWJ_JTAGDisable) || \
                              ((REMAP) == GPIO_Remap_SWJ_Disable)|| ((REMAP) == GPIO_Remap_SPI3) || \
                              ((REMAP) == GPIO_Remap_TIM2ITR1_PTP_SOF) || ((REMAP) == GPIO_Remap_PTP_PPS) || \
                              ((REMAP) == GPIO_Remap_TIM15) || ((REMAP) == GPIO_Remap_TIM16) || \
                              ((REMAP) == GPIO_Remap_TIM17) || ((REMAP) == GPIO_Remap_CEC) || \
                              ((REMAP) == GPIO_Remap_TIM1_DMA) || ((REMAP) == GPIO_Remap_TIM9) || \
                              ((REMAP) == GPIO_Remap_TIM10) || ((REMAP) == GPIO_Remap_TIM11) || \
                              ((REMAP) == GPIO_Remap_TIM13) || ((REMAP) == GPIO_Remap_TIM14) || \
                              ((REMAP) == GPIO_Remap_FSMC_NADV) || ((REMAP) == GPIO_Remap_TIM67_DAC_DMA) || \
                              ((REMAP) == GPIO_Remap_TIM12) || ((REMAP) == GPIO_Remap_MISC))
                              
/**
  * @}
  */ 

/** @defgroup GPIO_Port_Sources 
  * @{
  */

#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE       ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF       ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG       ((uint8_t)0x06)
#define IS_GPIO_EVENTOUT_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOE))

#define IS_GPIO_EXTI_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOE) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOF) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOG))

/**
  * @}
  */

/** @defgroup GPIO_Pin_sources 
  * @{
  */

#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))

/**
  * @}
  */

/** @defgroup Ethernet_Media_Interface 
  * @{
  */ 
#define GPIO_ETH_MediaInterface_MII    ((u32)0x00000000) 
#define GPIO_ETH_MediaInterface_RMII   ((u32)0x00000001)                                       

#define IS_GPIO_ETH_MEDIA_INTERFACE(INTERFACE) (((INTERFACE) == GPIO_ETH_MediaInterface_MII) || \
                                                ((INTERFACE) == GPIO_ETH_MediaInterface_RMII))

/**
  * @}
  */                                                
/**
  * @}
  */

/** @defgroup GPIO_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions
  * @{
  */

void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_AFIODeInit(void);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_EventOutputCmd(FunctionalState NewState);
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_GPIO_H */
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
