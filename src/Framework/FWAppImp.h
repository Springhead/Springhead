#ifndef FW_APP_IMP_H
#define FW_APP_IMP_H

#include<sstream>
#include<string>
#include<vector>
#include<Springhead.h>

using namespace Spr;
using namespace std;

class FWAppAdapter;

class FWAppImpDesc{
public:
	string sceneName;									// シーンの名前
	double pdt;
	double nIter;
	Vec3d gravity;
	double range;
	FWAppImpDesc(){
		sceneName = "";									// シーンの名前
		pdt = 0.05f;
		nIter = 45;
		gravity =  Vec3d(0, -9.8f, 0);
		range = 0.7;
	}
};

class FWAppImp :public FWAppImpDesc{
protected:
	// アダプタ，シーン
	FWAppAdapter* adapter;
	FWSceneIf* fwscene;
	HISpidarGIf* spidarG6;
public:	
	// Sdk関係
	FWAppImp();
	void SetAdapter(FWAppAdapter* a);			// FWAppAdapterの設定
	FWSdkIf* GetSdk();									// FWSdkIfの取得
	void SetScene(FWSceneIf* s);					// FWSceneIfの設定
	FWSceneIf* GetScene();							// FWSceneIfの取得
	void SetSpidar(HISpidarGIf* s);					// HISpidarGIfの設定
	HISpidarGIf* GetSpidar();							// HISpidarGIfの取得
	string GetSceneName();							// シーンの名前を取得

	// シミュレーション関係
	virtual void Init();									// シーンの作成
	virtual void ResetScene();							// シーンのリセット
	virtual void BuildScene();							// 初期シーンの構築 
	virtual void Idle();									// glutIdleFuncで呼ばれる
	virtual void Step();									// シミュレーションを進める
	virtual void UpdateHapticPointer();				// 力覚ポインタの位置，姿勢の更新								
	virtual void CallBack();								// MultiMediaTimerでのコールバック
	virtual void DebugMode(bool bDebug, GRDebugRenderIf* render);		// DebugModeがtrueの時の処理
	virtual void Keyboard(int key, int x, int y);	// キーボード
};

#endif