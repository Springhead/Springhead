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
	int nIter;
	Vec3d gravity;
	double range;
	FWAppImpDesc();
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
	void SetHapticInterface(HISpidarGIf* s);		// HISpidarGIfの設定
	HISpidarGIf* GetHapticInterface();				// HISpidarGIfの取得
	string GetSceneName();							// シーンの名前を取得

	//デバック表示に使う変数
	bool bDebug;
	bool bStep;
	bool bOneStep;

	// シミュレーション関係
	virtual void Init();									// シーンの作成
	virtual void ResetScene();							// シーンのリセット
	virtual void BuildScene();							// 初期シーンの構築
	virtual void Idle();									// glutIdleFuncで呼ばれる
	virtual void Step();									// シミュレーションを進める
	virtual void CallBack();								// MultiMediaTimerでのコールバック
	virtual void DebugMode(bool bDebug, GRDebugRenderIf* render);		// DebugModeがtrueの時の処理
	virtual void Keyboard(int key, int x, int y);	// キーボード
};

#endif