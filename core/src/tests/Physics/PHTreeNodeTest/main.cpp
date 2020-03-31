/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Physics/PHConstraintTest.cpp

【概要】
  ・接触モードに LCP法 を選択して、シミュレーションする。
  
【終了基準】
　・生成したブロックすべてが、数ステップ間、床の上に静止したら正常終了。　
   
 */
#include <Springhead.h>		//	Springheadのインタフェース
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

const int nSolid = 5;


struct LogFrame {
	Vec3d center[nSolid];
	Vec3d force[nSolid - 1];
};
LogFrame operator - (const LogFrame& a, const LogFrame& b) {
	LogFrame rv;
	for (int i = 0; i < nSolid; ++i) {
		rv.center[i] = a.center[i] - b.center[i];
	}
	for (int i = 0; i < nSolid-1; ++i) {
		rv.force[i] = a.force[i] - b.force[i];
	}
	return rv;
}
typedef std::vector<LogFrame> Log;
Log operator - (const Log& a, const Log& b) {
	assert(a.size() == b.size());
	Log rv;
	for (size_t i = 0; i < a.size(); ++i) {
		LogFrame r = a[i] - b[i];
		rv.push_back(r);
	}
	return rv;
}
std::ostream& operator << (std::ostream& ostr, const LogFrame& f) {
	std::string sep = "";
#if 0	//	pos
	for (int i = 0; i < nSolid; ++i) {
		ostr << sep << f.center[i];
		sep = " ";
	}
#else //force
	for (int i = 0; i < nSolid-1; ++i) {
		ostr << sep << f.force[i];
		sep = " ";
	}
#endif
	return ostr;
}
std::ostream& operator << (std::ostream& ostr, const Log& l) {
	for (auto f : l) {
		ostr << f << std::endl;
	}
	return ostr;
}

Log logs[2];
PHSceneIf* scene;
std::vector<PHSolidIf*> solids;
std::vector<PHJointIf*> joints;

void test(std::vector<LogFrame>& log) {
	scene->SetGravity(Vec3d());
	const int nSteps = 1000;
	scene->SetGravity(Vec3d(0, -100, 0));
	for (int t = 0; t < nSteps; ++t) {
#if 0
		if (t == 10) {
			scene->SetGravity(Vec3d(0, -100, 0));
		}
#endif
		if (t >=  10000) {
			for (auto j : joints) {
				PHBallJointIf* bj = j->Cast();
				if (bj) {
					double rad = ((int)t - 10) * Rad(30) / ((int)nSteps - 10);
					bj->SetTargetPosition(Quaterniond::Rot(rad, 'z'));
				}
				PHHingeJointIf* hj = j->Cast();
				if (hj) {
					double rad = ((int)t - 10) * Rad(30) / ((int)nSteps - 10);
					hj->SetTargetPosition(rad);
				}
			}
		}
		scene->Step();
		std::string sep = "";
		log.push_back(LogFrame());
		for (size_t i = 0; i < nSolid; ++i) {
			log.back().center[i] = solids[i]->GetCenterPosition();
		}
		for (size_t i = 0; i < nSolid-1; ++i) {
			PHHingeJointIf* hj = joints[i]->Cast();
			if (hj) {
				log.back().force[i] = Vec3d(0, 0, hj->GetMotorForce());
			}
			PHBallJointIf* bj = joints[i]->Cast();
			if (bj) {
				log.back().force[i] = bj->GetMotorForce();
			}
		}
	}
}

int SPR_CDECL main(int argc, char* argv[]){
	UTRef<PHSdkIf> sdk = PHSdkIf::CreateSdk();					// SDKの作成　
	PHSceneDesc dscene;
	dscene.numIteration = 100;
	dscene.timeStep = 0.02;
	scene = sdk->CreateScene(dscene);							// シーンの作成
	scene->SetContactMode(PHSceneDesc::MODE_NONE);				// 接触なし
	PHSolidDesc dsolid;
	dsolid.mass = 2.0;
	dsolid.inertia *= 2.0;
	for (int i = 0; i < nSolid; ++i) {
		dsolid.pose.PosX() = int(i*2);
		solids.push_back(scene->CreateSolid(dsolid));
		if (i == 0) {
			solids[i]->SetDynamical(false);
		}
		else {
			PHBallJointDesc djoint;
			djoint.poseSocket.Pos() = Vec3d(1, 0, 0);
			djoint.posePlug.Pos() = Vec3d(-1, 0, 0);
			djoint.posePlug.Ori() = Quaterniond::Rot(Rad(180), 'z');
			djoint.spring = 100;
			djoint.damper = 100;
			joints.push_back(scene->CreateJoint(solids[i-1], solids[i], djoint));
		}
	}
	UTRef<ObjectStatesIf> states = ObjectStatesIf::Create();
	states->SaveState(scene);
	test(logs[0]);
	states->LoadState(scene);
	scene->CreateTreeNodes(solids[0]);
	test(logs[1]);
	Log diff = logs[1] - logs[0];
	for (size_t i = 0; i < diff.size(); ++i) {
		DSTR << "A" << logs[1][i] << " - \tN" << logs[0][i] <<" = \t" << i << "D" << diff[i] << std::endl;
	}
	return 0;
}
