// Mode Macros
#define LCD_COMMAND 0
#define LCD_DATA    1

// Command Macros
#define LCD_CLEAR   0x01
#define LCD_HOME    0x02
#define LCD_ROW1    0x80
#define LCD_ROW2    0xC0

// Pin Macros
#define RS_PIN
#define E_PIN

void LCD_init(void); // initialize LCD
void LCD_command(uint8_t command); // send LCD a single 8-bit command
void LCD_write_char(uint8_t letter); // write a char to the LCD
void LCD_write(uint8_t value, uint8_t mode); // combination of LCD_command and LCD_write_char
