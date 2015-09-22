#ifndef ROTATIONAL_SPEED_CONTROL_H
#define ROTATIONAL_SPEED_CONTROL_H

#include "globals.h"
#include "float.h"

#define SAMPLING_TIME TIMER2_INTERVALL
#define MAX_ROTATIONAL_SPEED 9.0

//---Defines for PWM generation--------------------------------------
#define THROTTLE_P_MAX	 32767
#define THROTTLE_N_MAX	-32768
#define PWM_MAX	0x00	//PWM register value for maximal duty cycle
#define	PWM_MIN	0xFF	//PWM register value for minimal duty cycle
//-------------------------------------------------------------------

//---Function prototypes---------------------------------------------
void InitRotationalSpeedControlVariables (void);
void RotationalSpeedControl (U8 Motor);
//-------------------------------------------------------------------

//---Variables for RotationalSpeedControl----------------------------
extern F32 ReferenceRotationalSpeed[3];
extern F32 MeasuredRotationalSpeed[3];
//-------------------------------------------------------------------

#endif //ROTATIONAL_SPEED_CONTROL_H
