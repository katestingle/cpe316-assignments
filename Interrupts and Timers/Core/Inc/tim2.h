
#ifndef SRC_TIM2_H_
#define SRC_TIM2_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"

void TIM2_Init();
void GPIO_SYSCLCK_Init();
void GPIO_TIM2_Init();
void TIM2_IRQHandler(void);

#endif
