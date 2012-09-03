#include "../../SampleApp.h"

#include <EmbPython/EmbPython.h>

#include <sstream>

#include "windows.h"

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// アプリケーションクラス
class MyEPApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_TOGGLE_RENDER_BACKSCENE,
		ID_CALIB_SPIDAR,
		ID_RESET,
	};

	UTRef<HISdkIf>             hiSdk;
	UTRef<HISpaceNavigatorIf>  spaceNavigator0;
	UTRef<HISpaceNavigatorIf>  spaceNavigator1;

	int argc;
	char** argv;

	MyEPApp(){
		appName = "Python with Springhead";
		numScenes   = 1;
	}
	~MyEPApp(){}

	// 初期化
	virtual void Init(int argc, char* argv[]);

	virtual void OnStep(){
		UTAutoLock critical(EPCriticalSection);
		spaceNavigator0->Update(fwScene->GetPHScene()->GetTimeStep());
		spaceNavigator1->Update(fwScene->GetPHScene()->GetTimeStep());
		fwScene->Step();
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		UTAutoLock critical(EPCriticalSection);
		SampleApp::OnDraw(render);
	}

	virtual void OnAction(int menu, int id){
		UTAutoLock critical(EPCriticalSection);
		SampleApp::OnAction(menu, id);
	}

	// ----- ----- ----- ----- -----

	void Drop(int shape, int mat, Vec3d v, Vec3d w, Vec3d p, Quaterniond q){
		SampleApp::Drop(shape, mat, v, w, p, q);
	}

};
