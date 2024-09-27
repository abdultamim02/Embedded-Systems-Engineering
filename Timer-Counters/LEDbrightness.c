#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

/* Key Words definitions
	Registers (n = 0, 1, 2)
	. CS0[2:0]/CS1[2:0] – Clock Source Select
	. CTC – Clear Timer on Compare (CTC Mode)
	. TCNTn – Timer/Counter Register (8-bit)
	. OCRnA – Output Compare Register A (Defines the top value of the counter)
	. OCRnB – Output Compare Register B (Defines the value of the percentage of the duty cycle counter the counter should start from/at)
	. OCFnA/B – The OCFnA/B bits are set when a Compare Match occurs between the Timer/Counter0 and the data in OCRnA/B Output Compare Register0
	. PWM – Pulse Width Modulation (Mode)
	. TCCRnA/B – Timer/Counter Control Registers
	. TIFRn – Timer/Counter Interrupt Flag Register
		. TOV interrupt
		. Compare A&B interrupts
        - We use these to signify whenever the timer value reaches OCR0A or OCR0B (Used instead of using/checking the value of TCNTn directly
	. Duty Cycle (%) = HIGH/Period or HIGH/LOW+HIGH
	. BOTTOM – The counter reaches the BOTTOM when it becomes 0x00.
	. MAX – The counter reaches its MAXimum when it becomes 0xFF (decimal 255).
	. TOP – The counter reaches the TOP when it becomes equal to the highest value in the	count sequence. 
		    The TOP value can be assigned to be the fixed value 0xFF (MAX) or the value stored in the OCR0A Register. 
		    The assignment is	dependent on the mode of operation.
	. TOV0 – Timer/Counter Overflow Flag
*/

/*
Lab 4: Set timer0 for 10 ms with 100% duty cycle
NM, ps = 1  ––> 16 μs (too fast)
	ps = 8    ––> 128 μs (too fast)
	ps = 64   ––> 1024 μs (too fast)
	ps = 256  ––> 4096 μs (too fast)
	ps = 1024 ––> 16384 μs (good enough)
CTC mode, 256 steps ––> 16384 μs
	x  steps ––> 10 ms (10000 μs)
	x = 256 * 10 ms / 16384 = 156.25 (rounding down since its less than half) = 156 steps
	ps = 1024
	CTC mode
	OCR0A = 156 - 1 = 155
	OCR0B = 156 - 1 = 155 -> 0	(100% Duty Cycle(Goes from 0 to up to 155(100% duty cycle))
*/

void UART_init(){
	DDRB &= ~(1 << DDB7);			// Setting DDB7 as INPUT
	PORTB |= (1 << PORTB7);			// Setting PORTB7 as PULL-UP
}

void timer_init(){
	TCCR0A |= (1 << WGM01);				// Set the Timer Mode to CTC
	OCR0A = 0x9B;						// Represents 10ms timer
	OCR0B = 0x00;				// Represents 100% duty cycle or in other words 0% and goes up to 100%
	TCCR0B |= (1 << CS02) | (1 << CS00);		// Set pre-scaler to 1024 and start the timer
}

int main(void){
	UART_init();
	timer_init();
	DDRB |= (1 << DDB5);	// Set port bit B5 in data direction as an output

	while (1)
	{			
		//CREATE A LOGIC 1 SIGNAL (E.G. TURN ON LED)
		if(!(PINB &(1 << PORTB7))){
			if(OCR0B < OCR0A){		// If OCR0B is less than OCR0A increase it ascendingly until it reaches OCR0A  
				OCR0B++;		// Increment OCR0B (Increase it)
			}
		}
		PORTB |= (1 << PORTB5);	// Set port bit B5 to 1 to turn on the LED
		while ((TIFR0 & (1 << OCF0B)) == 0){	// wait for OCR0B Compare Match Event B (Wait until OCF0B == 1)
			// Do Nothing
		}		
		TIFR0 |= (1 << OCF0B);	// reset/clear OCR0B Compare Match Event B flag

		//CREATE A LOGIC 0 SIGNAL (E.G. TURN OFF LED)	
		if(PINB &(1 << PORTB7)){
			if(OCR0B > 0){	// If OCR0B is  greater than 0 decrease it descendingly until it reaches 0
				OCR0B--;	// Decrement OCR0B (Decrease it)
			}
		}
		PORTB &= ~(1 << PORTB5);		// Clear port bit B5 to 0 to turn off the LED
		while ((TIFR0 & (1 << OCF0A)) == 0){	// wait for OCR0A Compare Match Event A event (Wait until OCF0A == 1)
			// Do Nothing
		}
		TIFR0 |= (1 << OCF0A); 	// reset/clear OCR0A Compare Match Event A flag
	}
}
