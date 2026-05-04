#include <stdint.h>

// Mode Macros
#define LCD_COMMAND 0
#define LCD_DATA    1

// Command Macros
#define LCD_CLEAR   0x01
#define LCD_HOME    0x02
#define LCD_ROW1    0x80
#define LCD_ROW2    0xC0

// Pin Macros
#define E_PIN       ((uint16_t)0x0001)
#define RW_PIN      ((uint16_t)0x0002)
#define RS_PIN      ((uint16_t)0x0010)

// initialization/wake up D0-D7 states
#define FUNCTION_SET_WAKE 	    0x30 // used for wake up
#define FUNCTION_SET_SPEC		0x3C // used for setting LCD specs
#define CLEAR_HOME				0x02
#define ENTRY_MODE_SET 			0x07 // increment and shift = 1
#define DISPLAY_OFF				0x08
#define DISPLAY_ON_CURSOR_ON	0x0F // turn on display and blinking cursor
#define DISPLAY_CLEAR   		0x01

void LCD_init(void); // initialize LCD
void LCD_command(uint8_t command); // send LCD a single 8-bit command
void LCD_write_char(uint8_t letter); // write a char to the LCD
void LCD_write(uint8_t value, uint8_t mode); // combination of LCD_command and LCD_write_char
