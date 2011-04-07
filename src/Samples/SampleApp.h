/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SAMPLE_APP_H
#define SAMPLE_APP_H

/**
	サンプルプログラム共用アプリケーションクラス
 **/

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>

using namespace std;
using namespace Spr;

#define ESC		27

class SampleApp : public FWApp{
public:
	/** メニューID
		MENU_ALWAYSはいつでも表示され，それ以外のメニューはIDの順で並ぶ．
		ユーザ定義メニュー先に来るように1～99のIDを使用，100以降を基本メニューが使用．
	 */
	enum MenuID{
		MENU_ALWAYS = 0,		///< いつでも有効系
		MENU_USER   = 1,		///< ユーザ使用領域
		MENU_CONFIG = 100,		///< パラメータ設定系
		MENU_STATE,				///< ステート保存系
		MENU_EDITOR,			///< FWEditorを表示
	};
	/// アクションID
	enum ActionAlways{
		ID_EXIT,					///< 終了
		ID_TOGGLE,					///< シミュレーションの開始と停止
		ID_STEP,					///< ステップ実行
	};
	enum ActionState{
		ID_LOAD_STATE,
		ID_SAVE_STATE,
		ID_RELEASE_STATE,
		ID_READ_STATE,
		ID_WRITE_STATE,
		ID_DUMP,
	};
	enum ActionConfig{
		ID_SWITCH_LCP_PENALTY,		///< LCPとペナルティ法を切り替える
		ID_ENABLE_JOINT,			///< 関節の有効化と無効化
		ID_INC_TIMESTEP,			///< タイムステップを増やす
		ID_DEC_TIMESTEP,			///< タイムステップを減らす
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
		vector< pair<int, UTString> > keys;		///< キーと代替テキスト
		UTString	brief;						///< 簡単な説明
		UTString	detail;						///< 詳しい説明
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
	int						curMenu;
	stringstream			ss;
	
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
	bool					running;		///< シミュレーション実行中フラグ
	bool					useLCP;			///< LCPかpenalty法か
	bool					showHelp;		///< ヘルプ表示
	int						curScene;		///< アクティブなシーンの番号
	
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
	void AddAction(int menu, int id, UTString brief, UTString detail = ""){
		Action& act = menus[menu][id];
		act.id		= id;
		act.brief   = brief;
		act.detail  = (detail == "" ? brief : detail);
	}

	/// アクションとキーの対応
	void AddHotKey(int menu, int id, int key, UTString alt = ""){
		menus[menu][id].keys.push_back(make_pair(key, alt));
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
		GetSdk()->SwitchScene(GetSdk()->GetScene(id));
		fwScene = GetSdk()->GetScene();
		phScene = fwScene->GetPHScene();
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
			render->DrawFont(pos + offset, a.brief);

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

			// 現在表示中のメニュー
			DrawMenu(render, curMenu, pos);
			pos.y += yline;

			render->DrawFont(pos, "hit TAB to switch menu");
		}
		render->LeaveScreenCoordinate();
		render->SetLighting(true);
		render->SetDepthTest(true);
	}
	
	SampleApp(){
		running		= true;
		useLCP		= true;
		showHelp	= false;
		appName		= "untitled";
		numScenes	= 1;
		curScene	= 0;
		curMenu		= MENU_USER;

		ymargin		= 10;
		xmargin		= 20;
		yline		= 20;
		xkeys		= 0;
		xbrief		= 100;

		/// いつでも有効系
		AddMenu(MENU_ALWAYS, "");
		AddAction(MENU_ALWAYS, ID_EXIT, "exit", "exit the application");
		AddHotKey(MENU_ALWAYS, ID_EXIT, ESC, "ESC");
		AddHotKey(MENU_ALWAYS, ID_EXIT, 'q');
		AddHotKey(MENU_ALWAYS, ID_EXIT, 'Q');
		AddAction(MENU_ALWAYS, ID_TOGGLE, "start", "start simulation");
		AddHotKey(MENU_ALWAYS, ID_TOGGLE, ' ', "space");
		AddAction(MENU_ALWAYS, ID_STEP, "step", "step simulation");
		AddHotKey(MENU_ALWAYS, ID_STEP, ';');
		/// ステートの保存や復帰
		AddMenu(MENU_STATE, "< save and load states >");
		AddAction(MENU_STATE, ID_LOAD_STATE, "load state", "load saved state");
		AddHotKey(MENU_STATE, ID_LOAD_STATE, 'l');
		AddAction(MENU_STATE, ID_SAVE_STATE, "save state", "save current state");
		AddHotKey(MENU_STATE, ID_SAVE_STATE, 's');
		AddAction(MENU_STATE, ID_RELEASE_STATE, "release state", "release saved state");
		AddHotKey(MENU_STATE, ID_RELEASE_STATE, 'x');
		AddAction(MENU_STATE, ID_READ_STATE, "read state", "read state from state.bin");
		AddHotKey(MENU_STATE, ID_READ_STATE, 'r');
		AddAction(MENU_STATE, ID_WRITE_STATE, "write state", "write state to state.bin");
		AddHotKey(MENU_STATE, ID_WRITE_STATE, 'w');
		AddAction(MENU_STATE, ID_DUMP, "dump", "dump object data to dump.bin");
		AddHotKey(MENU_STATE, ID_DUMP, 'd');
		/// シミュレーション設定
		AddMenu(MENU_CONFIG, "< simulation settings >");
		AddAction(MENU_CONFIG, ID_SWITCH_LCP_PENALTY, "switch to penalty", "switch to penalty method");
		AddHotKey(MENU_CONFIG, ID_SWITCH_LCP_PENALTY, 'm');
		AddAction(MENU_CONFIG, ID_ENABLE_JOINT, "disable joints");
		AddHotKey(MENU_CONFIG, ID_ENABLE_JOINT, 'j');
		AddAction(MENU_CONFIG, ID_INC_TIMESTEP, "increase time step");
		AddHotKey(MENU_CONFIG, ID_INC_TIMESTEP, 'i');
		AddAction(MENU_CONFIG, ID_DEC_TIMESTEP, "decrease time step");
		AddHotKey(MENU_CONFIG, ID_DEC_TIMESTEP, 'd');
		
	}
	~SampleApp(){}

public: /** 派生クラスが実装する関数 **/

	///
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
			if(id == ID_TOGGLE){
				running = !running;
				if(running)
					 AddAction(MENU_ALWAYS, ID_TOGGLE, "pause", "pause simulation");
				else AddAction(MENU_ALWAYS, ID_TOGGLE, "start", "start simulation");
			}
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
				useLCP = !useLCP;
				if(useLCP){
					phScene->SetContactMode(PHSceneDesc::MODE_LCP);
					AddAction(MENU_CONFIG, ID_SWITCH_LCP_PENALTY, "switch to penalty", "switch to penalty method");
				}
				else{
					phScene->SetContactMode(PHSceneDesc::MODE_PENALTY);
					AddAction(MENU_CONFIG, ID_SWITCH_LCP_PENALTY, "switch to lcp", "switch to lcp method");
				}
			}
			if(id == ID_ENABLE_JOINT){
				static bool bEnable = true;
				bEnable = !bEnable;
				for(int i = 0; i < (int)phScene->NJoints(); i++)
					phScene->GetJoint(i)->Enable(bEnable);
			}
			if(id == ID_INC_TIMESTEP)
				phScene->SetTimeStep(2.0 * phScene->GetTimeStep());
			if(id == ID_DEC_TIMESTEP)
				phScene->SetTimeStep(std::max(0.001, 0.5 * phScene->GetTimeStep()));
		}
	}

public: /** FWAppの実装 **/

	virtual void Init(int argc, char* argv[]){
		SetGRAdaptee(TypeGLUT);
		GRInit(argc, argv);
		CreateSdk();

		FWWinDesc windowDesc;
		windowDesc.title = appName;
		CreateWin(windowDesc);
		InitWindow();

		/// 光源設定
		GRRenderIf* render = GetSdk()->GetRender();
		GRLightDesc ld;
		ld.diffuse  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.specular = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.ambient  = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		ld.position = Vec4f(25.0f, 50.0f, 20.0f, 1.0f);
		render->PushLight(ld);

		/// 床用の形状
		CDBoxDesc bd;
		bd.boxsize = Vec3d(60.0, 2.0, 40.0);
		shapeFloor = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));		
		bd.boxsize.y *= 6.0;
		shapeWall = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));

		// 形状の作成
		bd.boxsize = Vec3f(2,2,2);
		shapeBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
		
		CDSphereDesc sd;
		sd.radius = 1;
		shapeSphere = XCAST(GetSdk()->GetPHSdk()->CreateShape(sd));
		
		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 1;
		shapeCapsule = XCAST(GetSdk()->GetPHSdk()->CreateShape(cd));
		
		CDRoundConeDesc rcd;
		rcd.length = 3;
		shapeRoundCone= XCAST(GetSdk()->GetPHSdk()->CreateShape(rcd));
		
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
		if (running){
			OnStep();
		}
		// 再描画要求
		PostRedisplay();
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void Display() {
		GRRenderIf *render = GetSdk()->GetRender();
		render->ClearBuffer();
		render->BeginScene();

		GRCameraDesc camera = render->GetCamera();
		camera.front = 0.3;
		render->SetCamera(camera);
		render->SetViewMatrix(cameraInfo.view.inv());

		OnDraw(render);
		DrawHelp(render);

		render->EndScene();
		glutSwapBuffers();
	}

	virtual void Keyboard(int key, int x, int y) {
		// 'h' : ヘルプの表示切り替え
		if(key == 'h' || key == 'H'){
			showHelp = !showHelp;
			return;
		}
		// TAB : メニュー切り替え
		if(showHelp && key == '\t'){
			Menus::iterator it = menus.find(curMenu);
			if(++it == menus.end())
				 curMenu = MENU_USER;
			else curMenu = it->first;
		}

		// 数字キー　->　シーン切り替え
		if(key <= 0xff && isdigit(key)){
			SwitchScene(key - '0');
			return;
		}

		// キーに対応するアクションを実行
		int id = menus[MENU_ALWAYS].Query(key);
		if(id != -1)
			OnAction(MENU_ALWAYS, id);

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
