/*
 * weiming_ctl3.c
 *
 * Created: 2015/6/19 21:44:31
 *  Author: 305031727
 */ 

#include "libs/global.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include "libs/timer.h"
#include "libs/vdrop.h"
#include "libs/servo.h"
#include "libs/relay.h"
#include "mb.h"
#include "mbutils.h"


#define MB_BAUD 19200
uint8_t ucRegCoilsBuf[REG_COILS_SIZE/8];
uint16_t ucHoldingBuf[REG_HOLDING_SIZE];


void Sys_Init(void){
	uint8_t i;
	eeprom_busy_wait();
	if(eeprom_read_byte((uint8_t *)EEPROM_DATA_START)==0xee){
		ucRegCoilsBuf[0] = eeprom_read_byte((uint8_t *)EEPROM_DATA_START+1);
		for(i=0;i<8;i++)
			ucHoldingBuf[i]=eeprom_read_word((uint16_t *)(EEPROM_DATA_START+2+i*2));		
	}
	
	ucRegCoilsBuf[0] &= ~_BV(P_SERVO_AUTO_SWITCH);
	ucRegCoilsBuf[0] &=~_BV(P_AUTO_DROP_A);
	ucRegCoilsBuf[0] &=~_BV(P_AUTO_DROP_V);
	if (ucHoldingBuf[P_AUTO_DROP_A_T_C] ==0) ucHoldingBuf[P_AUTO_DROP_A_T_C]=2;
	if (ucHoldingBuf[P_AUTO_DROP_A_T_O] ==0) ucHoldingBuf[P_AUTO_DROP_A_T_O]=2;
	if (ucHoldingBuf[P_AUTO_DROP_V_T_C] ==0) ucHoldingBuf[P_AUTO_DROP_V_T_C]=2;
	if (ucHoldingBuf[P_AUTO_DROP_V_T_O] ==0) ucHoldingBuf[P_AUTO_DROP_V_T_O]=2;

	
	
	Timer0_Init();
	VDrop_Init();
	Servo_Init();
	Relay_Init();
	( void )eMBInit( MB_RTU, 12, 0, MB_BAUD, MB_PAR_EVEN);
	( void )eMBEnable( );
	
}

void USR_IO_Init(void){
	DDRD |= _BV(4)|_BV(5);
	PORTD |=_BV(4)|_BV(5);
	
}

int main(void)
{
	USR_IO_Init();
	Sys_Init();

	
    while(1)
    {
				( void )eMBPoll();
			
				Relay_Poll();
				Servo_Poll();
    }
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iNCoils = ( int )usNCoils;
	unsigned short  usBitOffset;
	
	/* Check if we have registers mapped at this block. */
	if( ( usAddress >= REG_COILS_START ) &&
	( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
	{
		usBitOffset = ( unsigned short )( usAddress - REG_COILS_START );
		switch ( eMode )
		{
			/* Read current values and pass to protocol stack. */
			case MB_REG_READ:
			while( iNCoils > 0 )
			{

				*pucRegBuffer++ =
				xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
				( unsigned char )( iNCoils >
				8 ? 8 :
				iNCoils ) );
				iNCoils -= 8;
				usBitOffset += 8;
			}
			break;
			
			/* Update current register values. */
			case MB_REG_WRITE:
			while( iNCoils > 0 )
			{
				xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
				( unsigned char )( iNCoils > 8 ? 8 : iNCoils ),
				*pucRegBuffer++ );
				iNCoils -= 8;
				usBitOffset += 8;
			}
			break;
		}
		
	}
	else
	{
		eStatus = MB_ENOREG;
	}
	return eStatus;
}





eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;
	short iNRegs = (short) usNRegs; 
	short Reg_offset;
	if((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START+REG_HOLDING_SIZE)){
		Reg_offset = usAddress - REG_HOLDING_START;
		switch (eMode) {
			case MB_REG_READ:
			while (iNRegs > 0) {
				*pucRegBuffer++ = (uint8_t)(ucHoldingBuf[Reg_offset]>>8);
				*pucRegBuffer++ = (uint8_t)(ucHoldingBuf[Reg_offset]&0xff);
				Reg_offset++; iNRegs--;
			}
			
			break;
			case MB_REG_WRITE:
			while (iNRegs>0) {
				ucHoldingBuf[Reg_offset] = *pucRegBuffer++ <<8;
				ucHoldingBuf[Reg_offset]|= *pucRegBuffer++;
				Reg_offset++;
				iNRegs--;
			}
			
			default:
			break;
		}
		
		
	}
	else{
		eStatus = MB_ENOREG;
	}
	
	return eStatus;
}



eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	return MB_ENORES;
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	return MB_ENOREG;
}