/*
 * ComponentID.c
 *
 * Created: 11/1/2016 10:46:16 AM
 * Author : cmcsweeney
 */ 
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "LCD.h"
#include <avr/interrupt.h>



int main(void)
{
	sei();

	lcd_init();

	writeStringToLCD("Hello World");
    /* Replace with your application code */
    while (1) 
    {
    }
}

