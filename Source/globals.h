#ifndef GLOBALS_H
#define GLOBALS_H

#include "compiler_defs.h"
#include "C8051F580_defs.h"
#include "float.h"
#include "RotationalSpeedControl.h"


//// Definition of Global Variables
//Generic
#define TRUE   1								//Definition of TRUE
#define FALSE  0								//Definition of FALSE

//---MCU parameters--------------------------------------------------
#define SYSCLK	24000000//Hz
#define TIMER2_INTERVALL 0.01//s
//-------------------------------------------------------------------

//---OmniRobot parameters--------------------------------------------
//#define RADIUS_ROBOT 0.139 //distance of robot center to wheels
//#define INVERSE_RADIUS_ROBOT 7.19424

#define RADIUS_ROBOT 0.173 //distance of robot center to wheels
#define INVERSE_RADIUS_ROBOT 5.78035
//-------------------------------------------------------------------

//---Defines and variables for rotational speed-measurement----------
#define PCA0_TIMEBASE ( (F32)1 / ( (F32)SYSCLK / (F32)12 ) )//[t] = s
#define	WHEEL_CIRCUMFERENCE ( (F32)(0.16) )					//[U] = m
#define GEAR_RATIO ( (F32)10.4 )
#define SECONDS_PER_MINUTE ( (F32)(60) )					//[c] = s/h
#define SECONDS_PER_HOUR ( (F32)(3600) )					//[c] = s/h
#define ENCODER_TICKS_PER_REVOLUTION ( (F32)(144) )
//---For rounds per second-------------------------------------------
#define	SPEEDFACTOR_RPS ( (F32)( 1.0 / ( ENCODER_TICKS_PER_REVOLUTION * GEAR_RATIO * PCA0_TIMEBASE ) ) )
#define RPS(PCA0_Ticks) ( (F32)(SPEEDFACTOR_RPS) / (F32)(PCA0_Ticks) )
//---Motor 1---------------------------------------------------------
#define M1 0
extern U32	PCA0CP0_Counts;
extern UU32	PCA0CP0_Count1;
extern UU32	PCA0CP0_Count2;
extern U8	PCA0CP0_EdgeCount;
//-------------------------------------------------------------------
//---Motor 2---------------------------------------------------------
#define M2 1
extern U32	PCA0CP1_Counts;
extern UU32	PCA0CP1_Count1;
extern UU32	PCA0CP1_Count2;
extern U8	PCA0CP1_EdgeCount;
//-------------------------------------------------------------------
//---Motor 3---------------------------------------------------------
#define M3 2
extern U32	PCA0CP2_Counts;
extern UU32	PCA0CP2_Count1;
extern UU32	PCA0CP2_Count2;
extern U8	PCA0CP2_EdgeCount;
//-------------------------------------------------------------------
extern U16	PCA0_Overflow;
//-------------------------------------------------------------------

//---Defines for Commissioning_FSM-----------------------------------
#define SPI_TEST		0
#define CONSTANT_PWM	1
#define ZERO_SPEED		2
#define CONSTANT_SPEED	3
#define DRIVE_CIRCLE	4
#define CLEAR_POSITION	5
#define GOTO_POSITION	6
/********************************************************************
 * 																	
 *			Date		: 	01.02.2011								
 *			Author		:	Haydar Al Attar							
 *			Institution	:	Kristianstad University					
 *
 *			Updated by	: Christian Bodelsson
 *			Date		: 	22-09-2012																	
 *																	
 *******************************************************************/
#define FORWARD	7
#define RIGHT	8
#define LEFT	9
#define BACK	10
#define Forward_max	11
#define Forward_min	12


#define Forward_minR	13
#define Forward_minL	14

#define Forward_maxR	15
#define Forward_maxL	16
#define STOP	17
#define Back_min	18
#define ROTATE	19
#define Rotate_min	20
#define Left_min	21
#define Right_min	22
/*******************************************************************/

//-------------------------------------------------------------------

//---Defines and variables for digital converted analog signals------
#define START_AD_CONVERSION		AD0BUSY = 1
#define ADC_CHANNEL				ADC0MX
#define CHANNEL_I_MOTOR1		0x00
#define CHANNEL_I_MOTOR2		0x01
#define CHANNEL_I_MOTOR3		0x02
#define CHANNEL_BATTERY_VOLTAGE	0x03
extern F32 MeasuredMotorCurrent[3];
extern F32 MeasuredBatteryVoltage;
//-------------------------------------------------------------------

//---Function Prototypes---------------------------------------------
void InitGlobalVariables (void);
void GetRotationalSpeeds (void);
void PrepareNextRotationalSpeedMeasurement (void);
//-------------------------------------------------------------------

//Controller-Hardware
SBIT(LED, SFR_P2, 0);							//Akku-LED
SBIT(START, SFR_P2, 1);							//Start-Button
SBIT(DIRECTION_M1, SFR_P2, 3);					//direction specifications for motor 1 from controller
SBIT(DIRECTION_M2, SFR_P2, 4);					//direction specifications for motor 2 from controller
SBIT(DIRECTION_M3, SFR_P2, 5);					//direction specifications for motor 3 from controller
SBIT(ENABLE, SFR_P2, 6);						//Enable for H-Bridges

SBIT(DIR1B0, SFR_P3, 0);						//Direction Signal from CPLD  of engine 1 bit0
SBIT(DIR1B1, SFR_P3, 1);						//Direction Signal from CPLD  of engine 1 bit1
SBIT(DIR2B0, SFR_P3, 2);						//Direction Signal from CPLD  of engine 2 bit0
SBIT(DIR2B1, SFR_P3, 3);						//Direction Signal from CPLD  of engine 2 bit1
SBIT(DIR3B0, SFR_P3, 4);						//Direction Signal from CPLD  of engine 3 bit0
SBIT(DIR3B1, SFR_P3, 5);						//Direction Signal from CPLD  of engine 3 bit1

//SBIT(CPLDRESET, SFR_P4, 7);

SBIT(NSS4, SFR_P1, 7);

/*
void OnOff(void);
*/


#endif //GLOBALS_H
