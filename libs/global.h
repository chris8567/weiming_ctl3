/*
 * global.h
 *
 * Created: 2015/6/19 22:37:08
 *  Author: 305031727
 */ 


#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifndef F_CPU 
#define F_CPU 7372800UL
#endif
#include <avr/io.h>
#include <stddef.h>

#ifndef bool
typedef uint8_t bool;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif
//the modbus register sizes:
#define REG_COILS_START 1000
#define REG_COILS_SIZE      128
#define REG_HOLDING_START 1000
#define REG_HOLDING_SIZE	32

//positions of each data in registers:
#define P_RELAY_A_SW 1
#define P_RELAY_B_SW 2

#define P_SERVO_AUTO_SWITCH 3
#define P_AUTO_DROP_V 4
#define P_AUTO_DROP_A 5

#define P_AUTO_DROP_V_T_C 3
#define P_AUTO_DROP_V_T_O 4
#define P_AUTO_DROP_A_T_C 5
#define P_AUTO_DROP_A_T_O 6

#define P_SERVO_SCANSPEED_REG 2
#define P_SERVO_POSITION_REG 1

extern uint8_t ucRegCoilsBuf[REG_COILS_SIZE/8];
extern uint16_t ucHoldingBuf[REG_HOLDING_SIZE];


#endif /* GLOBAL_H_ */