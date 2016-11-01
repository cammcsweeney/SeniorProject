/*
 * LCD.h
 *
 * Created: 10/6/2015 9:57:47 AM
 *  Author: cmcsweeney
 */ 

//#define  F_CPU 8000000UL


#define LCD_H_
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>

//		LCD Port Information (from microcontroller)
#define LCD_OUT		PORTA	
#define LCD_IN		PINA		
#define LCD_DDR		DDRA	
#define RS			6
#define RW			5
#define ENABLE		4
#define DB7			3
#define DB6			2
#define DB5			1
#define DB4			0

//		Register Select Constants
#define	DATA_REGISTER		1
#define COMMAND_REGISTER 	0

//		Display Dimensions
#define DISPLAY_ROWS		4
#define DISPLAY_COLUMNS		20

void lcd_init(void);
void writeStringToLCD(char output[16]);
uint8_t ReadByteFromLCD(uint8_t selectedRegister);
void send_byte(uint8_t info);
void WaitForLCD(void);
void WriteByteToLCDwait(uint8_t, uint8_t);
void clear_LCD(void);
void clear_row(uint8_t);
void new_line(void);
void moveCursorToRowColumn(uint8_t row, uint8_t column);
void backspaceLCD(void);
void writeIntegerToLCD(uint16_t integer);
void enable_high(void);
void enable_low(void);
void toggle_enable(void);