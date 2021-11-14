/*
 * LiquidCrystal_I2C.h
 *
 *  Created on: Jan 15, 2021
 *      Author: Hoang Ta
 */
#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

#include <stdbool.h> // bool type
#include <stdlib.h> // size_t type
#include <metal/i2c.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0B00000100  // Enable bit
#define Rw 0B00000010  // Read/Write bit
#define Rs 0B00000001  // Register select bit

/**
 * This is the driver for the Liquid Crystal LCD displays that use the I2C bus.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */

/**
 * Constructor
 *
 * @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
 *					LCD circuit board, or look in the supplied LCD documentation.
 * @param lcd_cols	Number of columns your LCD display has.
 * @param lcd_rows	Number of rows your LCD display has.
 * @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
 */
void LiquidCrystal_I2C_init(struct metal_i2c *i2c, unsigned char lcd_addr, unsigned char lcd_cols, unsigned char lcd_rows, unsigned char charsize);// /*= LCD_5x8DOTS*/);

/**
 * Set the LCD display in the correct begin state, must be called before anything else is done.
 */
void LiquidCrystal_I2C_begin();

 /**
  * Remove all the characters currently shown. Next print/write operation will start
  * from the first position on LCD display.
  */
void LiquidCrystal_I2C_clear();

/**
 * Next print/write operation will will start from the first position on the LCD display.
 */
void LiquidCrystal_I2C_home();

 /**
  * Do not show any characters on the LCD display. Backlight state will remain unchanged.
  * Also all characters written on the display will return, when the display in enabled again.
  */
void LiquidCrystal_I2C_noDisplay();

/**
 * Show the characters on the LCD display, this is the normal behaviour. This method should
 * only be used after noDisplay() has been used.
 */
void LiquidCrystal_I2C_display();

/**
 * Do not blink the cursor indicator.
 */
void LiquidCrystal_I2C_noBlink();

/**
 * Start blinking the cursor indicator.
 */
void LiquidCrystal_I2C_blink();

/**
 * Do not show a cursor indicator.
 */
void LiquidCrystal_I2C_noCursor();

/**
 * Show a cursor indicator, cursor can blink on not blink. Use the
 * methods blink() and noBlink() for changing cursor blink.
 */
void LiquidCrystal_I2C_cursor();

void LiquidCrystal_I2C_scrollDisplayLeft();
void LiquidCrystal_I2C_scrollDisplayRight();
void LiquidCrystal_I2C_printLeft();
void LiquidCrystal_I2C_printRight();
void LiquidCrystal_I2C_leftToRight();
void LiquidCrystal_I2C_rightToLeft();
void LiquidCrystal_I2C_shiftIncrement();
void LiquidCrystal_I2C_shiftDecrement();
void LiquidCrystal_I2C_noBacklight();
void LiquidCrystal_I2C_backlight();
bool LiquidCrystal_I2C_getBacklight();
void LiquidCrystal_I2C_autoscroll();
void LiquidCrystal_I2C_noAutoscroll();
void LiquidCrystal_I2C_createChar(unsigned char, unsigned char[]);
void LiquidCrystal_I2C_setCursor(unsigned char, unsigned char);
size_t LiquidCrystal_I2C_write(unsigned char);
void LiquidCrystal_I2C_command(unsigned char);

inline void LiquidCrystal_I2C_blink_on() { LiquidCrystal_I2C_blink(); }
inline void LiquidCrystal_I2C_blink_off() { LiquidCrystal_I2C_noBlink(); }
inline void LiquidCrystal_I2C_cursor_on() { LiquidCrystal_I2C_cursor(); }
inline void LiquidCrystal_I2C_cursor_off() { LiquidCrystal_I2C_noCursor(); }

// Compatibility API function aliases
void LiquidCrystal_I2C_setBacklight(unsigned char new_val);				// alias for backlight() and nobacklight()
void LiquidCrystal_I2C_load_custom_character(unsigned char char_num, unsigned char *rows);	// alias for createChar()
void LiquidCrystal_I2C_print(const char *);
void LiquidCrystal_I2C_printstr(const char *, int);

#endif // FDB_LIQUID_CRYSTAL_I2C_H
