#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "characters.h"
//#include "PLL.h"
void initGPIO(void); //Configures Port B (segment control) and D (digit & LED control) for digital output. Port A (button input) for digital input
void initSystick(void); //Configures interrupts for quick periodic input state change checking.
void incrementChar(int8_t digit, int8_t *currentChar); //Increments the currentChar array in main() when called, digits 0-3 

int main(void) {
	int8_t currentChar[4]={ch_0,ch_1,ch_2,ch_3}; //Pins of current character for each digit, to be currently displayed. Pull definition from characters.h
	int8_t currentCharPos=digset_1; // Digits 1-4, pull definition from characters.h
	int8_t programmode=1; //0=program mode, 1=display programmed mode
	int8_t buttonsPrevVal[6]={0};
	int32_t buttonDataRPrev; //Port A (button) data register previous value,  to check if it has changed.
	int ticks=0; //Keeps track of systick countdowns every 5ms, useful for display mode when alternating between words
	initGPIO();
	initSystick();
	buttonDataRPrev=GPIO_PORTE_DATA_R;
	while(1) {
		if(GPIO_PORTE_DATA_R!=buttonDataRPrev) { //If any button state has changed
			if((GPIO_PORTE_DATA_R&but_1)!=buttonsPrevVal[0]) { //If button 1 state has changed
				if((GPIO_PORTE_DATA_R&but_1)==but_1) { //...and it's currently HIGH
					incrementChar(0,currentChar);
				}
				buttonsPrevVal[0]=GPIO_PORTE_DATA_R&ch_1;
			} //No else, just if to support multiple buons being pressed at once
			if((GPIO_PORTE_DATA_R&but_2)!=buttonsPrevVal[1]) { //If button 1 state has changed
				if((GPIO_PORTE_DATA_R&but_2)==but_2) { //...and it's currently HIGH
					incrementChar(1,currentChar);
				}
				buttonsPrevVal[1]=GPIO_PORTE_DATA_R&but_2;
			}
			if((GPIO_PORTE_DATA_R&but_3)!=buttonsPrevVal[2]) { //If button 1 state has changed
				if((GPIO_PORTE_DATA_R&but_3)==but_3) { //...and it's currently HIGH
					incrementChar(2,currentChar);
				}
				buttonsPrevVal[2]=GPIO_PORTE_DATA_R&but_3;
			}
			if((GPIO_PORTE_DATA_R&but_4)!=buttonsPrevVal[3]) { //If button 1 state has changed
				if((GPIO_PORTE_DATA_R&but_4)==but_4) { //...and it's currently HIGH
					incrementChar(3,currentChar);
				}
				buttonsPrevVal[3]=GPIO_PORTE_DATA_R&but_3;
			}			
			buttonDataRPrev=GPIO_PORTE_DATA_R;
		}
		
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
			GPIO_PORTD_DATA_R|=currentCharPos; //Set current digit high
			ticks++;
			if(programmode) { //are we in display mode or program mode?
				
			} else {
				if(ticks==200) { //if 1000ms has passed, display next
					
				}
			}
		}
	}
	return 0;
}

void initGPIO(void) {
	SYSCTL_RCGC2_R=0x1A; //Set 1011 for D, B, and E runmode clock enable
	for(int8_t op=0,count=0;count<4;count++)
		op+=1;
	//GPIOE configuration (Inputs)
	GPIO_PORTE_DIR_R&=~0x3F;
	GPIO_PORTE_DEN_R|=0x3F;
	GPIO_PORTE_AMSEL_R&=0x00;
	GPIO_PORTE_AFSEL_R&=0x00;
	GPIO_PORTE_PCTL_R&=~0x3F;
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

void incrementChar(int8_t digit, int8_t *currentChar) {
	int8_t currentCharHex=currentChar[digit];
	switch(currentCharHex) { //It's not as easy as doing currentCharHex[digit]++ because we need specific segment pins turned on
		case ch_0:
			currentChar[digit]=ch_1;
		break;
		case ch_1:
			currentChar[digit]=ch_2;
		break;
		case ch_2:
			currentChar[digit]=ch_3;
		break;
		case ch_3:
			currentChar[digit]=ch_4;
		break;
		case ch_4:
			currentChar[digit]=ch_5;
		break;
		case ch_5:
			currentChar[digit]=ch_6;
		break;
		case ch_6:
			currentChar[digit]=ch_7;
		break;
		case ch_7:
			currentChar[digit]=ch_8;
		break;
		case (int8_t)ch_8:
			currentChar[digit]=ch_9;
		break;
		case ch_9:
			currentChar[digit]=ch_a;
		break;
		case ch_a:
			currentChar[digit]=ch_b;
		break;
		case ch_b:
			currentChar[digit]=ch_c;
		break;
		case ch_c:
			currentChar[digit]=ch_d;
		break;
		case ch_d:
			currentChar[digit]=ch_e;
		break;
		case ch_e:
			currentChar[digit]=ch_f;
		break;
		case ch_f:
			currentChar[digit]=ch_g;
		break;
		case ch_g:
			currentChar[digit]=ch_h;
		break;
		case ch_h:
			currentChar[digit]=ch_i;
		break;
		case ch_i:
			currentChar[digit]=ch_j;
		break;
		case ch_j:
			currentChar[digit]=ch_k;
		break;
		case ch_k:
			currentChar[digit]=ch_l;
		break;
		case ch_l:
			currentChar[digit]=ch_m;
		break;
		case ch_m:
			currentChar[digit]=ch_n;
		break;
		case ch_n:
			currentChar[digit]=ch_o;
		break;
		case ch_o:
			currentChar[digit]=ch_p;
		break;
		case ch_p:
			currentChar[digit]=ch_r;
		break;
		/*case ch_q:
			currentChar[digit]=ch_r;
		break;*/
		case ch_r:
			currentChar[digit]=ch_t;
		break;
		/*case ch_s:
			currentChar[digit]=ch_t;
		break;*/
		case ch_t:
			currentChar[digit]=ch_u;
		break;
		case ch_u:
			currentChar[digit]=ch_y;
		break;		
		/*case ch_v:
			currentChar[digit]=ch_w;
		break;
		case ch_w:
			currentChar[digit]=ch_x;
		break;
		case ch_x:
			currentChar[digit]=ch_y;
		break;*/
		case ch_y:
			currentChar[digit]=0x00;
		break;
		case 0x00:
				currentChar[digit]=ch_0;
		/*case ch_z:
			currentChar[digit]=ch_0;
		break;*/
	}
}
