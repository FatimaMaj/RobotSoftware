/*!\file        
 * \brief
 * 				OmniRobot-Software V2.0
 *			Original	:	University of Applied Sciences Trier	
 *			File 		:	main.c	
 */
//---Header files----------------------------------------------------
#include "compiler_defs.h"	//Compiler Definitions i.e. SDCC, Tasking, Keil
#include "C8051F580_defs.h"	//�C Library for Generic-Toolchain (Compiler)
#include <stdio.h>

#include "globals.h"		//Global Declarations
#include "float.h"			//Float operations. Only single precision with sdcc. See Manual p.99
//#include "RotationalSpeedControl.h"
//#include "Kinematics.h"
#include "TrajectoryControl.h"
#include "PositionControl.h"


//-------------------------------------------------------------------

//---External function declarations----------------------------------
extern void Init_Device (void);
//-------------------------------------------------------------------

//---Initialise device-----------------------------------------------
#ifdef SDCC
U8 _sdcc_external_startup (void)
{
	Init_Device();

	return 0;
}
#endif
//-------------------------------------------------------------------

typedef enum States{
	RUNNING,
	HALT,
	STILL,
	TEST

}States;

typedef enum Directions{

	NORTH,
	SOUTH,
	EAST,
	WEST,
	SPINN,
	NONE

}Directions;

static U8 Testbyte = 0; //for SPI testing
static U8 CommissioningState = SPI_TEST;
static BIT ButtonState = FALSE;
static U8 inputcharacter;                  // Used to store character from UART
static U16 x;
static U16 counter;

static States state;
static Directions direction;

void main (void) {	

	state = RUNNING;
	direction = SPINN;

	InitGlobalVariables();
	InitRotationalSpeedControlVariables();
	InitKinematicsVariables();
	InitTrajectoryControlVariables();
	InitPositionControlVariables();



   	SFRPAGE = ACTIVE_PAGE;              // Change for PCA0MD and SBUF0
  	PCA0MD &= ~0x40;                    // Disable the watchdog timer

	x = 0;
	counter = 0;
	ENABLE = 0;	//Enable H-Bridges
	NSS4 = 0;	//select SPI slave 4 (C8051F320)
	LED = 0;	//LED off
	//TODO
	//Should change on timer1 interrupt, with a predefined intervall. 
	while (1) {
		if(x==35000)
			counter+=1;

		if(counter == 200){
			direction = SOUTH;
			x =0;
		}else if(counter ==500){
			direction = NORTH;
			counter = 0;
			x =0;
		}

		switch(state){
			case RUNNING:
			x+=1;
				
				
				
				switch(direction){
					
					case NORTH:
						CommissioningState = FORWARD;
					//TODO
					break;
					case SOUTH:
						CommissioningState = BACK;
					//TODO
					break;
					case WEST:
						CommissioningState = LEFT;
					//TODO
					break;
					case EAST:
						CommissioningState = RIGHT;
					//TODO
					break;
					case SPINN:
						CommissioningState = ROTATE;
					//TODO
					break;
					case NONE:
						CommissioningState = STOP;
					//TODO
					break;
					default:
					//TODO
					break;
				}
				
			break;

			case HALT:
				//TODO
				state = STILL;
			break;

			case STILL:
				break;
			
		}
		
	}
/********************************************************************/
}

/*!\brief Interrupt vector 
 *for the timer2
 *         
 *
 *  Responsible for checking commissioningState 
 */
INTERRUPT(Timer2_ISR, INTERRUPT_TIMER2) {
	#ifdef NO_AUTO_PAGE_STACK
	U8 SFRPAGE_save = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;
	#endif

	EA = 0;
	
	TF2H = 0;

	GetRotationalSpeeds();

	//Commissioning FSM
	switch (CommissioningState) {

		case STOP:
			ReferenceRotationalSpeed[M1] = 0.0;
			ReferenceRotationalSpeed[M2] = 0.0;
			ReferenceRotationalSpeed[M3] = 0.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		
		case FORWARD:
			ReferenceRotationalSpeed[M1] = -2.0;
			ReferenceRotationalSpeed[M2] = 2.0;
			ReferenceRotationalSpeed[M3] = 0.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		
		case ROTATE:
			ReferenceRotationalSpeed[M1] = 2.0;
			ReferenceRotationalSpeed[M2] = 2.0;
			ReferenceRotationalSpeed[M3] = 2.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		
		case BACK:
			ReferenceRotationalSpeed[M1] = 2.0;
			ReferenceRotationalSpeed[M2] = -2.0;
			ReferenceRotationalSpeed[M3] = 0.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		
		case LEFT:
			ReferenceRotationalSpeed[M1] = -2.0;
			ReferenceRotationalSpeed[M2] = 0.0;
			ReferenceRotationalSpeed[M3] = 2.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		
		case RIGHT:
			ReferenceRotationalSpeed[M1] = 2.0;
			ReferenceRotationalSpeed[M2] = -2.0;
			ReferenceRotationalSpeed[M3] = 0.0;
			RotationalSpeedControl(M1);
			RotationalSpeedControl(M2);
			RotationalSpeedControl(M3);
			break;
		

		default: 
			CommissioningState = SPI_TEST;
		
	}
	switch (ButtonState) {
		case FALSE: 
			if (START)
				ButtonState = TRUE;
			break;
		
		case TRUE: 
			if (!START)
				ButtonState = FALSE;
			break;
		
		default: 
			ButtonState = FALSE;
		
	}

	PrepareNextRotationalSpeedMeasurement();

	if (CommissioningState != SPI_TEST) {
		ADC_CHANNEL = CHANNEL_I_MOTOR1;
		START_AD_CONVERSION;
	}

	EA = 1;
	#ifdef NO_AUTO_PAGE_STACK
	SFRPAGE = SFRPAGE_save;
	#endif
}
/**************************************Timer2_ISR_END***************************************/

//---Measure rotational speed of all motors--------------------------
INTERRUPT(PCA0_ISR, INTERRUPT_PCA0) {
	#ifdef NO_AUTO_PAGE_STACK
	U8 SFRPAGE_save = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;
	#endif
	EA = 0;
	//---Module 0---
	if (CCF0) { //CapComp Module 0 has triggered the interrupt
		//If both interrupt flags - CCF0 and CF - are set at this point,
		//the CPU was not able to vector to this ISR fast enough,
		//to distinguish between the 'capture' and the 'overflow' event.
		if (CF) { //CF: Overflow flag
			//When that occurs we have to decide whether the 'overflow' has to be
			//associated to the current captured 'PCA0CP0' value or not.
			if (PCA0CP0 < PCA0CP0_Count1.U16[LSB]) {
				PCA0_Overflow++;
				CF = 0;
			}
		}

		//Make sure to capture only two counter values
		if (PCA0CP0_EdgeCount == 0) {
			PCA0CP0_Count1.U16[MSB] = PCA0_Overflow;
			PCA0CP0_Count1.U16[LSB] = PCA0CP0;
		}
		if (PCA0CP0_EdgeCount == 1) {
			PCA0CP0_Count2.U16[MSB] = PCA0_Overflow;
			PCA0CP0_Count2.U16[LSB] = PCA0CP0;

			PCA0CPM0 = PCA0CPM0 & ~0x01; //Disable CCF0 interrupt
		}
		CCF0 = 0; //Clear Module 0 capture flag
		PCA0CP0_EdgeCount++;
	}

	//---Module 1---
	if (CCF1) { //CapComp Module 1 has triggered the interrupt
		if (CF) { //CF: Overflow flag
			if (PCA0CP1 < PCA0CP1_Count1.U16[LSB]) {
				PCA0_Overflow++;
				CF = 0;
			}
		}

		//Make sure to capture only two counter values
		if (PCA0CP1_EdgeCount == 0) {
			PCA0CP1_Count1.U16[MSB] = PCA0_Overflow;
			PCA0CP1_Count1.U16[LSB] = PCA0CP1;
		}
		if (PCA0CP1_EdgeCount == 1) {
			PCA0CP1_Count2.U16[MSB] = PCA0_Overflow;
			PCA0CP1_Count2.U16[LSB] = PCA0CP1;

			PCA0CPM1 = PCA0CPM1 & ~0x01; //Disable CCF1 interrupt
		}
		CCF1 = 0; //Clear Module 1 capture flag
		PCA0CP1_EdgeCount++;
	}

	//---Module 2---
	if (CCF2) { //CapComp Module 2 has triggered the interrupt
		if (CF) { //CF: Overflow flag
			if (PCA0CP2 < PCA0CP2_Count1.U16[LSB]) {
				PCA0_Overflow++;
				CF = 0;
			}
		}

		//Make sure to capture only two counter values
		if (PCA0CP2_EdgeCount == 0) {
			PCA0CP2_Count1.U16[MSB] = PCA0_Overflow;
			PCA0CP2_Count1.U16[LSB] = PCA0CP2;
		}
		if (PCA0CP2_EdgeCount == 1) {
			PCA0CP2_Count2.U16[MSB] = PCA0_Overflow;
			PCA0CP2_Count2.U16[LSB] = PCA0CP2;

			PCA0CPM2 = PCA0CPM2 & ~0x01; //Disable CCF2 interrupt
		}
		CCF2 = 0; //Clear Module 2 capture flag
		PCA0CP2_EdgeCount++;
	}

	if (CF) { //'Counter Overflow' has triggered the interrupt
		CF = 0;	//Clear Overflow flag
		PCA0_Overflow++;
	}

	EA = 1;
	#ifdef NO_AUTO_PAGE_STACK
	SFRPAGE = SFRPAGE_save;
	#endif
}
//-------------------------------------------------------------------

//---Measure all motor currents--------------------------------------
INTERRUPT (ADC0_ISR, INTERRUPT_ADC0_EOC) {
	#ifdef NO_AUTO_PAGE_STACK
	U8 SFRPAGE_save = SFRPAGE;
	SFRPAGE = ACTIVE_PAGE;
	#endif
	
	EA = 0;
	AD0INT = 0;

	switch (ADC_CHANNEL) {
		case 0x00: {
			MeasuredMotorCurrent[0] = (F32)ADC0;

			SPI0DAT = (U8)(ADC0 >> 4);

			ADC_CHANNEL = CHANNEL_I_MOTOR2;
			AD0BUSY = 1;
			break;
		}
		case 0x01: {
			MeasuredMotorCurrent[1] = (F32)ADC0;

			SPI0DAT = (U8)(ADC0 >> 4);

			ADC_CHANNEL = CHANNEL_I_MOTOR3;
			AD0BUSY = 1;
			break;
		}
		case 0x02: {
			MeasuredMotorCurrent[2] = (F32)ADC0;

			SPI0DAT = (U8)(ADC0 >> 4);

			ADC_CHANNEL = CHANNEL_BATTERY_VOLTAGE;
			AD0BUSY = 1;
			break;
		}
		case 0x03: {
			MeasuredBatteryVoltage = (F32)ADC0;

			SPI0DAT = (U8)(ADC0 >> 4);

			ADC_CHANNEL = CHANNEL_I_MOTOR1;
			break;
		}
		default: {
			ADC_CHANNEL = CHANNEL_I_MOTOR1;
		}
	}

	EA = 1;

	#ifdef NO_AUTO_PAGE_STACK
	SFRPAGE = SFRPAGE_save;
	#endif
}
//-------------------------------------------------------------------
