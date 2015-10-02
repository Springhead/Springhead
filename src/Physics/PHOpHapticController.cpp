#include "Physics\PHOpHapticController.h"



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


	/*void PHOpHapticController::BindCtcPlane(ConstrainPlaneInfo cif)
	{
		hcBindCpi = cif;
	}*/
	void PHOpHapticController::SetHcColliedFlag(bool flag)
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


		//PHOpParticleIf *dpif = GetMyHpProxyParticle();
		PHOpParticle* dp = GetMyHpProxyParticle();
		//dp->pVelocity = (dp->pCurrCtr - pos)/params.timeStep ;
		dp->pCurrCtr = pos;
		dp->pCurrOrint = dp->pNewOrint;
		//targetVts[0]= pos;

		Vec3f f;
		if (hcProxyOn)
		{
			winPose = winPose.Inv();
			f = winPose * (dp->pCurrCtr - userPos) * 3;// *params.alpha;
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


		currSpg->SetForce(f, Vec3f());
		//std::DSTR<<"Force = "<<f<<std::endl;
	}
	void PHOpHapticController::LogForce(TQuaternion<float> winPose)
	{
		if (logForce)
		{
			
			PHOpParticle* dp = GetMyHpProxyParticle();
			//Vec3f f = winPose * (dp->pCurrCtr - userPos) * 3;// *params.alpha;
			Vec3f f = winPose * (hcCurrPPos - hcCurrUPos) * 3;//debug hapticrate
			//const int len = 100;
			//char *s = new char[len +1];
			//fprintf(logForceFile, "%f %f %f\n",f.x,f.y,f.z);
			fprintf(logForceFile, "%f\n", f.y);
			//fprintf(logPosFile, "%f %f %f\n",dp->pCurrCtr.x,dp->pCurrCtr.y,dp->pCurrCtr.z);
			fprintf(logPPosFile, "%f\n", hcCurrPPos.y);
			//fprintf(logUPosFile, "%f\n", userPos.y);
			//debug hapticrate
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
		forceScale = 0.01f;
		hcReady = false;
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

		

		return initDevice();

	}
	bool PHOpHapticController::InitialHapticController(PHOpObj* opObject)
	{
		////if (hpMesh.enabled)
		//{
		//	PHOpObj::intialPHOpObj(&hpMesh);
		//	return true;
		//}
		//objOrigPos = new Vec3f[vtsNum];
		//initialOrgP = true;
		//objTargetVts = vts;//Tetgenため使う
		//objTargetVtsNum = vtsNum;

		//initialDeformVertex(vts, vtsNum);
		//if (!BuildParticles(vts, vtsNum, tmpPtclList, pSize))
		//	return false;

		
		hcObj = opObject;
		hpObjIndex = hcObj->objId;
		posScale = 150;
		forceScale = 0.01f;
		hcReady = false;
		hcCollied = false;
		//hcColliedPid = -1;
		//hcObj->objNoMeshObj = true;//->only use for no mesh objec(like one ball)
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

		
		
		return initDevice();
	}
	
	ObjectIf*PHOpHapticController:: GetHpOpObj()
	{
		return hcObj->Cast();
	}

	bool PHOpHapticController::initDevice()
	{
		// 力覚インタフェースとの接続設定
		hiSdk = HISdkIf::CreateSdk();

		// win32
		DRUsb20SimpleDesc usbSimpleDesc;
		hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
		DRUsb20Sh4Desc usb20Sh4Desc;
		for (int i = 0; i<10; ++i){
			usb20Sh4Desc.channel = i;
			hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
		}
		// win64
		DRCyUsb20Sh4Desc cyDesc;
		for (int i = 0; i<10; ++i){
			cyDesc.channel = i;
			hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
		}
		hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
		hiSdk->Print(DSTR);
		//hiSdk->Print(std::DSTR);

		UTRef<HISpidarGIf> spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
		hcReady = spg->Init(&HISpidarGDesc("SpidarG6X3R"));

		if (hcReady)
		{
			spg->Calibration();
			spg->Update(0.001f);
		}
		else return false;
		
		currSpg = spg;
		return true;
	}
	bool PHOpHapticController::doCalibration()
	{
		if (currSpg->Calibration())
		{
			currSpg->Update(0.001f);
			return true;
		}
		return false;
	}



}//namespace