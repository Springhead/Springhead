#pragma once

#include "CommandWROOM.h"
#ifdef SPRINGHEAD
#include <springhead.h>
using namespace Spr;
#define NMOTOR 1
#define NFORCE 1
#define NTARGET 1
#else
#include "UTRef.h"
#endif
#include <vector>
extern "C"{
#include "../../../PIC/fixed.h"
#include "../../../PIC/control.h"
}

class BoardCmdBase{
public:
	virtual ~BoardCmdBase(){}
	virtual short GetControlMode()=0;
	virtual short GetMotorPos(int i)=0;
	virtual short GetMotorVel(int i)=0;
	virtual unsigned short GetPeriod()=0;
	virtual unsigned short GetTargetCountWrite()=0;
	virtual short GetForceControlJacob(int j, int i)=0;
	virtual short GetParamType()=0;
	virtual short GetControlK(int i)=0;
	virtual short GetControlB(int i)=0;
	virtual short GetControlA(int i)=0;
	virtual short GetTorqueMin(int i)=0;
	virtual short GetTorqueMax(int i)=0;
	virtual short GetBoardId()=0;
	typedef ::ResetSensorFlags ResetSensorFlags;		//	in CommandIDs.cs
	virtual short GetResetSensorFlags()=0;	//	
};
class BoardRetBase{
public:
	virtual ~BoardRetBase();
	virtual void SetAll(ControlMode controlMode, unsigned char targetCountReadMin, unsigned char targetCountReadMax,
		unsigned short tickMin, unsigned short tickMax, 
		SDEC* pos, SDEC* vel, SDEC* current, SDEC* force, SDEC* touch)=0;
	virtual void SetMotorPos(short p, int i)=0;
	virtual void SetMotorVel(short v, int i)=0;
	virtual void SetTargetCountReadMin(unsigned char c)=0;
	virtual void SetTargetCountReadMax(unsigned char c)=0;
	virtual void SetTickMin(unsigned short t)=0;
	virtual void SetTickMax(unsigned short t)=0;
	virtual void SetCurrent(short c, int i)=0;
	virtual void SetForce(short f, int i)=0;
	virtual void SetTouch(short t, int i)=0;
	virtual void SetBoardInfo(int systemId, int nTarget, int nMotor, int nCurrent, int nForce, int nTouch)=0;
	virtual void SetParamType(short type)=0; 
	virtual void SetParamPD(short k, short b, int i)=0;
	virtual void SetParamCurrent(short a, int i)=0;
	virtual void SetParamHeat(short limit, short release, int i)=0;
	virtual void SetParamTorque(short minimum, short maximum, int i)=0;
};

#ifdef SPRINGHEAD
namespace Spr { class DRUARTMotorDriverImpl; }
#endif
struct RobotState;
class UdpCmdPacket;
class UdpRetPacket;
class BoardBase:public UTRefCount{
public:
#ifdef SPRINGHEAD
	Spr::DRUARTMotorDriverImpl* owner;
#endif
	static const char* Tag(){ return "Board"; };
	const unsigned char * cmdPacketLen;
	const unsigned char * retPacketLen;
	std::vector<char> motorMap;
	std::vector<char> currentMap;
	std::vector<char> forceMap;
	std::vector<char> touchMap;
	virtual ~BoardBase(){}
	virtual const char* GetName() = 0;
	virtual int GetModelNumber() = 0;
	virtual int GetNTarget() = 0;
	virtual int GetNMotor() = 0;
	virtual int GetNCurrent() = 0;
	virtual int GetNForce() = 0;
	virtual int GetNTouch() = 0;
	virtual int GetBoardId() = 0;
	virtual unsigned char GetTargetCountRead() = 0;
	virtual unsigned short GetTick() = 0;
	virtual void WriteCmd(unsigned short command, BoardCmdBase& packet) = 0;
	virtual void ReadRet(unsigned short command, BoardRetBase& packet) = 0;
	virtual int GetRetCommand() = 0;
	virtual unsigned char* CmdStart() = 0;
	virtual int CmdLen() = 0;
	virtual volatile unsigned char* RetStart() = 0;
	virtual int RetLen() = 0;
	virtual int RetLenForCommand() = 0;
};

class BoardFactoryBase :public UTRefCount {
public:
	static const char* Tag() { return "BFac"; }
	const unsigned char * cmdPacketLen;
	const unsigned char * retPacketLen;
	BoardFactoryBase(const unsigned char * c, const unsigned char * r) : cmdPacketLen(c), retPacketLen(r) {}
	virtual ~BoardFactoryBase(){}
	virtual const char* GetName() = 0;
	virtual int GetModelNumber() = 0;
	virtual BoardBase* Create(int id) = 0;
};
class BoardFactories :public std::vector< UTRef<BoardFactoryBase> > {
public:
	static const char* Tag(){ return "BFac"; };
	BoardFactories();
	BoardFactoryBase* Find(const char* name);
	BoardFactoryBase* Find(int modelNum);
};

class Boards :public std::vector< UTRef<BoardBase> > {	
	static BoardFactories factories;
public:
	static const char* Tag(){ return BoardBase::Tag(); };
	BoardBase* Create(const char* name, int boardId);
	BoardBase* Create(int modelNum, int boardId);
};
