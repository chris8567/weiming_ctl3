/*
 * servo.c
 *
 * Created: 2015/6/22 15:57:15
 *  Author: 305031727
 */ 

#include "global.h"
#include "servo.h"
#include "timer.h"
#include <avr/io.h>
#include <util//delay.h>

uint16_t ServoPositionBuffer;
bool Servo_AutoFlag=FALSE;
uint16_t Servo_ScanLimit=0;

void Servo_Enable(void){
	SERVO_EN_PORT &=~_BV(SERVO_EN);
}

void Servo_Disable(void){
	SERVO_EN_PORT |= _BV(SERVO_EN);
}

void Servo_Init(void){
	ServoPositionBuffer =0;
	SERVO_EN_DDR |= _BV(SERVO_EN);
	SERVO_IO_DDR |= _BV(SERVO_CLK)|_BV(SERVO_CW);
	Servo_Enable();
}


uint16_t Servo_GetPosition(void){
	return ServoPositionBuffer;
}

void Servo_SetPosition(uint16_t NewPosition){
	if((NewPosition > SERVO_POSITION_UPPER_LIMIT)||(NewPosition == ServoPositionBuffer))
		return;
	_delay_ms(2);
	if(NewPosition>ServoPositionBuffer)
		Servo_SendPulse(NewPosition-ServoPositionBuffer,TRUE);
	else
		Servo_SendPulse(ServoPositionBuffer-NewPosition, FALSE);
	
	_delay_ms(2);
	
}

void Servo_SendPulse(uint16_t pulses, bool dir){
	uint16_t i;
	if(dir)
		SERVO_IO_PORT |= _BV(SERVO_CW);
	else
		SERVO_IO_PORT &= ~_BV(SERVO_CW);
	for(i=0;i<pulses;i++){
		SERVO_IO_PORT |= _BV(SERVO_CLK);
		_delay_us(SERVO_PUL_WIDTH);
		SERVO_IO_PORT &=~_BV(SERVO_CLK);
		_delay_ms(SERVO_PUL_WIDTH);
		if(dir)
			ServoPositionBuffer++;
		else
			ServoPositionBuffer--;
	}
			
}
	


void Servo_Poll(void){

	if(ucRegCoilsBuf[0] & _BV(P_SERVO_AUTO_SWITCH)){
		if((Servo_AutoFlag==FALSE)&(&ucHoldingBuf[P_SERVO_POSITION_REG]!=0)){
			Servo_ScanLimit = ucHoldingBuf[P_SERVO_POSITION_REG];
			Timer0_RegisterCallBackFunction(Servo_AutoScan,ucHoldingBuf[P_SERVO_SCANSPEED_REG]*10);
			Servo_AutoFlag = TRUE;
		}
	}
	else{
		if(Servo_AutoFlag==TRUE)
			Servo_AutoFlag = FALSE;
			Timer0_RemoveCallBackFunction(Servo_AutoScan);
			ucHoldingBuf[P_SERVO_POSITION_REG] = Servo_ScanLimit;
	}
	Servo_SetPosition(ucHoldingBuf[P_SERVO_POSITION_REG]);
	
}

void Servo_AutoScan(void){
		static bool directions = 0;
		static bool pulse = TRUE;
		if(pulse){
			SERVO_IO_PORT |= _BV(SERVO_CLK);
			pulse = FALSE;
		}
		else
		{
			SERVO_IO_PORT &= ~(_BV(SERVO_CLK));
			pulse = TRUE;
		}
		if (pulse) {
			
			if (directions){
				SERVO_IO_PORT |= (_BV(SERVO_CW));
				if(ServoPositionBuffer < Servo_ScanLimit)
				ServoPositionBuffer++;
				
				else
				directions = 0;
			}
			else{
				SERVO_IO_PORT &= ~(_BV(SERVO_CW));
				if (ServoPositionBuffer > 0) {
					ServoPositionBuffer --;
				}
				else
				directions = 1;
			}
			ucHoldingBuf[P_SERVO_POSITION_REG]=ServoPositionBuffer;
		}	
	
}
