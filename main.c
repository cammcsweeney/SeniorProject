/*
 * ComponentID.c
 * 
 * Created: 11/1/2016 10:46:16 AM
 * Author : cmcsweeney
 * Written for ATMega128A
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
	
    
    while (1) 
    {
    }
}

