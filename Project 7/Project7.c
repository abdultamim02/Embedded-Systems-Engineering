#define F_CPU 16000000UL		// 16MHz clock from the debug processor
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// RedId: 123456159 --> 123456XYZ --> X = 1, Z = 5
uint8_t X = 1;			// Initialize X to 1 (from RedID)
uint8_t Y = 5;			// Initialize Y to 5 (from RedID)
uint8_t Z = 5;			// Initialize Z to 5 (from RedID)

int Period(uint8_t X){
	uint8_t period = X + 1;		// Calculate the period (ms or microseconds) using the given formula for timer0 (period = 1 + 1 = 2 ms = 2000 microseconds)
	return period;				// Return the period
}

int Duty_Cycle_Frequency(uint8_t Z){
	uint8_t frequency = (Z + 1) * 100;	// Calculate the Duty cycle frequency for the waveform of the LED using the given formula for timer2.
	uint8_t period = 1 / frequency;				// Calculate the period given the frequency. 1 / 600 = 1.667ms —> 1667 microsecond
	return period;								// Return the period
}

int Input_Channel(uint8_t Y){
	uint8_t channel = Y % 6;		// Calculates the input channel for the A/D. (channel = 5 % 6 = 5)
	return channel;					// Return the channel
}

void timer0_init(){
	OCR0A = 124;			// Calculate OCR0A using the returned period from Period(X) function for scanning
	TCCR0A |= 1 << WGM01;		// Set the Timer0 Mode to CTC
	TCCR0B |= (1 << CS02);		// Set pre-scaler to 256 and start the timer
}

void timer2_init(){
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);		// Set the Timer2 Control Mode to Fast PWM (Pulse Width Modulation) and to compare output mode that clears OC2B on compare match and sets OC2B at bottom
	TCCR2B |= (1 << WGM22) | (1 << CS22);	// Set pre-scaler to 256 and start the timer and Timer2 control Mode to Fast PWM (Pulse Width Modulation)
	TIMSK2 |= (1 << OCIE2A | 1 << OCIE2B);	// Enabling compare match A & B Interrupts
	OCR2A = 103;		// Calculate OCR2A using the returned period from Duty_Cycle_Frequency(Z) function for the duty cycle waveform on the LED
}

void adc_init(void)
{
	DDRC &= ~(1 << PINC5);	// Setting input pin C5 since Y % 6 = 5 % 6 = 5 (DDRC &= ~(1 << PINC(Input_Channel())))
	ADMUX |= (1 << REFS0) | (1 << MUX2) | (1 << MUX0);	// Setting VCC reference and the input channel selection to 5 (0101)
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// Enable ADC Auto Trigger & Conversion Complete Interrupt
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0);		// Timer/Counter0 Compare Match A
}

uint16_t adc_read(uint8_t ch) {
	// select the corresponding channel 0~7
	// ANDing with ’7′ will always keep the value of ‘ch’ between 0 and 7
	ch &= 0b00000111;		// AND operation with 7
	ADMUX = (ADMUX & 0xF8) | ch;	 // clears the bottom 3 bits before ORing
	// start single conversion, and write ’1′ to ADSC
	ADCSRA |= (1 << ADSC);
	// wait for conversion to complete ADSC becomes ’0′ again till then, run loop continuously
	while(ADCSRA & (1 << ADSC));
	// ADC is predefined to hold the 10-bit conversion value
	return (ADC);
}

int main(void)
{
	DDRB |= (1 << DDB5);		// Set port bit B5 in data direction as an output for LED output
	timer0_init();				// Calling function timer0_init()
	timer2_init();				// Calling function timer2_init()
	adc_init();					// Calling function adc_init()
	sei();						// Calling sei() function to enable interrupts
	
	while (1)
	{
		OCR2B = round(adc_read(Input_Channel(Y)));	// give the input channel to the adc_read function to control the timer2 and generate PWM
	}
}

ISR(ADC_vect)
{
	uint16_t variable = ADC;		// Initialize variable to ADC
	uint16_t a = variable;		// use this to adjust LED brightness
}

ISR(TIMER2_COMPA_vect)
{
	if(OCR2B > 0){
		PORTB |= (1 << PORTB5);	// Set port bit B5 to 1 to turn on the LED
	}
}

ISR(TIMER2_COMPB_vect)
{
	PORTB &= ~(1 << PORTB5);		// Clear port bit B5 to 0 to turn off the LED
}

