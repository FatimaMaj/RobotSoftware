#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "math.h"
#include "float.h"
#include "globals.h"
#include "RotationalSpeedControl.h"

#define X 0 // global/robot x-axis
#define Y 1 // global/robot y-axis
#define G 2 // global/robot gamma-axis (angle)

//---Coupling matrix elements----------------------------------------
//---row 1, column 1,2,3---
#define C_11 0.0
#define C_12 1.0
#define C_13 RADIUS_ROBOT

//---row 2, column 1,2,3---
#define C_21 -0.86602540378444
#define C_22 -0.5
#define C_23 RADIUS_ROBOT

//---row 3, column 1,2,3---
#define C_31 0.86602540378444
#define C_32 -0.5
#define C_33 RADIUS_ROBOT
//-------------------------------------------------------------------

//---Inverse coupling matrix elements--------------------------------
//---row 1, column 1,2,3---
#define IC_11 0.0
#define IC_12 -0.57735026918963
#define IC_13 0.57735026918963

//---row 2, column 1,2,3---
#define IC_21 0.66666666666667
#define IC_22 -0.33333333333333
#define IC_23 -0.33333333333333

//---row 3, column 1,2,3---
#define IC_31 0.33333333333333 * INVERSE_RADIUS_ROBOT
#define IC_32 0.33333333333333 * INVERSE_RADIUS_ROBOT
#define IC_33 0.33333333333333 * INVERSE_RADIUS_ROBOT
//-------------------------------------------------------------------

//---Function prototypes---------------------------------------------
void InitKinematicsVariables (void);
void DirectKinematics (void);
void InverseKinematics (void);
//-------------------------------------------------------------------

//---Variables for global speed control------------------------------
extern F32 ReferenceGlobalSpeed[3];
//-------------------------------------------------------------------

//---Variables for global position control---------------------------
extern F32 Position[3];
//-------------------------------------------------------------------

#endif //KINEMATICS_H