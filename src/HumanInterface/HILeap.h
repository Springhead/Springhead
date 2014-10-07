/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   

 */
#ifndef HI_LEAP_H
#define HI_LEAP_H

#include <SprDefs.h>

#include <Foundation/UTTimer.h>

#include <map>
#include <string>
#include <list>

#include <HumanInterface/HISkeletonSensor.h>
#include <HumanInterface/SprHILeap.h>

namespace Spr{;

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

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
	enum HandKind{
		RIGHT_HAND,
		LEFT_HAND,
	} handKind;

	static const int FINGER_NUM = 5;
	Spr::Vec3d position;
	Spr::Vec3d direction;
	Spr::Quaterniond orientation;
	LeapFinger leapFingers[FINGER_NUM];
	int recFingersNum;
	float confidence;
	float grabStrength;
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

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

/// Leapmotion 
class HILeap: public HILeapDesc, public HISkeletonSensor {
	void* leap;

public:
	SPR_OBJECTDEF(HILeap);

	HILeap(const HILeapDesc& desc = HILeapDesc()) {}
	~HILeap();

	// ----- ----- ----- ----- -----
	//	HIBase's API

	bool Init(const void* desc);
	void Update(float dt);
};


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----




//----- ----- ----- ----- -----
// Multi hand

union Uni {
	char ch[4];
	float fl;
	int i;
};

static const int LEAP_DISTANCE = 250;

class UDPInit {
public:
	static UDPInit* getInstance() {
		static UDPInit instance;
		return &instance;
	}

	UDPInit();

	~UDPInit();
};

class ProtocolPC: public UTRefCount {
private:

	int recvPort;

	UTTimerIf* recvThread;

	int nRecv;

	ProtocolPC();

	static void SPR_CDECL recvThreadFunc(int id, void* arg);

	void unpackData(std::vector<char>&, LeapFrame&);

public:
	void loadCalib();
	std::map<int, LeapData*> mapIdLeapData;

	std::list<int> usingLeapHandIdList;

	std::map<int, LeapHand*> mapLHIdLeapHand;

	std::vector< std::list<int> > bufIdLHIds;

	//static int bufsNum;

	static ProtocolPC* getInstance() {
		static UTRef<ProtocolPC> instance;
		if (!instance) instance = DBG_NEW ProtocolPC;
		return instance;
	}
		
	bool isSame(LeapHand* L1, LeapHand* L2, double sameHandDistance, double wrongHandDistance);

	bool calibratingFlag;
	std::vector<Vec3d> calibrateOffset;
	bool calibFileExist;
};


// LeapmotionUDP
class HILeapUDP: public HILeapUDPDesc, public HISkeletonSensor {

public:
	SPR_OBJECTDEF(HILeapUDP);

	HILeapUDP(const HILeapUDPDesc& desc = HILeapUDPDesc()) {
	}

	~HILeapUDP() {}

	// ----- ----- ----- ----- -----
	// HIBaseÇÃAPI

	bool Init(const void* desc);
	void Update(float dt);

	// ----- ----- ----- ----- -----
	// îÒAPIä÷êî

	bool calibrate(int formerLeapID);
		
	inline Vec3d ToSpr(Vec3d lv) {
		return rotation * Vec3d(lv.x, lv.y, lv.z) * scale;
	}
	inline Quaterniond ToSprQ(Vec3d lm) { //<!!> MatrixÇQuaternionÇ…Ç∑ÇÈä÷êîÇÃÇÕÇ∏ÅAà¯êîÇ™Vec3dÇ»ÇÃÇÕÇ®Ç©ÇµÇ¢
		Matrix3d m;
		for (int i=0; i<3; ++i) {
			m[0][i] = lm.x; m[1][i] = lm.y; m[2][i] = lm.z;
		}
		Quaterniond q; q.FromMatrix(m);
		return q;
	}

	int getLeapNum();	
};

float charToFloat(unsigned char* c);
int charToInt(unsigned char* c);
Vec3d charToVec3d(unsigned char* c);

}

#endif//HI_LEAP_H
