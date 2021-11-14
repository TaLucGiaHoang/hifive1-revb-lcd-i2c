/*
 * LiquidCrystal_I2C.c
 *
 *  Created on: Jan 15, 2021
 *      Author: Hoang Ta
 */
#include "LiquidCrystal_I2C.h"
#include <inttypes.h>

#include <metal/machine.h>
#include <metal/i2c.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

//
// Private variables and functions
//
static void send(unsigned char, unsigned char);
static void write4bits(unsigned char);
static void expanderWrite(unsigned char);
static void pulseEnable(unsigned char);
static unsigned char _addr;
static unsigned char _displayfunction;
static unsigned char _displaycontrol;
static unsigned char _displaymode;
static unsigned char _cols;
static unsigned char _rows;
static unsigned char _charsize;
static unsigned char _backlightval;
static struct metal_i2c *_i2c;

extern void delayMicroseconds(int microseconds);
extern void delay(uint32_t miliseconds);

void LiquidCrystal_I2C_init(struct metal_i2c *i2c, unsigned char lcd_addr, unsigned char lcd_cols, unsigned char lcd_rows, unsigned char charsize)
{
	_i2c = i2c;
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;
}

void LiquidCrystal_I2C_begin() {

	if (_i2c == NULL) {
		printf("error: I2C not available \n");
	} else {
		metal_i2c_init(_i2c, 100000, METAL_I2C_MASTER); // configure to 100000Hz, master mode
	}

	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1) {
		_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	delay(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	delay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	delayMicroseconds(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	delayMicroseconds(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	LiquidCrystal_I2C_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LiquidCrystal_I2C_display();

	// clear it off
	LiquidCrystal_I2C_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);

	LiquidCrystal_I2C_home();
}

/********** high level commands, for the user! */
void LiquidCrystal_I2C_clear(){
	LiquidCrystal_I2C_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_home(){
	LiquidCrystal_I2C_command(LCD_RETURNHOME);  // set cursor position to zero
	delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal_I2C_setCursor(unsigned char col, unsigned char row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	LiquidCrystal_I2C_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C_noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_display() {
	_displaycontrol |= LCD_DISPLAYON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_cursor() {
	_displaycontrol |= LCD_CURSORON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C_noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_I2C_blink() {
	_displaycontrol |= LCD_BLINKON;
	LiquidCrystal_I2C_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_scrollDisplayLeft(void) {
	LiquidCrystal_I2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_I2C_scrollDisplayRight(void) {
	LiquidCrystal_I2C_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_I2C_leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_I2C_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_I2C_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LiquidCrystal_I2C_command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_I2C_createChar(unsigned char location, unsigned char charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	LiquidCrystal_I2C_command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		LiquidCrystal_I2C_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C_noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LiquidCrystal_I2C_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
bool LiquidCrystal_I2C_getBacklight() {
  return _backlightval == LCD_BACKLIGHT;
}


/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal_I2C_command(unsigned char value) {
	send(value, 0);
}

inline size_t LiquidCrystal_I2C_write(unsigned char value) {
	send(value, Rs);
	return 1;
}


/************ low level data pushing commands **********/

// write either command or data
static void send(unsigned char value, unsigned char mode) {
	unsigned char highnib=value&0xf0;
	unsigned char lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

static void write4bits(unsigned char value) {
	expanderWrite(value);
	pulseEnable(value);
}

static void expanderWrite(unsigned char _data){
	_data |= _backlightval;
	metal_i2c_write(_i2c, (unsigned int)_addr, 1, &_data, METAL_I2C_STOP_ENABLE);
}

static void pulseEnable(unsigned char _data){
	expanderWrite(_data | En);	// En high
	delayMicroseconds(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);	// En low
	delayMicroseconds(50);		// commands need > 37us to settle
}

void LiquidCrystal_I2C_load_custom_character(unsigned char char_num, unsigned char *rows){
	LiquidCrystal_I2C_createChar(char_num, rows);
}

void LiquidCrystal_I2C_setBacklight(unsigned char new_val){
	if (new_val) {
		LiquidCrystal_I2C_backlight();		// turn backlight on
	} else {
		LiquidCrystal_I2C_noBacklight();		// turn backlight off
	}
}

void LiquidCrystal_I2C_printstr(const char *s, int len){
	for(int i = 0; i < len; i++) {
		LiquidCrystal_I2C_write(*s++);
	}
}

void LiquidCrystal_I2C_print(const char *s){
	LiquidCrystal_I2C_printstr(s, strlen(s));
}
