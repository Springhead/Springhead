/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageJoints 関節機能のデモ
 Springhead2/src/Samples/Joints/main.cpp

\ref pagePhysics の関節機能のデモプログラム。

\section secTestPatternJoints テストパターン
- シーン0：鎖
- シーン1：チェビシェフリンク
- シーン2：バネダンパ入りジョイントとバネダンパ
- シーン3：ボールジョイント1つのテスト
- シーン4：パスジョイント
- シーン5：バネダンパ（安定なバネダンパの確認デモ） 


\section secSpecJoints 仕様
- 共通の操作：
 - シーン番号0, 1, ...をタイプすると対応するシーンに切り替わる（デフォルト：シーン0)
  - 'x'をタイプすると関節の有効/無効が切り替わる
  - 'z'で、右方向からボックスを飛ばし、衝突させる		
  - 'Z'で、手前方向からボックスを飛ばし、衝突させる
  - 'C'で、右方向からカプセルを飛ばし、衝突させる
  - 'S'で、右方向から球を飛ばし、衝突させる
  - 'P'で、シミュレーションを止める
- シーン0： 鎖
  - '0'で、ヒンジシーンの設定を行う（シーン切換え）
  - ' 'あるいは'b'でヒンジ用のboxを生成
		　' 'の場合は内部アルゴリズムはABAとなる
- シーン1：
  - '1'で、チェビシェフリンクを生成する（シーン切換え）
  - 'a'で、モータートルクを1.0に設定する
  - 's'で、モータートルクを0.0に設定する
  - 'd'で、モータートルクを-1.0に設定する
  - 'f'で、目標速度を90.0に設定する
  - 'g'で、目標速度を0.0に設定する
  - 'h'で、目標速度を-90.0に設定する
  - 'j'で、バネ原点(バネの力が0となる関節変位)を1.0に設定する
  - 'k'で、バネ原点(バネの力が0となる関節変位)を0.0に設定する
  - 'l'で、バネ原点(バネの力が0となる関節変位)を-1.0に設定する
- シーン4：
  - '4'で、パスジョイントシーンの設定を行う（シーン切換え）
  - 'a'で、重力方向を、(0.0, -9.8, 0.0)に設定する
  - 'd'で、重力方向を、(0.0,  9.8, 0.0)に設定する
  - 
- シーン5：
  - '5'で、バネ・ダンパシーンの設定を行う（シーン切換え）
  - 'a'で、剛体のフレーム原点を、位置(-20.0, 30.0, 0.0)に設定する
  - 's'で、剛体のフレーム原点を、位置(-10.0, 20.0, 0.0)に設定する
  - 'd'で、剛体のフレーム原点を、位置( -5.0, 10.0, 0.0)に設定する
  - 'f'で、剛体のフレーム原点を、位置(  0.0, 10.0, 0.0)に設定する
  - 'g'で、剛体のフレーム原点を、位置(  5.0, 10.0, 0.0)に設定する
  - 'h'で、剛体のフレーム原点を、位置( 10.0, 20.0, 0.0)に設定する
  - 'j'で、剛体のフレーム原点を、位置( 20.0, 30.0, 0.0)に設定する
  - '.'で、コールバック関数の呼び出し時間を1/2倍する
  - ','で、コールバック関数の呼び出し時間を2倍する
  - 'n'で、バネ原点を+0.01する
  - 'm'で、バネ原点を-0.01する
*/

#include <ctime>
#include <string>
#include <sstream>

#include "ChainHandler.h"
#include "LinkHandler.h"
#include "PathHandler.h"
#include "ArmHandler.h"
#include "BallJointHandler.h"
#include "HingeJointHandler.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;
using namespace std;

#define ESC		27

class MyApp : public FWApp{
public:
	UTRef<ObjectStatesIf>		state;
	vector< UTRef<Handler> >	handlers;
	Handler*					activeHandler;
	FWEditorOverlay				editor;

	bool	bAutoStep;		///< 自動ステップ

public:
	void SwitchScene(int id){
		if(id < 0 || (int)handlers.size() <= id)
			return;

		GetSdk()->SwitchScene(GetSdk()->GetScene(id));
		editor.SetObject(GetSdk()->GetScene()->GetPHScene());
		activeHandler = handlers[id];
		if(!activeHandler->bReady)
			activeHandler->Build();
		cameraInfo.Fit(GetSdk()->GetRender()->GetCamera(), activeHandler->GetSceneRadius());
	}

	/**
	 brief 		キーボードコールバック関数 
	 param		<in/--> key　　 ASCIIコード
	 param 		<in/--> x　　　 キーが押された時のマウス座標
	 param 		<in/--> y　　　 キーが押された時のマウス座標
	 return 	なし
	 */
	virtual void Keyboard(int key, int x, int y){
		if(key == ESC || key == 'q')
			exit(0);
		if(key == ' ')
			bAutoStep = !bAutoStep;
		if(key == ';')
			Step();

		PHSceneIf* phScene = activeHandler->GetPHScene();
		if (key == 'W'){
			phScene->WriteState("state.bin");
		}
		else if (key == 'R'){
			phScene->ReadState("state.bin");
		}
		else if(key == ','){
			state->SaveState(phScene);
		}
		else if(key == '.'){
			state->LoadState(phScene);
		}
		else if(key == '/'){
			state->ReleaseState(phScene);
		}

		// FWEditorで処理されなかったキー入力のみ次に回す
		if(!editor.Key(key)){
			//シーン切り替え
			if(key <= 0xff && isdigit(key)){
				SwitchScene(key - '0');
			}
			else{
				activeHandler->OnKey(key);
				// FWEditorを更新
				editor.SetObject(GetSdk()->GetScene()->GetPHScene());
			}
		}
	}

	void Step(){
		GetSdk()->Step();
		activeHandler->OnTimer();
	}

	virtual void TimerFunc(int id){
		if(bAutoStep)
			Step();
		PostRedisplay();
	}	

	virtual void Init(int argc = 0, char* argv[] = NULL){
		SetGRAdaptee(TypeGLUT);
		GRInit(argc, argv);
		CreateSdk();
		
		FWWinDesc windowDesc;
		windowDesc.title = "Joints";
		CreateWin(windowDesc);
		InitWindow();

		UTTimerIf* timer = CreateTimer();				// タイマーの生成
		timer->SetInterval(50);

		cameraInfo.zoomRange[1] = 1000.0f;
		GetSdk()->SetDebugMode(true);

		/*GRRenderIf* render = GetSdk()->GetRender();
		GRLightDesc light;
		light.position = Vec4f(10.0, 20.0, 20.0, 1.0);
		light.diffuse = Vec4f(1,1,1,1) * 0.5f;
		light.specular = Vec4f(1,1,1,1) * 0.2f;
		light.ambient = Vec4f(1,1,1,1) * 0.0f;
		render->PushLight(light);*/

		// シーンの構築
		handlers.push_back(new ChainHandler());
		handlers.push_back(new LinkHandler());
		handlers.push_back(new ArmHandler());
		handlers.push_back(new BallJointHandler());
		handlers.push_back(new HingeJointHandler());
		handlers.push_back(new PathHandler());
		for(unsigned i = 0; i < handlers.size(); i++){
			FWSceneIf* fwScene = GetSdk()->CreateScene();
			fwScene->GetPHScene()->SetTimeStep(0.05);
			fwScene->GetPHScene()->SetNumIteration(20);
			fwScene->SetRenderMode(true, false);
			fwScene->EnableRenderAxis(true, false, false);
			fwScene->EnableRenderForce();
			fwScene->EnableRenderContact();
		}

		state = ObjectStatesIf::Create();
		SwitchScene(0);
		bAutoStep = false;
	}

	virtual void Display(){
		// 描画モードの設定
		GRRenderIf* render = GetCurrentWin()->render->Cast();
		GRCameraDesc cam = render->GetCamera();
		cam.front = 0.5f;
		render->SetCamera(cam);
		render->SetViewMatrix(cameraInfo.view.inv());

		// 描画の実行
		GetSdk()->Draw();

		// 情報のオーバレイ
		editor.Update();
		editor.Draw(render);
		
		glutSwapBuffers();
	}

} app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
}
