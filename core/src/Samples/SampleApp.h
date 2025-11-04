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
#include <map>

//#include <Collision/CDConvex.h>

using namespace std;
using namespace Spr;

class SampleApp : public FWApp{
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
		ID_TEST_STATE
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
		ID_DRAW_GRAPHICS,			///< グラフィクスシーンの表示
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
		SHAPE_ELLIPSOID,
		SHAPE_ROCK,
		SHAPE_BLOCK,
		SHAPE_COIN,
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
	UTRef<ObjectStatesIf>	states;			///< 状態保存用
	UTTimerIf*				timer;			///< タイマ

	/// 床用の形状
	CDBoxIf*				shapeFloor;
	CDBoxIf*				shapeWallX;
	CDBoxIf*				shapeWallZ;
	/// 作りおき形状
	CDBoxIf*				shapeBox;
	CDSphereIf*				shapeSphere;
	CDEllipsoidIf*			shapeEllipsoid;
	CDCapsuleIf*			shapeCapsule;
	CDRoundConeIf*			shapeRoundCone;
	CDConvexMeshIf*			shapeCoin;

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
	
	/// シェーダ
	GRShaderIf*	shaderBase;
	GRShaderIf* shaderShadowCreate;
	GRShaderIf* shaderShadowRender;

public:
	///	アクティブなシーン
	FWSceneIf*				GetFWScene(){ return GetCurrentWin() ? GetCurrentWin()->GetScene() : NULL; }
	PHSceneIf*				GetPHScene(){ return GetCurrentWin() ? GetCurrentWin()->GetScene() ? GetCurrentWin()->GetScene()->GetPHScene() : NULL: NULL; }
	/// メニューの登録
	void AddMenu(int menu, UTString brief){
		menus[menu].brief = brief;
	}
	/// アクションの登録
	Action& AddAction(int menu, int id, UTString desc, UTString msg = "", UTString descd = "", UTString msgd = ""){
		Action& act = menus[menu][id];
		act.id		= id;
		act.enabled = true;
		act.desc[0] = desc;
		act.desc[1] = (descd == "" ? desc : descd);
		act.message[0] = (msg == "") ? desc: msg;
		act.message[1] = (msgd == "" ? msg : msgd);
		return act;
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
	PHSolidIf* CreateFloor(bool bWall){
		PHSolidIf* soFloor = GetPHScene()->CreateSolid();
		soFloor->SetName("soFloor");
		soFloor->SetDynamical(false);
	
		soFloor->AddShape(shapeFloor);
		soFloor->SetShapePose(0, Posed::Trn(0, -shapeFloor->GetBoxSize().y/2, 0));
		if (bWall) {
			soFloor->AddShape(shapeWallZ);
			soFloor->AddShape(shapeWallX);
			soFloor->AddShape(shapeWallZ);
			soFloor->AddShape(shapeWallX);
			double y = shapeWallZ->GetBoxSize().y / 2 - shapeFloor->GetBoxSize().y;
			soFloor->SetShapePose(1, Posed::Trn(-(shapeFloor->GetBoxSize().x + shapeWallZ->GetBoxSize().x) / 2, y, 0));
			soFloor->SetShapePose(2, Posed::Trn(0, y, -(shapeFloor->GetBoxSize().z + shapeWallX->GetBoxSize().z) / 2));
			soFloor->SetShapePose(3, Posed::Trn((shapeFloor->GetBoxSize().x + shapeWallZ->GetBoxSize().x) / 2, y, 0));
			soFloor->SetShapePose(4, Posed::Trn(0, y, (shapeFloor->GetBoxSize().z + shapeWallX->GetBoxSize().z) / 2));
		}
		GetFWScene()->SetSolidMaterial(GRRenderIf::GRAY, soFloor);
		soFloor->CompInertia();

		return soFloor;
	}

	void SwitchScene(int id){
		if(id < 0 || numScenes <= id)
			return;
		curScene = id;
		// id番目のシーンを選択
		GetCurrentWin()->SetScene(GetSdk()->GetScene(id));
		editor.SetObject(GetPHScene());
		//cameraInfo.Fit(GetSdk()->GetRender()->GetCamera(), activeHandler->GetSceneRadius());
	}
	
	/** @brief	色々な形の物体を作成
		@param	shape	形
	 */
	void Drop(int shape, int mat, Vec3d v, Vec3d w, Vec3d p, Quaterniond q){
		// ステートを解放
		states->ReleaseState(GetPHScene());
	
		// 剛体を作成
		PHSolidIf* solid = GetPHScene()->CreateSolid();
		// マテリアルを設定
		GetFWScene()->SetSolidMaterial(mat, solid);
			
		// 形状の割当て
		if(shape == SHAPE_BOX)
			solid->AddShape(shapeBox);
		if (shape == SHAPE_CAPSULE)
			solid->AddShape(shapeCapsule);
		if(shape == SHAPE_ROUNDCONE)
			solid->AddShape(shapeRoundCone);
		if (shape == SHAPE_SPHERE)
			solid->AddShape(shapeSphere);
		if (shape == SHAPE_ELLIPSOID)
			solid->AddShape(shapeEllipsoid);
		if(shape == SHAPE_ROCK){
			CDConvexMeshDesc md;
			int nv = rand()%100 + 50;
			for(int i=0; i < nv; ++i){
				Vec3d v;
				for(int c=0; c<3; ++c){
					v[c] = ((rand() % 100) / 100.0 - 0.5) * 5 * 1.3 * ShapeScale();
				}
				md.vertices.push_back(v);
			}
			solid->AddShape(GetSdk()->GetPHSdk()->CreateShape(md));
		}
		if(shape == SHAPE_BLOCK){
			for(int i = 0; i < 7; i++)
				solid->AddShape(shapeBox);
			Posed pose;
			pose.Pos() = ShapeScale() * Vec3d( 3,  0,  0); solid->SetShapePose(1, pose);
			pose.Pos() = ShapeScale() * Vec3d(-3,  0,  0); solid->SetShapePose(2, pose);
			pose.Pos() = ShapeScale() * Vec3d( 0,  3,  0); solid->SetShapePose(3, pose);
			pose.Pos() = ShapeScale() * Vec3d( 0, -3,  0); solid->SetShapePose(4, pose);
			pose.Pos() = ShapeScale() * Vec3d( 0,  0,  3); solid->SetShapePose(5, pose);
			pose.Pos() = ShapeScale() * Vec3d( 0,  0, -3); solid->SetShapePose(6, pose);
		}
		if (shape == SHAPE_COIN) {
			solid->AddShape(shapeCoin);
		}
		solid->SetVelocity(v);
		solid->SetAngularVelocity(w);
		solid->SetFramePosition(p);
		solid->SetOrientation(q);
		solid->CompInertia();
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

	virtual double ShapeScale() {
		return 1.0;
	}
	
	SampleApp(){
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
		AddHotKey(MENU_ALWAYS, ID_EXIT, DVKeyCode::ESC, "ESC");
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
		AddAction(MENU_STATE, ID_TEST_STATE, "test", "test state.");
		AddHotKey(MENU_STATE, ID_TEST_STATE, 'T');
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
		AddAction(MENU_DRAW, ID_DRAW_GRAPHICS,
			"enable graphics scene", "graphics scene enabled.",
			"disable graphics scene", "graphics scene disabled.");
		AddHotKey(MENU_DRAW, ID_DRAW_GRAPHICS, 'G');
		AddAction(MENU_DRAW, ID_DRAW_WIREFRAME,
			"enable wireframe", "wireframe enabled.",
			"disable wireframe", "wireframe disabled.");
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
	~SampleApp(){}

public: /** 派生クラスが実装する関数 **/

	/// シーン構築を行う．
	virtual void BuildScene(){}

	/// 1ステップのシミュレーション
	virtual void OnStep(){
		FWSceneIf* fwScene = GetFWScene();
		if (fwScene) fwScene->Step();
	}

	/// 描画
	virtual void OnDraw(GRRenderIf* render){
		GetFWScene()->Draw(render);
	}

	/// アクション処理
	virtual void OnAction(int menu, int id){
		/// いつでも有効アクション
		if(menu == MENU_ALWAYS){
			if (id == ID_EXIT) {
				Cleanup();
				EndMainLoop();
			}
			if (id == ID_RUN) {
				ToggleAction(menu, id);
			}
			if (id == ID_STEP) {
				OnStep();
				PostRedisplay();
			}
		}
		if(menu == MENU_STATE){
			if (id == ID_LOAD_STATE)
				states->LoadState(GetPHScene());
			if(id == ID_SAVE_STATE)
				states->SaveState(GetPHScene());
			if(id == ID_RELEASE_STATE)
				states->ReleaseState(GetPHScene());
			if(id == ID_READ_STATE){
				GetFWScene()->ReadState("state.bin");
			}
			if(id == ID_WRITE_STATE){
				GetFWScene()->WriteState("state.bin");
			}
			if(id == ID_DUMP){
				std::ofstream f("dump.bin", std::ios::binary|std::ios::out);
				GetPHScene()->DumpObjectR(f);
			}
			if (id == ID_TEST_STATE) {
				std::ofstream fBefore("before.bin", std::ios::binary | std::ios::out);
				std::ofstream fAfter("after.bin", std::ios::binary | std::ios::out);
				std::ofstream fState("state.bin", std::ios::binary | std::ios::out);
				//ObjectIfs objs;
				//objs.Push(GetPHScene());
				//GetSdk()->SaveObjects("before.spr", &objs);
				states->SaveState(GetPHScene());
				char* buf = states->GetStateBuffer();
				if (buf) {
					fState.write(buf, states->GetStateBufferLen());
				}
				GetPHScene()->Step();
//				GetPHScene()->GetConstraintEngine()->UpdateForStateDebug();
				GetPHScene()->DumpObjectR(fBefore);	// Binarly dump of scene before the load state.
				GetPHScene()->Step();
				GetPHScene()->Step();
				states->LoadState(GetPHScene());
				GetPHScene()->Step();
//				GetPHScene()->GetConstraintEngine()->UpdateForStateDebug();
				GetPHScene()->DumpObjectR(fAfter);	// Binarly dump of scene after the load state.
				//GetSdk()->SaveObjects("after.spr", &objs);		// ファイルのセーブテスト
			}
		}
		if(menu == MENU_CONFIG){
			if(id == ID_SWITCH_LCP_PENALTY){
				if(ToggleAction(menu, id))
					 GetPHScene()->SetContactMode(PHSceneDesc::MODE_LCP);
				else GetPHScene()->SetContactMode(PHSceneDesc::MODE_PENALTY);
			}
			if(id == ID_TOGGLE_GRAVITY){
				if(ToggleAction(menu, id)){
					GetPHScene()->SetGravity(tmpGravity);
					//for(int i = 0; i < GetPHScene()->NSolids(); i++)
					//	GetPHScene()->GetSolids()[i]->SetGravity(true);
				}
				else{
					tmpGravity = GetPHScene()->GetGravity();
					GetPHScene()->SetGravity(Vec3d());
					//for(int i = 0; i < GetPHScene()->NSolids(); i++)
					//	GetPHScene()->GetSolids()[i]->SetGravity(false);
				}
			}
			if(id == ID_TOGGLE_JOINT){
				bool on = ToggleAction(menu, id);
				for(int i = 0; i < (int)GetPHScene()->NJoints(); i++)
					GetPHScene()->GetJoint(i)->Enable(on);
			}
			if(id == ID_TOGGLE_ABA){
				bool on = ToggleAction(menu, id);
				for(int i=0; i<GetPHScene()->NRootNodes(); ++i){
					PHRootNodeIf* rn = GetPHScene()->GetRootNode(i);
					rn->Enable(on);
				}
			}
			if(id == ID_INC_TIMESTEP){
				GetPHScene()->SetTimeStep(std::min(0.1, 2.0 * GetPHScene()->GetTimeStep()));
				ss.str("");
				ss << "time step is now " << GetPHScene()->GetTimeStep();
				message = ss.str();
			}
			if(id == ID_DEC_TIMESTEP){
				GetPHScene()->SetTimeStep(std::max(0.001, 0.5 * GetPHScene()->GetTimeStep()));
				ss.str("");
				ss << "time step is now " << GetPHScene()->GetTimeStep();
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
			if(id == ID_DRAW_GRAPHICS){
				
			}
			if(id == ID_DRAW_SOLID)
				GetFWScene()->SetRenderMode(ToggleAction(menu, id), menus[menu][ID_DRAW_WIREFRAME].enabled);
			if(id == ID_DRAW_WIREFRAME)
				GetFWScene()->SetRenderMode(menus[menu][ID_DRAW_SOLID].enabled, ToggleAction(menu, id));
			if(id == ID_DRAW_AXIS){
				bool on = ToggleAction(menu, id);
				GetFWScene()->EnableRenderAxis(on, on, on);
			}
			if(id == ID_DRAW_FORCE){
				bool on = ToggleAction(menu, id);
				GetFWScene()->EnableRenderForce(on, on);
			}
			if(id == ID_DRAW_CONTACT){
				GetFWScene()->EnableRenderContact(ToggleAction(menu, id));
			}
		}
		// この時点でメッセージが設定されていなければデフォルトメッセージ
		if(message == ""){
			Action& act = menus[menu][id];
			message = act.message[!act.enabled];
		}
	}

public: /** FWAppの実装 **/

	virtual void Init(int argc, char* argv[]){
		CreateSdk();
		GRInit(argc, argv);
		
		FWWinDesc windowDesc;
		windowDesc.width = 1024;
		windowDesc.height = 768;
		// windowDesc.width = 1280;
		// windowDesc.height = 720;
		windowDesc.title = appName;
		FWWinIf* win = CreateWin(windowDesc);		
		win->GetTrackball()->SetPosition(Vec3f(30,30,50));
		win->GetTrackball()->SetTarget(Vec3f(0,0,0));

		/// シェーダ
		string shaderDir = "../../../../shader/";
		GRRenderIf* render = GetCurrentWin()->GetRender();

	/***
		GRShaderDesc shd;
		shd.vsname = shaderDir + "vsbase.txt";
		shd.fsname = shaderDir + "fsbase.txt";
		shd.bEnableLighting  = true;
		shd.bEnableTexture2D = true;
		shd.bEnableTexture3D = false;
		shd.bShadowCreate    = false;
		shd.bShadowRender    = false;
		shaderBase = render->CreateShader(shd);

		shd.bEnableLighting  = false;
		shd.bEnableTexture2D = false;
		shd.bEnableTexture3D = false;
		shd.bShadowCreate    = true;
		shd.bShadowRender    = false;
		shaderShadowCreate = render->CreateShader(shd);

		shd.bEnableLighting  = true;
		shd.bEnableTexture2D = true;
		shd.bEnableTexture3D = false;
		shd.bShadowCreate    = false;
		shd.bShadowRender    = true;
		shaderShadowRender = render->CreateShader(shd);
	***/
		
		/// 光源設定
		GRLightDesc ld;
		ld.diffuse  = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.specular = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		ld.ambient  = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		ld.position = Vec4f(25.0f, 50.0f, 20.0f, 1.0f);
		render->PushLight(ld);

		/// 床用の形状
		CDBoxDesc bd;
		bd.boxsize = ShapeScale() * Vec3d(60, 2, 40);
		shapeFloor = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		bd.boxsize.y = ShapeScale() * 6;
		CDBoxDesc wd = bd;
		wd.boxsize.x = ShapeScale() * 2;
		wd.boxsize.z += ShapeScale() * 4;
		shapeWallZ = GetSdk()->GetPHSdk()->CreateShape(wd)->Cast();
		wd = bd;
		wd.boxsize.z = ShapeScale() * 2;
		shapeWallX = GetSdk()->GetPHSdk()->CreateShape(wd)->Cast();

		// 形状の作成
		bd.boxsize = ShapeScale() * Vec3f(2, 2, 2);
		shapeBox = GetSdk()->GetPHSdk()->CreateShape(bd)->Cast();
		
		CDSphereDesc sd;
		sd.radius = ShapeScale() * 1;
		shapeSphere = GetSdk()->GetPHSdk()->CreateShape(sd)->Cast();

		CDEllipsoidDesc ed;
		ed.radius *= ShapeScale();
		shapeEllipsoid = GetSdk()->GetPHSdk()->CreateShape(ed)->Cast();

		CDCapsuleDesc cd;
		cd.radius = ShapeScale() * 1;
		cd.length = ShapeScale() * 4;
		shapeCapsule = GetSdk()->GetPHSdk()->CreateShape(cd)->Cast();
		
		CDRoundConeDesc rcd;
		rcd.length = ShapeScale() * 6;
		rcd.radius = ShapeScale() * Vec2d(1, 2);
		shapeRoundCone= GetSdk()->GetPHSdk()->CreateShape(rcd)->Cast();

		CDConvexMeshDesc coind;
		const int nDiv = 12;
		for (int i = 0; i < nDiv; ++i) {
			const double r = ShapeScale() * 0.8;
			const double h = ShapeScale() * 0.2;
			coind.vertices.push_back(Quaterniond::Rot(Rad(360.0/nDiv * i), 'y')* Vec3d(r, -h / 2, 0));
			coind.vertices.push_back(Quaterniond::Rot(Rad(360.0/nDiv * i), 'y')*Vec3d(r, +h / 2, 0));
		}
		shapeCoin = GetSdk()->GetPHSdk()->CreateShape(coind)->Cast();
		
		/// シーンの作成
		for(int i = 0; i < numScenes; i++){
			GetCurrentWin()->SetScene(GetSdk()->CreateScene());
			/// 描画設定
			//GetFWScene()->SetSolidMaterial(GRRenderIf::WHITE);
			GetFWScene()->SetWireMaterial(GRRenderIf::WHITE);
			GetFWScene()->SetRenderMode(true, true);				///< ソリッド描画，ワイヤフレーム描画
			GetFWScene()->EnableRenderAxis(true, true, true);		///< 座標軸
			GetFWScene()->SetAxisStyle(FWSceneIf::AXIS_ARROWS);	///< 座標軸のスタイル
			GetFWScene()->EnableRenderForce(false, true);			///< 力
			GetFWScene()->EnableRenderContact(true);				///< 接触断面

			GetPHScene()->SetGravity(Vec3f(0.0f, -9.8f, 0.0f));	// 重力を設定
			GetPHScene()->SetTimeStep(0.05);
			GetPHScene()->SetNumIteration(15);
			//GetPHScene()->SetNumIteration(10, 1);	// correction iteration
			//GetPHScene()->SetNumIteration(10, 2);	// contact iteration
		
			//scene->GetConstraintEngine()->SetUseContactSurface(true); //面接触での力計算を有効化
			
			// シーン構築
			curScene = i;
			BuildScene();
		}
		states = ObjectStatesIf::Create();
		SwitchScene(0);
		
		// タイマ
		timer = CreateTimer(UTTimerIf::FRAMEWORK);
		timer->SetInterval(25);
		EnableIdleFunc(false);
	}
	virtual void Cleanup() {
		states->ReleaseState(GetPHScene());
	}

	// タイマコールバック関数．タイマ周期で呼ばれる
	virtual void TimerFunc(int id) {
		/// 時刻のチェックと画面の更新を行う
		if (menus[MENU_ALWAYS][ID_RUN].enabled){
			OnStep();
			// 再描画要求
			PostRedisplay();
		}
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void Display() {
		FWWinIf* win = GetCurrentWin();
		GRRenderIf *render = win->GetRender();
		render->BeginScene();
		
		/* OpenGLのバージョンチェックが正しく機能していないため，GL3.0未対応の処理系で不具合
		   応急処置としてシャドウ描画を無効化

		// 直上から照らす点光源で影を生成
		GRShadowLightDesc sld;
		sld.directional = false;
		//sld.directional = true;
		sld.position    = Vec3f(0.0f, 100.0f, 0.0f);
		sld.lookat      = Vec3f(0.0f, 0.0f, 0.0f);
		sld.up          = Vec3f(1.0f, 0.0f, 0.0f);
		sld.fov         = (float)Rad(45.0);
		sld.size        = Vec2f(30.0f, 30.0f);
		sld.front       =  50.0f;
		sld.back        = 110.0f;
		sld.color       = Vec4f(0.0f, 0.0f, 1.0f, 0.3f);
		render->SetShadowLight(sld);
		
		// シャドウマップ生成パス
		if (render->SetShader(shaderShadowCreate)){
			render->EnterShadowMapGeneration();
			render->ClearBuffer(false, true);
			OnDraw(render);
			// メインパス用レンダラに戻す
			render->SetShader(shaderShadowRender);
			render->LeaveShadowMapGeneration();
		}
		*/
		// メインパス
		render->ClearBuffer(true, true);
		GRCameraDesc camera = render->GetCamera();
		camera.front = 0.3f;
		render->SetCamera(camera);
		render->SetViewMatrix(win->GetTrackball()->GetAffine().inv());
		OnDraw(render);		
		render->SetMaterial(GRRenderBaseIf::WHITE);
		DrawHelp(render);

		render->EndScene();
		render->SwapBuffers();
	}

	virtual void Keyboard(int key, int x, int y) {
		// 特殊キーは無視
		if(key > 0xff)
			return;

		// 'h' : ヘルプの表示切り替え
		if(key == 'h' || key == 'H'){
			showHelp = !showHelp;
			return;
		}
		// TAB : メニュー切り替え
		if(key == '\t'){
			showHelp = true;
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
			editor.SetObject(GetPHScene());
		}
		else{
			id = menus[curMenu].Query(key);
			if(id != -1)
				OnAction(curMenu, id);
		}
	}

};

#endif
