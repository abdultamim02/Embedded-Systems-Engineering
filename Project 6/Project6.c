#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4					// Define ROWS to 4 (4x4 keypad)
#define COLS 4					// Define COLS to 4 (4x4 keypad)
#define Percentage 0.1			// Define Percentage to 0.1 for the duty cycle percentage of the key pressed to get multiplied to

// RedId: 123456159 --> 123456XYZ --> X = 1, Z = 5
uint8_t X = 1;			// Initialize X to 1 (from RedID)
uint8_t Z = 5;			// Initialize Z to 5 (from RedID)

// Keypads' keys Buttons
uint8_t keypad[ROWS][COLS] = {{1, 2, 3, 0},
                              {4, 5, 6, 0},
                              {7, 8, 9, 0},
                              {0, 0, 0, 0}};

int rows[] = {PORTD4, PORTD5, PORTD6, PORTD7};	// Array of the rows to iterate over in the for loop
int cols[] = {PORTB0, PORTB1, PORTB2, PORTB3};	// Array of the cols to iterate over in the for loop

void initialization(){
	DDRD |= (1 << DDD4 | 1 << DDD5 | 1 << DDD6 | 1 << DDD7);		// Setting ROWS DDD4, DDD5, DDD6, DDD7 as OUTPUTS
	DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);	// Setting COLS DDB0, DDB1, DDB2, DDB3 as INPUTS
	PORTB |= (1 << PORTB0 | 1 << PORTB1 | 1 << PORTB2 | 1 << PORTB3);	// Setting COLS PORTB0, PORTB1, PORTB2, PORTB3 as PULL-UPS
	PORTD |= (1 << PORTD4 | 1 << PORTD5 | 1 << PORTD6 | 1 << PORTD7);	// Setting ROWS PORTD4, PORTD5, PORTD6, PORTD7 as LOGIC HIGH
	DDRB |= (1 << DDB5);		// Set port bit B5 in data direction as an output for LED output
}

int Period(uint8_t X){
	uint8_t period = X + 1;		// Calculate the period (ms or microseconds) using the given formula for timer0 (period = 1 + 1 = 2 ms = 2000 microseconds)
	return period;				// Return the period
}

int Duty_Cycle_Frequency(uint8_t Z){
	uint8_t frequency = (Z + 1) * 100;	// Calculate the Duty cycle frequency for the waveform of the LED using the given formula for timer2. (5 + 1) * 100 =  6 * 100 = 600Hz
	uint8_t period = 1 / frequency;				// Calculate the period given the frequency. 1 / 600 = 1.667ms —> 1667 microsecond
	return period;								// Return the period
}

void timer0_init(){
	OCR0A = 124;			// Calculate OCR0A using the returned period from Period(X) function for scanning (((256 * (Period(X) * 1000)) / 4096) - 1 = 124)
	TCCR0A |= 1 << WGM01;			// Set the Timer0 Mode to CTC
	TCCR0B |= (1 << CS02);			// Set pre-scaler to 256 and start the timer
	TIMSK0 |= 1 << OCIE0A;			// Enabling compare match A Interrupt
}

void timer2_init(){
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);		// Set the Timer2 Control Mode to Fast PWM (Pulse Width Modulation) and to compare output mode that clears OC2B on compare match and sets OC2B at bottom
	TCCR2B |= (1 << WGM22) | (1 << CS22);	// Set pre-scaler to 256 and start the timer and Timer2 control Mode to Fast PWM (Pulse Width Modulation)
	TIMSK2 |= (1 << OCIE2A | 1 << OCIE2B);	// Enabling compare match A & B Interrupts
	OCR2A = 103;		// Calculate OCR2A using the returned period from Duty_Cycle_Frequency(Z) function for the duty cycle waveform on the LED (256 * (Duty_Cycle_Frequency(Z) * 1000) / 4096 - 1 = 103)
}

int main(void)
{
	initialization();			// Calling the initialization() function
	timer0_init();				// Calling the timer0_init() function
	timer2_init();				// Calling the timer2_init() function
	sei();						// Calling sei() function to enable interrupts
	
	while (1)
	{
	}
}

ISR(TIMER2_COMPA_vect)
{
	PORTB |= (1 << PORTB5);			// Set port bit B5 to 1 to turn on the LED
}

ISR(TIMER2_COMPB_vect)
{
	PORTB &= ~(1 << PORTB5);		// Clear port bit B5 to 0 to turn off the LED
}

ISR(TIMER0_COMPA_vect)
{
	for(uint8_t i = 0; i < ROWS; i++){		// For loop to iterate over the rows
		PORTD &= ~(1 << rows[i]);		// Set row(i) to LOGIC LOW, one at a time and check each column
		for(uint8_t j = 0; j < COLS; j++){			// For loop to iterate over the columns
			if(!(PINB & (1 << cols[j]))){		// Check each column(j) to see which is pulled LOW (becomes 0)
				uint8_t button_pressed = keypad[i][j];		// Initialize variable button_pressed to the pressed key on the keypad
				if(button_pressed >= 0){			// Check if the button_pressed is greater than or equal to 0
					OCR2B = round(OCR2A * button_pressed * Percentage);	// Calculate OCR2B that will generate the LED and make it turn on to a specific percentage depending on the specialized key pressed on the keypad
				}
			}
		}
		PORTD |= (1 << rows[i]);			// Set back row(i) to LOGIC HIGH
	}
}
