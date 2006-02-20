/**
 Springhead2/src/tests/Physics/PHShapeGL/main.cpp

【概要】
  グラフィックスレンダラークラスのAPIを使い、GLデバイスでレンダリングを行う。　
  
【終了基準】
  ・5秒後に強制終了。

【処理の流れ】
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。  
  ・与えられた条件により⊿t秒後の位置の変化を積分し、OpenGLでシミュレーションする。

 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <WinBasis/WinBasis.h>
#include <windows.h>
#include <gl/glut.h>
#pragma hdrstop
using namespace Spr;
#define ESC				27
#define EXIT_TIMER		3000
#define WINSIZE_WIDTH	480
#define WINSIZE_HEIGHT	360
#define NUM_BLOCKS		5

GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

PHSdkIf* phSdk;
PHSceneIf* scene;
PHSolidIf* soFloor;
std::vector<PHSolidIf*> soBlock;

// 材質の設定
GRMaterial mat_blue(Vec4f(0.5, 0.5, 0.7, 0.85),		// ambient	
					Vec4f(0.1, 0.5, 0.8, 0.85),		// diffuse			
					Vec4f(1.0, 1.0, 1.0, 0.85),		// specular		
					Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive		
					100.0);							// power	

GRMaterial mat_white(Vec4f(0.5, 0.5, 0.5, 0.85),	
					 Vec4f(1.0, 1.0, 1.0, 1.0),			
					 Vec4f(0.8, 0.8, 0.8, 0.85),		
					 Vec4f(0.0, 0.0, 0.0, 0.85),		
					 20.0);

// カメラの設定
GRCamera camera2(Vec2f(WINSIZE_WIDTH, WINSIZE_HEIGHT), Vec2f(0.0, 0.0), 1.0, 5000.0);


/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	//	バッファクリア
	render->ClearBuffer();

	// 視点を再設定する
	Affinef view;
	view.Pos() = Vec3f(5.0, 15.0, 15.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	Affined ad;
	//-----------------------------------
	//		床(soFloor) 
	//-----------------------------------
	render->SetAlphaTest(true);
	render->SetAlphaMode(render->BF_ONE, render->BF_ZERO);

	render->SetMaterial(mat_white);		// マテリアル設定
	render->PushModelMatrix();			// 行列スタックをプッシュ
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);
	render->MultModelMatrix(ad);		// 現在のmodelView * model
	render->DrawSolid(soFloor);
	render->PopModelMatrix();			// 行列スタックをポップ

	//-----------------------------------
	//		ブロック(soBlock)
	//-----------------------------------
	render->SetDepthWrite(false);
	render->SetAlphaMode(render->BF_SRCALPHA, render->BF_INVSRCALPHA);

	for(unsigned int blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		render->SetMaterial(mat_blue);
		render->PushModelMatrix();
		pose = soBlock[blockCnt]->GetPose();
		ad = Affined(pose);
		render->SetModelMatrix(ad);			// view * model（この場合、render->MultModelMatrix(ad); としても同様）
		render->DrawSolid(soBlock[blockCnt]);
		render->PopModelMatrix();
	}

	render->SetDepthWrite(true);
	render->SetAlphaTest(true);

	//-----------------------------------
	//				軸
	//-----------------------------------
	render->SetMaterial(mat_white);
	Vec3f vtx[4] = {Vec3f(0,0,0), Vec3f(10,0,0), Vec3f(0,10,0), Vec3f(0,0,10)};
	size_t vtxIndex[6] = {0, 1, 0, 2, 0, 3};
	render->SetLineWidth(2.0);
	render->DrawIndexed(render->LINES, vtxIndex, vtxIndex + 6, vtx);

	//-----------------------------------
	//		テキスト描画/フォント
	//-----------------------------------
	GRFont font1;
	font1.height = 30.0;
	font1.width	= 0.0;
	font1.weight	= FW_NORMAL;
	font1.color  = 0xFFFFFF;
	font1.bItalic = TRUE;
	font1.face   = "ARIAL";
	std::string str = "X";
	render->DrawFont(Vec3f(10.0, 1.0, -1.0), str, font1);	
	font1.face = "ＭＳ 明朝";
	font1.color = 0xFFFF00;
	str = "Y";
	render->DrawFont(Vec3f(1.0, 10.0, 0.0), str, font1);		
	GRFont font2;
	font2 = font1;
	font2.color = 0x00FFFF;
	str = "Z";
	render->DrawFont(Vec3f(-2.0, 1.0, 10.0), str, font2);	



	render->EndScene();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLight light0;
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

	GRLight light1;
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
void reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(w,h));
}
/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == ESC) exit(0);

}	
/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	scene->Step();
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
int main(int argc, char* argv[]){
	phSdk = CreatePHSdk();					// SDKの作成　
	scene = phSdk->CreateScene();				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;

	unsigned int blockCnt;
	for (blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		soBlock.push_back(scene->CreateSolid(desc));		// 剛体をdescに基づいて作成
	}

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
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
		meshBlock = ICAST(CDConvexMeshIf, phSdk->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 30;
		}
		meshFloor = ICAST(CDConvexMeshIf, phSdk->CreateShape(md));
	}
	
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	for (blockCnt=0; blockCnt<NUM_BLOCKS; ++blockCnt){
		soBlock[blockCnt]->AddShape(meshBlock);
		soBlock[blockCnt]->SetFramePosition(Vec3f(3, 15*(blockCnt+1), 3));
	}

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINSIZE_WIDTH, WINSIZE_HEIGHT);
	int window = glutCreateWindow("GRSimple");
	grSdk = CreateGRSdk();
	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);

	// 初期設定
	grDevice->Init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	
	render->SetDevice(grDevice);	
	render->SetCamera(camera2);	

	setLight();

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}
