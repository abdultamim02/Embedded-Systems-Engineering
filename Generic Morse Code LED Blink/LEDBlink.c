#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void dot(){
	PORTB |= (1<<PORTB5);		//Set port bit B5 to 1 to turn on the LED
	_delay_ms(200);			//delay 200ms (A dot is equivalent one unit (200ms))
	PORTB &= ~(1<<PORTB5);		 //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200);	               //delay 200ms (The space between the dot/dash units of the same letter is one unit (200ms))
}
void dash(){
	PORTB |= (1<<PORTB5);		//Set port bit B5 to 1 to turn on the LED
	_delay_ms(600);			//delay 600ms (A dash is equivalent to three units (600ms))
	PORTB &= ~(1<<PORTB5);		 //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(200);                    //delay 200ms (The space between the dot/dash units of the same letter is one unit (200ms))
}
void space(){
	PORTB &= ~(1<<PORTB5);	 	//Clear port bit B5 to 0 to turn off the LED
	_delay_ms(600);			//delay 600ms (The space between different letters is three units (600ms))
}
void space_between_words(){
	PORTB &= ~(1<<PORTB5);		 //Clear port bit B5 to 0 to turn off the LED
	_delay_ms(1400);			//delay 1400ms (The space between words is seven units (1400ms))
}
void output_LED_name(char *name){
	DDRB |= (1<<DDB5);		//0x20 (hex) // Set port bit B5 in data direction
	unsigned int i = 0;			// initialize i to 0 to use in the while loop for the name's indices
	while(1)
	{
		if(name[i] == 'A' || name[i] == 'a'){
			dot();
			dash();
			space();
		}
		else if(name[i] == 'B' || name[i] == 'b'){
			dash();
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == 'C' || name[i] == 'c'){
			dash();
			dot();
			dash();
			dot();
			space();
		}
		else if(name[i] == 'D' || name[i] == 'd'){
			dash();
			dot();
			dot();
			space();
		}
		else if(name[i] == 'E' || name[i] == 'e'){
			dot();
			space();
		}
		else if(name[i] == 'F' || name[i] == 'f'){
			dot();
			dot();
			dash();
			dot();
			space();
		}
		else if(name[i] == 'G' || name[i] == 'g'){
			dash();
			dash();
			dot();
			space();
		}
		else if(name[i] == 'H' || name[i] == 'h'){
			dot();
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == 'I' || name[i] == 'i'){
			dot();
			dot();
			space();
		}
		else if(name[i] == 'J' || name[i] == 'j'){
			dot();
			dash();
			dash();
			dash();
			space();
		}
		else if(name[i] == 'K' || name[i] == 'k'){
			dash();
			dot();
			dash();
			space();
		}
		else if(name[i] == 'L' || name[i] == 'l'){
			dot();
			dash();
			dot();
			dot();
			space();
		}
		else if(name[i] == 'M' || name[i] == 'm'){
			dash();
			dash();
			space();
		}
		else if(name[i] == 'N' || name[i] == 'n'){
			dash();
			dot();
			space();
		}
		else if(name[i] == 'O' || name[i] == 'o'){
			dash();
			dash();
			dash();
			space();
		}
		else if(name[i] == 'P' || name[i] == 'p'){
			dot();
			dash();
			dash();
			dot();
			space();
		}
		else if(name[i] == 'Q' || name[i] == 'q'){
			dash();
			dash();
			dot();
			dash();
			space();
		}
		else if(name[i] == 'R' || name[i] == 'r'){
			dot();
			dash();
			dot();
			space();
		}
		else if(name[i] == 'S' || name[i] == 's'){
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == 'T' || name[i] == 't'){
			dash();
			space();
		}
		else if(name[i] == 'U' || name[i] == 'u'){
			dot();
			dot();
			dash();
			space();
		}
		else if(name[i] == 'V' || name[i] == 'v'){
			dot();
			dot();
			dot();
			dash();
			space();
		}
		else if(name[i] == 'W' || name[i] == 'w'){
			dot();
			dash();
			dash();
			space();
		}
		else if(name[i] == 'X' || name[i] == 'x'){
			dash();
			dot();
			dot();
			dash();
			space();
		}
		else if(name[i] == 'Y' || name[i] == 'y'){
			dash();
			dot();
			dash();
			dash();
			space();
		}
		else if(name[i] == 'Z' || name[i] == 'z'){
			dash();
			dash();
			dot();
			dot();
			space();
		}
		else if(name[i] == '1'){
			dot();
			dash();
			dash();
			dash();
			dash();
			space();
		}
		else if(name[i] == '2'){
			dot();
			dot();
			dash();
			dash();
			dash();
			space();
		}
		else if(name[i] == '3'){
			dot();
			dot();
			dot();
			dash();
			dash();
			space();
		}
		else if(name[i] == '4'){
			dot();
			dot();
			dot();
			dot();
			dash();
			space();
		}
		else if(name[i] == '5'){
			dot();
			dot();
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == '6'){
			dash();
			dot();
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == '7'){
			dash();
			dash();
			dot();
			dot();
			dot();
			space();
		}
		else if(name[i] == '8'){
			dash();
			dash();
			dash();
			dot();
			dot();
			space();
		}
		else if(name[i] == '9'){
			dash();
			dash();
			dash();
			dash();
			dot();
			space();
		}
		else if(name[i] == '0'){
			dash();
			dash();
			dash();
			dash();
			dash();
			space();
		}
		else if(name[i] == ' '){
			space_between_words();
		}
		else if(name[i] == '\0'){	
			i = 0;
		}

		++i;
	}
}
int main(void)
{
	char name[] = "Abdul Karim 123456789 ";
	output_LED_name(name);
}
