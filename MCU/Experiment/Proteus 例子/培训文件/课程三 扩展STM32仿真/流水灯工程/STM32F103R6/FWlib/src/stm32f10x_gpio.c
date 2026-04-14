/**
  ******************************************************************************
  * @file    stm32f10x_gpio.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file provides all the GPIO firmware functions.
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
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup GPIO 
  * @brief GPIO driver modules
  * @{
  */ 

/** @defgroup GPIO_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Defines
  * @{
  */

/* ------------ RCC registers bit address in the alias region ----------------*/
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR Register -----*/

/* EVOE位的别名字地址 */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))


/* ---  MAPR Register ---*/ 
/* Alias word address of MII_RMII_SEL bit */ 
#define MAPR_OFFSET                 (AFIO_OFFSET + 0x04) 
#define MII_RMII_SEL_BitNumber      ((u8)0x17) 
#define MAPR_MII_RMII_SEL_BB        (PERIPH_BB_BASE + (MAPR_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))


#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)
/**
  * @}
  */

/** @defgroup GPIO_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Functions
  * @{
  */

/**
  * @brief  取消初始化GPIOx外设寄存器为其默认复位值。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
  }
  else
  {
    if (GPIOx == GPIOG)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
    }
  }
}

/**
  * @brief  取消初始化备用功能（重新映射，事件控制和EXTI配置）注册到它们的默认重置值。
  *   
  * @param  None
  * @retval None
  */
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/**
  * @brief  根据指定初始化GPIOx外设GPIO_InitStruct中的参数。
  *     
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_InitStruct：指向一个GPIO_InitTypeDef结构的指针包含指定的GPIO外设的配置信息。
  *         
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /*检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*----------------------------GPIO模式配置  -----------------------*/   
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);  //取GPIO_Mode低四位
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)  //比较GPIO_Mode第五位值为1时初始化为输出。（如果不为1，则是输入模式）
  { 
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* 输出模式 */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;      //或上01,10,11。设置输出最大速度
  }
/*---------------------------- GPIO CRL配置 ------------------------*/
  /* 配置八个低端口引脚 */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)//判断低八位
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* 获取端口引脚位置 */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)//判断出引脚0-7
      {
        pos = pinpos << 2;//一个引脚配置需要4位所以右移2
        /*清除相应的低位控制寄存器位  */
        pinmask = ((uint32_t)0x0F) << pos;//锁定寄存器引脚控制位
        tmpreg &= ~pinmask;    //不改变其他引脚的CRL寄存器，只改变传输过来的引脚对应的CRL寄存器的4位值--0000
        /*将模式配置写入相应的位  */
        tmpreg |= (currentmode << pos);	//写入传输过来的引脚对应的CRL寄存器的4位值
        /* 重置相应的ODR位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)//输入下拉模式
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos); //端口复位寄存器（将pinpos位置0）
        }
        else
        {
          /* 设置相应的ODR位 */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)//输入上拉模式
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);//端口复位寄存器（将pinpos位置1）
          }
        }
      }
    }
    GPIOx->CRL = tmpreg; 
  }
/*---------------------------- GPIO CRH配置 ------------------------*/
  /* 配置八个高端口引脚 */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* 获取端口引脚位置 */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* 清除相应的高位控制寄存器位 */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /*  将模式配置写入相应的位*/
        tmpreg |= (currentmode << pos);
        /* 重置相应的ODR位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* 设置相应的ODR位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

/**
  * @brief  用默认值填充每个GPIO_InitStruct成员。
  * @param  GPIO_InitStruct：指向将被初始化的GPIO_InitTypeDef结构的指针。
  *         
  * @retval None
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)  //复位GPIO为初始化状态（浮空输入状态）
{
  /* 重置GPIO初始化结构参数值 */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/**
  * @brief  读取指定的输入端口引脚。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_Pin：指定要读取的端口位。 该参数可以是GPIO_Pin_x，其中x可以是（0..15）。
  * @retval 输入端口引脚值。
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //读出GPIOx_IDR的GPIO_Pin位的值(输入寄存器)
{
  uint8_t bitstatus = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)//GPIOx_IDR复位为0，判断GPIOx_IDR的GPIO_Pin位的值
  {
    bitstatus = (uint8_t)Bit_SET;	//为1
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;//为0
  }
  return bitstatus;//读出GPIOx_IDR的GPIO_Pin位的值
}

/**
  * @brief  读取指定的GPIO输入数据端口。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @retval GPIO输入数据端口值。
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx) //读出GPIOx_IDR所有位的值(输入寄存器)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  读取指定的输出数据端口位。
  * @param   GPIO_Pin：指定要读取的端口位。
  *此参数可以是GPIO_Pin_x，其中x可以是（0..15）。
  * @retval 输出端口引脚值。
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)//读出GPIOx_ODR的GPIO_Pin位的值(输出寄存器)
{
  uint8_t bitstatus = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @brief  读取指定的GPIO输出数据端口。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @retval GPIO输出数据端口值。
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)//读出GPIOx_ODR所有位的值(输出寄存器)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  设置选定的数据端口位。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_Pin：指定要写入的端口位。
  *此参数可以是GPIO_Pin_x的任意组合，其中x可以是（0..15）。
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //对GPIOx_ODR输出寄存器某一位操作（GPIOx_ODR只能字操作（16位））
//GPIOx_BSRR端口位设置复位寄存器（高16写1复位，低16写1置位；低优先有效） ；只用置位低16位。
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/**
  * @brief  清除选定的数据端口位。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_Pin：指定要写入的端口位。
  *此参数可以是GPIO_Pin_x的任意组合，其中x可以是（0..15）。
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)//端口位复位寄存器(GPIOx_BRR),写1复位GPIOx_ODR对应位
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/**
  * @brief  设置或清除选定的数据端口位。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_Pin：指定要写入的端口位。
  *   该参数可以是GPIO_Pin_x之一，其中x可以是（0..15）。
  * @param  BitVal：指定要写入选定位的值。
  *此参数可以是BitAction枚举值之一：
  *     @arg Bit_RESET：清除端口引脚
  *     @arg Bit_SET：设置端口引脚
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal) //GPIO_ODR的GPIO_Pin位  复位或置位
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/**
  * @brief  将数据写入指定的GPIO数据端口。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  PortVal：指定要写入端口输出数据寄存器的值。
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal) //GPIO_ODR输出寄存器直接字（16位）操作
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
  * @brief  锁定GPIO引脚配置寄存器。
  * @param  GPIOx：其中x可以是（A..G）来选择GPIO外设。
  * @param  GPIO_Pin：指定要写入的端口位。
  *此参数可以是GPIO_Pin_x的任意组合，其中x可以是（0..15）。
  * @retval None
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)//端口配置锁定寄存器(GPIOx_LCKR)写1为锁定引脚寄存器（CRL,CRH）配置
{
  uint32_t tmp = 0x00010000;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
 //(第17位)锁键的写入序列：写1 -> 写0 -> 写1 -> 读0 -> 读1 
  tmp |= GPIO_Pin;
  /* 设置LCKK位 */
  GPIOx->LCKR = tmp;
  /* 重置LCKK位 */
  GPIOx->LCKR =  GPIO_Pin;
  /* 设置LCKK位 */
  GPIOx->LCKR = tmp;
  /* 读取LCKK位*/
  tmp = GPIOx->LCKR;
  /*读取LCKK位 */
  tmp = GPIOx->LCKR;
}

/**
  * @brief  选择用作事件输出的GPIO引脚。
  * @param  GPIO_PortSource：选择要用作源的GPIO端口
  *用于事件输出。
  *   此参数可以是GPIO_PortSourceGPIOx，其中x可以是（A..E）。
  * @param  GPIO_PinSource：指定事件输出的引脚。
  *此参数可以是GPIO_PinSourcex，其中x可以是（0..15）。
  * @retval None
  */
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)//
{
  uint32_t tmpreg = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* 清除PORT [6：4]和PIN [3：0]位 */
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (uint32_t)GPIO_PortSource << 0x04;
	/*PORT[2:0]：端口选择
选择用于输出Cortex的EVENTOUT信号的端口：
000：选择PA  001：选择PB  010：选择PC  011：选择PD
100：选择PE */
  tmpreg |= GPIO_PinSource; //PIN[3:0]：管脚选择
	/*0000：选择Px0  0001：选择Px1  0010：选择Px2  0011：选择Px3
0100：选择Px4  0101：选择Px5  0110：选择Px6  0111：选择Px7
1000：选择Px8  1001：选择Px9  1010：选择Px10 1011：选择Px11
1100：选择Px12 1101：选择Px13 1110：选择Px14 1111：选择Px15*/
  AFIO->EVCR = tmpreg;
}

/**
  * @brief  启用或禁用事件输出。
  * @param  NewState：事件输出的新状态。
  *   该参数可以是：ENABLE或DISABLE。
  * @retval None
  *///事件控制寄存器(AFIO_EVCR)第7位EVOE位写开跟关控制 
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) EVCR_EVOE_BB = (uint32_t)NewState;
}

/**
	* @brief更改指定引脚的映射。
	* @param GPIO_Remap：选择引脚重新映射。
    *  该参数可以是以下值之一：
	* @arg GPIO_Remap_SPI1：SPI1备用功能映射
	* @arg GPIO_Remap_I2C1：I2C1备用功能映射
	* @arg GPIO_Remap_USART1：USART1备用功能映射
	* @arg GPIO_Remap_USART2：USART2备用功能映射
	* @arg GPIO_PartialRemap_USART3：USART3部分替代功能映射
	* @arg GPIO_FullRemap_USART3：USART3完全替代功能映射
	* @arg GPIO_PartialRemap_TIM1：TIM1部分替代函数映射
	* @arg GPIO_FullRemap_TIM1：TIM1完整替代功能映射
	* @arg GPIO_PartialRemap1_TIM2：TIM2 Partial1备用功能映射
	* @arg GPIO_PartialRemap2_TIM2：TIM2 Partial2备用功能映射
	* @arg GPIO_FullRemap_TIM2：TIM2完全替代功能映射
	* @arg GPIO_PartialRemap_TIM3：TIM3部分交替函数映射
	* @arg GPIO_FullRemap_TIM3：TIM3完全替代功能映射
	* @arg GPIO_Remap_TIM4：TIM4备用功能映射
	* @arg GPIO_Remap1_CAN1：CAN1备用功能映射
	* @arg GPIO_Remap2_CAN1：CAN1备用功能映射
	* @arg GPIO_Remap_PD01：PD01备用功能映射
	* @arg GPIO_Remap_TIM5CH4_LSI：LSI连接到TIM5通道4输入捕捉以进行校准
	* @arg GPIO_Remap_ADC1_ETRGINJ：ADC1外部触发器注入转换重新映射
	* @arg GPIO_Remap_ADC1_ETRGREG：ADC1外部触发器定期转换重新映射
	* @arg GPIO_Remap_ADC2_ETRGINJ：ADC2外部触发器注入转换重新映射
	* @arg GPIO_Remap_ADC2_ETRGREG：ADC2外部触发器定期转换重新映射
	* @arg GPIO_Remap_ETH：以太网重新映射（仅适用于连接线设备）
	* @arg GPIO_Remap_CAN2：CAN2重新映射（仅适用于连接线设备）
	* @arg GPIO_Remap_SWJ_NoJTRST：完整SWJ启用（JTAG-DP + SW-DP），但没有JTRST
	* @arg GPIO_Remap_SWJ_JTAGDisable：禁用JTAG-DP并启用SW-DP
	* @arg GPIO_Remap_SWJ_Disable：完全禁用SWJ（JTAG-DP + SW-DP）
    *     @arg GPIO_Remap_SPI3             : SPI3 / I2S3替代功能映射（仅适用于连接线设备）
    *                                        当使用此功能重新映射SPI3 / I2S3时，SWJ被配置为全SWJ使能（JTAG-DP + SW-DP），但没有JTRST。  
    *     @arg GPIO_Remap_TIM2ITR1_PTP_SOF : 以太网PTP输出或USB OTG SOF（起始帧）连接到TIM2内部触发器1用于校准（仅适用于连接线设备）
										   如果启用GPIO_Remap_TIM2ITR1_PTP_SOF，则TIM2 ITR1连接到以太网PTP输出。 
										   当复位TIM2 ITR1连接到USB OTG SOF输出时。   
	* @arg GPIO_Remap_PTP_PPS：PB05上的以太网MAC PPS_PTS输出（仅适用于连接线设备）
	* @arg GPIO_Remap_TIM15：TIM15备用功能映射（仅适用于值线设备）
	* @arg GPIO_Remap_TIM16：TIM16备用功能映射（仅适用于值线设备）
	* @arg GPIO_Remap_TIM17：TIM17替代功能映射（仅适用于值线设备）
	* @arg GPIO_Remap_CEC：CEC备用功能映射（仅适用于Value线路设备）
	* @arg GPIO_Remap_TIM1_DMA：TIM1 DMA请求映射（仅限值线设备）
	* @arg GPIO_Remap_TIM9：TIM9备用功能映射（仅用于XL密度设备）
	* @arg GPIO_Remap_TIM10：TIM10替代功能映射（仅适用于XL密度设备）
	* @arg GPIO_Remap_TIM11：TIM11备用功能映射（仅适用于XL密度设备）
	* @arg GPIO_Remap_TIM13：TIM13备用功能映射（仅适用于高密度值线和XL密度设备）
	* @arg GPIO_Remap_TIM14：TIM14备用功能映射（仅适用于高密度值线和XL密度设备）
	* @arg GPIO_Remap_FSMC_NADV：FSMC_NADV替代功能映射（仅适用于高密度值线和XL密度设备）
	* @arg GPIO_Remap_TIM67_DAC_DMA：重新映射TIM6 / TIM7和DAC DMA请求（仅适用于高密度值线路设备）
	* @arg GPIO_Remap_TIM12：TIM12替代功能映射（仅适用于高密度值线设备）
  *     @arg GPIO_Remap_MISC             : 其他重新映射（DMA2通道5位置和DAC触发重新映射，仅适用于高密度值线设备）     
  * @param  NewState:端口引脚重新映射的新状态。
  *   该参数可以是：ENABLE或DISABLE。
  * @retval None
  */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if((GPIO_Remap & 0x80000000) == 0x80000000)
  {
    tmpreg = AFIO->MAPR2;
  }
  else
  {
    tmpreg = AFIO->MAPR;
  }

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;  //保留GPIO_Remap的16-19位，
  tmp = GPIO_Remap & LSB_MASK; //  保留GPIO_Remap的0-15位
//以下if结构为初始化对应位为0.
  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
	  //GPIO_Remap &0x0030 0000     判断GPIO_Remap的20-21位值为3 则操作SWJ_CFG[2:0]：串行线JTAG配置
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;    	//	000：完全SWJ(JTAG-DP + SW-DP)：复位状态
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK; 
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
	  //判断GPIO_Remap的20位值为1
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;//第16-19位的值分别为4，6，8，10，13（）
    tmpreg &= ~tmp1;   //将  复用重映射和调试I/O配置寄存器(AFIO_MAPR)特定位 复位为0 
    tmpreg |= ~DBGAFR_SWJCFG_MASK;//保证SWJ_CFG[2:0]：串行线JTAG配置不被更改
  }
  else
	   //判断GPIO_Remap的20-21位值为其他值（剩下的第21位为0和1）
  {
	  //第21位值为0--  tmpreg &= ~tmp  ；为1-- tmpreg &= ~tmp<<16
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10)); //复位特定位置为0
	  
    tmpreg |= ~DBGAFR_SWJCFG_MASK;//保证SWJ_CFG[2:0]：串行线JTAG配置不被更改
  }
//开使能，对应位写1
  if (NewState != DISABLE)
  {
	 //GPIO_Remap的第21位为0，GPIO_Remap的0-15位写在AFIO_MAPR的0-15位，为1写在16-31位
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  if((GPIO_Remap & 0x80000000) == 0x80000000) //开使能，对应位写1 
  {
    AFIO->MAPR2 = tmpreg;
  }
  else
  {
    AFIO->MAPR = tmpreg;
  }  
}

/**
  * @brief  选择用作EXTI线路的GPIO引脚.
  * @param  GPIO_PortSource：选择要用作EXTI线路源的GPIO端口。
  *   此参数可以是GPIO_PortSourceGPIOx，其中x可以是（A..G）.
  * @param  GPIO_PinSource：指定要配置的EXTI线路。
  *   该参数可以是GPIO_PinSourcex，其中x可以是（0..15）。
  * @retval None
  */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));//0x0f左移0,4,5,12
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;//右移2位表示一个寄存器控制4个口。这句是将GPIO_PinSourcex复位0000
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));//引脚写入AFIO_EXTICR特定的寄存器
}

/**
  * @brief  选择以太网媒体接口。
  * @note   该功能仅适用于STM32的CL设备。  
  * @param GPIO_ETH_MediaInterface：指定媒体接口模式。
  *   该参数可以是以下值之一：
  *     @arg GPIO_ETH_MediaInterface_MII: MII mode
  *     @arg GPIO_ETH_MediaInterface_RMII: RMII mode    
  * @retval None
  */
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface) 
{ 
  assert_param(IS_GPIO_ETH_MEDIA_INTERFACE(GPIO_ETH_MediaInterface)); 

  /* Configure MII_RMII selection bit */ 
  *(__IO uint32_t *) MAPR_MII_RMII_SEL_BB = GPIO_ETH_MediaInterface; 
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
