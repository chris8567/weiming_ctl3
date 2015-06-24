/*
 * vdrop.h
 *
 * Created: 2015/6/19 23:45:38
 *  Author: 305031727
 */ 


#ifndef VDROP_H_
#define VDROP_H_
#include <avr/io.h>
#define EEPROM_DATA_START 0x00

void VDrop_Init(void);

void VDrop_Event(void);
 



#endif /* VDROP_H_ */