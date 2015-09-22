#include "Kinematics.h"

U8 i;

F32 SpeedRobot[3];
F32 SpeedGlobal[3];
F32 ReferenceGlobalSpeed[3];

F32 Position[3];

U8 NormalizeVector;

F32 NormSquare;
F32 Norm;

void InitKinematicsVariables (void) {

	for (i=0; i<3; i++) {
		SpeedRobot[i] = 0.0;
		SpeedGlobal[i] = 0.0;
		Position[i] = 0.0;
	}

	NormalizeVector = FALSE;

	NormSquare = 0.0;
	Norm = 0.0;

}

//---calculate global speed vector from measured wheel speeds--------
void DirectKinematics (void) {

	//--- V_R = C^-1 * V_w ---
	SpeedRobot[X] = IC_11 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M1]
				  + IC_12 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M2]
				  + IC_13 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M3];
	
	SpeedRobot[Y] = IC_21 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M1]
				  + IC_22 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M2]
				  + IC_23 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M3];
	
	SpeedRobot[G] = IC_31 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M1]
				  + IC_32 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M2]
				  + IC_33 * WHEEL_CIRCUMFERENCE * MeasuredRotationalSpeed[M3];

	//--- V_G = T^-1 * V_R ---
	SpeedGlobal[X] =  cosf(Position[G]) * SpeedRobot[X]
					- sinf(Position[G]) * SpeedRobot[Y];
	
	SpeedGlobal[Y] =  sinf(Position[G]) * SpeedRobot[X]
					+ cosf(Position[G]) * SpeedRobot[Y];
	
	SpeedGlobal[G] = SpeedRobot[G];

	//--- s = Integral(v, dt); gamma = Integral(w, dt)---
	Position[X] = Position[X] + SpeedGlobal[X] * SAMPLING_TIME;
	Position[Y] = Position[Y] + SpeedGlobal[Y] * SAMPLING_TIME;
	Position[G] = Position[G] + SpeedGlobal[G] * SAMPLING_TIME;
}
//-------------------------------------------------------------------

//---calculate reference wheel speeds for a given--------------------
//---global speed vector---------------------------------------------
void InverseKinematics (void) {
	
	//--- V_R = T * V_G ---
	SpeedRobot[X] =  cosf(Position[G]) * ReferenceGlobalSpeed[X]
				   + sinf(Position[G]) * ReferenceGlobalSpeed[Y];
	
	SpeedRobot[Y] = -sinf(Position[G]) * ReferenceGlobalSpeed[X]
				   + cosf(Position[G]) * ReferenceGlobalSpeed[Y];
	
	SpeedRobot[G] = ReferenceGlobalSpeed[G];

	//--- V_w = C * V_R ---
	ReferenceRotationalSpeed[M1] = C_11 * SpeedRobot[X] + C_12 * SpeedRobot[Y] + C_13 * SpeedRobot[G];
	ReferenceRotationalSpeed[M2] = C_21 * SpeedRobot[X] + C_22 * SpeedRobot[Y] + C_23 * SpeedRobot[G];
	ReferenceRotationalSpeed[M3] = C_31 * SpeedRobot[X] + C_32 * SpeedRobot[Y] + C_33 * SpeedRobot[G];

	ReferenceRotationalSpeed[M1] = (1 / WHEEL_CIRCUMFERENCE) * ReferenceRotationalSpeed[M1];
	ReferenceRotationalSpeed[M2] = (1 / WHEEL_CIRCUMFERENCE) * ReferenceRotationalSpeed[M2];
	ReferenceRotationalSpeed[M3] = (1 / WHEEL_CIRCUMFERENCE) * ReferenceRotationalSpeed[M3];

	//--- Normalize ReferenceRotationalSpeed Vector if needed ---
	NormalizeVector = FALSE;
	for (i=0; i<3; i++) {
		if (ReferenceRotationalSpeed[i] > MAX_ROTATIONAL_SPEED) {
			NormalizeVector = TRUE;
			break;
		}
	}

	if (NormalizeVector == TRUE) {
		
		NormSquare = 0.0;
		
		for (i=0; i<3; i++) {
			NormSquare = NormSquare + ReferenceRotationalSpeed[i] * ReferenceRotationalSpeed[i];
		}
	
		Norm = sqrtf(NormSquare);
	
		for (i=0; i<3; i++) {
			ReferenceRotationalSpeed[i] = MAX_ROTATIONAL_SPEED * ReferenceRotationalSpeed[i] / Norm;
		}
	}
}
//-------------------------------------------------------------------