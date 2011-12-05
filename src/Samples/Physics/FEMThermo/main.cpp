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
		ID_TEMPUP,
		ID_TEMPDN
	};
	bool debugRender;

public:
	MyApp():debugRender(false){
		appName = "FEMThermo";

		AddAction(MENU_MAIN, ID_TEMPUP, "Tc UP 20degree");
		AddHotKey(MENU_MAIN, ID_TEMPUP, 'o');
		AddAction(MENU_MAIN, ID_TEMPDN, "Tc DOWN 20degree");
		AddHotKey(MENU_MAIN, ID_TEMPDN, 'O');
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
		GetSdk()->LoadScene("sceneTHtest.spr", import);			// ファイルのロード			// scene.spr:negiをロード, scene2.spr:デバッグ用の直方体, scene3.spr:穴あきcheeseをロード, sceneTHtest.spr:フライパンなどインポートのテスト
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
			}else if(id=ID_TEMPUP){
				FWObjectIf** fwobject = fwScene->GetObjects();	
				//fwScene
			}else if(id=ID_TEMPDN){

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

		//	フライパンを取ってくる
		FWObjectIf* pan		=	DCAST(FWObjectIf, GetSdk()->GetScene()->FindObject("fwPan"));
		//	食材を取ってくる
		FWFemMeshIf* tmesh	= GetSdk()->GetScene()->FindObject("fwNegi")->Cast();
		//	ワールド座標に変換する

		//DSTR <<"pan: " << pan << std::endl;
		//DSTR <<"tmesh: " << tmesh << std::endl;

		//curScene = id;
		//// id番目のシーンを選択
		//fwScene = GetSdk()->GetScene(id);
		//phScene = fwScene->GetPHScene();
		//GetCurrentWin()->SetScene(fwScene);
		//editor.SetObject(phScene);

		//tmesh->GetScene();
		
		if(pan && tmesh){
			Affinef afPan = pan->GetGRFrame()->GetWorldTransform();
			Affinef afMesh = tmesh->GetGRFrame()->GetWorldTransform();
			Affinef afMeshToPan = afPan.inv() * afMesh;
			//DSTR << afPan <<std::endl;
			//DSTR << afMesh <<std::endl;
			//DSTR << afMeshToPan <<std::endl;

	
			//size_t nchObj = tmesh->NChildObject();
			PHFemMeshThermoIf* pfem = NULL;
			for(int i=0; i<tmesh->NChildObject() && !pfem; ++i){
				pfem = tmesh->GetChildObject(i)->Cast();
				int kattoon =0;
				if(pfem){
					DSTR <<"i : " << i << "th pfem->GetName(): " << pfem->GetName() << std::endl;
					//pfem->GetPHMesh();
					int katton1 =1;
				}
			}
			DSTR << "tmesh->GetPHMesh(): " << tmesh->GetPHMesh()<< std::endl;;
			PHFemMeshIf* phm = tmesh->GetPHMesh();
			int nOfSurfaceVtx = phm->NSurfaceVertices();
			for(unsigned i =0; i <nOfSurfaceVtx;i++){
				DSTR << i <<  "th phm->GetSurfaceVertex(i): " << phm->GetSurfaceVertex(i) << std::endl;
			} 
			int kattton =0;


			//////	最外殻の節点のフライパンからの座標
			//for(unsigned i=-0; i < surfaceVertices.size();i++){
			//	if(vertices[surfaceVertices[i]].pos){};
			//	//vertices[surfaceVertices[i]].Tc;
			//}

		//	for
		//	Vec3f posOnPan = afMeshToPan * tmesh.tvtxs[j].GetPos();
		//	int katoon =1;
		}

		//	フライパンにとっての原点からの距離に応じて、加熱する

		//char grName = "";
		//char phName = "";

		//GRFrameIf* findFrame = DCAST(GRFrameIf, GetSdk()->GetScene()->GetGRScene()->FindObject(grName));
		//PHSolidIf* findSolid = DCAST(PHSolidIf, GetSdk()->GetScene()->GetPHScene()->FindObject(phName));

		//if(pan){
		//	Affinef afPan = pan->GetGRFrame()->GetWorldTransform();
		//	Affinef afMesh = tmesh.obj->GetGRFrame()->GetWorldTransform();
		//	Affinef afMeshToPan = afPan.inv() * afMesh;	
		//}
		//PHFemMeshThermoオブジェクトを作り、main関数で使えるようにする関数
			//CreatePHFemMeshThermo();
		//総節点数、総要素数、節点座標、要素の構成などを登録
			//PHFemMeshThermoのオブジェクトを作る際に、ディスクリプタに値を設定して作る
			
		//節点の初期温度を設定する⇒{T}縦ベクトルに代入
			//{T}縦ベクトルを作成する。以降のK,C,F行列・ベクトルの節点番号は、この縦ベクトルの節点の並び順に合わせる。
			//CreateMeshTempVec();
		//熱伝導率、熱伝達率、密度、比熱などのパラメーターを設定・代入
			//PHFemMEshThermoのメンバ変数の値を代入
			//SetThermoConductionParam();
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
		//CreatePHFemMeshThermo();
		//[K]行列を作る
		//CreateKele();
		//[C]行列を作る
		//{F}ベクトルを作る
		//面積を求める
		//体積を求める
		
		//行列計算のテストを行う関数
		//TestMatrixFunc();
	}
	void CreateMeshTempVec(){

	}
	void SetThermoConductionParam(){

	}
	void CreatePHFemMeshThermo(){
		PHFemMeshThermoIf* phmesht;
		PHFemMeshThermoDesc desc;
	}
	void CreateKele(){

	}

	void CopyTheFood(){

	}

//	bool CopyTheFood(const char* phName, const char* grName){
//	if(GetSdk()->GetScene()->GetPHScene() && GetSdk()->GetScene()->GetGRScene()){
//		std::cout << phName << " : " << GetSdk()->GetScene()->GetPHScene()->FindObject(phName) << std::endl;
//		if(GetSdk()->GetScene()->GetPHScene()->FindObject(phName)){
//			PHSolidIf* findSolid = DCAST(PHSolidIf, GetSdk()->GetScene()->GetPHScene()->FindObject(phName));
//			PHSolidDesc sDesc;
//			if (findSolid) {
//				findSolid->GetDesc(&sDesc);
//				std::cout << "dupl : solid found : " << findSolid->GetName() << std::endl;
//			} else {
//				std::cout << "dupl : solid NOT found : " << phName << std::endl;
//			}
//
//			GRFrameIf* findFrame = DCAST(GRFrameIf, GetSdk()->GetScene()->GetGRScene()->FindObject(grName));
//			GRFrameDesc fDesc;
//			
//			cout << "GRFrame Name : " << findFrame->GetName() 
//				 << " GRMesh Name  : " << DCAST(NamedObjectIf, findFrame->GetChildObject(0))->GetName() << std::endl;
//			GRMesh* origMesh = findFrame->GetChildObject(0)->Cast();
//			GRMeshDesc& md = *(GRMeshDesc*)origMesh->GetDescAddress();		//GRMeshDesc型の値(ポインタの指す値)mdに、上でアドレスを格納したポインタ meshのGetDescAddressという関数の返り値を格納する。
//			GRMesh* cm = GetSdk()->GetScene()->GetGRScene()->CreateObject(md)->Cast();	//cm:create mesh? mdに格納されたポインタのさすオブジェクトをcreate(生成)して、GRMesh*型にキャストする。
//			cm->tex3d = true;												//3次元テクスチャを使用可にする。
//			cm->material = origMesh->material;									//cmのmaterialに　meshのmaterialListの値を格納する。
//			cm->materialList = origMesh->materialList;							//cmのmaterialListにmeshのmaterialListを格納する。
//			cm->faceNormals  = origMesh->faceNormals;							//cmのmeshの面の法線ベクトルに、meshのそれを格納する。
//			cm->originalFaceIds = origMesh->originalFaceIds;					//meshのoriginalFaceIds(３次元メッシュの元の頂点番号)をcmのそれに格納する。
//			cm->originalFaces  = origMesh->originalFaces;						//meshのoriginalFacesをcmのそれに格納する。
//
//			// FWObjectIf* copied = GetSdk()->GetScene()->CreateObject(sDesc, fDesc);
//			FWObjectIf* copied = GetSdk()->GetScene()->CreateFWObject();
//			copied->SetPHSolid(GetSdk()->GetScene()->GetPHScene()->CreateSolid(sDesc));
//			copied->SetGRFrame(GetSdk()->GetScene()->GetGRScene()->CreateVisual(fDesc.GetIfInfo(), fDesc)->Cast());
//
//			for (unsigned int i=0; i<findSolid->NChildObject(); ++i) {
//				copied->GetPHSolid()->AddChildObject(findSolid->GetChildObject(i));
//			}
//			copied->GetGRFrame()->AddChildObject(cm->Cast());
//
//			std::stringstream sout;
//			sout << "soCopiedFood" << copiedNum++;
//			copied->GetPHSolid()->SetName(sout.str().c_str());
//			Posed p = pan->GetPHSolid()->GetPose();					//フライパンの上に入れる
//			//食材は常にフライパンに入れる
//			copied->GetPHSolid()->SetFramePosition(Vec3d((double)rand() / (double)RAND_MAX * 0.03 * cos((double)rand()) + p.px,
//														(double)rand() / (double)RAND_MAX * 0.03 + 0.07 + p.py,
//														(double)rand() / (double)RAND_MAX * 0.03 * sin((double)rand()) +p.pz)
//														);
//			//copied->GetPHSolid()->SetAngularVelocity(Vec3d( (double)rand() / (double)RAND_MAX * 2.9,  (double)rand() / (double)RAND_MAX * 0.7,	(double)rand() / (double)RAND_MAX * 1.7 ));
//			copied->GetPHSolid()->SetAngularVelocity(Vec3d(0.0,0.0,0.0));
//			copied->GetPHSolid()->SetOrientation(Quaternionf::Rot(Rad((double)rand() / (double)RAND_MAX) * cos((double)rand()) + Rad(15) , 'x') 
//															  * Quaternionf::Rot(Rad((double)rand()), 'y')
//															  * Quaternionf::Rot(Rad((double)rand() / (double)RAND_MAX) * sin((double)rand()), 'z'));
//			copied->GetPHSolid()->SetVelocity(Vec3d(0.0,0.0,0.0));
//			sout.str("");
//			sout << "frCopiedFood" << copiedNum;
//			copied->GetGRFrame()->SetName(sout.str().c_str());
//			
//			foodObjs.push_back(copied);
//			GRMesh* mesh = foodObjs.back()->GetGRFrame()->GetChildObject(0)->Cast();
//			mesh->Render(GetCurrentWin()->render);
//
//			if(mesh && mesh->GetNVertex()){
//			
//				GRThermoMesh tmesh;
//				tmesh.Init(mesh, foodObjs.back());
//
//				std::stringstream foodName;
//				foodName << phName;
//				//copythings = 1;					//要検討
//				if (foodName.str().find("negi")!= string::npos || foodName.str().find("Negi")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmeshnegis.push_back(tmesh);
//					MyApp::InitNegi();
//				}
//				else if (foodName.str().find("carrot")!= string::npos || foodName.str().find("Carrot")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmeshcarrots.push_back(tmesh);
//					MyApp::InitCarrot();
//				}
//				else if (foodName.str().find("shrimp")!= string::npos || foodName.str().find("Shrimp")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmeshshrimps.push_back(tmesh);
//					MyApp::InitShrimp();
//				}
//				else if (foodName.str().find("asteakc")!= string::npos || foodName.str().find("Asteakc")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmesh.swtovtx = true;			//断面と本体の同じ位置座標を持つ頂点のペアを作る処理のフラグ
//					tmeshasteaks_c.push_back(tmesh);
//					MyApp::InitAsteak_c();
//				}
//				else if (foodName.str().find("asteak")!= string::npos || foodName.str().find("Asteak")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmesh.swtovtx = true;
//					tmeshasteaks.push_back(tmesh);
//					MyApp::InitAsteak();
//				}
//				else if (foodName.str().find("nsteakc")!= string::npos || foodName.str().find("Nsteakc")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmesh.swtovtx = true;
//					tmeshnsteaks_c.push_back(tmesh);
//					MyApp::InitNsteak_c();
//				}
//				else if (foodName.str().find("nsteak")!= string::npos || foodName.str().find("Nsteak")!= string::npos){
//					tmesh.copyInit =true;
//					tmesh.fwobj = copied;			//foodObjsの最後の要素がFWObjectIfなら、それを格納
//					tmesh.swtovtx = true;
//					tmeshnsteaks.push_back(tmesh);
//					MyApp::InitNsteak();
//				}
//				
//				tmeshes.push_back(tmesh);
//				DSTR		<< "Copy number of the food : " << copiedNum << std::endl;
//				std::cout	<< "Copy number of the food : " << copiedNum << std::endl;
//			}
//		}
//	}
//	//コピーが失敗したときのエラー表示
//	//DSTR		<< "Failed to copy." << std::endl;
//	//std::cout	<< "Failed to copy." << std::endl;
//	return false;
//}

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

