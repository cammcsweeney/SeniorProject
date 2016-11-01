/*
 * LCD display control code
 * Made for HD44780 LCD displays
 *		Modifications are probably required for displays with only 2 lines
 *		This should work fine with displays of any width
 *
 * Created: 10/6/2015 9:13:20 AM
 *  Author: cmcsweeney
 */

//#define F_CPU 8000000UL
//#include <avr/io.h>
//#include <util/delay.h>
//#include <string.h>
#include "LCD.h"

void writeIntegerToLCD(uint16_t integer)
{
	uint8_t x = 0;
	char outputStr[7] = {0};
	sprintf(outputStr, "%u", integer);
	
	for(x; x<strlen(outputStr);x++)
	{
		WriteByteToLCDwait(DATA_REGISTER, outputStr[x]);
	}
}

void writeStringToLCD(char output[16]){
	uint8_t x = 0;
	for(x; x<strlen(output);x++)
	{
		WriteByteToLCDwait(DATA_REGISTER, output[x]);
	}
}

//moves the cursor to a given row and column, but just leaves it there
void moveCursorToRowColumn(uint8_t row, uint8_t column)
{
	if (column > DISPLAY_COLUMNS-1)	//make sure the column address is not exceeded. Default to last column
	{
		column = DISPLAY_COLUMNS-1;
	}
	
	switch (row)
	{
		case 1:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 + column);
			break;
		case 2:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 |(0x40 + column));
			break;
		case 3:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 |(DISPLAY_COLUMNS + column));
			break;
		case 4:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 | 0x40 | (DISPLAY_COLUMNS + column) );
			break;
		default: WriteByteToLCDwait(COMMAND_REGISTER, 0x80);
	}
}

//reads the current position, goes back one column (with row wraparound) and writes a space 
//	in its place
void backspaceLCD(void)
{
	uint8_t address = 0;
	uint8_t row = 0;		//bits 4,5,6 of address
	uint8_t column = 0;		//lower nibble of address
	address = ReadByteFromLCD(COMMAND_REGISTER) & ~(0x80);	//read address without busy flag
	
	if (address <= DISPLAY_COLUMNS-1){
		row = 1;
	}
	else if((address >= DISPLAY_COLUMNS)&(address <= DISPLAY_COLUMNS*2)){
		row = 3;
	}
	else if((address >= 0x40) & (address <= (0x40 | DISPLAY_COLUMNS))){
		row = 2;
	}
	else if((address >= (0x40 | DISPLAY_COLUMNS)) & (address <= (0x40 | DISPLAY_COLUMNS*2))){
		row = 4;
	}
	else{
		row = 1;
		column = 0;
	}
	
	switch (row){
		case 1:
			column = address;
			break;
		case 2: 
			column = address - 0x40;
			break;
		case 3:
			column = address - DISPLAY_COLUMNS;
			break;
		case 4:
			column = address - 0x40 - DISPLAY_COLUMNS;
			break;
	}	

	if (column == 0)
	{
		switch (row)
		{
			case 1:
				moveCursorToRowColumn(4, DISPLAY_COLUMNS-1);
				WriteByteToLCDwait(DATA_REGISTER, ' ');
				moveCursorToRowColumn(4, DISPLAY_COLUMNS-1);
				break;
			case 2:
				moveCursorToRowColumn(1, DISPLAY_COLUMNS-1);
				WriteByteToLCDwait(DATA_REGISTER, ' ');
				moveCursorToRowColumn(1, DISPLAY_COLUMNS-1);
				break;
			case 3:
				moveCursorToRowColumn(2, DISPLAY_COLUMNS-1);
				WriteByteToLCDwait(DATA_REGISTER, ' ');
				moveCursorToRowColumn(2, DISPLAY_COLUMNS-1);
				break;
			case 4: 
				moveCursorToRowColumn(3, DISPLAY_COLUMNS-1);
				WriteByteToLCDwait(DATA_REGISTER, ' ');
				moveCursorToRowColumn(3, DISPLAY_COLUMNS-1);
				break;
			default: moveCursorToRowColumn(1,0);
		}
	}
	else
	{
		moveCursorToRowColumn(row, (column-1));
		WriteByteToLCDwait(DATA_REGISTER, ' ');
		moveCursorToRowColumn(row, (column-1));
	}
}

void clear_row(uint8_t row){
	moveCursorToRowColumn(row, DISPLAY_COLUMNS-1);
	
	for(uint8_t i=DISPLAY_COLUMNS-1; i>0; i--){
		backspaceLCD();
	}
	
}

//reads current line position, then puts the cursor on the next line in the first column
void new_line(void)
{
	uint8_t address = 0;
	uint8_t row = 0;		//bits 4,5,6 of address
	address = ReadByteFromLCD(COMMAND_REGISTER) & ~(0x80);	//read address without busy flag
	
	row = address >> 4;
	
	switch (row)
	{
		case 0x00:
			row = 1;
			break;
		case 0x04:
			row = 2;
			break;
		case 0x01:
			row = 3;
			break;
		case 0x05:
			row = 4;
			break;
		default: row = 1;
	}
	
	switch (row)
	{
		case 1:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 |0x40);
			break;
		case 2:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 |DISPLAY_COLUMNS);
			break;
		case 3:
			WriteByteToLCDwait(COMMAND_REGISTER, 0x80 | 0x40 | DISPLAY_COLUMNS);
			break;
		default: WriteByteToLCDwait(COMMAND_REGISTER, 0x80);
	}
}

//clears LCD and sets cursor to position 1
void clear_LCD(void)
{
	WriteByteToLCDwait(COMMAND_REGISTER, 0x01);
}

//initialize the LCD for use
void lcd_init(void)
{
	LCD_DDR = 0xFF;
	LCD_OUT = 0x0F;
		
	_delay_ms(100);
	send_byte(0x03);
		
	_delay_ms(5);
	send_byte(0x03);
		
	_delay_us(150);
	send_byte(0x03);
		
	_delay_ms(10);
		
	send_byte(0x02);
		
	_delay_ms(10);
		
	//specifies display lines and character font
	send_byte(0x02);
	send_byte(0b00001100);
		
	_delay_ms(10);
		
	//turns screen on, cursor on, blinking on
	send_byte(0x00);
	send_byte(0x0e);
		
	_delay_ms(10);
		
	//clears screen
	send_byte(0x00);
	send_byte(0x01);
		
	_delay_ms(10);
		
	//sets entry mode
	send_byte(0x00);
	send_byte(0b00000110);
		
	_delay_ms(10);
		
	//makes sure the display is on, cursor is shown, and cursor is blinking
	send_byte(0x00);
	send_byte(0x0e);
		
	//delays to prevent missed information immediately after initialization
	_delay_ms(10);
	
	
}

//When operating in 4-bit mode, this will send a byte of data: upper nibble then lower nibble
//There is no need to wait for the busy flag between upper and lower nibbles
//writes a symbol or address to the LCD depending on the register selected
void WriteByteToLCDwait(uint8_t SelectedRegister, uint8_t byte)
{
	send_byte((SelectedRegister << 6) | ((byte>>4) & 0x0F));
	send_byte((SelectedRegister << 6) | ((byte) & 0x0F));
	WaitForLCD();
}

//selected register determines whether we read ASCII data (DATA_REGISTER) from the LCD or the address data (COMMAND_REGISTER)
uint8_t ReadByteFromLCD(uint8_t selectedRegister)
{
	uint8_t nibbleRead = 0;
	
	LCD_DDR = 0xF0;	//change lower nibble of PortA to input
	
	LCD_OUT = (selectedRegister<<RS | 1<<RW);	//selects which register to read from
	
	enable_high();		
	_delay_us(1);	//allow time for LCD to output register information
	nibbleRead = (LCD_IN)<<4;
	
	enable_low();	//reset enable pin to get the lower nibble of information
	_delay_us(1);
	
	enable_high();	//allow time for LCD to output register information
	_delay_us(1);
	nibbleRead |= (LCD_IN & 0x0F);
	
	enable_low();
	LCD_DDR = 0xFF;	//change port back to all outputs
	
	return nibbleRead;
}

//waits for busy flag to go low
void WaitForLCD(void)
{
	uint8_t data = ReadByteFromLCD(COMMAND_REGISTER);
	PORTD = 0xFF;
	
	while ((data & 0x80)==0x80)
	{
		data = ReadByteFromLCD(COMMAND_REGISTER);
		PORTD = 0x00;
	}
}

//sets enable high
void enable_high(void)
{
	LCD_OUT |= (1<<ENABLE);
}

//sets enable low
void enable_low(void)
{
	LCD_OUT &= ~(1<<ENABLE);
}

//sends a literal byte of information, including the register and command information
void send_byte(uint8_t info)
{
	LCD_OUT = info;
	toggle_enable();
}

//wiggles the enable pin high, then low. 1us delay between operations
void toggle_enable(void)
{
	_delay_us(1);
	enable_high();
	_delay_us(1);
	enable_low();
}