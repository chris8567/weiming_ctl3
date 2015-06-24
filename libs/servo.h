/*
 * servo.h
 *
 * Created: 2015/6/22 15:46:21
 *  Author: 305031727
 */ 


#ifndef SERVO_H_
#define SERVO_H_
#define SERVO_EN_PORT PORTB
#define SERVO_EN_DDR DDRB
#define SERVO_EN 3
#define SERVO_IO_PORT PORTD
#define SERVO_IO_DDR DDRD
#define SERVO_CW 7
#define SERVO_CLK 6
#define SERVO_POSITION_UPPER_LIMIT 2860
#define SERVO_PUL_WIDTH 600

void Servo_Init(void);
void Servo_Poll(void);
uint16_t Servo_GetPosition(void);
void Servo_SetPosition(uint16_t NewPosition);
void Servo_SendPulse(uint16_t pulses, bool dir);
void Servo_AutoScan(void);





#endif /* SERVO_H_ */