#include <HumanInterface/HIXbox360Controller.h>
#include <Foundation/UTDllLoader.h>

namespace Spr{;

#define MAX_CONTROLLERS 4
#define DIS_UCHAR 255
#define DIS_SHORT 32767
#define DIS_USHORT 65535

static int Ncontrollers = 0;

class HIXbox360ControllerImpl{
public:
	HIXbox360Controller* owner;

#ifdef _WIN32
	XINPUT_STATE state;
#endif

public:
#ifdef _WIN32
	static Spr::UTDllLoader dllLoader;
	typedef DWORD TXInputGetState(
		DWORD        dwUserIndex,
		XINPUT_STATE* pState
	);
	typedef DWORD TXInputSetState(
		DWORD            dwUserIndex,
		XINPUT_VIBRATION* pVibration
	);

	static TXInputGetState* xInputGetState;
	static TXInputSetState* xInputSetState;
	bool Init(){
		if (!dllLoader.Load("Xinput1_3.dll")) return false;
		if (!xInputGetState) xInputGetState = (TXInputGetState*)dllLoader.GetProc("XInputGetState");
		if (!xInputSetState) xInputSetState = (TXInputSetState*)dllLoader.GetProc("XInputSetState");

		ZeroMemory( &state, sizeof(XINPUT_STATE) );
		DWORD dwResult = xInputGetState( owner->controllerID, &state);
		if( dwResult == ERROR_SUCCESS ){ 
			DSTR << "Succeed to connect." << std::endl;
			return true;
		}else{
			DSTR << "Failed to connect Xbox Controller." << std::endl;
			return false;
		}
	}
	void Update(float dt){
		UpdateState();
		XINPUT_GAMEPAD g = state.Gamepad;

		// 各種キー
		for(int i = 0; i < 14; i++){
			owner->key[i] = false;
		}
		if(g.wButtons & XINPUT_GAMEPAD_DPAD_UP)			owner->key[0] = true;
		if(g.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)		owner->key[1] = true;
		if(g.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)		owner->key[2] = true;
		if(g.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)		owner->key[3] = true;
		if(g.wButtons & XINPUT_GAMEPAD_START)			owner->key[4] = true;
		if(g.wButtons & XINPUT_GAMEPAD_BACK)			owner->key[5] = true;
		if(g.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)		owner->key[6] = true;
		if(g.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)		owner->key[7] = true;
		if(g.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)	owner->key[8] = true;
		if(g.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)	owner->key[9] = true;
		if(g.wButtons & XINPUT_GAMEPAD_A)				owner->key[10] = true;
		if(g.wButtons & XINPUT_GAMEPAD_B)				owner->key[11] = true;
		if(g.wButtons & XINPUT_GAMEPAD_X)				owner->key[12] = true;
		if(g.wButtons & XINPUT_GAMEPAD_Y)				owner->key[13] = true;
	
		// アナログスティック、ボタン
		owner->thumbL = Vec2i((int)g.sThumbLX, (int)g.sThumbLY);
		owner->thumbR = Vec2i((int)g.sThumbRX, (int)g.sThumbRY);
		owner->leftTrigger = (int)g.bLeftTrigger;
		owner->rightTrigger = (int)g.bRightTrigger;

		// 正規化
		owner->n_thumbL = (Vec2f)owner->thumbL / DIS_SHORT;
		owner->n_thumbR = (Vec2f)owner->thumbR / DIS_SHORT;
		owner->n_rightTrigger = (float)owner->rightTrigger / DIS_UCHAR;
		owner->n_leftTrigger  = (float)owner->leftTrigger / DIS_UCHAR;
	}
	void UpdateState(){
		ZeroMemory( &state, sizeof(XINPUT_STATE) );

		DWORD dwResult = xInputGetState(owner->controllerID, &state);
		if(dwResult != ERROR_SUCCESS){
			//std::cout << "Can not update the Xbox360Controller states." << std::endl;
		}
		CheckDeadZone();
	}
	void CheckDeadZone(){
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
	void SetVibration(Vec2f lr){
		owner->vibScale = 1000.0f;
		if(lr.x > owner->vibScale) lr.x = owner->vibScale;
		if(lr.y > owner->vibScale) lr.y = owner->vibScale;

		XINPUT_VIBRATION vib;
		vib.wLeftMotorSpeed  = lr.x / owner->vibScale * DIS_USHORT;
		vib.wRightMotorSpeed = lr.y / owner->vibScale * DIS_USHORT;
		DWORD dwResult;
		dwResult = xInputSetState(owner->controllerID, &vib);
		if(dwResult != ERROR_SUCCESS){
			std::cout << "Error at setting vibration of XBox Controller." << std::endl;
		}
	}
#else
	bool Init(){ return false; }
	void Update(float dt){}
	void UpdateState(){}
	void CheckDeadZone(){}
	void SetVibration(Vec2f lr){}		
#endif
};
#ifdef _WIN32
Spr::UTDllLoader HIXbox360ControllerImpl::dllLoader;
HIXbox360ControllerImpl::TXInputGetState* HIXbox360ControllerImpl::xInputGetState;
HIXbox360ControllerImpl::TXInputSetState* HIXbox360ControllerImpl::xInputSetState;
#endif


HIXbox360Controller::HIXbox360Controller (const HIXbox360ControllerDesc& desc): impl(NULL) {
	controllerID = Ncontrollers;
	Init();
	Ncontrollers += 1;
	if(Ncontrollers > MAX_CONTROLLERS) DSTR << "Can not connect a XboxController anymore!" << std::endl;
}

HIXbox360Controller::~HIXbox360Controller(){
	if(impl)
		delete impl;
}

bool HIXbox360Controller::Init(){
	DSTR << "Connecting Xbox Controller " << controllerID << "." << std::endl;
	if (!impl) {
		impl = new HIXbox360ControllerImpl;
		impl->owner = this;
	}
	return impl->Init();
}

void HIXbox360Controller::Update(float dt){
	impl->Update(dt);

	UpdatePose(dt);
}
void HIXbox360Controller::UpdateState(){
	impl->UpdateState();
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
	impl->CheckDeadZone();
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
	impl->SetVibration(lr);
}

}
