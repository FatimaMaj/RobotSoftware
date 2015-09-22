#include "globals.h"
#include <string.h>

//---Variables for rotational speed-measurement----------------------
//---Motor 1---------------------------------------------------------
U32		PCA0CP0_Counts;
UU32	PCA0CP0_Count1;
UU32	PCA0CP0_Count2;
U8		PCA0CP0_EdgeCount;
//-------------------------------------------------------------------
//---Motor 2---------------------------------------------------------
U32		PCA0CP1_Counts;
UU32	PCA0CP1_Count1;
UU32	PCA0CP1_Count2;
U8		PCA0CP1_EdgeCount;
//-------------------------------------------------------------------
//---Motor 3---------------------------------------------------------
U32		PCA0CP2_Counts;
UU32	PCA0CP2_Count1;
UU32	PCA0CP2_Count2;
U8		PCA0CP2_EdgeCount;
//-------------------------------------------------------------------
U16		PCA0_Overflow;
//-------------------------------------------------------------------

//---Variables for motor current and battery voltage measurement-----
F32 MeasuredMotorCurrent[3];
F32 MeasuredBatteryVoltage;
//-------------------------------------------------------------------

void InitGlobalVariables (void) {
	U8 i;

	//---Variables for rotational speed-measurement----------------------
	//---Motor 1---------------------------------------------------------
	PCA0CP0_Counts = 0;
	PCA0CP0_Count1.U32 = 0;
	PCA0CP0_Count2.U32 = 0;
	PCA0CP0_EdgeCount = 0;
	//-------------------------------------------------------------------
	//---Motor 2---------------------------------------------------------
	PCA0CP1_Counts = 0;
	PCA0CP1_Count1.U32 = 0;
	PCA0CP1_Count2.U32 = 0;
	PCA0CP1_EdgeCount = 0;
	//-------------------------------------------------------------------
	//---Motor 3---------------------------------------------------------
	PCA0CP2_Counts = 0;
	PCA0CP2_Count1.U32 = 0;
	PCA0CP2_Count2.U32 = 0;
	PCA0CP2_EdgeCount = 0;
	//-------------------------------------------------------------------
	PCA0_Overflow = 0;
	//-------------------------------------------------------------------

	//---Variables for motor current and battery voltage measurement-----
	for (i=0; i<3; i++) {
		MeasuredMotorCurrent[i] = 0.0;
	}

	MeasuredBatteryVoltage = 0.0;
	//-------------------------------------------------------------------
}

void GetRotationalSpeeds (void) {
	//Calculate 'rotational speed'
	if (!(PCA0CPM0 & 0x01)) {	//if the CCF0 interrupt is disabled,
		//the corresponding rotational speed can be calculated

		if(DIR1B1==1 && DIR1B0==0) { //Rotating Direction Information from CPLD
			//Rotating Direction Motor1: Counter-Clockwise
			MeasuredRotationalSpeed[M1] = -RPS(PCA0CP0_Count2.U32 - PCA0CP0_Count1.U32);
		}
		else if(DIR1B1==0 && DIR1B0==1){
			//Rotating Direction Motor1: Clockwise
			MeasuredRotationalSpeed[M1] = RPS(PCA0CP0_Count2.U32 - PCA0CP0_Count1.U32);
		}
	}
	else {
		MeasuredRotationalSpeed[M1] = 0.0;
		//v = v2 + v2 - v1;
	}
	//v1 = v2;
	//v2 = v;

	if (!(PCA0CPM1 & 0x01)) {	//if the CCF1 interrupt is disabled,
		//the corresponding rotational speed can be calculated

		if(DIR2B1==1 && DIR2B0==0) { //Rotating Direction Information from CPLD
			//Rotating Direction Motor2: Counter-Clockwise
			MeasuredRotationalSpeed[M2] = -RPS(PCA0CP1_Count2.U32 - PCA0CP1_Count1.U32);
		}
		else if(DIR2B1==0 && DIR2B0==1){
			//Rotating Direction Motor2: Clockwise
			MeasuredRotationalSpeed[M2] = RPS(PCA0CP1_Count2.U32 - PCA0CP1_Count1.U32);
		}
	}
	else MeasuredRotationalSpeed[M2] = 0.0;

	if (!(PCA0CPM2 & 0x01)) {	//if the CCF2 interrupt is disabled,
		//the corresponding rotational speed can be calculated

		if(DIR3B1==1 && DIR3B0==0) {		//Rotating Direction Information from CPLD
			//Rotating Direction Motor3: Counter-Clockwise
			MeasuredRotationalSpeed[M3] = -RPS(PCA0CP2_Count2.U32 - PCA0CP2_Count1.U32);
		}
		else if(DIR3B1==0 && DIR3B0==1){
			//Rotating Direction Motor3: Clockwise
			MeasuredRotationalSpeed[M3] = RPS(PCA0CP2_Count2.U32 - PCA0CP2_Count1.U32);
		}
	}
	else MeasuredRotationalSpeed[M3] = 0.0;
}

void PrepareNextRotationalSpeedMeasurement (void) {
	PCA0CP0_EdgeCount = 0; //reset number of measured time stamps
	PCA0CP1_EdgeCount = 0; //reset number of measured time stamps
	PCA0CP2_EdgeCount = 0; //reset number of measured time stamps
	PCA0CPM0 = PCA0CPM0 | 0x01; //enable CapComp 0 interrupt
	PCA0CPM1 = PCA0CPM1 | 0x01; //enable CapComp 1 interrupt
	PCA0CPM2 = PCA0CPM2 | 0x01; //enable CapComp 2 interrupt

	CCF0 = 0; //clear CapComp 0 flag
	CCF1 = 0; //clear CapComp 1 flag
	CCF2 = 0; //clear CapComp 2 flag
}
