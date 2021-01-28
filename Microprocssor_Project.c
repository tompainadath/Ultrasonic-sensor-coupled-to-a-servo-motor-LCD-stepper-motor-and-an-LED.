/*
 *    Filename: Microprocessor_Project.c
 *      Author: Tom Painadath
 */ 


#define F_CPU 16000000UL // Define CPU frequency for delay.h
#include <util/delay.h>
#include <avr/interrupt.h>
#include "SSD1306.h"
//#include "SSD1306.c" //Not sure why I need to include this file here
// Pin definitions
#define PINGPin PD0
// Variable declarations
volatile int dist = 0;
char str [100];
int count = 0;
int main(void) {
	int8_t displayInverted = 1;
	uint8_t dispNum = 0x00;
	char text[] = "TEMPLE MADE ECE";
	char text1[] = "GO EAGLES!";
	OLED_Init();  //initialize the OLED
	_delay_ms(1);
	OLED_Clear(); //clear the display (for good measure)
	//set up Pin Change Interrupt Registers
	PORTD |= (1<<0);// Set Pull up resistor PD0
	PCICR |= (1<<PCIE3);// Pin Change Interrupt 1 Mask for PD0 is at PCIE3
	PCIFR|=(1<<PCIF3);// Pin Change Interrupt Flag for PD0 is at PCIF3
	PCMSK3 |= (1<<PCINT24) ; //Enable PCINT24
	
	//set up TIMER2 Registers
	TCCR2B = (1<<CS21);// Set TIMER2 to Normal Mode with 1:8 prescaler for 1st output pulse
	TIMSK2 = (1<<TOIE2);// Unmask TIMER2 in Normal Mode
	OCR2B = 255;// Set output compare register to 255
	
	sei(); // Enable global interrupts
	// set portb,a,d,e,as output
	PORTB = 0xFF;
	DDRA=0XFF;
	DDRD=0XFF;
	DDRE=0XFF;
	
	
	while (1) {
		dispNum++;
		OLED_SetCursor(0, 0);        //set the cursor position to (0, 0)
		OLED_SetCursor(1, 0);        // set the cursor position to (1, 0)
		OLED_DisplayString(text);     // display text
		OLED_SetCursor(2, 0);    //set the cursor position to (2, 0)
		OLED_DisplayString(text1); // display text1
		OLED_SetCursor(4, 0); //set the cursor position to (4, 0)
		OLED_Printf(" distance: ");
		OLED_DisplayNumber(C_DECIMAL_U8,dist,3);
		
		if(dist >20)  // if distance greater then 20
		{
			
			// servo and stepper motor run ccw
			PORTD = 0b00000100;
			_delay_ms(1.3);//_delay_ms(1.3);
			PORTD = 0b00000000;
			_delay_ms(20);//_delay_ms(20);
			
			PORTE = 0x30;
			_delay_ms (10);//_delay_ms(10);
			PORTE = 0x60;
			_delay_ms (10);//_delay_ms(10);
			PORTE = 0x48;
			_delay_ms (10);//_delay_ms(10);
			PORTE = 0x18;
			_delay_ms (10);//_delay_ms(10);
		}
		else if ( dist >5&& dist<20) //if between 5 and 20
		{
			// led light up
			PORTA=0b11111111;
			_delay_ms(dist);
			PORTA=0b00000000;
			_delay_ms(dist);
			if (dist>10) // if distance greater then 10
			{
				//stepper motor run cw
				PORTE = 0x30;
				_delay_ms (10); //_delay_ms(10);
				PORTE = 0x18;
				_delay_ms (10);//_delay_ms(10);
				PORTE = 0x48;
				_delay_ms (10);//_delay_ms(10);
				PORTE = 0x60;
				_delay_ms (10);//_delay_ms(10);
			}
		}
		else if (dist<=5) // if distance equal or smaller then 5
		{
			// servo motor and stepper motor run ccw
			PORTA=0b11111111;
			PORTE = 0x30;
			_delay_ms (10); //_delay_ms(10);
			PORTE = 0x18;
			_delay_ms (10);//_delay_ms(10);
			PORTE = 0x48;
			_delay_ms (10);//_delay_ms(10);
			PORTE = 0x60;
			_delay_ms (10);//_delay_ms(10);
			PORTD = 0b00000100;
			_delay_ms(1.7); //_delay_ms(1.7);
			PORTD = 0b00000000;
			_delay_ms(20); //_delay_ms(20);
		}
		
	}
	
	return 0; // never reached
}
ISR(TIMER2_OVF_vect) {
	if (OCR2B == 255) {
		TCCR2B = (1<<CS21);// Set TIMER2 to Normal Mode with 1:8 prescaler
		DDRD |= (1<<PINGPin);// Set data direction of PD0 to output
		PORTD |= (1<<PINGPin);// Send ultrasonic pulse
		OCR2B = 3;// Set output compare register to 3
		return;// Return to main
	}
	else if (OCR2B == 3) {
		PORTD &= ~(1<<PINGPin);// Stop ultrasonic pulse
		OCR2B = 2;// Set output compare register to 2
		return;// Return to main
	}
	else if (OCR2B == 2) {
		DDRD &= ~(1<<PINGPin);// Set data direction of PB0 to input
		OCR2B = 255;// Set output compare register to 255
		TCCR2B = (1<<CS22)|(1<<CS21);// Set TIMER2 to Normal Mode with 1:256 prescaler
		return;// Return to main
	}
}
ISR(PCINT3_vect) {
	count++;// Increment count
	if (count == 3) TCNT2 = 0;// If ISR executed 3 times, reset timer
	if (count == 4) {// If ISR executed 4 times
		dist = (TCNT2*.107);// Calculate distance
		OCR2B = 255;// Set output compare register to 255
		count = 0;// Reset count
		return;// Return to main
	}
	return;// Return to main
}
