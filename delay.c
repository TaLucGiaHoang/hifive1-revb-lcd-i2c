/*
 * delay.c
 *
 *  Created on: Jan 15, 2021
 *      Author: Hoang Ta
 */
#include <stdint.h>

void delayMicroseconds(int microseconds)
{
	volatile uint32_t ul;
	for(ul = 0; ul < microseconds; ul++)
	{
		__asm__ volatile ("nop");
	}
}

void delay(uint32_t miliseconds)
{
	for(; miliseconds > 0; miliseconds--)
	{
		delayMicroseconds(1000);
	}
}
