/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/*
	OrientedParticle例の使用方法
	1.始まったら、ウサギのモデルをロードして、画面を表示します
	2.'g'keyを押すと、Opに重力を掛ける
	3.'d'keyを押すと、physicsSceneに切り替えて、'p'keyを押すと、粒子と向きが表示されます。
	4.それから'y'keyを押すと、Mouseの左ボタンで粒子を遊べる。
	5.'+'/'-'keyを押すと、モデルの硬さを調整することができる。
	6.'F5'から'F8'までいずれか、下が固定しているウサギモデルをロードすることができる

	Manual of example of Oriented particle
	1.Initialor will load the bunny model, and display it
	2.Push 'g' to enable gravity
	3.Push 'd' to swich to physical scene, and then push 'p' to display particle and orientation
	4.After step 3, push 'y', enable mouse left button to catch particle
	5.Push '+'/'-' to adjust stiffness of the model
	6.You can use any button from 'F5' to 'F8' to load a bunny with fixed bottom
*/


#ifndef PHOPDEMO_H						// ヘッダファイルを作る時のおまじない
#define PHOPDEMO_H						// 同上

#include <Springhead.h>								// SpringheadのAPIを使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル
#include <Physics/PHOpParticle.h>


using namespace Spr;								// Springheadの名前空間

class PHOpDemo : public FWApp{
public:
	std::string fileName;							///< ロードするファイル名

	Vec3f infiniP;

	PHOpDemo();							///< コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		///< アプリケーションの初期化
	virtual void Keyboard(int key, int x, int y);	///< キーボード関数

	void InitCameraView();							///< 初期カメラ位置の設定		
	void Reset();									///< リセット
	void TimerFunc(int id);
	void Display();
	bool OnMouseMove(int button, int x, int y, int zdelta);
	void MouseButton(int button, int state, int x, int y);
	bool OnMouse(int button, int state, int x, int y);

	//粒子描画スウィチ
	bool drawPs;
	bool gravity;
	bool DrawHelpInfo;
	bool drawlink;

	//Mouse control
	float recX,recY;
	float mouseX, mouseY;
	Vec3f cameraPosition;
	TPose<float> cameraPose;
	Vec3f cameraTarget;

	int mousehitP;
	int ediObj;
	int ediPtcl;
	int mousehitObj;
	int  mouselockObj;
	int  mouselockP;
	bool mouseHit;
	//マウス操作スウィチ
	bool useMouseSelect;
	//粒子グループリンクモード
	bool addGrpLink;
	//粒子固定
	bool fixPs;
	//オブジェクト操作スウィチ
	bool useMouseSlcObj;
	//マウスで粒子移動の縦と横の切り替え
	bool mouseVertmotion;
	//左ボタン
	bool mouseLbtn;
	//グループ編集モードスウィチ
	bool pgroupEditionModel;
	//初回粒子
	bool ediFirP;
	//マウスが触ったとき粒子の情報を表示
	bool checkPtclInfo;

	//opのTimerId
	int opSimuTimerId;

	//OpFileStateStore
	//FileOp fileOp;
	//meshname
	const char* mymeshname;
}; 

#endif
