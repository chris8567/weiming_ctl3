/*
 * vdrop.c
 *
 * Created: 2015/6/19 23:48:14
 *  Author: 305031727
 */ 
#include "global.h"
#include "vdrop.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "vdrop.h"

void VDrop_Init(void){
	DDRD &=~_BV(1);
	PORTD |=_BV(1);
	EICRA = 0x00; EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	sei();
}



ISR(INT0_vect){
	cli();
	VDrop_Event();
	_delay_ms(2000);
}



void VDrop_Event(void){
	eeprom_busy_wait();
	eeprom_write_word((uint16_t *)EEPROM_DATA_START,0x0011);
	
}