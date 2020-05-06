/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/FileIO/FILoader/main.cpp

【概要】
  sprファイルをロードし、Physicsエンジンと接続してシミュレーションする。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。  
 
【処理の流れ】
  ・sprファイルをロードする。
  ・ロードした情報を出力する。
  ・Physicsエンジンと接続し、シミュレーションさせる。
	ロードするファイル=test.spr  : 凸形状(mesh)と球(sphere)と直方体(box)のテスト
 */
#include <Springhead.h>
#include <GL/glut.h>

#define	ESC				27				// Esc key
//#define EXIT_TIMER		12000			// 強制終了させるステップ数
#define EXIT_TIMER		1200


namespace Spr{
	UTRef<PHSdkIf> phSdk;
	UTRef<GRSdkIf> grSdk;
	UTRef<FWSdkIf> fwSdk;
	PHSceneIf* scene;
	FWSceneIf* fwScene;
	GRDeviceGLIf* grDevice;
	GRRenderIf* render;
	void PHRegisterTypeDescs();
	void CDRegisterTypeDescs();
	void GRRegisterTypeDescs();
	void FIRegisterTypeDescs();
}
using namespace Spr;

Vec3f *vtx=NULL;


// 材質の設定
GRMaterialDesc mat_red(Vec4f(0.0, 0.0, 0.0, 1.0),		// ambient
						Vec4f(0.7, 0.0, 0.0, 1.0),		// diffuse
						Vec4f(1.0, 1.0, 1.0, 1.0),		// specular
						Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive
						100.0);							// power
GRMaterialDesc mat_green(Vec4f(0.0, 0.0, 0.0, 1.0),		
						Vec4f(0.0, 0.7, 0.0, 1.0),		
						Vec4f(1.0, 1.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.0, 1.0),		
						20.0);							
GRMaterialDesc mat_blue(Vec4f(0.0, 0.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.7, 1.0),		
						Vec4f(1.0, 1.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.0, 1.0),		
						20.0);							
GRMaterialDesc mat_yellow(Vec4f(0.0, 0.0, 1.0, 1.0),		
						  Vec4f(1.0, 0.7, 0.0, 1.0),		
						  Vec4f(1.0, 1.0, 1.0, 1.0),		
						  Vec4f(0.0, 0.0, 0.0, 1.0),		
						  20.0);							
std::vector<GRMaterialDesc> material;

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL display(){
	render->ClearBuffer();
	if (!fwScene){
		fwScene = fwSdk->CreateScene();
		fwScene->AddChildObject(scene);
	}
	if (fwScene)
		fwScene->DrawPHScene(render);
	render->EndScene();
	render->SwapBuffers();
	return;
}
/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0;
	light0.ambient			= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	light0.diffuse			= Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	light0.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	light0.position			= Vec4f(0.0f, 10.0f, 0.0f, 1.0f);
	light0.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
	light0.spotCutoff		= 145.0f;
	light0.spotFalloff		= 30.0f;
	render->PushLight(light0);

	GRLightDesc light1;
	light1.diffuse			= Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
	light1.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	light1.position			= Vec4f(0.0f, 10.0f, 10.0f, 0.0f);
	light1.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
	render->PushLight(light1);
}
/**
 brief     	材質の設定
 param	 	なし
 return 	なし
 */
void setMaterial() {
	material.push_back(mat_red);
	material.push_back(mat_green);
	material.push_back(mat_blue);
	material.push_back(mat_yellow);
}

/**
 brief  	glutReshapeFuncで指定したコールバック関数
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void SPR_CDECL reshape(int w, int h){
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void SPR_CDECL keyboard(unsigned char key, int x, int y){
	if (key == ESC) {
		delete[] vtx;
		std::cout << "exited by ESC key" << std::endl;
		exit(0);
	}
	if ('0'<= key && key <= '9' && phSdk->NScene()){
		int i = key-'0';
		static UTRef<Spr::ObjectStatesIf> states[10];
		if (states[i]){
			states[i]->LoadState(scene);
		}else{
			states[i] = ObjectStatesIf::Create();
			states[i]->SaveState(scene);
		}
	}
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL idle(){
	if(scene) {
		scene->Step();
	}
	glutPostRedisplay();
	static int count;
	count ++;
	if (count > EXIT_TIMER){
		delete[] vtx;
		std::cout << EXIT_TIMER << " count passed." << std::endl;
		exit(0);
	}
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char* argv[]){
	{	//	glutMainLoop()を呼ぶと帰ってこないため、メモリリークが起きるので。
		PHSdkIf::RegisterSdk();
		GRSdkIf::RegisterSdk();
		FWSdkIf::RegisterSdk();

		UTRef<FISdkIf> fiSdk = FISdkIf::CreateSdk();
		FIFileIf* file = fiSdk->CreateFileFromExt(".spr");
		ObjectIfs objs;
		if (argc>=2){
			phSdk = PHSdkIf::CreateSdk();					//	PHSDKを用意して，
			objs.push_back(phSdk);		
			file->Load(objs, argv[1]);				//	ファイルローダに渡す方式
		}else{
			fwSdk = FWSdkIf::CreateSdk();					//	FWSDKを用意して，
			objs.push_back(fwSdk);		
			if (! file->Load(objs, "test.spr") ) {	//	PHSDKごとロードして，
				std::cerr << "Error: Cannot open load file. " << std::endl;
				exit(EXIT_FAILURE);
			}
			phSdk = NULL;
			grSdk = NULL;
			for(unsigned  i=0; i<objs.size(); ++i){	
				objs[i]->Print(std::cout);
				if(!phSdk) phSdk = DCAST(PHSdkIf, objs[i]);	//	PHSDKを受け取る方式
				if(!grSdk) grSdk = DCAST(GRSdkIf, objs[i]);	//	GRSdkも受け取る
			}
		}
		if (phSdk && phSdk->NScene()){
			ObjectIfs objs;
	//		objs.Push(phSdk->GetScenes()[0]);
			objs.Push(phSdk);
			if(grSdk) objs.Push(grSdk);
			file->Save(objs, "out.spr");
		}
		fiSdk = NULL;	//	ファイルローダのメモリを解放．
		objs.clear();
		phSdk->Print(std::cout);

		scene = phSdk->GetScene(0);		// Sceneの取得
	//	shape = phSdk->GetShapes();		// Shapeの取得
		std::cout << "Loaded : " << "NScene=" << phSdk->NScene() << ", NShape=" << phSdk->NShape() << std::endl;
		if (scene) scene->SetTimeStep(0.01);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		int window = glutCreateWindow("FILoader");

		// Graphics Sdk
		grSdk = GRSdkIf::CreateSdk();
		render = grSdk->CreateRender();
		grDevice = grSdk->CreateDeviceGL();
		grDevice->Init();
		render->SetDevice(grDevice);

		// 視点設定
		Affinef view;
		view.Pos() = Vec3f(0.0, 3.0, 3.0);									// eye
			view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
		view = view.inv();	
		render->SetViewMatrix(view);

		setLight();			// 光源設定
		setMaterial();		// 材質設定

		vtx = DBG_NEW Vec3f[4];
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(idle);
	}
	glutMainLoop();
	return 0;
}
