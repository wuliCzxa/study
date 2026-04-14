#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#define LED_CLK                          RCC_APB2Periph_GPIOC
#define LED_PORT                         GPIOC
#define LED_PIN                          GPIO_Pin_13
           
void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
#endif
