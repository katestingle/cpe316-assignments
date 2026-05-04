/*
 * tim2.c
 *
 *  Created on: Apr 28, 2026
 *      Author: kates
 */

#ifndef SRC_TIM2_C_
#define SRC_TIM2_C_

#include "tim2.h"


void TIM2_Init(){
	  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable TIMER 2 clock
	  __enable_irq(); // global interrupt EN
	  NVIC->ISER[0] = (1<<(TIM2_IRQn & 0x1F)); // periph specific EN at NVIC level
	  TIM2->CR1 &= ~(TIM_CR1_DIR); // count up
	  TIM2->DIER |= TIM_DIER_CC1IE; // Capture/Compare 1 interrupt enable
	  TIM2->DIER |= TIM_DIER_UIE; // Update interrupt enable
	  TIM2->SR &= ~TIM_SR_CC1IF; // clear cc1 interrupt flag if set
	  TIM2->SR &= ~TIM_SR_UIF; // clear update interrupt flag if set
	  TIM2->PSC = 0; // PSC = 0 (max resolution)
	  TIM2->ARR = 799; // ARR = 799, so f_update = 5kHz
	  TIM2->CCR1 = 200; // CCR1 = 200, so duty cycle = 25%
	  //TIM2->CCMR1 &= ~(TIM_CCMR1_CC1S); /// cc1 channel configured as output
	  // set PWM mode
	  //TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M); // clear 4 bits for Output compare 1 mode
	  //TIM2->CCMR1 |= 6<<4; // OC1M PWM mode 1 0110 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1, else inactive.
	  //TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // preload register TIM2_CCPR1 enabled, loaded in active reg at each UIE
	  // TIM2->CR1 |= TIM_CR1_ARPE; // auto reload preload reg enable
	  // TIM2->CR1 &= ~(TIM_CR1_URS); // affirm UIF also is generated
	  // event generation
	  //TIM2->EGR |= 3; // update and cc1g interrupt generation enabled
	  //TIM2->CCER |= TIM_CCER_CC1E; // output enabled OC1
	  //TIM2->CNT = 0;
	  TIM2->CR1 |= TIM_CR1_CEN; // enable counter
}

// works
void GPIO_SYSCLCK_Init(){
	// Enable MCO, select MSI (4 MHz source)
	RCC->CFGR = ((RCC->CFGR & ~(RCC_CFGR_MCOSEL)) | (RCC_CFGR_MCOSEL_0));
	// Configure MCO output on PA8
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	GPIOA->MODER &= ~(GPIO_MODER_MODE8); // alternate function mode
	GPIOA->MODER |= (2 << GPIO_MODER_MODE8_Pos);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT8); // Push-pull output
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD8); // no resistor
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8); // high speed
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL8); // select MCO function
}

void GPIO_TIM2_Init(){
	// Configure MCO output on PA8
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	GPIOA->MODER &= ~(GPIO_MODER_MODE5);
	GPIOA->MODER |= (GPIO_MODER_MODE5_0); // output mode
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5); // Push-pull output
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); // no resistor
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5); // high speed is enough
}

// fix 1st culrpti
void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF; // Disable the ARR Interrupt Flag
		GPIOA->BSRR = (1 << 5);   // Turn on PA5
	}

	if(TIM2->SR & TIM_SR_CC1IF){
		TIM2->SR &= ~TIM_SR_CC1IF; // Disable the CC1 Interrupt Flag
		GPIOA->BRR = (1 << 5);   // Turn off PA5
	}

	// what it's supposed to be:
//	void TIM2_IRQHandler(void)
//	{
//	  if(TIM2->SR & TIM_SR_CC1IF) {
//	      GPIOA->BRR = GPIO_PIN_5;
//	      TIM2->SR &= ~(TIM_SR_CC1IF);
//	  } else {
//	      GPIOA->BSRR = GPIO_PIN_5;
//	      TIM2->SR &= ~(TIM_SR_UIF);
//	  }
//	}

}


#endif /* SRC_TIM2_C_ */
