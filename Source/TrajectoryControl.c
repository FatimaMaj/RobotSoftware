#include "TrajectoryControl.h"

F32 Omega;
F32 Time;

void InitTrajectoryControlVariables (void) {
	Omega = PI/4;
	Time = 0.0;
}

void Trajectory1 (void) { //simply drive forward
	ReferenceGlobalSpeed[X] = 0.5;
	ReferenceGlobalSpeed[Y] = 0.0;
	ReferenceGlobalSpeed[G] = 0.0;
}

void Trajectory2 (void) { //drive in a circle, one direction
	Time = Time + SAMPLING_TIME;
	
	ReferenceGlobalSpeed[X] = 0.4 * cosf(Omega * Time);
	ReferenceGlobalSpeed[Y] = 0.4 * sinf(Omega * Time);
	ReferenceGlobalSpeed[G] = 0.0;
}
