#ifndef FWAPPSAMPLE_H								// ヘッダファイルを作る時のおまじない
#define FWAPPSAMPLE_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include "../../SampleApp.h"

class FWMyApptest : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_BOX,
	};

public:	
	//FWMyApptest();
	//コンストラクタ
	FWMyApptest(){
		appName = "FEMThermo";

		AddAction(MENU_MAIN, ID_BOX, "drop object");
		AddHotKey(MENU_MAIN, ID_BOX, 'd');

	}
	//デストラクタ
	~FWMyApptest(){}

	virtual void BuildScene(){
		UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
		GetSdk()->LoadScene("scene.spr", import);			// ファイルのロード
	}

	//1ステップのシミュレーション	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void OnStep() {
		SampleApp::OnStep();
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		SampleApp::OnDraw(render);

		std::ostringstream sstr;
		sstr << "NObj = " << phScene->NSolids();
		render->DrawFont(Vec2f(-21, 23), sstr.str());
	}

	//アクション処理
	virtual void OnAction(int menu, int id){
		if(menu == MENU_MAIN){
			Vec3d v, w(0.0, 0.0, 0.2), p(0.5, 20.0, 0.0);
			Quaterniond q = Quaterniond::Rot(Rad(30.0), 'y');

			if(id == ID_BOX){
				Drop(SHAPE_BOX, GRRenderIf::RED, v, w, p, q);
				message = "box dropped.";
			}
		}
		SampleApp::OnAction(menu, id);
	}

};

#endif