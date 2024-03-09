#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4				// Define ROWS to 4 (4x4 keypad)
#define COLS 4				// Define COLS to 4 (4x4 keypad)
#define DUTY_CYCLE 0.5		// Define DUTY_CYCLE to 0.5 for 50% duty cycle

// Keypads' Characters Buttons
uint8_t keypad[ROWS][COLS] = {{0, 1, 2, 3},
                              {4, 5, 6, 7},
                              {8, 9, 10, 11},
                              {12, 13, 14, 15}};

int rows[] = {PORTD4, PORTD5, PORTD6, PORTD7};	// Array of the rows to iterate over in the for loop
int cols[] = {PORTB0, PORTB1, PORTB2, PORTB3};	// Array of the cols to iterate over in the for loop

void initialization(){
  	DDRD |= (1 << DDD4 | 1 << DDD5 | 1 << DDD6 | 1 << DDD7);	// Setting ROWS DDD4, DDD5, DDD6, DDD7 as OUTPUTS
  	DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);	// Setting COLS DDB0, DDB1, DDB2, DDB3 as INPUTS
  	PORTB |= (1 << PORTB0 | 1 << PORTB1 | 1 << PORTB2 | 1 << PORTB3);	// Setting COLS PORTB0, PORTB1, PORTB2, PORTB3 as PULL-UPS
  	PORTD |= (1 << PORTD4 | 1 << PORTD5 | 1 << PORTD6 | 1 << PORTD7);	// Setting ROWS PORTD4, PORTD5, PORTD6, PORTD7 as LOGIC HIGH
  	DDRD |= (1 << DDD3 | 1 << DDD2);				// Setting PINS DDD3 and DDD2 as OUTPUT PINS
}

void timer_init(){
  	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1); // Set the Timer2 Control Mode to Fast PWM (Pulse Width Modulation) and to compare output mode that clears OC0B on compare match and sets OC0B at bottom
  	TCCR2B |= (1 << WGM22) | (1 << CS22);				// Set pre-scaler to 256 and start the timer and Timer2 control Mode to Fast PWM (Pulse Width Modulation)
}

int Step_Size_Frequency(uint8_t x){
  	float frequency = 1 / (440 * pow(pow(2, (1/12.0)), x)) * pow(10, 6.0);	// Calculate the frequency using the given formula
  	return (uint8_t)frequency - 1;
}

void PlayTone(uint8_t i, uint8_t j){
  	while(!(PINB & (1 << j))){		// Check each j to see which is pulled LOW (becomes 0)
    		uint8_t step_size = (Step_Size_Frequency(keypad[i][j]));		// initialized step_size to the returned frequency from Step_Size_Frequency function
    		OCR2A = step_size;			// initialize OCR2A to the step_size
    		OCR2B = OCR2A * DUTY_CYCLE;	// initialize OCR2B to 50% duty cycle by multiplying the value of OCR2A by the DUTY_CYCLE (0.5)
  	}
  	OCR2A = 0;		// Initialize OCR2A to 0 for the sound to stop when not pressing on the keypad
  	OCR2B = 0;		// Initialize OCR2B to 0 for the sound to stop when not pressing on the keypad
}

void keypad_scan(){
  	for(uint8_t i = 0; i < ROWS; i++){		// For loop to iterate over the rows
  		  PORTD &= ~(1 << rows[i]);			// Set row(i) to LOGIC LOW, one at a time, and check each column
  		for(uint8_t j = 0; j < COLS; j++){	// For loop to iterate over the columns
  		  	PlayTone(i, j);			// Transmit that row and column to the PlayTone function to send the output-specific musical tone from the keypad
  		}
  		PORTD |= (1 << rows[i]);			// Set back row(i) to LOGIC HIGH
  	}
}

int main(void){
  	timer_init();			// Calling the timer_init() function
  	initialization();		// Calling the initialization() function
  	
  	while (1)
  	{
  	  	keypad_scan();		// Calling keypad_scan() function
  	}
  }
