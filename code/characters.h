/*
Copyright © 2021, Gary T Smith.
All rights reserved.
*/
//by Gary T Smith: diygary.com/[link] or github.com/beauticode
#ifndef CHARACTERS_H
#define CHARACTERS_H

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
#define digclr 0xb1 //Clears all digit bits in use (except LED and reserved pins)
//-----------------------------

/*
LED (Port D): 
*/
//-----------------------------
#define LEDbit 0x01
//-----------------------------

/*
Segments (Port B):
7  6 5 4 3 2 1 0
dp g f e d c b a
*/
//-----------------------------
#define ch_0 0x3f //10111110
#define ch_1 0x06 //00000110
#define ch_2 0x5b  //01011011
#define ch_3 0x4f //01001111
#define ch_4 0x66
#define ch_5 0x6d
#define ch_6 0x7d
#define ch_7 0x07
#define ch_8 0xff
#define ch_9 0x67
#define ch_a 0x5f
#define ch_b 0x7c
#define ch_c 0x39
#define ch_d 0x5e
#define ch_e 0x79
#define ch_f 0x71
#define ch_g 0x6f
#define ch_h 0x74
#define ch_i 0x30
#define ch_j 0x1e
#define ch_k 0x76 //kinda not rly supported. use h
#define ch_l 0x38
#define ch_m 0x54 //not rly supported
#define ch_n 0x37
#define ch_o 0x5c
#define ch_p 0x73
#define ch_q 0x67 //not rly supported. use 9 
#define ch_r 0x77
#define ch_s 0x6d //not rly supported. use 5
#define ch_t 0x78
#define ch_u 0x3e
#define ch_v 0x3e //kinda not rly supported. use u
#define ch_w 0x3e //not rly supported 
#define ch_x 0x76 //not rly supported
#define ch_y 0x7a
#define ch_z 0x5b //not rly supported
//-----------------------------

/*
Buttons (Port E):
  7    6          5        4       3    2     1     0       
[na] [na]     playstop progerase but_4 but_3 but_2 but_1 
*/
//-----------------------------
#define but_1 0x01 
#define but_2 0x02  
#define but_3 0x04 
#define but_4 0x08 
#define but_progerase 0x10
#define but_playstop 0x20
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


#define maxSets 10


#endif