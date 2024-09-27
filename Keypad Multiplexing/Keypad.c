#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4				// Define ROWS to 4 (4x4 keypad)
#define COLS 4				// Define COLS to 4 (4x4 keypad)

// define some macros
#define BAUD 9600			// define baud
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)		// set baud rate value for UBRR

int rows[] = {PORTD4, PORTD5, PORTD6, PORTD7};	// Array of the rows to iterate over in the for loop
int cols[] = {PORTB0, PORTB1, PORTB2, PORTB3};	// Array of the cols to iterate over in the for loop

void uart_init (void){
	UBRR0H = (BAUDRATE >> 8);	// shift the register right by 8 bits (UBRR0H: clear high bits to 0 (high byte))
	UBRR0L = BAUDRATE;		// set baud rate (UBRR0L: Baud Rate Calculated (low byte))
	UCSR0B |= (1 << TXEN0);				// enable transmitter
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);	// set data frame: 8-bit data format (USART Control and Status Register C (UCSRnC) selects between asynchronous and synchronous operation)
}

void UART_init(){
	DDRD |= (1 << DDD4 | 1 << DDD5 | 1 << DDD6 | 1 << DDD7);			// Setting ROWS DDD4, DDD5, DDD6, DDD7 as OUTPUTS
	DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3));		// Setting COLS DDB0, DDB1, DDB2, DDB3 as INPUTS
	PORTB |= (1 << PORTB0 | 1 << PORTB1 | 1 << PORTB2 | 1 << PORTB3);		// Setting COLS PORTB0, PORTB1, PORTB2, PORTB3 as PULL-UPS
	PORTD |= (1 << PORTD4 | 1 << PORTD5 | 1 << PORTD6 | 1 << PORTD7);		// Setting ROWS PORTD4, PORTD5, PORTD6, PORTD7 as LOGIC HIGH
}

void UART_Tx(unsigned char keypad){
	// Check if buffer is empty
	while(!(UCSR0A &(1 << UDRE0))){		// While the buffer is not empty, do nothing and wait until it becomes empty (wait while register is free)
		// Do nothing
	}
	// Write keypad into the buffer/register (UDR0)
	UDR0 = keypad;		// load keypad in the buffer/register UDR0, sends the keypad (UDR0: one of the two level receive buffers)
	_delay_ms(50);		// 50 ms delay after each bit is sent
}

void keypad_scan(unsigned char keypad[ROWS][COLS]){
	unsigned int i;			// i iterates over rows
	unsigned int j;			// j iterates over columns
	
	for(i = 0; i < ROWS; i++){		// For loop to iterate over the rows
		PORTD &= ~(1 << rows[i]);	// Set row(i) to LOGIC LOW, one at a time and check each column
		for(j = 0; j < COLS; j++){		// For loop to iterate over the columns
			if(!(PINB & (1 << cols[j]))){		// Check each column(j) to see which is pulled LOW (becomes 0)
				UART_Tx(keypad[i][j]);		// Transmit that row and column to UART_Tx function to send the specified character from the keypad
				_delay_ms(50);		// 50 ms delay for switch bounce 
			}
		}
		PORTD |= (1 << rows[i]);			// Set back row(i) to LOGIC HIGH
	}
}

int main(void)
{
	// Keypads' Characters Buttons
	unsigned char keypad[ROWS][COLS] = {{'1', '2', '3', 'A'},
					                            {'4', '5', '6', 'B'},
					                            {'7', '8', '9', 'C'},
					                            {'*', '0', '#', 'D'}};

	uart_init();			// Calling the uart_init()
	UART_init();			// Calling the UART_init()
	
	while (1)
	{
		keypad_scan(keypad);		// Calling the keypad_scan function in the while loop to run endlessly 
	}
}

