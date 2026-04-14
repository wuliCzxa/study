#include "LED.h"

void LED_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void LED_On(void) {
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
void LED_Off(void) {
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
void LED_Toggle(void) {
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    } else {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}
//void LED_Init(void) {
//    RCC->APB2ENR |= LED_CLK; // 1<< 4
//    LED_PORT->CRH &= 0XFF0FFFFF;
//    LED_PORT->CRH |= 0X00700000;
//}
//void LED_On(void) {
//    LED_PORT->ODR |= LED_PIN;
//    //LED_PORT->BSRR = LED_PIN;
//}
//void LED_Off(void) {
//    LED_PORT->ODR &= ~LED_PIN;
//    //LED_PORT->BRR = LED_PIN;
//}
//void LED_Toggle(void) {
//    if(LED_PORT->ODR & LED_PIN) {
//        //LED_PORT->ODR &= ~LED_PIN;
//        //LED_PORT->BSRR = LED_PIN << 16;
//        LED_PORT->BRR = LED_PIN;
//    } else {
//        //LED_PORT->ODR |= LED_PIN;
//        LED_PORT->BSRR = LED_PIN;
//    }
//}
