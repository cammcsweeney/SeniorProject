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

void adc_init(void);

uint8_t i = 0;		//will be used for indices in ADC
uint8_t adc_start_avg = 0;
uint8_t adc_avg_flag = 0;	//used to indicate if initial value has already been read

int main(void)
{
	sei();
	lcd_init();
	adc_init();
    
    while (1) 
    {
    }
}

ISR(ADC_vect){
	i++;
	if (i>7){
		ADCSRA &= ~(1<<ADEN);
	}
	
	if 
}

void adc_init(void){
	ADMUX = (1<<REFS0) | (1<<ADLAR);	//set up ADC to use AVCC as reference, left adjust results
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADFR);	//set up ADC to be on, trigger interrupt upon completion, free run on
}

void adc_start(void){
	i=0;
	ADCSRA |= (1<<ADSC);
}
