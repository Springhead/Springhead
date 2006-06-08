/**
 Springhead2/src/Samples/Vehicles/main.cpp

【概要】Springhead2の総合的なデモ第１段
 
【仕様】

*/

#include <Springhead.h>		//	Springheadのインタフェース
//#include <ctime>
//#include <string>
#include <GL/glut.h>
//#include <sstream>
#include "robot1.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

PHSdkIf* phSdk;			// SDKインタフェース
GRSdkIf* grSdk;
PHSceneIf* scene;		// Sceneインタフェース
GRDebugRenderIf* render;
GRDeviceGLIf* device;

double lookAtY, lookAtZ;

Robot1 robot;

void CreateFloor(){
	CDBoxDesc desc;
	desc.boxsize = Vec3f(30.0f, 1.0f, 20.0f);
	PHSolidDesc sd;
	PHSolidIf* soFloor = scene->CreateSolid(sd);
	soFloor->AddShape(phSdk->CreateShape(desc));
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	soFloor->SetDynamical(false);			// 床は外力によって動かないようにする
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
}

/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	render->PushLight(light0);
	render->PushLight(light1);
}

/**
 brief     	初期化処理
 param	 	なし
 return 	なし
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lookAtY = 0.2;
	lookAtZ = 1.0;
	gluLookAt(0.0,lookAtY,lookAtZ, 
		      0.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	setLight();
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		なし
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
	unsigned int i = 0;
	switch (key) {
	//終了
	case ESC:		
	case 'q':
		exit(0);
		break;
	case 'a':
		robot.Forward();
		break;
	case 's':
		robot.Backward();
		break;
	case 'd':
		robot.TurnLeft();
		break;
	case 'f':
		robot.TurnRight();
		break;
	default:
		break;
	}
}	

/**
 brief  	glutTimerFuncで指定したコールバック関数
 param	 	<in/--> id　　 タイマーの区別をするための情報
 return 	なし
 */
void timer(int id){
	/// 時刻のチェックと画面の更新を行う
	for(int i = 0; i < 5; i++)
		scene->Step();
	glutPostRedisplay();
	glutTimerFunc(50, timer, 0);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){

	// SDKの作成　
	phSdk = CreatePHSdk();
	grSdk = CreateGRSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;	// 接触エンジンを選ぶ
	dscene.timeStep = 0.01;
	scene = phSdk->CreateScene(dscene);				// シーンの作成
	
	// シーンの構築
	CreateFloor();
	robot.Build(scene, phSdk);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("Joints");

	render = grSdk->CreateDebugRender();
	device = grSdk->CreateDeviceGL(window);
	device->Init();
	render->SetDevice(device);	// デバイスの設定

	glutTimerFunc(20, timer, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	
	initialize();

	glutMainLoop();

	delete phSdk;
	delete grSdk;
}
