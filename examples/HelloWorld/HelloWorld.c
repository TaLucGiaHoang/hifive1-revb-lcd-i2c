/*
 * HelloWorld.c
 *
 *  Created on: Jan 15, 2021
 *      Author: Hoang Ta
 */

#include <stdio.h>
#include "LiquidCrystal_I2C.h"

struct metal_i2c *lcd_i2c;

int main(void)
{
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
	LiquidCrystal_I2C_print("Hello, world!");

	while (1)
	{
		// Do nothing here...
	}

	return 0;
}

