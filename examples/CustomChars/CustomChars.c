/*
 * CustomChars.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Hoang Ta
 */

#include <stdio.h>
#include "LiquidCrystal_I2C.h"

struct metal_i2c *lcd_i2c;

unsigned char bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
unsigned char note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
unsigned char clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
unsigned char heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
unsigned char duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
unsigned char check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
unsigned char cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
unsigned char retarrow[8] = {	0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};

extern void delay(uint32_t miliseconds);

// display all keycodes
void displayKeyCodes(void) {
	unsigned char i = 0;
	char str[16] = {0};

	while (1) {
		LiquidCrystal_I2C_clear();
		sprintf(str, "Codes 0x%02x-0x%02x", i, i + 16);
		LiquidCrystal_I2C_print(str);
		LiquidCrystal_I2C_setCursor(0, 1);

		for (int j = 0; j < 16; j++) {
			LiquidCrystal_I2C_write(i + j);
		}
		i += 16;

		delay(4000);
	}
}

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

	LiquidCrystal_I2C_createChar(0, bell);
	LiquidCrystal_I2C_createChar(1, note);
	LiquidCrystal_I2C_createChar(2, clock);
	LiquidCrystal_I2C_createChar(3, heart);
	LiquidCrystal_I2C_createChar(4, duck);
	LiquidCrystal_I2C_createChar(5, check);
	LiquidCrystal_I2C_createChar(6, cross);
	LiquidCrystal_I2C_createChar(7, retarrow);
	LiquidCrystal_I2C_home();

	LiquidCrystal_I2C_print("Hello world...");
	LiquidCrystal_I2C_setCursor(0, 1);
	LiquidCrystal_I2C_print(" i ");
	LiquidCrystal_I2C_write(3);
	LiquidCrystal_I2C_print(" SiFive!");
	delay(5000);
	displayKeyCodes();

	while (1)
	{
		// Do nothing here...
	}

	return 0;
}
