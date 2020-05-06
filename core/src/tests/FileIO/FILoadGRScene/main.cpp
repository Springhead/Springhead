/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/FileIO/FILoadGRScene/main.cpp

【概要】
  ・Xファイルをロードし、Graphicsシーンをロード・描画する。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。  
 
【処理の流れ】
  ・Xファイルをロードする。
  ・ロードした情報を出力する。
  ・Graphicsエンジンと接続し、描画する。

【Note】
  ・本ファイルの30行目 " #define TEST_FILEX " にて、入力ファイル名を指定する。
  
*/
#include <Springhead.h>
#include <GL/glut.h>
#define	ESC				27				// Esc key
#define EXIT_TIMER	12000			// 強制終了させるステップ数
#define TEST_FILEX	"GRTest.x"		// ロードするXファイル

namespace Spr{
	UTRef<PHSdkIf> phSdk;
	UTRef<GRSdkIf> grSdk;
	GRSceneIf* scene;
	GRDeviceGLIf* grDevice;
	GRRenderIf* render;
	void PHRegisterTypeDescs();
	void CDRegisterTypeDescs();
	void GRRegisterTypeDescs();
	void FIRegisterTypeDescs();
}
using namespace Spr;


/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl display(){
	if (!scene){
		std::cout << "scene == NULL. File may not found." << std::endl;
		exit(-1);
	}
	//	バッファクリア
	render->ClearBuffer();
	scene->Render(render);
	render->EndScene();
	glutSwapBuffers();
}

/**
 brief  	glutReshapeFuncで指定したコールバック関数
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void __cdecl reshape(int w, int h){
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void __cdecl keyboard(unsigned char key, int x, int y){
	if (key == ESC) {
		std::cout << "exited by ESC key" << std::endl;
		exit(0);
	}
	if ('0'<= key && key <= '9' && phSdk && phSdk->NScene()){
		int i = key-'0';
		static UTRef<Spr::ObjectStatesIf> states[10];
		if (states[i]){
			states[i]->LoadState(scene);
		}else{
			states[i] = Spr::ObjectStatesIf::Create();
			states[i]->SaveState(scene);
		}
	}
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl idle(){
//	if(scene && *scene) (*(scene))->Step();
	glutPostRedisplay();
	static int count;
	count ++;
	if (count > EXIT_TIMER){
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
int __cdecl main(int argc, char* argv[]){
	PHSdkIf::RegisterSdk();
	GRSdkIf::RegisterSdk();

	UTRef<FISdkIf> fiSdk = FISdkIf::CreateSdk();
	FIFileXIf* fileX = fiSdk->CreateFileX();
	ObjectIfs objs;
	if (argc>=2){
		phSdk = PHSdkIf::CreateSdk();			//	PHSDKを用意して，
		objs.push_back(phSdk);		
		fileX->Load(objs, argv[1]);				//	ファイルローダに渡す方式
	}else{
		if (! fileX->Load(objs, TEST_FILEX) ) {	//	PHSDKごとロードして，
			std::cerr << "Error: Cannot open load file. " << std::endl;
			exit(EXIT_FAILURE);
		}
		phSdk = NULL;
		grSdk = NULL;
		for(unsigned  i=0; i<objs.size(); ++i){	
			if(!phSdk) phSdk = DCAST(PHSdkIf, objs[i]);	//	PHSDKを受け取る方式
			if(!grSdk) grSdk = DCAST(GRSdkIf, objs[i]);	//	GRSdkも受け取る
		}
	}
	if (!grSdk) return -1;
	objs.clear();
	objs.Push(grSdk);
	fileX->Save(objs, "out.x");
	
	fiSdk=NULL;	//	ファイルローダのメモリを解放．
	objs.clear();
	grSdk->Print(std::cout);
	scene = grSdk->GetScene(0);		// Sceneの取得
	std::cout << "Loaded : " << "NScene=" << (int)grSdk->NScene() << std::endl;
	scene->Print(std::cout);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("FILoadGRScene");

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

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
