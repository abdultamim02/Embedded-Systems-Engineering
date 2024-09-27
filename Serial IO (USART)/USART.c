#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define some macros
#define BAUD 9600		// define baud
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)		// set baud rate value for UBRR

/* Key words definitions
	USART: Universal Synchronous and Asynchronous Serial Receiver and Transmitter
	UART: Universal Asynchronous Serial Receiver and Transmitter
	UBRR: USART Baud Rate Register
	UCSRnC: (USART Control and Status Register C (UCSRnC) selects between asynchronous and synchronous operation)
	UCSR0B: The USART Receiver and Transmitter are enabled by writing the Receive Enable (RXENn) and the Transmit Enable (TXENn) bits in the UCSRnB Register to one.
	UCSRnA: Checks if the buffer is empty or not
	UDR0: one of the two level receive buffers
	UCSZn2:0: Character Size
*/	

// function to initialize UART
void uart_init (void){
	UBRR0H = (BAUDRATE >> 8);			// shift the register right by 8 bits (UBRR0H: clear high bits to 0 (high byte))
	UBRR0L = BAUDRATE;			// set baud rate (UBRR0L: Baud Rate Calculated (low byte))
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);		// enable receiver and transmitter
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);		// set data frame: 8-bit data format (USART Control and Status Register C 
// (UCSRnC) selects between asynchronous and synchronous operation)
}

// function to transmit/send data over USART or UART
void uart_transmit(unsigned char data){
	// Check if buffer is empty
	while(!(UCSR0A &(1 << UDRE0))){	// While the buffer is not empty, do nothing and wait until it becomes empty (wait while register is free)
		// Do nothing
	}
	// Write data into the buffer/register (UDR0)
	UDR0 = data;		// load data in the buffer/register UDR0, sends the data (UDR0: one of the two level receive buffers)
}

// function to receive data from USART or UART
unsigned char uart_receive(void){
	while(!(UCSR0A &(1 << RXC0))){		// While the receive bit did not receive anything, do nothing and wait until it receives something (wait while data is being received)
		// Do nothing
	}
	return UDR0;			// Get and return received 8-bit data from buffer
}

void send_data(char *data){
	unsigned int i;
	// Send a sequence of bytes
	for(i = 0; data[i] != '\0'; i++){		// A for loop iterates through the data (RedID), until it reaches the null character, and sends each index (8 bit for a character) of data to the transmit function
		uart_transmit(data[i]);				// send/transmit/load data in the register UDR0
	}
}

int main(void)
{
	char data[] = "123456789\r\n";			// Initialize a char data[] that holds the RedID
	uart_init();							// Call the uart_init() function
	while (1)
	{
		send_data(data);				// Send the data (RedID) to the send_data(data) function
		_delay_ms(500);					// 500ms delay
	}
	return 0;
}
