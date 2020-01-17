#pragma once
#include "BoardBase.h"
#include "esp_log.h"
#ifndef SPRINGHEAD
#include "UdpCom.h"
#endif

#ifdef SPRINGHEAD

namespace Spr {
	void DRUARTMotorDriverImplUpdateMotorPos(DRUARTMotorDriverImpl* owner, short mpos, int index);
}
#define updateMotorPos(mpos, index)		Spr::DRUARTMotorDriverImplUpdateMotorPos(owner, mpos, index)
#define MOTOROFFSET(i)	0

#else

inline void updateMotorPos(SDEC mpos, char index){
	SDEC diff = mpos - (SDEC)(allBoards.motorPos[(int)index]);
	allBoards.motorPos[(int)index] += diff;
}
#define MOTOROFFSET(i)	allBoards.motorOffset[(int)motorMap[i]]

#endif


template <class CMD, class RET>
class Board: public BoardBase{
public:
	typedef CMD CmdPacket;
	typedef RET RetPacket;
	CmdPacket cmd;						//	UART command packet for this board. 
	volatile RetPacket ret;				//	UART return packet for this board.
	Board(int bid, const unsigned char * c, const unsigned char * r) {
		cmd.boardId = bid;
		cmdPacketLen = c;
		retPacketLen = r;
	}
	const char* GetName() { return CmdPacket::GetBoardName(); }
	int GetModelNumber() { return CmdPacket::GetModelNumber(); }
	int GetNTarget() { return CmdPacket::GetNTarget();  }
	int GetNMotor() { return CmdPacket::GetNMotor(); }
	int GetNCurrent() { return CmdPacket::GetNCurrent(); }
	int GetNForce() { return CmdPacket::GetNForce(); }
	int GetNTouch() { return CmdPacket::GetNTouch(); }
	int GetBoardId() { return cmd.boardId;  }
	int GetRetCommand() { return ret.commandId; }
	unsigned char* CmdStart() { return cmd.bytes;  }
	int CmdLen() { return cmdPacketLen[cmd.commandId]; }
	volatile unsigned char* RetStart() { return ret.bytes; }
	int RetLen() { return retPacketLen[ret.commandId]; }
	int RetLenForCommand() { return retPacketLen[cmd.commandId]; }
	unsigned char GetTargetCountRead(){
		if (ret.commandId == CI_ALL){
			return ret.all.targetCountRead;
		}else{
			return ret.interpolate.targetCountRead;
		}
	}
	unsigned short GetTick(){
		if (ret.commandId == CI_ALL){
			return ret.all.tick;
		}else{
			return ret.interpolate.tick;
		}
	}
	void WriteCmd(unsigned short command, BoardCmdBase& packet) {
		cmd.commandId = command;
		switch (command){
		case CI_ALL:
			cmd.all.controlMode = packet.GetControlMode();
			cmd.all.targetCountWrite = packet.GetTargetCountWrite();
			cmd.all.period = packet.GetPeriod();
			for (int i = 0; i < GetNMotor(); ++i) {
				cmd.all.pos[i] = packet.GetMotorPos(motorMap[i]) + MOTOROFFSET(i);
				for (int j = 0; j < GetNForce(); ++j) {
/*					assert(GetNMotor() == 3);
					assert(GetNForce() == 2);
					assert(forceMap[j] < 4);
	*/				cmd.all.jacob[j][i] = packet.GetForceControlJacob(forceMap[j], i);
				}
			}
			break;
		case CI_DIRECT:
			for (int i = 0; i < GetNMotor(); ++i) {
				cmd.direct.pos[i] = packet.GetMotorPos(motorMap[i]) + MOTOROFFSET(i);
				cmd.direct.vel[i] = packet.GetMotorVel(motorMap[i]);
			}
			break;
		case CI_CURRENT:
			for (int i = 0; i < GetNMotor(); ++i) {
				cmd.current.current[i] = packet.GetMotorPos(motorMap[i]);
			}
			break;
		case CI_INTERPOLATE:
			for (int i = 0; i < GetNMotor(); ++i) {
				cmd.interpolate.pos[i] = packet.GetMotorPos(motorMap[i]) + MOTOROFFSET(i);
			}
			cmd.interpolate.period = packet.GetPeriod();
			cmd.interpolate.targetCountWrite = packet.GetTargetCountWrite();
			break;
		case CI_FORCE_CONTROL:
			for (int i = 0; i < GetNMotor(); ++i) {
				cmd.forceControl.pos[i] = packet.GetMotorPos(motorMap[i]) + MOTOROFFSET(i);
				for (int j = 0; j < GetNForce(); ++j) {
					assert(GetNMotor() == 3);
					assert(GetNForce() == 2);
					assert(forceMap[j] < 4);
					cmd.forceControl.jacob[j][i] = packet.GetForceControlJacob(forceMap[j], i);
				}
				cmd.forceControl.period = packet.GetPeriod();
				cmd.forceControl.targetCountWrite = packet.GetTargetCountWrite();
			}
			break;
		case CI_SET_PARAM:
			cmd.param.type = packet.GetParamType();
			if (cmd.param.type == PT_PD){
				for (int i = 0; i < GetNMotor(); ++i) {
					cmd.param.pd.k[i] = packet.GetControlK(motorMap[i]);
					cmd.param.pd.b[i] = packet.GetControlB(motorMap[i]);
				}
			}else if (cmd.param.type == PT_CURRENT){
				for (int i = 0; i < GetNMotor(); ++i) {
					cmd.param.a[i] = packet.GetControlA(motorMap[i]);
				}
			}else if (cmd.param.type == PT_TORQUE_LIMIT){
				for (int i = 0; i < GetNMotor(); ++i) {
					cmd.param.torque.min[i] = packet.GetTorqueMin(motorMap[i]);
					cmd.param.torque.max[i] = packet.GetTorqueMax(motorMap[i]);
				}
			}
			break;
		case CI_RESET_SENSOR:
			cmd.resetSensor.flags = packet.GetResetSensorFlags();
			break;
		case CI_SENSOR:
			break;	//	nothing todo
		case CI_GET_PARAM:
			cmd.param.type = packet.GetParamType();
			break;
		default:
			ESP_LOGE(Tag(), "WriteCmd(): Command Id error %d",  command);
			assert(0);
		}
	}
	void ReadRet(unsigned short cmdId, BoardRetBase& packet) {
		switch (cmdId) {
		case CI_ALL:
			for (int i = 0; i < GetNMotor(); ++i) {
				updateMotorPos(ret.all.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorPos(ret.all.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorVel(ret.all.vel[i], motorMap[i]);
			}
			for (int i = 0; i < GetNCurrent(); ++i) {
				packet.SetCurrent(ret.all.current[i], currentMap[i]);
			}
			for (int i = 0; i < GetNForce(); ++i) {
				packet.SetForce(ret.all.force[i], forceMap[i]);
			}
			for (int i = 0; i < GetNTouch(); ++i) {
				packet.SetTouch(ret.all.touch[i], touchMap[i]);
			}
			break;
		case CI_DIRECT:
			for (int i = 0; i < GetNMotor(); ++i) {
				updateMotorPos(ret.direct.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorPos(ret.direct.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorVel(ret.direct.vel[i], motorMap[i]);
			}
			//ESP_LOGI(Tag(), "Direct Motor Pos: %d %d %d %d\n", packet.MotorPos(0),  packet.MotorPos(1), packet.MotorPos(2),  packet.MotorPos(3));
			break;
		case CI_CURRENT:
			for (int i = 0; i < GetNMotor(); ++i) {
				updateMotorPos(ret.direct.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorPos(ret.direct.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorVel(ret.direct.vel[i], motorMap[i]);
			}
			//ESP_LOGI(Tag(), "Direct Motor Pos: %d %d %d %d\n", packet.MotorPos(0),  packet.MotorPos(1), packet.MotorPos(2),  packet.MotorPos(3));
			break;
		case CI_INTERPOLATE:
		case CI_FORCE_CONTROL:
			for (int i = 0; i < GetNMotor(); ++i) {
				updateMotorPos(ret.interpolate.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorPos(ret.interpolate.pos[i] - MOTOROFFSET(i), motorMap[i]);
			}
			//ESP_LOGI(Tag(), "Motor Pos: %d %d %d %d\n", packet.MotorPos(0),  packet.MotorPos(1), packet.MotorPos(2),  packet.MotorPos(3));
			break;
		case CI_SENSOR:
			//ESP_LOGI(Tag(), "M0:%x", (int)ret.sensor.pos[0]);
			for (int i = 0; i < GetNMotor(); ++i) {
				updateMotorPos(ret.sensor.pos[i] - MOTOROFFSET(i), motorMap[i]);
				packet.SetMotorPos(ret.sensor.pos[i] - MOTOROFFSET(i), motorMap[i]);
			}
			for (int i = 0; i < GetNCurrent(); ++i) {
				packet.SetCurrent(ret.sensor.current[i], currentMap[i]);
			}
			for (int i = 0; i < GetNForce(); ++i) {
				packet.SetForce(ret.sensor.force[i], forceMap[i]);
			}
			for (int i = 0; i < GetNTouch(); ++i) {
				packet.SetTouch(ret.sensor.touch[i], touchMap[i]);
			}
			break;
		case CI_GET_PARAM:
			packet.SetParamType(ret.param.type);
			switch(ret.param.type){
				case PT_PD:
					for (int i = 0; i < GetNMotor(); ++i) {
						packet.SetParamPD(ret.param.pd.k[i], ret.param.pd.b[i], motorMap[i]);
						//ESP_LOGI(Tag(), "SetParamPD(%d, %d, at%d)", ret.param.pd.k[i], ret.param.pd.b[i], motorMap[i]);
					}
					break;
				case PT_CURRENT:
					for (int i = 0; i < GetNMotor(); ++i) {
						packet.SetParamCurrent(ret.param.a[i], motorMap[i]);
					}
					break;
				case PT_TORQUE_LIMIT:
					for (int i = 0; i < GetNMotor(); ++i) {
						packet.SetParamTorque(ret.param.torque.min[i], ret.param.torque.max[i], motorMap[i]);
					}
					break;
				case PT_MOTOR_HEAT:
					for (int i = 0; i < GetNMotor(); ++i) {
						packet.SetParamHeat(ret.param.heat.limit[i], ret.param.heat.release[i], motorMap[i]);
					}
					break;
				default:
					ESP_LOGE(Tag(), "ReadRet(): CI_GET_PARAM type error %d", ret.param.type);
					assert(0);
					break;
			}
			break;
		case CI_BOARD_INFO:
			break;
		default:
			ESP_LOGE(Tag(), "ReadRet(): Command Id error %d", cmdId);
			assert(0);
		}
	}
};

template <class BOARD> class BoardFactory:public BoardFactoryBase{
public:
	BoardFactory(const unsigned char * c, const unsigned char * r) : BoardFactoryBase(c, r) {}
	virtual BoardBase* Create(int id) {
		return new BOARD(id, cmdPacketLen, retPacketLen);
	}
	virtual const char* GetName() {
		return BOARD::CmdPacket::GetBoardName();
	}
	virtual int GetModelNumber() {
		return BOARD::CmdPacket::GetModelNumber();
	}
};
#define BOARD_FACTORY(BOARD)	BoardFactory< Board<CommandPacket##BOARD, ReturnPacket##BOARD> >(cmdPacketLen##BOARD, retPacketLen##BOARD)
