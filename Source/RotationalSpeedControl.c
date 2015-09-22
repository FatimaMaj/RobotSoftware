#include "RotationalSpeedControl.h"
#include <stdio.h>

//---Variables for RotationalSpeedControl----------------------------
F32 u;

F32 ReferenceRotationalSpeed[3];
F32 MeasuredRotationalSpeed[3];

F32 e[3];
F32 esum[3];
F32 Kp[3];
F32 Ki[3];
//-------------------------------------------------------------------

void InitRotationalSpeedControlVariables (void) {
	U8 i = 0;
	
	u = 0.0;

	for (i=0; i<3; i++) {
		ReferenceRotationalSpeed[i] = 0.0;
		MeasuredRotationalSpeed[i] = 0.0;

		e[i] = 0.0;
		esum[i] = 0.0;
		Kp[i] = 0.25;
		Ki[i] = 3.0;
	}
}

void RotationalSpeedControl (U8 Motor) {

	// e = r - y
	// u = Kp * e + Ki * Integral(e,dt)
	
	e[Motor] = ReferenceRotationalSpeed[Motor] - MeasuredRotationalSpeed[Motor];
//printf ("\nemotor : %d\n",e[Motor]);
//printf ("\nCharacter entered : %c",inputcharacter);

	u = Kp[Motor] * e[Motor] + Ki[Motor] * esum[Motor];

	if(((u < 1.0) || (e[Motor] < 0.0)) && ((u > -1.0) || (e[Motor] > 0.0))) { //Anti-Wind-Up
		esum[Motor] = esum[Motor] + e[Motor] * SAMPLING_TIME; //Integration
		u = Kp[Motor] * e[Motor] + Ki[Motor] * esum[Motor];
	}

	if (u >  1.0) u =  1.0;
	if (u < -1.0) u = -1.0;

	switch (Motor) {
		case M1: {
			if (u < 0) {
				PCA0CPH3 = PWM_MIN - (U8)( -u * (F32)PWM_MIN );
				DIRECTION_M1 = 1;
			}
			else {
				PCA0CPH3 = PWM_MIN - (U8)(  u * (F32)PWM_MIN );
				DIRECTION_M1 = 0;
			}
			break;
		}
		case M2: {
			if (u < 0) {
				PCA0CPH4 = PWM_MIN - (U8)( -u * (F32)PWM_MIN );
				DIRECTION_M2 = 1;
			}
			else {
				PCA0CPH4 = PWM_MIN - (U8)(  u * (F32)PWM_MIN );
				DIRECTION_M2 = 0;
			}
			break;
		}
		case M3: {
			if (u < 0) {
				PCA0CPH5 = PWM_MIN - (U8)( -u * (F32)PWM_MIN );
				DIRECTION_M3 = 1;
			}
			else {
				PCA0CPH5 = PWM_MIN - (U8)(  u * (F32)PWM_MIN );
				DIRECTION_M3 = 0;
			}
			break;
		}
		default: {
			PCA0CPH3 = PWM_MIN;
			PCA0CPH4 = PWM_MIN;
			PCA0CPH5 = PWM_MIN;
		}
	}
}
