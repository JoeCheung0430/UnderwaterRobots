#ifndef __SEND_YPR_H
#define __SEND_YPR_H

#include "usart.h"
#include "delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#define MAG_ADDRESS   0x0C              //¥≈≥°µÿ÷∑
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08

void Get_ypr(void);
void Update_Magnetometer(void);
u8 Mpu_Init(u8 mode);
extern int16_t Direction;								// Compass Direction
extern short T_X,T_Y,T_Z;		 			    	//Compass X,Y,Z÷·
extern float Pitch,Roll,Yaw;
#endif

