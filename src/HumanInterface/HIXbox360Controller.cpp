#include <HumanInterface/HIXbox360Controller.h>

namespace Spr{;

#define MAX_CONTROLLERS 4
#define DIS_UCHAR 255
#define DIS_SHORT 32767
#define DIS_USHORT 65535

static int Ncontrollers = 0;
bool HIXbox360Controller::Init(){
	DSTR << "Connecting Xbox Controller." << std::endl;
	DWORD dwResult;    
	for (DWORD i = Ncontrollers; i < MAX_CONTROLLERS; i++ ){
		ZeroMemory( &state, sizeof(XINPUT_STATE) );
		dwResult = XInputGetState( i, &state);

		if( dwResult == ERROR_SUCCESS ){ 
			DSTR << "Succeed to connect." << std::endl;
			DSTR << "Controller ID " << i << std::endl;
			controllerID = i;
			Ncontrollers += 1;
			return true;
		}else{
			DSTR << "Failed to connect Xbox Controller." << std::endl;
			return false;
		}
	}
	return false;
}

void HIXbox360Controller::Update(float dt){
	UpdateState();
	XINPUT_GAMEPAD g = state.Gamepad;

	thumbL = Vec2i((int)g.sThumbLX, (int)g.sThumbLY);
	thumbR = Vec2i((int)g.sThumbRX, (int)g.sThumbRY);
	leftTrigger = (int)g.bLeftTrigger;
	rightTrigger = (int)g.bRightTrigger;

	// ³‹K‰»
	n_thumbL = (Vec2f)thumbL / DIS_SHORT;
	n_thumbR = (Vec2f)thumbR / DIS_SHORT;
	n_rightTrigger = (float)rightTrigger / DIS_UCHAR;
	n_leftTrigger = (float)leftTrigger / DIS_UCHAR;

	UpdatePose(dt);
}

void HIXbox360Controller::UpdateState(){
	ZeroMemory( &state, sizeof(XINPUT_STATE) );

	DWORD dwResult = XInputGetState(controllerID, &state);
	if(dwResult != ERROR_SUCCESS){
		//std::cout << "Can not update the Xbox360Controller states." << std::endl;
	}
	CheckDeadZone();
}

void HIXbox360Controller::UpdatePose(float dt){
	ts = maxVelocity * dt;
	rs = maxAngularVelocity * dt;
	Comp6DoF();
	//Comp3DoF();
	Vec3f pos = GetPosition();
	Quaternionf ori = GetOrientation();
	Vec3f v = (pos - lastPos) / dt;
	Vec3f av = (ori * lastOri.Inv()).Rotation() / dt;
	vel = alpha*vel + (1-alpha)*v;
	angVel = alpha*angVel + (1-alpha)*av;
	lastPos = pos;
	lastOri = ori;
}

void HIXbox360Controller::Comp6DoF(){
	// pos
	Vec3f dtrn;
	Vec2f dTL = n_thumbL * ts;
	float dRT = n_rightTrigger * ts;
	float dLT = n_leftTrigger * ts;

	dtrn.x = dTL.x;
	dtrn.z = -1 * dTL.y;
	dtrn.y = dRT - dLT;
	currPose.Pos() += dtrn;

	// rot
	Vec2f dTR = n_thumbR * rs;
	Vec3f drot = Vec3f(-1 * dTR.y, dTR.x, 0.0f);
	currPose.Ori() = Quaterniond::Rot(drot) * currPose.Ori();
}

void HIXbox360Controller::Comp3DoF(){
	// pos
	Vec3f dtrn;
	Vec2f dTL = n_thumbL * ts;
	Vec2f dTR = n_thumbR * ts;
	dtrn.x = dTL.x;
	dtrn.z = -1 * dTL.y;
	dtrn.y = dTR .y;
	currPose.Pos() += dtrn;
}

void HIXbox360Controller::CheckDeadZone(){
	// Zero value if thumbsticks are within the dead zone 
	if( (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && 
		(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
		 state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
	{	
	   state.Gamepad.sThumbLX = 0;
	   state.Gamepad.sThumbLY = 0;
	}

	if( (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
		 state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && 
		(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
	state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ) 
	{
	   state.Gamepad.sThumbRX = 0;
	   state.Gamepad.sThumbRY = 0;
	}
}

void HIXbox360Controller::SetMaxVelocity(float v){
	maxVelocity = v;
}

void HIXbox360Controller::SetMaxAngularVelocity(float v){
	maxAngularVelocity = v;
}

Posef HIXbox360Controller::GetPose(){
	return currPose;
}

Vec3f HIXbox360Controller::GetPosition(){
	return currPose.Pos();
}

Quaternionf HIXbox360Controller::GetOrientation(){
	return currPose.Ori();
}

Vec3f HIXbox360Controller::GetVelocity(){
	return vel;
}

Vec3f HIXbox360Controller::GetAngularVelocity(){
	return angVel;
}

void HIXbox360Controller::SetVibration(Vec2f lr){
	vibScale = 1000.0f;
	if(lr.x > vibScale) lr.x = vibScale;
	if(lr.y > vibScale) lr.y = vibScale;

	XINPUT_VIBRATION vib;
	vib.wLeftMotorSpeed = lr.x / vibScale * DIS_USHORT;
	vib.wRightMotorSpeed = lr.y / vibScale * DIS_USHORT;
	DWORD dwResult;
	dwResult = XInputSetState(controllerID, &vib);
	if(dwResult != ERROR_SUCCESS){
		std::cout << "Error at setting vibration of XBox Controller." << std::endl;
	}
}

}