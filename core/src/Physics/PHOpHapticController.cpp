/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#include "Physics/PHOpHapticController.h"

#ifdef	__linux__
#  define fopen_s(fptr,name,mode)	*(fptr) = fopen(name,mode)
#endif


namespace Spr {
	;


	bool PHOpHapticController::BeginLogForce()
	{
		fileindex++;
		std::stringstream ss;
		ss << "forceLog" << fileindex << ".dfmopf";
		std::string fname;
		fname.append(ss.str());
		// fname.append(".dfmopf");
		//const char* filename;//ss.str().c_str();//"forceLog" + ss.str() +".dfmOpf";
		fopen_s(&logForceFile, fname.c_str(), "w+");
		if (!logForceFile) {
			DSTR << "forceLog file cannot open" << std::endl;
			return false;
		}

		std::stringstream ss2;
		ss2 << "posLog" << fileindex << ".dfmopp";
		std::string fname2;
		fname2.append(ss2.str());
		//fname2.append(".dfmopf");
		//const char* filename2 = "posLog2.dfmopp";// = ss.str().c_str();
		fopen_s(&logPosFile, fname2.c_str(), "w+");
		if (!logPosFile){
			DSTR << "posLog file cannot open" << std::endl;
		}
		std::stringstream ss3;
		ss3 << "PposLog" << fileindex << ".dfmopp";
		std::string fname3;
		fname3.append(ss3.str());
		//fname2.append(".dfmopf");
		//const char* filename2 = "posLog2.dfmopp";// = ss.str().c_str();
		fopen_s(&logPPosFile, fname3.c_str(), "w+");
		if (!logPPosFile){
			DSTR << "PposLog file cannot open" << std::endl;
			return false;
		}
		std::stringstream ss4;
		ss4 << "UposLog" << fileindex << ".dfmopp";
		std::string fname4;
		fname4.append(ss4.str());
		//fname2.append(".dfmopf");
		//const char* filename2 = "posLog2.dfmopp";// = ss.str().c_str();
		fopen_s(&logUPosFile, fname4.c_str(), "w+");
		if (!logUPosFile){
			DSTR << "UposLog file cannot open" << std::endl;
			return false;
		}
		return true;
	}
	void PHOpHapticController::EndLogForce()
	{
		fclose(logForceFile);
		fclose(logPPosFile);
		fclose(logPosFile);
		fclose(logUPosFile);
	}

	void PHOpHapticController::ClearColliedPs()
	{
		std::vector<PvsHcCtcPInfo> s;
		s.swap(hcColliedPs);

	}
	Vec3f PHOpHapticController::GetCurrentOutputForce()
	{
		return	outForce.v();
	}
	void PHOpHapticController::setC_ObstacleRadius(float r)
	{
		c_obstRadius = r;
	}
	int PHOpHapticController::GetHpObjIndex()
	{
		return hpObjIndex;
	}
	
	PHOpParticle* PHOpHapticController::GetMyHpProxyParticle()
	{
		return &hcObj->objPArr[0];
	}
	void PHOpHapticController::AddColliedPtcl(int pIndex, int objindex, Vec3f ctcPos)
	{
		PvsHcCtcPInfo  c;
		c.objIndex = objindex;
		c.ptclIndex = pIndex;
		c.ctcPos = ctcPos;
		hcColliedPs.push_back(c);

	}
	void PHOpHapticController::SetHCReady(bool flag)
	{
		hcReady = flag;
	}

	bool PHOpHapticController::GetHCReady()
	{
		return hcReady;
	}
	bool PHOpHapticController::GetHCForceReady()
	{
		return IsSetForceReady;
	}
	void PHOpHapticController::SetHCForceReady(bool flag)
	{
		IsSetForceReady = flag;
		if (flag == false)
		{
			outForce.clear();
		}
	}
	Vec3f PHOpHapticController::GetHCPosition()
	{
		return userPos;
	}
	void PHOpHapticController::SetHCPosition(Vec3f pos)
	{
		userPos = pos * posScale;
	}
	Posef PHOpHapticController::GetHCPose()
	{
		return userPose;
	}
	void PHOpHapticController::SetHCPose(Posef pose)
	{
		pose.Pos() = pose.Pos() * posScale;
		pose.Ori() = pose.Ori() * rotScale;
		userPose = pose;
	}
	
	void PHOpHapticController::SetHCColliedFlag(bool flag)
	{
		hcCollied = flag;
	}
	
	bool PHOpHapticController::CheckProxyState()
	{
		return hcProxyOn;
	}
	void PHOpHapticController::UpdateProxyPosition(Vec3f &pos, TQuaternion<float> winPose)
	{
		hcCollied = false;


		
		PHOpParticle* dp = GetMyHpProxyParticle();
		
		dp->pCurrCtr = pos;
		dp->pCurrOrint = dp->pNewOrint;
		
		Vec3f f;
		if (hcProxyOn)
		{
			winPose = winPose.Inv();
			f = winPose * (dp->pCurrCtr - userPos) * 3;
			float magni = f.norm();
			if (magni > 10.0f)
			{
				DSTR << "Big Force Output!" << std::endl;
				f.clear();
			}
		}
		else {
			f.clear();
		}


		SetForce(f);
		
	}
	void PHOpHapticController::LogForce(TQuaternion<float> winPose)
	{
		if (logForce)
		{
			
			PHOpParticle* dp = GetMyHpProxyParticle();
			
			Vec3f f = winPose * (hcCurrPPos - hcCurrUPos) * 3;
			fprintf(logForceFile, "%f\n", f.y);
			fprintf(logPPosFile, "%f\n", hcCurrPPos.y);
			fprintf(logUPosFile, "%f\n", hcCurrUPos.y);
		}
	}
	bool PHOpHapticController::InitialHapticController()
	{//
		Vec3f* OrigPos = new Vec3f[1];
		OrigPos[0] = OrigPos[0].Zero();

		hcObj = new PHOpObj();
		if (!hcObj->initialPHOpObj(OrigPos, 1, 1.0f))
			return false;

		hcObj->objNoMeshObj = true;
		posScale = 150;
		rotScale = 1.0f;
		forceScale = 0.01f;
		hcCollied = false;

		hcObj->objType = 1;
		suspObjid = -1;
		proxyRadius = 0.03f;
		hcProxyOn = false;
		couterPlaneN.clear();
		collectCount = 4;
		collectItrtor = 0;
		constrainCount = 0;
		logForce = false;
		fileindex = 0;
		surrCnstrs = 0;

		//add 3 constrains
		ConstrainPlaneInfo tmpcpinfo, tmpcpinfo1, tmpcpinfo2, tmpcpinfo3;
		cpiVec.push_back(tmpcpinfo);
		cpiVec.push_back(tmpcpinfo1);
		cpiVec.push_back(tmpcpinfo2);
		cpiVec.push_back(tmpcpinfo3);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		//cpiHpVec.resize();
		cpiLastHpVec.push_back(tmpcpinfo);

		

		return true; //initDevice(hisdk);

	}
	bool PHOpHapticController::InitialHapticController(PHOpObj* opObject)
	{
		
		hcObj = opObject;
		hpObjIndex = hcObj->objId;
		posScale = 150;
		rotScale = 1.0f;
		forceScale = 0.01f;
		hcCollied = false;
		hcObj->objType = 1;
		suspObjid = -1;
		proxyRadius = 0.03f;
		hcProxyOn = false;
		couterPlaneN.clear();
		collectCount = 4;
		collectItrtor = 0;
		constrainCount = 0;
		logForce = false;
		fileindex = 0;
		surrCnstrs = 0;

		//add 3 constrains
		ConstrainPlaneInfo tmpcpinfo, tmpcpinfo1, tmpcpinfo2, tmpcpinfo3;
		cpiVec.push_back(tmpcpinfo);
		cpiVec.push_back(tmpcpinfo1);
		cpiVec.push_back(tmpcpinfo2);
		cpiVec.push_back(tmpcpinfo3);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		cpiHpVec.push_back(tmpcpinfo);
		cpiLastHpVec.push_back(tmpcpinfo);

		
		
		return true;
	}
	
	ObjectIf*PHOpHapticController:: GetHpOpObj()
	{
		return hcObj->Cast();
	}
	bool PHOpHapticController::SetForce(Vec3f f)
	{
		if (fabs(f.norm()) < max_output_force)
		{
			
			SetHCForceReady(true);
			outForce.v() = f;
			return true;
		}
		else return false;
	}
	
	SpatialVector PHOpHapticController::GetHCOutput()
	{
		SpatialVector tempForce = outForce;
		return tempForce;
	}


}//namespace
