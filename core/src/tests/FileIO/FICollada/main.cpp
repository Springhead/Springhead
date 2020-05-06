/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/FileIO/FICollada/main.cpp

【概要】
  多分未完成。
  ファイル入出力SDKで、daeファイルをロード。表示する。
  多分未完成。

【終了基準】
  ・プログラムが正常終了したら0を返す。  


*/
#include <Springhead.h>
#include <GL/glut.h>
#define	ESC				27				// Esc key
//#define EXIT_TIMER	20000
#define	EXIT_TIMER	1000
#define TEST_FILECOLLADA	"test1.dae"


namespace Spr{
	UTRef<GRSdkIf> grSdk;
	UTRef<GRSceneIf> scene;
	UTRef<FISdkIf> fiSdk; 
	GRDeviceGLIf* grDevice;
	GRRenderIf* render;
}
using namespace Spr;


/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl display(){
	//	バッファクリア
	render->ClearBuffer();
	scene->Render(render);
	if (!scene){
		std::cerr << "scene == NULL. File may not found." << std::endl;
		exit(-1);
	}
	render->EndScene();
	glutSwapBuffers();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0;
	light0.ambient	= Vec4f(0.2, 0.2, 0.2, 1.0);
	light0.diffuse	= Vec4f(0.8, 0.8, 0.8, 1.0);
	light0.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light0.position = Vec4f(-10.81733, 100.99039, -21.19059, 0.0);
	light0.attenuation0  = 1.0;
	light0.attenuation1  = 0.0;
	light0.attenuation2  = 0.0;
	light0.spotDirection = Vec3f(0.0, -1.0, 0.0);
	light0.spotFalloff   = 10.0;
	light0.spotInner	 = 20;
	light0.spotCutoff	 = 150.0;
	render->PushLight(light0);

	GRLightDesc light1;
	light1.ambient	= Vec4f(0.2, 0.2, 0.2, 1.0);
	light1.diffuse	= Vec4f(0.8, 0.8, 0.8, 1.0);
	light1.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light1.position = Vec4f(-1.85930, 100.10970, -49.58080, 0.0);
	light1.attenuation0  = 1.0;
	light1.attenuation1  = 0.0;
	light1.attenuation2  = 0.0;
	light1.spotDirection = Vec3f(0.0, -1.0, 0.0);
	light1.spotFalloff   = 0.0;
	light1.spotInner	 = 10;
	light1.spotCutoff	 = 150.0;
	render->PushLight(light1);
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
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl idle(){
//	if(scene && *scene) (*(scene))->Step();
	glutPostRedisplay();
	static int count=0;
	count++;
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
//	FISdkIf::RegisterSdk();

	fiSdk = FISdkIf::CreateSdk();
	FIFileCOLLADAIf* fileCOLLADA = fiSdk->CreateFileCOLLADA();
	ObjectIfs objs;

	grSdk = GRSdkIf::CreateSdk();	
	objs.push_back(grSdk);
	scene = grSdk->CreateScene(GRSceneDesc());
	objs.push_back(scene);
	fileCOLLADA->Load(objs, TEST_FILECOLLADA);

	if (!grSdk) return -1;
	objs.clear();
	objs.Push(grSdk);
	fileCOLLADA->Save(objs, "out.dae");
	
	fiSdk->Clear();	//	ファイルローダのメモリを解放．
	objs.clear();
	grSdk->Print(DSTR);
	//scene = grSdk->GetScene(0);		// Sceneの取得
	
	grSdk->Print(DSTR);

	DSTR << "Loaded : " << "NScene=" << (int)grSdk->NScene() << std::endl;
	scene->Print(DSTR);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("FILoadMesh");

	render = grSdk->CreateRender();
	grDevice = grSdk->CreateDeviceGL();
	grDevice->Init();
	render->SetDevice(grDevice);

	// 視点設定
	Affinef view;
	view.Pos() = Vec3f(0.0, 3.0,-80.0);									// eye
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	// 光源の設定
	setLight();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
