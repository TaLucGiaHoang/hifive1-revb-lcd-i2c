/*
 * BlinkingCursor.c
 *
 *  Created on: Jan 15, 2021
 *      Author: Hoang Ta
 */

#include <stdio.h>
#include "LiquidCrystal_I2C.h"

struct metal_i2c *lcd_i2c;

extern void delayMicroseconds(int microseconds);
extern void delay(uint32_t miliseconds);

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

	bool blinking = true;
	LiquidCrystal_I2C_cursor();

	while (1)
	{
		if (blinking) {
			LiquidCrystal_I2C_clear();
			LiquidCrystal_I2C_print("No cursor blink");
			LiquidCrystal_I2C_noBlink();
			blinking = false;
		} else {
			LiquidCrystal_I2C_clear();
			LiquidCrystal_I2C_print("Cursor blink");
			LiquidCrystal_I2C_blink();
			blinking = true;
		}

		delay(4000);
	}

	return 0;
}


