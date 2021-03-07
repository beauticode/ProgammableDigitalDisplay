#ifndef CHARACTERS_H
#define CHARACTERS_H
//by Gary T Smith: diygary.com/[link] or github.com/beauticode


/*
Digits (Port D): For OR operations, leaves LED bit untouched (0)
(Active low)
*/
//-----------------------------
#define digset_4 0xfc //basically equivalent to ~0x03, LED bit 0 (meant for OR), PD1 bit 0.
#define digset_3 0xfa 
#define digset_2 0xf6
#define digset_1 0xbe
//-----------------------------

/*
Digits (Port D): For AND operations, leaves LED bit untouched (1)
(Active low)
*/
//-----------------------------
#define digclr 0xb1 //Clears all digit bits in use
//-----------------------------


/*
Pins(Port B):
7  6 5 4 3 2 1 0
dp g f e d c b a
*/
//-----------------------------
#define ch_1 0x06 //00000110
#define ch_2 0x5b  //01011011
#define ch_3 0x4f //01001111
//-----------------------------


/*
Systick reload value for 5ms delay between each digit cycle, used for multiplexing
(5*10^(-3))*(16*10^(6)) - 1 = 2^n - 1 = 79999
Systick end-of-countdown flag bit
*/
//-----------------------------
#define sysTickReload_Multiplex 79999
#define sysTickFlag 0x10000
//-----------------------------


#endif