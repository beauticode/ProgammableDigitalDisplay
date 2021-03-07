#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "characters.h"
//#include "PLL.h"
void initGPIO(void); //Configures Port B (segment control) and D (digit & LED control) for digital output. Port A (button input) for digital input
void initSystick(void); //Configures interrupts for quick periodic input state change checking.


int main(void) {
	int8_t currentChar[4]={ch_2,ch_3,ch_1,0xff}; //Pins of current character for each digit, to be currently displayed. Pull definition from characters.h
	int8_t currentCharPos=digset_1; // Digits 1-4, pull definition from characters.h
	int8_t displaymode=0; //0=program mode, 1=display programmed mode
	int8_t ticks=0; //Keeps track of systick countdowns every 5ms, useful for display mode when alternating between words
	//PLL_Init();
	initGPIO();
	initSystick();
	/*GPIO_PORTD_DATA_R&=digclr_2;
	GPIO_PORTD_DATA_R|=digset_2;//Debug line: Display 1 on digit 1
	GPIO_PORTB_DATA_R|=ch_3; //Debug line continued*/
	while(1) {
		if((NVIC_ST_CTRL_R & sysTickFlag)==sysTickFlag) { //If the flag is set (5ms has passed)
			if((currentCharPos&(int8_t)digset_1)==(int8_t)digset_1) { //Change digit positionn
				GPIO_PORTD_DATA_R&=digclr; //Clear current digit pins, but not all bc LED may be high
				currentCharPos=digset_2; //Set next digit
				//Actual GPIOD data register will be set further down
				GPIO_PORTB_DATA_R&=~GPIO_PORTB_DATA_R; //Clear current segments (all HIGH are fine, since entire PORTB is dedicated to segments)
				GPIO_PORTB_DATA_R|=currentChar[1]; //Set segments designated for 2nd character
			} else if((currentCharPos&(int8_t)digset_2)==(int8_t)digset_2) {
				GPIO_PORTD_DATA_R&=digclr;
				currentCharPos=digset_3;					
				GPIO_PORTB_DATA_R&=~GPIO_PORTB_DATA_R;
				GPIO_PORTB_DATA_R|=currentChar[2];
			} else if((currentCharPos&(int8_t)digset_3)==(int8_t)digset_3) {
				GPIO_PORTD_DATA_R&=digclr;
				currentCharPos=digset_4;			
				GPIO_PORTB_DATA_R&=~GPIO_PORTB_DATA_R;
				GPIO_PORTB_DATA_R|=currentChar[3];				
			} else if((currentCharPos&(int8_t)digset_4)==(int8_t)digset_4) {
				GPIO_PORTD_DATA_R&=digclr;
				currentCharPos=digset_1;		
				GPIO_PORTB_DATA_R&=~GPIO_PORTB_DATA_R;
				GPIO_PORTB_DATA_R|=currentChar[0];
			}
			if(!displaymode) { //are we in display mode?
				
			} else {
				
			}
			GPIO_PORTD_DATA_R|=currentCharPos; //Set current digit high
			ticks++;
		}
	}
	return 0;
}

void initGPIO(void) {
	SYSCTL_RCGC2_R=0x0B; //Set 1011 for D, B, and A runmode clock enable
	for(int8_t op=0,count=0;count<4;count++)
		op+=1;
	//GPIOA configuration (Inputs)
	GPIO_PORTA_DIR_R&=~0xFC;
	GPIO_PORTA_DEN_R|=0xFC;
	GPIO_PORTA_AMSEL_R&=0x00;
	GPIO_PORTA_AFSEL_R&=0x00;
	GPIO_PORTA_PCTL_R&=~0xFC;
	//GPIOB configuration (Segments)
	GPIO_PORTB_DIR_R|=0xFF;
	GPIO_PORTB_DEN_R|=0xFF;
	GPIO_PORTB_AMSEL_R&=0x00;
	GPIO_PORTB_AFSEL_R&=0x00;
	GPIO_PORTB_PCTL_R&=0x00;
	//GPIOD configuration (digits & LED)
	GPIO_PORTD_DIR_R|=0xcf; //All output except for 4 and 5, which are unavailable for GPIO
	GPIO_PORTD_DEN_R|=0xcf;
	GPIO_PORTD_AMSEL_R&=0x00;
	GPIO_PORTD_AFSEL_R&=0x00;
	GPIO_PORTD_PCTL_R&=~0x9f;
}

void initSystick(void) {
	NVIC_ST_CTRL_R=0;
	NVIC_ST_RELOAD_R=sysTickReload_Multiplex;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R|=0x05; //no interrupt, just checking time
}

/*void SysTick_Handler(void) {
	
}*/

