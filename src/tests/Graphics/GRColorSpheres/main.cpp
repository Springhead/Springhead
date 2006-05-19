/**
 Springhead2/src/tests/Graphics/GRColorSpheres/main.cpp

【概要】
  グラフィックスレンダラークラスの DrawScene APIを使い、シーンを一括でレンダリングする。
  DrawScene API で設定されているマテリアルマテリアルサンプルを用いて、カラフルなボックスをレンダリングする。　
  
【終了基準】
  ・5000ステップ後に強制終了。

 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <GL/glut.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;
#define ESC				27
#define EXIT_TIMER		5000
#define WINSIZE_WIDTH	480
#define WINSIZE_HEIGHT	360
#define NUM_BLOCKS		30

GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

PHSdkIf* phSdk;
PHSceneIf* scene;
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
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLight light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	render->PushLight(light0);
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
	PHSceneDesc sd;
	sd.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;
	sd.timeStep = 0.01;
	scene = phSdk->CreateScene(sd);				// シーンの作成
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
	soFloor->SetFramePosition(Vec3f(0,-3,0));
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
	
	render->SetDevice(grDevice);	// デバイスの設定

	// 視点を設定する
	Affinef view;
	view.Pos() = Vec3f(5.0, 15.0, 15.0);								// eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));			// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);
	
	
	setLight();

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}
