/*
 * SerialDisplay.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Hoang Ta
 *
 *  LCD1602
 *  UART configured to 9600, 8, 0, 1
 */

#include <stdio.h>
#include "LiquidCrystal_I2C.h"
#include "metal/uart.h"

#define LCD_END_INDEX_1  16
#define LCD_END_INDEX_2  32

struct metal_i2c *lcd_i2c;
struct metal_uart *uart;
static int first = 1;

int main(void)
{
	int baud = 0;
	char str[16] = {0};
	int c, index = 0;

	lcd_i2c = metal_i2c_get_device(0);
	if (lcd_i2c == NULL) {
		printf("I2C not available \n");
		return -1;
	}

	// Set the LCD address to 0x27 for a 16 chars and 2 line display
	LiquidCrystal_I2C_init(lcd_i2c, 0x27, 16, 2, LCD_5x8DOTS);
	LiquidCrystal_I2C_begin(); 	// initialize the LCD

	// Turn on the blacklight and print a message.
	LiquidCrystal_I2C_backlight();

	// Initialize the serial port at a default speed of 115200 baud
	uart = metal_uart_get_device(0); // COM port
	if(uart == NULL) {
		printf("UART not available \n");
		return -1;
	}

	printf("Warning: set serial speed of terminal to 9600\r\n");
	metal_uart_init(uart, 9600);
	baud = metal_uart_get_baud_rate(uart);
	printf("baudrate %d\r\n", baud);
	LiquidCrystal_I2C_print("Baud rate:");
	LiquidCrystal_I2C_setCursor(0, 1); // set cursor to bottom left
	sprintf(str, "%d", baud);
	LiquidCrystal_I2C_print(str);

	while (1)
	{
		// If characters arrived over the serial port...
		metal_uart_getc(uart, &c);
		if (c != -1)
		{
			// Clear LCD screen before print first character
			if(first)
			{
				LiquidCrystal_I2C_clear();  // Clear the screen
				first = 0;  // clear flag
			}

			LiquidCrystal_I2C_write(c); // print character to LCD
			index++;
			if(index == LCD_END_INDEX_1)
			{
				LiquidCrystal_I2C_setCursor(0, 1); // set cursor to bottom left
			}
			else if(index == LCD_END_INDEX_2)
			{
				LiquidCrystal_I2C_setCursor(0, 0); // set cursor to top left
				index = 0;
			}
		}
	}

	return 0;
}
