/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_EP_APP_H
#define SPR_EP_APP_H

/**
	Python用アプリケーションベースクラス．Python上でNewされることを想定している．
 **/

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>
#include <map>

#include <EmbPython/SprEPVersion.h>
#include PYTHON_H_PATH

using namespace std;
namespace Spr{;

#define ESC		27

class EPApp : public FWApp {
	enum HumanInterface{
		SPIDAR,				
		XBOX,				
		FALCON,
	} humanInterface;

	enum EngineType{
		SINGLE = 0,				// シングルすれっど
		MULTI,					// マルチスレッド
		LD,						// マルチ+局所シミュレーション
	} engineType;

	PHSceneIf* phScene;			// PHSceneへのポインタ
	PHHapticPointerIf* pointer; // 力覚ポインタへのポインタ
	float pdt;					// 物理スレッドの刻み
	float hdt;					// 力覚スレッドの刻み
	int physicsTimerID;			// 物理スレッドのタイマ
	int hapticTimerID;			// 力覚スレッドのタイマ
	UTRef<HIBaseIf> spg;		// 力覚インタフェースへのポインタ

	PyObject* afterStepFunc;	// Step後にCallbackされるPythonの関数

	bool bPhysicsEnabled;		// シミュレーションを実行するか

public:
	// ----- ----- ----- ----- -----

	EPApp() {
		pdt = 0.02f;
		hdt = 0.001f;

		engineType		= MULTI;
		humanInterface	= SPIDAR;
		afterStepFunc	= NULL;

		bPhysicsEnabled = false;
	}

	void SetAfterStepFunc(PyObject* func) {
		if (!PyCallable_Check(func)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return;
		}
		Py_XINCREF(func);
		Py_XDECREF(afterStepFunc);									// 以前のコールバックを捨てる
		afterStepFunc = func;										// 新たなコールバックを記憶
	}

	void CallAfterStepFunc() {
		if (afterStepFunc != NULL) {
			PyEval_InitThreads();
			PyGILState_STATE state = PyGILState_Ensure();
			PyObject_CallObject(afterStepFunc, NULL);
			PyGILState_Release(state);
		}
	}

	void EnablePhysics(bool e) {
		UTAutoLock LOCK(EPCriticalSection);
		bPhysicsEnabled = e;
	}

	void InitInterface() {
		UTAutoLock LOCK(EPCriticalSection);

		HISdkIf* hiSdk = GetSdk()->GetHISdk();

		if(humanInterface == SPIDAR){
			// x86
			DRUsb20SimpleDesc usbSimpleDesc;
			hiSdk->AddRealDevice(DRUsb20SimpleIf::GetIfInfoStatic(), &usbSimpleDesc);
			DRUsb20Sh4Desc usb20Sh4Desc;
			for(int i=0; i< 10; ++i){
				usb20Sh4Desc.channel = i;
				hiSdk->AddRealDevice(DRUsb20Sh4If::GetIfInfoStatic(), &usb20Sh4Desc);
			}
			// x64
			DRCyUsb20Sh4Desc cyDesc;
			for(int i=0; i<10; ++i){
				cyDesc.channel = i;
				hiSdk->AddRealDevice(DRCyUsb20Sh4If::GetIfInfoStatic(), &cyDesc);
			}
			hiSdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());
			hiSdk->Print(DSTR);
			hiSdk->Print(std::cout);

			spg = hiSdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
			spg->Init(&HISpidarGDesc("SpidarG6X3R"));
			spg->Calibration();
		}else if(humanInterface == XBOX){
			spg = hiSdk->CreateHumanInterface(HIXbox360ControllerIf::GetIfInfoStatic())->Cast();
		}else if(humanInterface == FALCON){
			spg = hiSdk->CreateHumanInterface(HINovintFalconIf::GetIfInfoStatic())->Cast();
			spg->Init(NULL);
		}
	}

	void CreatePointer() {
		UTAutoLock LOCK(EPCriticalSection);

		FWSdkIf* fwSdk = GetSdk();
		PHSdkIf* phSdk = fwSdk->GetPHSdk();
		phScene = phSdk->GetScene(0);

		pointer = phScene->CreateHapticPointer();	// 力覚ポインタの作成

		CDSphereDesc cd;
		cd.radius = 0.1f;
		cd.material.mu = 0.4f;
		pointer->AddShape(phSdk->CreateShape(cd));	// シェイプの追加

		Posed defaultPose;
		defaultPose.Pos() = Vec3d(0.0, -0.35, 0.0);	
		pointer->SetDefaultPose(defaultPose);		// 力覚ポインタ初期姿勢の設定
		pointer->SetInertia(pointer->GetShape(0)->CalcMomentOfInertia());	// 慣性テンソルの設定
		pointer->SetLocalRange(0.1f);				// 局所シミュレーション範囲の設定
		pointer->SetPosScale(50);					// 力覚ポインタの移動スケールの設定
		pointer->SetReflexSpring(5000);				// バネ係数の設定
		pointer->SetReflexDamper(0.1 * 0.0);		// ダンパ係数の設定
		pointer->EnableFriction(false);				// 摩擦を有効にするかどうか

		FWHapticPointerIf* fwPointer = GetSdk()->GetScene(0)->CreateHapticPointer();	// HumanInterfaceと接続するためのオブジェクトを作成
		fwPointer->SetHumanInterface(spg);		// HumanInterfaceの設定
		fwPointer->SetPHHapticPointer(pointer); // PHHapticPointerIfの設定
	}

	void Initialize(){
		// ----- ----- ----- ----- -----
		// 最も基本的な初期化処理
		{
			UTAutoLock LOCK(EPCriticalSection);
			CreateSdk();									// SDK初期化
		}
		InitInterface();									// インタフェースの初期化
		InitScene();

		instance = this;
	}

	void InitScene() {
		UTAutoLock LOCK(EPCriticalSection);

		// for (int i=0; i<NTimers(); ++i) { GetTimer(i)->Stop(); }

		// GetSdk()->Clear();
		GetSdk()->CreateScene();							// シーンを作成

		// ----- ----- ----- ----- -----
		// HapticAppの初期化処理
		CreatePointer();									// 力覚ポインタの作成
		PHHapticEngineIf* he = phScene->GetHapticEngine();	// 力覚エンジンをとってくる
		he->Enable(true);            						// 力覚エンジンの有効化

		if(engineType == SINGLE){
			// シングルスレッドモード
			he->SetHapticStepMode(PHHapticEngineDesc::SINGLE_THREAD);
			phScene->SetTimeStep(hdt);
		}else if(engineType == MULTI){
			// マルチスレッドモード
			he->SetHapticStepMode(PHHapticEngineDesc::MULTI_THREAD);
			phScene->SetTimeStep(pdt);
		}else if(engineType == LD){
			// 局所シミュレーションモード
			he->SetHapticStepMode(PHHapticEngineDesc::LOCAL_DYNAMICS);
			phScene->SetTimeStep(pdt);
		}

		{
			UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 物理スレッド用のマルチメディアタイマを作成
			timer->SetResolution(1);								// 分解能(ms)
			timer->SetInterval(unsigned int(pdt * 1000));			// 刻み(ms)h
			physicsTimerID = timer->GetID();						// 物理スレッドのタイマIDの取得
			timer->Start();											// タイマスタート
		}

		{
			UTTimerIf* timer = CreateTimer(UTTimerIf::MULTIMEDIA);	// 力覚スレッド用のマルチメディアタイマを作成
			timer->SetResolution(1);								// 分解能(ms)
			timer->SetInterval(unsigned int(hdt * 1000));			// 刻み(ms)h
			hapticTimerID = timer->GetID();							// 力覚スレッドのタイマIDの取得
			timer->Start();											// タイマスタート
		}
	}

	virtual void Display(){
		if (bThread) {
			UTAutoLock LOCK(EPCriticalSection);
			GetCurrentWin()->Display();
		}else{
			GetCurrentWin()->Display();
		}
	}
	void TimerFunc(int id) {
		if (! bPhysicsEnabled) { return; }

		if(engineType == 0){
			if(hapticTimerID == id){
				GetSdk()->GetScene(0)->UpdateHapticPointers();
			}
		}else if(engineType > 0){
			if(hapticTimerID == id){
				UTAutoLock LOCK(EPCriticalSection);
				GetSdk()->GetScene(0)->UpdateHapticPointers();
				phScene->StepHapticLoop();
			}else{
				PHHapticEngineIf* he = phScene->GetHapticEngine();
				he->StepPhysicsSimulation();
				/* <!!>
				if (he->IsAfterStepPhysicsSimulation()) {
					CallAfterStepFunc();
				}
				*/
			}
		}
	}

	void Finalize() {
		GetTimer(0)->Stop();
		GetTimer(1)->Stop();
	}
};




#if 0
class EPApp : public FWApp{
public:
	/** メニューID
		MENU_ALWAYSはいつでも表示される
		シーンに対応するメニューは1～99のIDを使用(シーンは99個まで)
		100以降を共有メニューが使用
	 */
	enum MenuID{
		MENU_ALWAYS		= 0,			///< いつでも有効なメニュー
		MENU_SCENE		= 1,			///< シーンメニュー
		MENU_COMMON		= 100,			///< 共有メニュー
		MENU_CONFIG		= MENU_COMMON,	///< パラメータ設定系
		MENU_DRAW,						///< 描画設定系
		MENU_STATE,						///< ステート保存系
		MENU_EDITOR,					///< FWEditorを表示
		MENU_COMMON_LAST,
	};
	/// アクションID
	/// 常につかえるアクション
	enum ActionAlways{
		ID_EXIT,					///< 終了
		ID_RUN,						///< シミュレーションの開始と停止
		ID_STEP,					///< ステップ実行
	};
	/// ステートの保存と復帰（未検証）
	enum ActionState{
		ID_LOAD_STATE,
		ID_SAVE_STATE,
		ID_RELEASE_STATE,
		ID_READ_STATE,
		ID_WRITE_STATE,
		ID_DUMP,
	};
	/// 物理シミュレーションの設定
	enum ActionConfig{
		ID_SWITCH_LCP_PENALTY,		///< LCPとペナルティ法を切り替える
		ID_TOGGLE_GRAVITY,			///< 重力の有効化と無効化
		ID_TOGGLE_JOINT,			///< 関節の有効化と無効化
		ID_TOGGLE_ABA,				///< ABAの有効化と無効化
		ID_INC_TIMESTEP,			///< タイムステップを増やす
		ID_DEC_TIMESTEP,			///< タイムステップを減らす
		ID_INC_TIMER,				///< タイマ周期を増やす
		ID_DEC_TIMER,				///< タイマ周期を減らす
	};
	/// 描画の設定
	enum ActionDraw{
		ID_DRAW_SOLID,				///< ソリッド
		ID_DRAW_WIREFRAME,			///< ワイヤフレーム
		ID_DRAW_AXIS,				///< 座標軸
		ID_DRAW_FORCE,				///< 力
		ID_DRAW_CONTACT,			///< 接触断面
	};

	/// 形状ID
	enum ShapeID{
		SHAPE_BOX,
		SHAPE_CAPSULE,
		SHAPE_ROUNDCONE,
		SHAPE_SPHERE,
		SHAPE_ROCK,
		SHAPE_BLOCK,
	};
	
	/// アクション情報
	struct Action{
		int			id;							///< アクションID
		bool		toggle;						///< On/Offタイプのアクション
		bool		enabled;					///< OnかOff
		vector< pair<int, UTString> > keys;		///< キーと代替テキスト
		UTString	desc[2];					///< 説明(0: Onにするとき, 1: Offにするとき)
		UTString	message[2];					///< アクション後メッセージ(0: Onにしたとき，1: Offにしたとき）
	};
	struct Menu : map<int, Action>{
		UTString	brief;						///< メニューの説明
		/// キーに対応するアクションIDを返す
		int Query(int key){
			for(iterator it = begin(); it != end(); it++){
				Action& a = it->second;
				for(int i = 0; i < (int)a.keys.size(); i++){
					if(a.keys[i].first == key)
						return a.id;
				}
			}
			return -1;
		}
	};
	typedef map<int, Menu>	Menus;
	Menus					menus;
	int						curMenu;		///< 表示中の共有メニュー
	stringstream			ss;
	UTString				message;		///< 一行メッセージ
	
	FWEditorOverlay			editor;			///< 内部情報の表示/編集機能
	FWSceneIf*				fwScene;		///< アクティブなシーン
	PHSceneIf*				phScene;
	UTRef<ObjectStatesIf>	states;			///< 状態保存用
	UTTimerIf*				timer;			///< タイマ

	/// 床用の形状
	CDBoxIf*				shapeFloor;
	CDBoxIf*				shapeWall;
	/// 作りおき形状
	CDBoxIf*				shapeBox;
	CDSphereIf*				shapeSphere;
	CDCapsuleIf*			shapeCapsule;
	CDRoundConeIf*			shapeRoundCone;
	
	/// 状態
	bool					showHelp;		///< ヘルプ表示
	int						curScene;		///< アクティブなシーンの番号
	Vec3d					tmpGravity;		///< 重力無効化時の退避変数
	
	/// 属性: 派生クラスがコンストラクタで設定する
	UTString				appName;		///< サンプル名. 派生クラスがコンストラクタで設定する
	int						numScenes;		///< シーンの数
	
	/// ヘルプの描画属性
	float ymargin;
	float xmargin;
	float yline;
	float xkeys;
	float xbrief;

public:
	/// メニューの登録
	void AddMenu(int menu, UTString brief){
		menus[menu].brief = brief;
	}
	/// アクションの登録
	void AddAction(int menu, int id, UTString desc, UTString msg = "", UTString descd = "", UTString msgd = ""){
		Action& act = menus[menu][id];
		act.id		= id;
		act.enabled = true;
		act.desc[0] = desc;
		act.desc[1] = (descd == "" ? desc : descd);
		act.message[0] = msg;
		act.message[1] = (msgd == "" ? msg : msgd);
	}
	/// アクションとキーの対応
	void AddHotKey(int menu, int id, int key, UTString alt = ""){
		menus[menu][id].keys.push_back(make_pair(key, alt));
	}
	/// On/Offの反転
	bool ToggleAction(int menu, int id){
		Action& act = menus[menu][id];
		return act.enabled = !act.enabled;
	}

	/// 床の作成
	PHSolidIf* CreateFloor(){
		PHSolidIf* soFloor = phScene->CreateSolid();
		soFloor->SetDynamical(false);
		soFloor->SetFramePosition(Vec3f(0,-1,0));
	
		soFloor->AddShape(shapeFloor);
		soFloor->AddShape(shapeWall);
		soFloor->AddShape(shapeWall);
		soFloor->AddShape(shapeWall);
		soFloor->AddShape(shapeWall);
		soFloor->SetShapePose(1, Posed::Trn(-60, 0,   0));
		soFloor->SetShapePose(2, Posed::Trn(  0, 0, -40));
		soFloor->SetShapePose(3, Posed::Trn( 60, 0,   0));
		soFloor->SetShapePose(4, Posed::Trn(  0, 0,  40));

		fwScene->SetSolidMaterial(GRRenderIf::GRAY, soFloor);

		return soFloor;
	}

	void SwitchScene(int id){
		if(id < 0 || numScenes <= id)
			return;
		curScene = id;
		// id番目のシーンを選択
		fwScene = GetSdk()->GetScene(id);
		phScene = fwScene->GetPHScene();
		GetCurrentWin()->SetScene(fwScene);
		editor.SetObject(phScene);
		//cameraInfo.Fit(GetSdk()->GetRender()->GetCamera(), activeHandler->GetSceneRadius());
	}
	
	/** @brief	色々な形の物体を作成
		@param	shape	形
	 */
	void Drop(int shape, int mat, Vec3d v, Vec3d w, Vec3d p, Quaterniond q){
		// ステートを解放
		states->ReleaseState(phScene);
	
		// 剛体を作成
		PHSolidIf* solid = phScene->CreateSolid();
		// マテリアルを設定
		fwScene->SetSolidMaterial(mat, solid);
			
		// 形状の割当て
		if(shape == SHAPE_BOX)
			solid->AddShape(shapeBox);
		if(shape == SHAPE_CAPSULE)
			solid->AddShape(shapeCapsule);
		if(shape == SHAPE_ROUNDCONE)
			solid->AddShape(shapeRoundCone);
		if(shape == SHAPE_SPHERE)
			solid->AddShape(shapeSphere);
		if(shape == SHAPE_ROCK){
			CDConvexMeshDesc md;
			int nv = rand()%100 + 50;
			for(int i=0; i < nv; ++i){
				Vec3d v;
				for(int c=0; c<3; ++c){
					v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
				}
				md.vertices.push_back(v);
			}
			solid->AddShape(GetSdk()->GetPHSdk()->CreateShape(md));
		}
		if(shape == SHAPE_BLOCK){
			for(int i = 0; i < 7; i++)
				solid->AddShape(shapeBox);
			Posed pose;
			pose.Pos() = Vec3d( 3,  0,  0); solid->SetShapePose(1, pose);
			pose.Pos() = Vec3d(-3,  0,  0); solid->SetShapePose(2, pose);
			pose.Pos() = Vec3d( 0,  3,  0); solid->SetShapePose(3, pose);
			pose.Pos() = Vec3d( 0, -3,  0); solid->SetShapePose(4, pose);
			pose.Pos() = Vec3d( 0,  0,  3); solid->SetShapePose(5, pose);
			pose.Pos() = Vec3d( 0,  0, -3); solid->SetShapePose(6, pose);
		}

		solid->SetVelocity(v);
		solid->SetAngularVelocity(w);
		solid->SetFramePosition(p);
		solid->SetOrientation(q);
	}

	///
	void Shoot(int shape, int mat){

	}

	/// メニューの表示
	void DrawMenu(GRRenderIf* render, int id, Vec2f& offset){
		Vec2f pos;

		Menu& menu = menus[id];

		render->DrawFont(pos + offset, menu.brief);
		pos.y += yline;

		for(Menu::iterator it = menu.begin(); it != menu.end(); it++){
			Action& a = it->second;
			// ホットキー
			ss.str("");
			for(int i = 0; i < (int)a.keys.size(); i++){
				if(a.keys[i].second.empty())
					 ss << (char)a.keys[i].first;
				else ss << a.keys[i].second;
				ss << ' ';
			}
			render->DrawFont(pos + offset, ss.str());

			// 説明
			pos.x = xbrief;
			render->DrawFont(pos + offset, (a.enabled ? a.desc[1] : a.desc[0]));

			pos.y += yline;
			pos.x = 0;
		}
		offset += pos;
	}
	/// 付加情報の表示
	void DrawHelp(GRRenderIf* render){
		render->SetLighting(false);
		render->SetDepthTest(false);
		render->EnterScreenCoordinate();
		Vec2f pos(xmargin, ymargin);

		// ヘルプについて
		if(showHelp)
			 render->DrawFont(pos, "hit \'h\' to hide help");
		else render->DrawFont(pos, "hit \'h\' to show help");
		pos.y += yline;
		// シーン情報
		ss.str("");
		ss << "# of scenes : " << GetSdk()->NScene() << " hit [0-9] to switch scene";
		render->DrawFont(pos, ss.str());
		pos.y += yline;
		
		if(showHelp){
			// いつでも表示系メニュー
			DrawMenu(render, MENU_ALWAYS, pos);
			pos.y += yline;

			// シーンメニュー
			DrawMenu(render, MENU_SCENE + curScene, pos);
			pos.y += yline;

			// 共有メニュー
			DrawMenu(render, curMenu, pos);
			pos.y += yline;

			render->DrawFont(pos, "hit TAB to switch menu");
			pos.y += yline;
		}

		// メッセージ
		render->DrawFont(pos, message);

		render->LeaveScreenCoordinate();
		render->SetLighting(true);
		render->SetDepthTest(true);
	}
	
	EPApp(){
		showHelp	= false;
		appName		= "untitled";
		numScenes	= 1;
		curScene	= 0;
		curMenu		= MENU_COMMON;

		ymargin		= 10;
		xmargin		= 20;
		yline		= 20;
		xkeys		= 0;
		xbrief		= 100;

		/// いつでも有効系
		AddMenu(MENU_ALWAYS, "");
		AddAction(MENU_ALWAYS, ID_EXIT, "exit");
		AddHotKey(MENU_ALWAYS, ID_EXIT, ESC, "ESC");
		AddHotKey(MENU_ALWAYS, ID_EXIT, 'q');
		AddHotKey(MENU_ALWAYS, ID_EXIT, 'Q');
		AddAction(MENU_ALWAYS, ID_RUN,
			"start", "simulation started.",
			"pause", "simulation paused.");
		AddHotKey(MENU_ALWAYS, ID_RUN, ' ', "space");
		AddAction(MENU_ALWAYS, ID_STEP, "step", "one step proceeded.");
		AddHotKey(MENU_ALWAYS, ID_STEP, ';');

		/// 共有コマンドはシーンコマンドとの衝突回避のために大文字を割り当てる
		/// ステートの保存や復帰
		AddMenu(MENU_STATE, "< save and load states >");
		AddAction(MENU_STATE, ID_LOAD_STATE, "load state", "state loaded.");
		AddHotKey(MENU_STATE, ID_LOAD_STATE, 'L');
		AddAction(MENU_STATE, ID_SAVE_STATE, "save state", "state saved.");
		AddHotKey(MENU_STATE, ID_SAVE_STATE, 'S');
		AddAction(MENU_STATE, ID_RELEASE_STATE, "release state", "saved state is released.");
		AddHotKey(MENU_STATE, ID_RELEASE_STATE, 'X');
		AddAction(MENU_STATE, ID_READ_STATE, "read state", "state read from state.bin.");
		AddHotKey(MENU_STATE, ID_READ_STATE, 'R');
		AddAction(MENU_STATE, ID_WRITE_STATE, "write state", "state written to state.bin.");
		AddHotKey(MENU_STATE, ID_WRITE_STATE, 'W');
		AddAction(MENU_STATE, ID_DUMP, "dump", "object data dumped to dump.bin.");
		AddHotKey(MENU_STATE, ID_DUMP, 'D');
		/// シミュレーション設定
		AddMenu(MENU_CONFIG, "< simulation settings >");
		AddAction(MENU_CONFIG, ID_SWITCH_LCP_PENALTY,
			"switch to penalty", "switched to penalty method",
			"switch to lcp", "switched to lcp method");
		AddHotKey(MENU_CONFIG, ID_SWITCH_LCP_PENALTY, 'M');
		AddAction(MENU_CONFIG, ID_TOGGLE_GRAVITY,
			"enable gravity", "gravity enabled.",
			"disable gravity", "gravity disabled.");
		AddHotKey(MENU_CONFIG, ID_TOGGLE_GRAVITY, 'G');
		AddAction(MENU_CONFIG, ID_TOGGLE_JOINT,
			"enable joints", "joints enabled.",
			"disable joints", "joints disabled.");
		AddHotKey(MENU_CONFIG, ID_TOGGLE_JOINT, 'J');
		AddAction(MENU_CONFIG, ID_TOGGLE_ABA,
			"enable ABA", "ABA enabled.",
			"disable ABA", "ABA disabled.");
		AddHotKey(MENU_CONFIG, ID_TOGGLE_ABA, 'A');
		AddAction(MENU_CONFIG, ID_INC_TIMESTEP, "increase time step");
		AddHotKey(MENU_CONFIG, ID_INC_TIMESTEP, 'I');
		AddAction(MENU_CONFIG, ID_DEC_TIMESTEP, "decrease time step");
		AddHotKey(MENU_CONFIG, ID_DEC_TIMESTEP, 'D');
		AddAction(MENU_CONFIG, ID_INC_TIMER, "increase timer interval");
		AddHotKey(MENU_CONFIG, ID_INC_TIMER, 'F');
		AddAction(MENU_CONFIG, ID_DEC_TIMER, "decrease timer interval");
		AddHotKey(MENU_CONFIG, ID_DEC_TIMER, 'S');
		/// 描画設定系
		AddMenu(MENU_DRAW, "< drawing setting >");
		AddAction(MENU_DRAW, ID_DRAW_WIREFRAME,
			"enable wireframe", "wireframe enabled.",
			"disable wireframe", "wireframe disabled");
		AddHotKey(MENU_DRAW, ID_DRAW_WIREFRAME, 'W');
		AddAction(MENU_DRAW, ID_DRAW_SOLID,
			"enable solid", "solid enabled.",
			"disable solid", "solid disabled");
		AddHotKey(MENU_DRAW, ID_DRAW_SOLID, 'S');
		AddAction(MENU_DRAW, ID_DRAW_AXIS,
			"enable axis drawing", "axis drawing enabled.",
			"disable axis drawing", "axis drawing disabled.");
		AddHotKey(MENU_DRAW, ID_DRAW_AXIS, 'A');
		AddAction(MENU_DRAW, ID_DRAW_FORCE,
			"enable force drawing", "force drawing enabled.",
			"disable force drawing", "force drawing disabled.");
		AddHotKey(MENU_DRAW, ID_DRAW_FORCE, 'F');
		AddAction(MENU_DRAW, ID_DRAW_CONTACT,
			"enable contact drawing", "contact drawing enabled.",
			"disable contact drawing", "contact drawing disabled.");
		AddHotKey(MENU_DRAW, ID_DRAW_CONTACT, 'C');
	}
	~EPApp(){}

public: /** 派生クラスが実装する関数 **/

	/// シーン構築を行う．
	virtual void BuildScene(){}

	/// 1ステップのシミュレーション
	virtual void OnStep(){
		fwScene->Step();
	}

	/// 描画
	virtual void OnDraw(GRRenderIf* render){
		fwScene->DrawPHScene(render);
	}

	/// アクション処理
	virtual void OnAction(int menu, int id){
		/// いつでも有効アクション
		if(menu == MENU_ALWAYS){
			if(id == ID_EXIT)
				exit(0);
			if(id == ID_RUN)
				ToggleAction(menu, id);
			if(id == ID_STEP)
				fwScene->Step();
		}
		if(menu == MENU_STATE){
			if(id == ID_LOAD_STATE)
				states->LoadState(phScene);
			if(id == ID_SAVE_STATE)
				states->SaveState(phScene);
			if(id == ID_RELEASE_STATE)
				states->ReleaseState(phScene);
			if(id == ID_READ_STATE)
				phScene->ReadState("state.bin");
			if(id == ID_WRITE_STATE)
				phScene->WriteState("state.bin");
			if(id == ID_DUMP){
				std::ofstream f("dump.bin", std::ios::binary|std::ios::out);
				phScene->DumpObjectR(f);
			}
		}
		if(menu == MENU_CONFIG){
			if(id == ID_SWITCH_LCP_PENALTY){
				if(ToggleAction(menu, id))
					 phScene->SetContactMode(PHSceneDesc::MODE_LCP);
				else phScene->SetContactMode(PHSceneDesc::MODE_PENALTY);
			}
			if(id == ID_TOGGLE_GRAVITY){
				if(ToggleAction(menu, id)){
					phScene->SetGravity(tmpGravity);
					//for(int i = 0; i < phScene->NSolids(); i++)
					//	phScene->GetSolids()[i]->SetGravity(true);
				}
				else{
					tmpGravity = phScene->GetGravity();
					phScene->SetGravity(Vec3d());
					//for(int i = 0; i < phScene->NSolids(); i++)
					//	phScene->GetSolids()[i]->SetGravity(false);
				}
			}
			if(id == ID_TOGGLE_JOINT){
				bool on = ToggleAction(menu, id);
				for(int i = 0; i < (int)phScene->NJoints(); i++)
					phScene->GetJoint(i)->Enable(on);
			}
			if(id == ID_TOGGLE_ABA){
				bool on = ToggleAction(menu, id);
				for(int i=0; i<phScene->NRootNodes(); ++i){
					PHRootNodeIf* rn = phScene->GetRootNode(i);
					rn->Enable(on);
				}
			}
			if(id == ID_INC_TIMESTEP){
				phScene->SetTimeStep(std::min(0.1, 2.0 * phScene->GetTimeStep()));
				ss.str("");
				ss << "time step is now " << phScene->GetTimeStep();
				message = ss.str();
			}
			if(id == ID_DEC_TIMESTEP){
				phScene->SetTimeStep(std::max(0.001, 0.5 * phScene->GetTimeStep()));
				ss.str("");
				ss << "time step is now " << phScene->GetTimeStep();
				message = ss.str();
			}
			if(id == ID_INC_TIMER){
				timer->SetInterval(std::min(1000, 2 * (int)timer->GetInterval()));
				ss.str("");
				ss << "timer interval is now " << timer->GetInterval();
				message = ss.str();
			}
			if(id == ID_DEC_TIMER){
				timer->SetInterval(std::max(10, (int)timer->GetInterval() / 2));
				ss.str("");
				ss << "timer interval is now " << timer->GetInterval();
				message = ss.str();
			}
		}
		if(menu == MENU_DRAW){
			if(id == ID_DRAW_SOLID)
				fwScene->SetRenderMode(ToggleAction(menu, id), menus[menu][ID_DRAW_WIREFRAME].enabled);
			if(id == ID_DRAW_WIREFRAME)
				fwScene->SetRenderMode(menus[menu][ID_DRAW_SOLID].enabled, ToggleAction(menu, id));
			if(id == ID_DRAW_AXIS){
				bool on = ToggleAction(menu, id);
				fwScene->EnableRenderAxis(on, on, on);
			}
			if(id == ID_DRAW_FORCE){
				bool on = ToggleAction(menu, id);
				fwScene->EnableRenderForce(on, on);
			}
			if(id == ID_DRAW_CONTACT){
				fwScene->EnableRenderContact(ToggleAction(menu, id));
			}
		}
		// この時点でメッセージが設定されていなければデフォルトメッセージ
		if(message == ""){
			Action& act = menus[menu][id];
			message = act.message[!act.enabled];
		}
	}

public: /** FWAppの実装 **/

	void Initialize(){
		Init(0, NULL);
	}

	virtual void Init(int argc, char* argv[]){
		CreateSdk();
		GRInit(argc, argv);
		
		FWWinDesc windowDesc;
		windowDesc.width = 1024;
		windowDesc.height = 768;
		windowDesc.title = appName;
		CreateWin(windowDesc);
		
		/// 光源設定
		GRRenderIf* render = GetCurrentWin()->GetRender();
		GRLightDesc ld;
		ld.diffuse  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.specular = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.ambient  = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		ld.position = Vec4f(25.0f, 50.0f, 20.0f, 1.0f);
		render->PushLight(ld);

		/// 床用の形状
		CDBoxDesc bd;
		bd.boxsize = Vec3d(60.0, 2.0, 40.0);
		shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		bd.boxsize.y *= 6.0;
		shapeWall = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();

		// 形状の作成
		bd.boxsize = Vec3f(2,2,2);
		shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		
		CDSphereDesc sd;
		sd.radius = 1;
		shapeSphere = GetSdk()->GetPHSdk()->CreateShape(sd)->Cast();
		
		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 1;
		shapeCapsule = GetSdk()->GetPHSdk()->CreateShape(cd)->Cast();
		
		CDRoundConeDesc rcd;
		rcd.length = 3;
		shapeRoundCone= GetSdk()->GetPHSdk()->CreateShape(rcd)->Cast();
		
		/// シーンの作成
		for(int i = 0; i < numScenes; i++){
			fwScene = GetSdk()->CreateScene();
			/// 描画設定
			//fwScene->SetSolidMaterial(GRRenderIf::WHITE);
			fwScene->SetWireMaterial(GRRenderIf::WHITE);
			fwScene->SetRenderMode(true, true);				///< ソリッド描画，ワイヤフレーム描画
			fwScene->EnableRenderAxis(true, true, true);		///< 座標軸
			fwScene->SetAxisStyle(FWSceneIf::AXIS_ARROWS);	///< 座標軸のスタイル
			fwScene->EnableRenderForce(false, true);			///< 力
			fwScene->EnableRenderContact(true);				///< 接触断面

			phScene = fwScene->GetPHScene();
			phScene->SetGravity(Vec3f(0.0f, -9.8f, 0.0f));	// 重力を設定
			phScene->SetTimeStep(0.05);
			phScene->SetNumIteration(15);
			//phScene->SetNumIteration(10, 1);	// correction iteration
			//phScene->SetNumIteration(10, 2);	// contact iteration
		
			phScene->SetStateMode(true);
			//scene->GetConstraintEngine()->SetUseContactSurface(true); //面接触での力計算を有効化

			// シーン構築
			curScene = i;
			BuildScene();
		}
		states = ObjectStatesIf::Create();
		SwitchScene(0);
		
		// タイマ
		timer = CreateTimer(UTTimerIf::FRAMEWORK);
		timer->SetInterval(10);
	}

	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void TimerFunc(int id) {
		/// 時刻のチェックと画面の更新を行う
		if (menus[MENU_ALWAYS][ID_RUN].enabled){
			OnStep();
		}
		// 再描画要求
		PostRedisplay();
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void Display() {
		FWWinIf* win = GetCurrentWin();
		GRRenderIf *render = win->GetRender();
		render->ClearBuffer();
		render->BeginScene();

		GRCameraDesc camera = render->GetCamera();
		camera.front = 0.3f;
		render->SetCamera(camera);
		render->SetViewMatrix(win->GetTrackball()->GetAffine().inv());

		OnDraw(render);
		DrawHelp(render);

		render->EndScene();
		render->SwapBuffers();
	}

	virtual void Keyboard(int key, int x, int y) {
		// 'h' : ヘルプの表示切り替え
		if(key == 'h' || key == 'H'){
			showHelp = !showHelp;
			return;
		}
		// TAB : メニュー切り替え
		if(showHelp && key == '\t'){
			if(++curMenu == MENU_COMMON_LAST)
				curMenu = MENU_COMMON;
		}

		// 数字キー　->　シーン切り替え
		if(key <= 0xff && isdigit(key)){
			SwitchScene(key - '0');
			return;
		}

		// キーに対応するアクションを実行
		int id;
		message = "";
		// 常時表示メニュー
		id = menus[MENU_ALWAYS].Query(key);
		if(id != -1)
			OnAction(MENU_ALWAYS, id);
		// シーンメニュー
		id = menus[MENU_SCENE + curScene].Query(key);
		if(id != -1)
			OnAction(MENU_SCENE + curScene, id);
		// 共有メニュー
		if(curMenu == MENU_EDITOR){
			editor.Key(key);
			editor.SetObject(phScene);
		}
		else{
			id = menus[curMenu].Query(key);
			if(id != -1)
				OnAction(curMenu, id);
		}
	}

};
#endif

}

#endif
