#include "PositionControl.h"

F32 Kp_Pos;
F32 ReferencePosition[3];

void InitPositionControlVariables (void) {
	U8 i;

	for (i=0; i<3; i++) {
		ReferencePosition[i] = 0.0;
	}

	Kp_Pos = 1.0;
}

void GoToReferencePosition (void) {

	DirectKinematics();

	//calculate and amplify the errors
	ReferenceGlobalSpeed[X] = Kp_Pos * (ReferencePosition[X] - Position[X]);
	ReferenceGlobalSpeed[Y] = Kp_Pos * (ReferencePosition[Y] - Position[Y]);
	ReferenceGlobalSpeed[G] = Kp_Pos * (ReferencePosition[G] - Position[G]);

	InverseKinematics();

	RotationalSpeedControl(M1);
	RotationalSpeedControl(M2);
	RotationalSpeedControl(M3);
}
