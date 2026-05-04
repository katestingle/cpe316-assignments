#include "lcd.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <stdint.h>

void LCD_init(void) { // initialize LCD
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN); // USign GPIOC for Data bus 8-bit transfer
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);  // Using GPIOA for Reg Select, R/W and Enable Signal Regs
	// set GPIO MODES
	GPIOA->MODER &= ~(3 << 8 | 3 << 2 | 3); // set RS, RW, E as output registers
	GPIOC->MODER = GPIO_MODER_MODE0_Pos; // Clear MODER register for D0-D7 (set = 0x0000)
	// SET GPIO VALS
	for(int i = 0; i < 8; i++){
		GPIOC->MODER |= (0b01 << (2 * i));
	}
	LCD_wakeup(); // LCD Wake Up Sequence
}

// requires GPIOA0-2 are E, RW, and RS and GPIOC0-7 are D0-D7 & preconfig as OUTPUT
void LCD_wakeup(void){
	// clear busy flag
	GPIOA->BSRR = E_PIN
	GPIOA->BRR = RS_PIN;
	GPIO->BSRR = RW_PIN;
	GPIOA->BRR = E_PIN;

	HAL_Delay(15);
	GPIOA->BSRR = E_PIN;
	LCD_function_set_wakeup();
	GPIOA->BRR = E_PIN;
	HAL_Delay(4.1); // wait > 4.1 ms
	GPIOA->BSRR = E_PIN;
	LCD_function_set_wakeup();
	GPIOA->BRR = E_PIN;
	HAL_Delay(1);  	// wait > 100 microsec
	// set rd, rw 0, data 0X30
	GPIOA->BSRR = E_PIN;
	LCD_function_set_wakeup();
	GPIOA->BRR = E_PIN;
	// function set Write 0x38 (8-bit mode, 2 lines, 5x8 font).
	GPIOA->BSRR = E_PIN;
	GPIOC->ODR = (FUNCTION_SET_SPEC); // set 2 lines, 8 bit interface, 5x11 text size
	GPIOC->ODR = (DISPLAY_OFF); 	// Display off
	GPIOC->ODR = (DISPLAY_CLEAR);	// Display clear
	GPIOC->ODR = (ENTRY_MODE_SET);	// entry mode set
	// DISPLAY ON, CURSOR ON
	GPIOA->BRR = (RW_PIN | RS_PIN); // clear RW and RS
	GPIOC->ODR = (DISPLAY_ON_CURSOR_ON);
	GPIOA->BSRR = E_PIN;
}

void LCD_function_set_wakeup(){
	// set rd, rw 0, data 0X30
	GPIOA->BRR = (RW_PIN | RS_PIN); // clear RW and RS
	GPIOC->ODR = (FUNCTION_SET_WAKE); // SET D0-D7 to 0x30
}

void LCD_command(uint8_t command){ // send LCD a single 8-bit command
	GPIOA->BSRR = E_PIN;
	// set pins
	if(command){
		GPIOC->BSRR = RS_PIN; // set RS PIN HIGH
	} else {
		GPIOC->BRR = RS_PIN; // set RS PIN LOW
	}
	// wait (HAL DElay)
	HAL_Delay(); // TO DO figure out how much delay for setting LCD_command
	GPIO->BRR = E_PIN;
	HAL_Delay(); // TO DO figure out how much delay for setting LCD_command
}

void LCD_write_char(uint8_t letter){ // write a char to the LCD

}

void LCD_write(uint8_t value, uint8_t mode){ // combination of LCD_command and LCD_write_char
	// 1. Set RS: 0 for Command, 1 for Data
	if (mode == LCD_COMMAND) {
	    GPIOC->BSRR = (1 << RS_PIN) << 16; // Reset RS
	} else {
	    GPIOC->BSRR = (1 << RS_PIN);       // Set RS
	}

	// 2. Put 8-bit data
	// Clear lower 8 bits of ODR and set to value
	GPIOC->ODR = (GPIOC->ODR & ~0xFF) | value;

	// 3. Toggle Enable (E) - The Falling Edge Latches Data
	GPIOC->BSRR = (1 << E_PIN);    // E High
	HAL_Delay(1);                  // Small delay for E pulse width
	GPIOC->BSRR = (1 << E_PIN) << 16; // E Low

	// 4. Execution delay (10x safety margin)
	HAL_Delay(2);
}
