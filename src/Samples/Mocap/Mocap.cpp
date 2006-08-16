// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <tchar.h>
#include <Springhead.h>
#include <SprFramework.h>
#include <Foundation/WBPreciseTimer.h>
#include <Framework/FWAppGL.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "THuman.h"
#include "TDiffFile.h"


namespace Spr{
	void SPR_CDECL PHRegisterTypeDescs();
	void SPR_CDECL CDRegisterTypeDescs();
	void SPR_CDECL GRRegisterTypeDescs();
	void SPR_CDECL FIRegisterTypeDescs();

	void SPR_CDECL FWRegisterTypeDescs();
	void SPR_CDECL FWRegisterFactories();

class MyApp:public FWAppGL{
public:
	THuman human;						//	Virtual Human
	TDiffFile diff;						//	Diff ファイル
	enum TAppState{						//	アプリケーションの状態
		INVALID,						//	無効
		MAKE_ANGLE,						//	DIFFファイルの頂点情報から角度ファイルを生成
		MAKE_FORCE,						//	床反力計算
		NON_GRAVITY						//	腰固定のトルク入力
	} appState;
	int stepCount;
	int waitCount;
	int stepTime;
	double time;
	WBPreciseTimer ptimer;

	MyApp(){
		appState = INVALID;
		stepCount = 0;
		waitCount = 0;
	}
	void Display(){
		FWAppGL::Display();

		if (stepTime==0) stepTime=1000;
		std::ostringstream os;
		os << std::setw(5) << 1000000/stepTime << "FPS";
		if (appState == MAKE_ANGLE){
			os << "    マーカ位置計測データ → 関節角データ  データ変換中";
		}else{
			os << "    関節角データ再生 ＋ 床反力シミュレーション中";
		}
		grRender->DrawFont(Vec2f(0,0), os.str().c_str());
	}
	void Step(){
		stepTime = ptimer.CountUS();
		//-------- Diffファイル再生＆各関節角度を保存------------//
		if (appState == MAKE_ANGLE){
			if (waitCount++ < 300) time = 0;
			bool rv = diff.AddSpringForce(human, time, (float)phScene->GetTimeStep());
			if (time > 0) human.SaveJointAngle();
			if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
				Load();
				waitCount = 0;
			}
		}
	//-------- 保存された各関節角度の再生＆腰の補正＆各関節のトルクの保存------------//
		else if (appState == MAKE_FORCE){
			if (waitCount++ < 500){
				time = 0.0f;
				diff.AddSpringForce(human, time, (float)phScene->GetTimeStep());
				human.SetSpring((float)phScene->GetTimeStep());
				human.LoadJointAngle(time, (float)phScene->GetTimeStep());
			}else if (waitCount++ < 2000){
				time = 0.0f;
				human.LoadJointAngle(time, (float)phScene->GetTimeStep());
			}else{
				//human.SaveTorque(time, scene->GetTimeStep());
				bool rv = human.LoadJointAngle(time, phScene->GetTimeStep());
				if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
					std::ofstream of("force.txt");
					for(float t=0.001f; t<time; t+= 0.001f){
						int c = t / phScene->GetTimeStep();
						TDiffRecord pos, vel;
						diff.frf.GetRecord(pos, vel, t);
						if (c < (int)human.angles.size()){
							of << t << "\t";
							of << pos.data[1].X() << "\t";
							of << pos.data[1].Y() << "\t";
							of << pos.data[1].Z() << "\t";
							of << human.angles[c].force.X() << "\t";
							of << human.angles[c].force.Y() << "\t";
							of << human.angles[c].force.Z() << "\t";
							for(int i=0; i<human.joints.size(); ++i){
								of << human.angles[c].jointTorque[i] << "\t";
							}
							of << std::endl;
						}
					}
					Load();
					waitCount = 0;
				}
			}
		}
		FWAppGL::Step();
	}
	void Load(){
		LoadFile(filename);
	}
};

}

using namespace Spr;

int _tmain(int argc, _TCHAR* argv[])
{
	PHRegisterTypeDescs();
	CDRegisterTypeDescs();
	GRRegisterTypeDescs();
	FIRegisterTypeDescs();
	FWRegisterTypeDescs();
	FWRegisterFactories();

	PHRegisterSdk();
	GRRegisterSdk();
	FWRegisterSdk();

	FWAppGLIf* fwApp = CreateFWAppGL();
//	fwApp->SetDebugMode(FWAppGLDesc::DM_DEBUG);
	fwApp->StartApp("vhOld.x");

	return 0;
}

