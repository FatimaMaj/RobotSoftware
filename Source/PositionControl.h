#ifndef POSITION_CONTROL_H
#define POSITION_CONTROL_H

#include "math.h"
#include "float.h"
#include "globals.h"
#include "Kinematics.h"

extern F32 ReferencePosition[3];

void InitPositionControlVariables (void);
void GoToReferencePosition (void);

#endif //POSITION_CONTROL_H