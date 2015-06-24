/*
 * timer.c
 *
 * Created: 2015/6/19 22:01:25
 *  Author: 305031727
 */ 
#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include "timer.h"

timer_event EventList[TIMER0_NUM_CALLBACKS];
uint8_t CountDownTimers[TIMER0_NUM_COUNTDOWNTIMERS];

void Timer0_Init(void){
	uint8_t i;
	for(i=0;i<TIMER0_NUM_CALLBACKS;i++){
		EventList[i].pFunc = NULL;
		EventList[i].counter =0;
		EventList[i].event_intval = 0;
		
	}
	for(i=0;i<TIMER0_NUM_COUNTDOWNTIMERS;i++)
		CountDownTimers[i] = 0xff;
	TIMSK0 = _BV(OCIE0A);
	TCCR0A = _BV(WGM01);
	TCCR0B = 0x00;
	TCCR0B |= _BV(CS02)|_BV(CS00);
	
	OCR0A = 72;
	sei();
	
}

ISR(TIMER0_COMPA_vect){
	uint8_t i;
	static uint16_t tmp_counter=0;
	for(i=0;i<TIMER0_NUM_CALLBACKS;i++){
		if(EventList[i].pFunc != NULL){
			if(EventList[i].counter == EventList[i].event_intval){
				EventList[i].counter =0;
				EventList[i].pFunc();
			}
			else
				EventList[i].counter++;
		}
	}
	if(tmp_counter++ == 100){
		for(i=0;i<TIMER0_NUM_COUNTDOWNTIMERS;i++){
			if(CountDownTimers[i] != 0xff && CountDownTimers[i]!=0)
				CountDownTimers[i]--;
		}
		tmp_counter = 0;
	}
}

bool Timer0_RegisterCallBackFunction(TIMER_CALLBACK_FUNC pFunc, uint16_t intv_time){
	uint8_t i;
	if(intv_time<10) intv_time=10;
	for(i=0;i<TIMER0_NUM_CALLBACKS;i++){
		if(EventList[i].pFunc == pFunc)
		return TRUE;
	}
	for(i=0;i<TIMER0_NUM_CALLBACKS;i++){
		if(EventList[i].pFunc == NULL){
			EventList[i].pFunc = pFunc;
			EventList[i].event_intval = intv_time/10;
			EventList[i].counter=0;
			return TRUE;
		}
	}
	return FALSE;
}

bool Timer0_RemoveCallBackFunction(TIMER_CALLBACK_FUNC pFunc){
	uint8_t i;
	for(i=0;i<TIMER0_NUM_CALLBACKS;i++){
		if(EventList[i].pFunc == pFunc){
			EventList[i].pFunc = NULL;
			EventList[i].counter = 0;
			EventList[i].event_intval = 0;
			return TRUE;
		}
	}
	
	return FALSE;
	
}

uint8_t Timer0_AllocateCountdownTimer(void){
	uint8_t i;
	for(i=0;i<TIMER0_NUM_COUNTDOWNTIMERS;i++){
		if(CountDownTimers[i] == 0xff){
			CountDownTimers[i] = 0;
			return i+1;
		}
	}	
	return 0;
	
}

uint8_t Timer0_GetCountdownTimer(uint8_t timer){
	uint8_t t;
	cli();
	t=CountDownTimers[timer-1];
	sei();
	return t;
	
}

void Timer0_SetCountdownTimer(uint8_t timer, uint8_t countdown_value){
	if(CountDownTimers[timer-1]==0)
		CountDownTimers[timer-1] = countdown_value;
	
}

void Timer0_ReleaseCountdownTimer(uint8_t timer){
	cli();
	CountDownTimers[timer-1] = 0xff;
	sei();
	
}

