/*
 * relay.c
 *
 * Created: 2015/6/22 15:29:34
 *  Author: 305031727
 */ 

#include "global.h"
#include "relay.h"
#include "timer.h"
#include <avr/io.h>


void Relay_Init(void){
	RELAY_DDR |= _BV(RELAY_A)|_BV(RELAY_B);
	RELAY_PORT |= _BV(RELAY_A)|_BV(RELAY_B);
}


void Relay_Poll(void){
	uint8_t Relay_A_Switch =  ucRegCoilsBuf[0] & _BV(P_RELAY_A_SW);
	uint8_t Relay_B_Switch = ucRegCoilsBuf[0] & _BV(P_RELAY_B_SW);
	if(Relay_A_Switch)
		RELAY_PORT &= ~_BV(RELAY_A);
	else
		RELAY_PORT |= _BV(RELAY_B);
	
	if(Relay_B_Switch)
		RELAY_PORT &= ~_BV(RELAY_B);
	else
		RELAY_PORT |= _BV(RELAY_B);
		
	if(ucRegCoilsBuf[0] & _BV(P_AUTO_DROP_A))
		Timer0_RegisterCallBackFunction(A_AutoDrop,1000);
	else
		Timer0_RemoveCallBackFunction(A_AutoDrop);
		
	if(ucRegCoilsBuf[0] & _BV(P_AUTO_DROP_V))
		Timer0_RegisterCallBackFunction(V_AutoDrop,1000);
	else
		Timer0_RemoveCallBackFunction(V_AutoDrop);
	
}

void A_AutoDrop(void){
	static uint8_t count=0;
	if(count++<= ucHoldingBuf[P_AUTO_DROP_A_T_C])
		RELAY_PORT &= ~_BV(RELAY_A);
	else if(count<=ucHoldingBuf[P_AUTO_DROP_A_T_C]+ucHoldingBuf[P_AUTO_DROP_A_T_O])
		RELAY_PORT |= _BV(RELAY_A);
	else 
		count =0;
}

void V_AutoDrop(void){
	static uint8_t count=0;
	if(count++<=ucHoldingBuf[P_AUTO_DROP_A_T_C])
		RELAY_PORT &= ~_BV(RELAY_B);
	else if(count <= ucHoldingBuf[P_AUTO_DROP_V_T_C]+ucHoldingBuf[P_AUTO_DROP_V_T_O])
		RELAY_PORT |= _BV(RELAY_B);
	else
		count =0;
	
}

