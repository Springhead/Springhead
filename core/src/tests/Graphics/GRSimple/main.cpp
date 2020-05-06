/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Graphics/GRSimple/main.cpp

【概要】
  グラフィックスレンダラークラスのAPIを使い、GLデバイスでレンダリングを行う。　
  
【終了基準】
  ・5000ステップ後に強制終了。

【処理の流れ】
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。  
  ・与えられた条件により⊿t秒後の位置の変化を積分し、OpenGLでシミュレーションする。

 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <GL/glut.h>

using namespace Spr;
#define ESC				27
#define EXIT_TIMER		5000
#define WINSIZE_WIDTH	800
#define WINSIZE_HEIGHT	600
#define NUM_BLOCKS		5

UTRef<FWSdkIf>	fwSdk;
FWSceneIf*	fwScene;

UTRef<GRSdkIf> grSdk;
GRRenderIf* render;
GRDeviceGLIf* grDevice;

UTRef<PHSdkIf> phSdk;
PHSceneIf* phScene;
PHSolidIf* soFloor;
std::vector<PHSolidIf*> soBlock;

// 材質の設定
GRMaterialDesc matFloor(Vec4f(0.2, 0.2, 0.2, 1.0),		// ambient
					Vec4f(0.6, 0.6, 0.6, 1.0),		// diffuse
					Vec4f(0.2, 0.2, 0.2, 1.0),		// specular
					Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive
					10.0);							// power
GRMaterialDesc matBlock(Vec4f(0.5, 0.5, 0.7, 0.9),		
					Vec4f(0.1, 0.5, 0.8, 0.9),		
					Vec4f(1.0, 1.0, 1.0, 0.9),			
					Vec4f(0.0, 0.0, 0.0, 1.0),		
					10.0);
GRMaterialDesc matLine(Vec4f(1.0, 1.0, 1.0, 1.0),		
					Vec4f(1.0, 1.0, 1.0, 1.0),		
					Vec4f(1.0, 1.0, 1.0, 1.0),		
					Vec4f(0.0, 0.0, 0.0, 1.0),		
					100.0);	


/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void __cdecl display(){
	//	バッファクリア
	render->ClearBuffer();
	render->BeginScene();

	// 視点を再設定する
	Affinef view;
	view.Pos() = Vec3f(5.0, 15.0, 15.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	//-----------------------------------
	//		床(soFloor) 
	//-----------------------------------
	render->SetAlphaTest(true);
	render->SetAlphaMode(render->BF_ONE, render->BF_ZERO);

	render->SetMaterial(matFloor);		// マテリアル設定
	fwScene->DrawSolid(render, soFloor, true);

	//-----------------------------------
	//		ブロック(soBlock)
	//-----------------------------------
	render->SetDepthWrite(false); 
	render->SetAlphaMode(render->BF_SRCALPHA, render->BF_ONE);
	for(unsigned int blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		render->SetMaterial(matBlock);
		fwScene->DrawSolid(render, soBlock[blockCnt], true);
	}

	render->SetDepthWrite(true);
	render->SetAlphaTest(false);

	//-----------------------------------
	//				軸
	//-----------------------------------
	render->SetMaterial(matLine);
	Vec3f vtx[4] = {Vec3f(0,0,0), Vec3f(10,0,0), Vec3f(0,10,0), Vec3f(0,0,10)};
	GLuint vtxIndex[6] = {0, 1, 0, 2, 0, 3};
	render->SetLineWidth(2.0);
	render->SetVertexFormat(GRVertexElement::vfP3f);
	render->DrawIndexed(render->LINES, vtxIndex, vtx, 6);


	//-----------------------------------
	//		テキスト描画/フォント
	//-----------------------------------
	GRFont font1;
	font1.height = 30;
	font1.width	= 0;
	font1.weight = 400;
	font1.color  = 0xFFFFFF;
	font1.bItalic = true;
	font1.face   = "ARIAL";
	std::string str = "X";
	render->SetFont(font1);
	render->DrawFont(Vec3f(8.0, 1.0, -1.0), str);
	font1.face = "ＭＳ 明朝";
	font1.color = 0xFFFF00;
	str = "Y";
	render->SetFont(font1);
	render->DrawFont(Vec3f(1.0, 7.0, 0.0), str);
	GRFont font2;
	font2 = font1;
	font2.color = 0x00FFFF;
	str = "Z";
	render->SetFont(font2);
	render->DrawFont(Vec3f(-1.0, 1.0, 9.0), str);

	render->EndScene();
	render->SwapBuffers();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0;
	light0.ambient	= Vec4f(0.0, 0.0, 0.0, 1.0);
	light0.diffuse	= Vec4f(0.7, 1.0, 0.7, 1.0);
	light0.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light0.attenuation0  = 1.0;
	light0.attenuation1  = 0.0;
	light0.attenuation2  = 0.0;
	light0.spotDirection = Vec3f(-2.0, -3.0, -5.0);
	light0.spotFalloff   = 0.0;
	light0.spotCutoff	 = 70.0;
	render->PushLight(light0);

	GRLightDesc light1;
	light1.ambient	= Vec4f(0.5, 0.0, 1.0, 1.0);
	light1.diffuse	= Vec4f(1.0, 0.0, 1.0, 1.0);
	light1.specular	= Vec4f(0.0, 1.0, 0.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	render->PushLight(light1);
}
/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		 なし
 */
void __cdecl reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(0,0), Vec2f(w,h));
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
//		phSdk=NULL;
//		grSdk = NULL;
		exit(0);
	}
}	
/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl idle(){
	fwScene->Step();
	glutPostRedisplay();
	static int count;
	count++;
	if (++count > EXIT_TIMER) exit(0);
}
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int __cdecl main(int argc, char* argv[]){
	fwSdk = FWSdkIf::CreateSdk();
	phSdk = fwSdk->GetPHSdk();
	PHSceneDesc sd;
	sd.timeStep = 0.01;

	fwScene = fwSdk->CreateScene(sd, GRSceneDesc());
	phScene = fwScene->GetPHScene();
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;

	unsigned int blockCnt;
	for (blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		soBlock.push_back(phScene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = phScene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	
	//	形状の作成
	CDConvexMeshIf* meshFloor=NULL;
	CDConvexMeshIf* meshBlock=NULL;
	{
		CDConvexMeshDesc md;
		md.vertices.push_back(Vec3f(-1,-1,-1));
		md.vertices.push_back(Vec3f(-1,-1, 1));	
		md.vertices.push_back(Vec3f(-1, 1,-1));	
		md.vertices.push_back(Vec3f(-1, 1, 1));
		md.vertices.push_back(Vec3f( 1,-1,-1));	
		md.vertices.push_back(Vec3f( 1,-1, 1));
		md.vertices.push_back(Vec3f( 1, 1,-1));
		md.vertices.push_back(Vec3f( 1, 1, 1));
		meshBlock = DCAST(CDConvexMeshIf, phSdk->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 30;
		}
		meshFloor = DCAST(CDConvexMeshIf, phSdk->CreateShape(md));
	}
	
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3f(0,-30,0));
	for (blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		soBlock[blockCnt]->AddShape(meshBlock);
		soBlock[blockCnt]->SetFramePosition(Vec3f(3, 15*(blockCnt+1), 3));
	}

	phScene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINSIZE_WIDTH, WINSIZE_HEIGHT);
	int window = glutCreateWindow("GRSimple");
	grSdk = GRSdkIf::CreateSdk();
	render = grSdk->CreateRender();
	grDevice = grSdk->CreateDeviceGL();

	// 初期設定
	grDevice->Init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	render->SetDevice(grDevice);	
	
	setLight();

	glutMainLoop();
}
