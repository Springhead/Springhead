#ifndef _CONTROL_H
#define _CONTROL_H
//	This file works for both PIC and WROOM

#include "env.h"
#include "fixed.h"
#ifndef SPRINGHEAD
#include "command.h"
#endif

#ifdef __XC32
#include "mcc_generated_files/mcc.h"
#endif
#include <assert.h>

///	motor contorl paramters
//@{

///	Heat limit
#define MOTOR_HEAT_RELEASE	((SDEC)(0.5 * SDEC_ONE))                  //  0.5
#define MOTOR_HEAT_LIMIT	(20 * 10 * MOTOR_HEAT_RELEASE)            //	20sec * 10Hz
extern SDEC motorHeatRelease[NMOTOR];		//	heat release from motor / loop (10Hz)
extern long motorHeatLimit[NMOTOR];			//	limit for heat amount of the motor
extern long motorHeat[NMOTOR];				//	current heat amount
extern SDEC lastRatio[NMOTOR];				//	pwm ratio actually applied to motor

///	PD control and current control
#define PDPARAM_K   SDEC_ONE
#define PDPARAM_B   ((SDEC)(1.5*SDEC_ONE))
#define PDPARAM_A   ((SDEC)(0.5*SDEC_ONE))
struct PdParam{
    SDEC k[NMOTOR];
    SDEC b[NMOTOR];
    SDEC a[NMOTOR];
};
extern struct PdParam pdParam;
///	Torque limit
struct TorqueLimit{
    SDEC min[NMOTOR];
    SDEC max[NMOTOR];
};
extern struct TorqueLimit torqueLimit;

void saveMotorParam();
void loadMotorParam();
//@}


//	device depended functions
void readADC();								//	read adc and set it to mcos and msin
void setPwm(int ch, SDEC torque);			//	set pwm of motor
void setPwmWithLimit(int ch, SDEC torque);	//	limit the torque to torqueLimit then call setPwm 


//	The control routine. Should be called periodically.
void controlLoop();


//  data buffer
struct MotorState{
    LDEC pos[NMOTOR];
    LDEC vel[NMOTOR];
};
extern struct MotorState motorTarget, motorState;
extern SDEC currentTarget[NMOTOR];
extern SDEC forceControlJK[NFORCE][NMOTOR];
#define NAXIS	(NMOTOR+NFORCE/2)	//	NAXIS=NMOTOR+NFORCE/2
extern SDEC mcos[NAXIS], msin[NAXIS];
extern SDEC currentSense[NMOTOR];
extern const SDEC mcosOffset[NAXIS];
extern const SDEC msinOffset[NAXIS];


struct Target{
	short period;	//	period to reach this target.
	SDEC pos[NMOTOR];
	SDEC Jacob[NFORCE][NMOTOR];
};
struct Targets{
	volatile unsigned short tick;			//	current tick count
	volatile unsigned char targetCountRead;	//	couner value of buf[read]
	volatile char read;                 	//	interpolation works between "pos[read]" and "pos[read+1]".
	volatile char write;                	//	cursor to add new data. pos[write] = newdata.
	volatile struct Target buf[NTARGET];
};
extern struct Targets targets;

enum ControlMode{
    CM_SKIP,				//	This command dose not contain control information and must be skipped.
	CM_TORQUE,				//	Torque control mode.
	CM_DIRECT,				//	Set target positions and velocities directly.
	CM_CURRENT,				//	Set target currents.
    CM_INTERPOLATE,			//	Interpolate target positions.
    CM_FORCE_CONTROL,		//	Interpolate target positions + local feedback loop for force control/
};
extern enum ControlMode controlMode;

void targetsInit();
void targetsAddOrUpdate(short* pos, short period, unsigned char count);
void targetsForceControlAddOrUpdate(SDEC* pos, SDEC JK[NFORCE][NMOTOR],short period, unsigned char count);
void targetsWrite();
inline unsigned char targetsWriteAvail(){
	signed char len = targets.read - targets.write;
	if (len < 0) len += NTARGET;
#if 0
	if (len > NTARGET){
		PIC_LOGE("targetsWriteAvail() w:%d r:%d len:%d", targets.write, targets.read, len);
		assert(len <= NTARGET);
	}
#endif
	return len;
}
inline unsigned char targetsReadAvail(){
	signed char len = targets.write - targets.read;
	if (len <= 0) len += NTARGET;
#if 0
	if (len > NTARGET){
		PIC_LOGE("targetsReadAvail w:%d r:%d len:%d", targets.write, targets.read, len);
		assert(len <= NTARGET);
	}
#endif
	return len;
}
int targetsCountMin();
int targetsCountMax();

void controlInit();
void controlSetMode(enum ControlMode m);
void controlLoop();
void updateMotorState();
void setPwmWithLimit(int ch, SDEC ratio);


extern SDEC forceOffset[NFORCE];

inline SDEC getForceRaw(int ch){
	if (ch == 0) return mcos[3];
	if (ch == 1) return msin[3];
	return 0;
}
inline SDEC getForce(int ch){
	if (ch == 0) return mcos[3] - forceOffset[ch];
	if (ch == 1) return msin[3] - forceOffset[ch];
	return 0;
}

// #ifdef PIC
extern int coretimerRemainTime;
extern uint32_t coretimerCompare;
extern uint32_t controlCount;
// #endif

#endif
