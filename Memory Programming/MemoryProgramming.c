#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t EEMEM Addess = 0;
uint8_t Duty_Cycle;

/*
Lab 8: Set timer0 for 5 ms with 100% duty cycle
NM, ps = 1    ––> 16 μs (too fast)
	ps = 8    ––> 128 μs (too fast)
	ps = 64   ––> 1024 μs (too fast)
	ps = 256  ––> 4096 μs (too fast)
	ps = 1024 ––> 16384 μs (good enough)
CTC mode, 256 steps ––> 16384 μs
	x  steps ––> 5 ms (5000 μs)
	x = 256 * 5 ms / 16384 = 78.125 (rounding down since its less than half) = 78 steps
	ps = 1024
	CTC mode
	OCR0A = 78 - 1 = 77
	OCR0B = 0 (100% Duty Cycle(Goes from 0 to up to 77(100% duty cycle))
*/

void timer_init(){
	TCCR0A |= (1 << WGM01);			// Set the Timer Mode to CTC
	OCR0A = 77;					// Represents 5ms timer
	OCR0B = 0;						// Represents 100% duty cycle or in other words 0% and goes up to 100%
	TCCR0B |= (1 << CS02) | (1 << CS00);			// Set pre-scaler to 1024 and start the timer
	TIMSK0 |= (1 << OCIE0A | 1 << OCIE0B);	// Enabling compare match A & B Interrupts
}

int main(void)
{
	timer_init();			// Calling function timer_init()
	DDRB |= (1 << DDB5);		// Set port bit B5 in data direction as an output for LED output
		
	Duty_Cycle = eeprom_read_byte(&Addess);		// Read the address using the eeprom_read_byte function and initialize it to Duty_Cycle
	
	if(Duty_Cycle >= 100.0){			// if the Duty_Cycle is greater than or equal to 100 (100% maximum duty cycle and then goes back to 0 and restart)
		Duty_Cycle = 0;			// Initialize it to 0
	}
	else{
		Duty_Cycle = Duty_Cycle + 10.0;		// increment the Duty_Cycle by 10 (10% everytime we plug and unplug)
	}
	eeprom_update_byte(&Addess, Duty_Cycle);		// Update the eeprom byte Duty_Cycle using the eeprom_update_byte function at the address
	
	OCR0B = round(OCR0A * (Duty_Cycle / 100.0));		// Calculate OCR0B
	sei();			// Calling sei() function to enable interrupts
	
	while (1)
	{	
	}
}

ISR(TIMER0_COMPA_vect)
{	
if(OCR0B > 0){
		PORTB |= (1 << PORTB5);	// Set port bit B5 to 1 to turn on the LED
	}
}

ISR(TIMER0_COMPB_vect)
{
	PORTB &= ~(1 << PORTB5);		// Clear port bit B5 to 0 to turn off the LED
}
