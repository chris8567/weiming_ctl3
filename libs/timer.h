/*
 * timer.h
 *
 * Created: 2015/6/19 21:51:13
 *  Author: 305031727
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#include "global.h"

typedef void (*TIMER_CALLBACK_FUNC)(void);

#define TIMER0_NUM_CALLBACKS 4
#define TIMER0_NUM_COUNTDOWNTIMERS 4

typedef struct Event_Struct{
	TIMER_CALLBACK_FUNC pFunc;
	uint16_t event_intval;
	uint16_t counter;
	}timer_event;

void Timer0_Init(void);
bool Timer0_RegisterCallBackFunction(TIMER_CALLBACK_FUNC pFunc,uint16_t intv_time);
bool Timer0_RemoveCallBackFunction(TIMER_CALLBACK_FUNC pFunc);

uint8_t Timer0_AllocateCountdownTimer(void);
uint8_t Timer0_GetCountdownTimer(uint8_t timer);
void Timer0_SetCountdownTimer(uint8_t timer, uint8_t countdown_value);
void Timer0_ReleaseCountdownTimer(uint8_t timer);
void Timer0_Event(void);



#endif /* TIMER_H_ */ 