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
- シーン2：シリアルリンク

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

//#include <ctime>
//#include <string>
//#include <sstream>

#include "../../SampleApp.h"

#include "ChainHandler.h"
#include "SpringHandler.h"
#include "LinkHandler.h"
#include "ArmHandler.h"
#include "GearHandler.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace Spr;
using namespace std;

#define ESC		27

class MyApp : public SampleApp{
public:
	vector< UTRef<Handler> >	handlers;
	
public:
	MyApp(){
		appName		= "Joints";
		
		// ハンドラ登録
		handlers.push_back(DBG_NEW ChainHandler (this));
		handlers.push_back(DBG_NEW SpringHandler(this));
		handlers.push_back(DBG_NEW LinkHandler  (this));
		handlers.push_back(DBG_NEW ArmHandler   (this));
		handlers.push_back(DBG_NEW GearHandler  (this));
		numScenes	= (int)handlers.size();
	}
	virtual ~MyApp(){}

	virtual void BuildScene(){
		handlers[curScene]->BuildScene();
	}

	virtual void OnAction(int menu, int id){
		if(menu == MENU_SCENE + curScene)
			handlers[curScene]->OnAction(id);
		SampleApp::OnAction(menu, id);
	}

	virtual void OnStep(){
		SampleApp::OnStep();
		handlers[curScene]->OnStep();
	}

	virtual void OnDraw(GRRenderIf* render){
		SampleApp::OnDraw(render);
		handlers[curScene]->OnDraw(render);
	}

} app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
#if _MSC_VER <= 1500
#include <GL/glut.h>
#endif
int SPR_CDECL main(int argc, char* argv[]){
	//_CrtSetBreakAlloc(176373);
	app.Init(argc, argv);
	app.StartMainLoop();
}
