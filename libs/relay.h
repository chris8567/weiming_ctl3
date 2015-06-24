/*
 * relay.h
 *
 * Created: 2015/6/22 15:26:17
 *  Author: 305031727
 */ 


#ifndef RELAY_H_
#define RELAY_H_

#include "global.h"
#include <avr/io.h>

#define RELAY_PORT PORTD
#define RELAY_DDR DDRD
#define RELAY_A 4
#define RELAY_B 5

void Relay_Init(void);
void Relay_Poll(void);
void Relay_BoundContrlRegs(uint8_t *ptr, uint16_t *reg);
void A_AutoDrop(void);
void V_AutoDrop(void);




#endif /* RELAY_H_ */