#include "lcd.h"

void LCD_init(void) { // initialize LCD
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN); // USign GPIOC for Data bus 8-bit transfer
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);  // Using GPIOA for Reg Select, R/W and Enable Signal Regs
	// Wait > 15ms after Vcc rises to 4.5V
	HAL_Delay(15);
	// set rd, rw 0, data 0X30

	// configure Data GPIOS
	GPIOC->MODER &= ~(0xFFFFF); // Clear data bits
	// Set D0-D7 bits to 01 (Output)

	// configure RD, RW, E GPIOS
	// clear
	// set rd to output & 0 (command mode)
	// set rw to output & 0 (write mode)
	// set E to output & pulse(to latch)?
	// wait > 4.1 ms
	HAL_Delay(4.1);
	// set rd, rw 0, data 0X30
	// pulse E?
	// wait > 100 microsec
	HAL_Delay(0.001); // **TO DO can do dec? **
	// set rd, rw 0, data 0X30
	// function set Write 0x38 (8-bit mode, 2 lines, 5x8 font).
	// pulse E?
	// Display Control: Write 0x0C (Display ON, Cursor OFF) or 0x0F (Everything ON + Blinking).
	// pulse E?
	// Clear Display: Write 0x01.
	// pulse E?
}

void LCD_command(uint8_t command){ // send LCD a single 8-bit command
	// set E high
	// set pins
	// wait (HAL DElay)
	// set E low
	// wait
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
