/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
\page pageFEMThermo FEMによる温度シミュレーションのサンプル
Springhead2/src/Samples/FEMThermo

\section secQuitBoxStack 終了基準
- Escか'q'で強制終了。
	
*/
//
#include "../../SampleApp.h"

#pragma hdrstop

using namespace PTM;

using namespace Spr;
using namespace std;



class MyApp : public SampleApp{
public:
	/// ページID
	enum {
		MENU_MAIN = MENU_SCENE,
	};
	/// アクションID
	enum {
		ID_BOX,
		ID_SWITCHRENDER,
	};
	bool debugRender;

public:
	MyApp():debugRender(false){
		appName = "FEMThermo";

		AddAction(MENU_MAIN, ID_BOX, "drop box");
		AddHotKey(MENU_MAIN, ID_BOX, 'b');
		AddAction(MENU_MAIN, ID_SWITCHRENDER, "switch physics(debug)/graphics rendering");
		AddHotKey(MENU_MAIN, ID_SWITCHRENDER, 'd');
	}
	~MyApp(){}

	virtual void Init(int argc, char* argv[]){
		CreateSdk();
		GetSdk()->CreateScene();
		SetGRAdaptee(TypeGLUT);
		GRInit(argc, argv);
		//CreateSdk();

		FWWinDesc windowDesc;
		windowDesc.width = 1024;
		windowDesc.height = 768;
		windowDesc.title = appName;
		CreateWin(windowDesc);
		InitWindow();


		// 形状の作成
		CDBoxDesc bd;
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
		
		//	ファイルのロード
		UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
		GetSdk()->LoadScene("scene.spr", import);			// ファイルのロード
		numScenes = GetSdk()->NScene();
		if (numScenes) SwitchScene(GetSdk()->NScene()-1);

		/// 描画設定
		if (fwScene){
			fwScene->SetWireMaterial(GRRenderIf::WHITE);
			fwScene->SetRenderMode(true, true);				///< ソリッド描画，ワイヤフレーム描画
			fwScene->EnableRenderAxis(false, true, true);		///< 座標軸
			fwScene->SetAxisStyle(FWSceneIf::AXIS_LINES);	///< 座標軸のスタイル
			fwScene->EnableRenderForce(false, true);			///< 力
			fwScene->EnableRenderContact(true);				///< 接触断面
			phScene = fwScene->GetPHScene();
		}

		//	セーブ用のステートの用意
		states = ObjectStatesIf::Create();		
		// タイマ
		timer = CreateTimer(UTTimerIf::FRAMEWORK);
		timer->SetInterval(10);
	}

	// 描画関数．描画要求が来たときに呼ばれる
	virtual void OnDraw(GRRenderIf* render) {
		if (debugRender){
			fwScene->DrawPHScene(render);
		}else{
			fwScene->Sync();
			fwScene->GetGRScene()->Render(render);
		}

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
			}else if(id==ID_SWITCHRENDER){
				debugRender = !debugRender; 
			}
		}
		SampleApp::OnAction(menu, id);
	}
	//ステップ実行
	virtual void OnStep() {
		SampleApp::OnStep();
		PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
		//if(phscene)		cout << phscene << endl;
		SceneObjectIf* phSceneObject;
		PHFemMeshIf* phFemMesh;
		size_t Nobject = GetSdk()->GetPHSdk()->GetObjectIf()->NChildObject();
		//なんかしらアドレスが取れているのは分かった
		//for(int i=0; i< Nobject ;i++){
		//	ObjectIf* object = GetSdk()->GetPHSdk()->GetChildObject(i);
		//	cout << "Object" << i << " : " << object << endl;
		//}
		//NamedObjectIf* nobject = GetSdk()->FindObject("negi");
		//phFemMeshを作って、phFemMeshの処理をを実行させてみたい。
		//
		//熱シミュレーションステップ
		HeatConductionStep();
	}
	void HeatConductionStep(){
		//PHFemMeshThermoオブジェクトを作り、main関数で使えるようにする関数
			CreatePHFemMeshThermo();
		//総節点数、総要素数、節点座標、要素の構成などを登録
			//PHFemMeshThermoのオブジェクトを作る際に、ディスクリプタに値を設定して作る
			
		//節点の初期温度を設定する⇒{T}縦ベクトルに代入
			//{T}縦ベクトルを作成する。以降のK,C,F行列・ベクトルの節点番号は、この縦ベクトルの節点の並び順に合わせる。
			CreateMeshTempVec();
		//熱伝導率、熱伝達率、密度、比熱などのパラメーターを設定・代入
			//PHFemMEshThermoのメンバ変数の値を代入
			SetThermoConductionParam();
			//これら、変数値は後から計算の途中で変更できるようなSetParam()関数を作っておいたほうがいいかな？

		//計算に用いるマトリクス、ベクトルを作成（メッシュごとの要素剛性行列/ベクトル⇒全体剛性行列/ベクトル）
			//{T}縦ベクトルの節点の並び順に並ぶように、係数行列を加算する。係数行列には、面積や体積、熱伝達率などのパラメータの積をしてしまったものを入れる。
		//CreateK1();
		//CreateK2();
		//CreateC();
		//CreateF();
		//PrepareGaussSeidel();
			//ガウスザイデルで計算するために、クランクニコルソンの差分式の形で行列を作る。行列DやF、-bなどを作り、ガウスザイデルで計算ステップを実行直前まで
		//ガウスザイデルの計算を単位ステップ時間ごとに行う
			//ガウスザイデルの計算
			//CalcGaussSeidel();
		//（形状が変わったら、マトリクスやベクトルを作り直す）
		//温度変化・最新の時間の{T}縦ベクトルに記載されている節点温度を基に化学変化シミュレーションを行う
			//SetChemicalSimulation();
			//化学変化シミュレーションに必要な温度などのパラメータを渡す
		//温度変化や化学シミュレーションの結果はグラフィクス表示を行う
			
		//以下、削除予定
		CreatePHFemMeshThermo();
		//[K]行列を作る
		CreateKele();
		//[C]行列を作る
		//{F}ベクトルを作る
		//面積を求める
		//体積を求める
		
		//行列計算のテストを行う関数
		TestMatrixFunc();
	}
	void CreateMeshTempVec(){
		//TetGen()で作成したデータから、節点の並び順を取得して、温度が入った縦ベクトルを作る
		//ベクトルの数を代入する
		//int n = 
		
		//TVector<n,double> vec; 
		//行列の各
	}
	void SetThermoConductionParam(){

	}
	void CreatePHFemMeshThermo(){
		PHFemMeshThermoIf* phmesht;
		PHFemMeshThermoDesc desc;
		//FWFemMesh::CreatePHFromGR()でTetgenに代入して PHFemMeshThermoDesc pmd; を作っている。
		////四面体の数だけ代入する
		////int Ntets = //Tetgenで得た？要素の数
		//for(int i=0 ; i < Ntets ; i++){
		//	desc.tets[i] = ;
		//}
		////節点の数だけ代入する
		//int NVertices = //Tetgenで得た？節点の数
		//for(int i=0 ; i < Nvertics ; i++){
		//	desc.vertices[i] = ;
		//}



	}
	void CreateKele(){

	}

	void TestMatrixFunc(){

		////ディスクリプタの実体を作って、オブジェクトを作る時に、ディスクリプタを入れる。SPIDARのディスクリプタ使う時と同じ感じ。
		//PHFemMeshThermoDesc desc;
		//desc.tets = ;
		//desc.vertice = ;
		////シーンを取ってくる phscene sceneobject に属しているオブジェクト シーンオブジェクトは、作る時に、どのシーンに属しているのかを伝える必要がある。
		//PHFemMeshThermo phftmesh(desc,s);//引数を入れる

		PTM::VMatrixCol<double> Tvec;
		TMatrixRow<4,4,double> mat;              // 2行2列の行が詰った行列を宣言
												   // TMatrixColだと列が詰った行列になる
		//四面体vectorなどから再帰命令などで入力
		mat[0][0] = 1;  mat[0][1] = 1;	mat[0][2] = 1;	mat[0][3] = 1;          // 0行0列 = 1;  0行1列 = 2;
		mat[1][0] = 3;  mat[1][1] = 4;	mat[1][2] = 2;	mat[1][3] = 2;          // 1行0列 = 3;  1行1列 = 4;
		mat[2][0] = 2;	mat[2][1] = 2;	mat[2][2] = 2;	mat[2][3] = 2;
		mat[3][0] = 2;	mat[3][1] = 2;	mat[3][2] = 2;	mat[3][3] = 2;

		TVector<4,float> vec;                   // 4次元のベクトルを宣言
		vec[0] = 1; vec[1] = 0; vec[2] = 1; vec[3] = 0; 
		std::cout << mat;
		std::cout << vec << std::endl;
		std::cout << mat * vec << std::endl;    // 掛け算
		std::cout << mat + mat << std::endl;    // 足し算
		std::cout << mat - mat << std::endl;    // 引き算
		std::cout << mat.trans() << std::endl;  // 転置
		std::cout << mat.inv() << std::endl;    // 逆行列
		//return 0;

		//double elemK2[4][4];
		//double elemK3[4][4];


	}



};


////////////////////////////////////////////////////////////////////////////////////////////////////////////

//FWMyApptest app;
MyApp app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char *argv[]) {
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}

