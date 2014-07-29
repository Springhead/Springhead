/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HI_LEAP_H
#define HI_LEAP_H

// Libraries/LeapSdk��`�F�b�N�A�E�g���Ĉȉ���define�̃R�����g�A�E�g��O����Leapmotion���g����悤�ɂȂ�B
// #define USE_LEAP

#include <Foundation/UTTimer.h>
//#include <Foundation/UTSocket.h>

//#include <vector>
#include <map>
#include <string>
#include <list>

#ifdef USE_LEAP
#include "../../Libraries/LeapSDK/include/Leap.h"
#pragma comment(lib, "Leap.lib")
#endif
#include <HumanInterface/HISkeletonSensor.h>
#include <HumanInterface/SprHILeap.h>
namespace Spr{;

// Leapmotion
class HILeap: public HILeapDesc, public HISkeletonSensor {

#ifdef USE_LEAP
	Leap::Controller* leap;
#endif

public:
	SPR_OBJECTDEF(HILeap);

	HILeap(const HILeapDesc& desc = HILeapDesc()) {
	}

	~HILeap() {
#ifdef USE_LEAP
		if (leap) { delete leap; }; leap = NULL;
#endif
	}

	// ----- ----- ----- ----- -----
	// HIBase��API

	bool Init(const void* desc);
	void Update(float dt);

	// ----- ----- ----- ----- -----
	// ��API�֐�

	/// Leap���W�n����iscale��l��������ł́jSpringhead���W�n��
#ifdef USE_LEAP
	inline Vec3d ToSpr(Leap::Vector lv) {
		return rotation * Vec3d(lv.x, lv.y, lv.z) * scale;
	}
	inline Quaterniond ToSpr(Leap::Matrix lm) {
		Matrix3d m;
		for (int i=0; i<3; ++i) {
			m[0][i] = lm.xBasis[i]; m[1][i] = lm.yBasis[i]; m[2][i] = lm.zBasis[i];
		}
		Quaterniond q; q.FromMatrix(m);

		return q;
	}
#endif

};


//----- ----- ----- ----- -----
// �ʐM����

union Uni {
	char ch[4];
	float fl;
	int i;
};

struct LeapBone {
	Spr::Vec3d position;
	Spr::Vec3d direction;
	double length;

	LeapBone();
};

struct LeapFinger {
	static const int BONE_NUM = 4;
	LeapBone bones[BONE_NUM];

	LeapFinger();
};

struct LeapHand {
	static const int FINGER_NUM = 5;
	Spr::Vec3d position;
	Spr::Vec3d direction;
	LeapFinger leapFingers[FINGER_NUM];
	int recFingersNum;
	float confidence;
	inline int getRecFingersNum() { return recFingersNum; }

	int originalLeapHandID;
	int bufID;

	int leapID;

	LeapHand();
	~LeapHand();
};

struct LeapFrame {
	std::vector<LeapHand> leapHands;
	int recHandsNum;
	int leapID;
	LeapFrame();
};

class LeapData {
public:
	enum WriteMode {
		WRITING,
		WRITE_COMP,
	} writeMode;
	
	enum ReadMode {
		READING,
		READ_COMP,
	} readMode;

	int write, keep, read;
	LeapData();
	LeapFrame leapFrameBufs[3];

	std::string hostName;
};

static const int DISTANCE = 300;

class UDPInit {
public:
	static UDPInit* getInstance() {
		static UDPInit instance;
		return &instance;
	}

	UDPInit();

	~UDPInit();
};

class ProtocolPC {
private:

	/// ��M�p�|�[�g�ԍ�
	int recvPort;

	/// ��M�X���b�h
	UTTimerIf* recvThread;

	/// ��M�p�P�b�g��
	int nRecv;

	ProtocolPC();

	static void SPR_CDECL recvThreadFunc(int id, void* arg);

	void unpackData(std::vector<char>&, LeapFrame&);

public:
	//Leap�ŗLID����LeapData�C���X�^���X�ւ̃}�b�v
	std::map<int, LeapData*> mapIdLeapData;

	std::list<int> usingLeapHandIdList;

	//LeapHandID���炻��ID������Ă���LeapHand�ւ̃}�b�v
	std::map<int, LeapHand*> mapLHIdLeapHand;

	//�o�b�t�@ID���珊��LeapHandID��
	std::vector< std::list<int> > bufIdLHIds;

	//static int bufsNum;

	static ProtocolPC* getInstance() {
		static ProtocolPC instance;
		return &instance;
	}
		
	bool isSame(LeapHand* L1, LeapHand* L2, double distance);

};


// LeapmotionUDP
class HILeapUDP: public HILeapUDPDesc, public HISkeletonSensor {

public:
	SPR_OBJECTDEF(HILeapUDP);

	HILeapUDP(const HILeapUDPDesc& desc = HILeapUDPDesc()) {
	}

	~HILeapUDP() {}

	// ----- ----- ----- ----- -----
	// HIBase��API

	bool Init(const void* desc);
	void Update(float dt);

	// ----- ----- ----- ----- -----
	// ��API�֐�

	/// Leap���W�n����iscale��l��������ł́jSpringhead���W�n��
#ifdef USE_LEAP
	inline Vec3d ToSpr(Vec3d lv) {
		return rotation * Vec3d(lv.x, lv.y, lv.z) * scale;
	}
	inline Quaterniond ToSprQ(Vec3d lm) {
		Matrix3d m;
		for (int i=0; i<3; ++i) {
			m[0][i] = lm.x; m[1][i] = lm.y; m[2][i] = lm.z;
		}
		Quaterniond q; q.FromMatrix(m);
		return q;
	}

	
	
#endif
	
};

float charToFloat(unsigned char* c);
int charToInt(unsigned char* c);
Vec3d charToVec3d(unsigned char* c);


}

#endif//HI_LEAP_H
