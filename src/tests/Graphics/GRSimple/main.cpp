/**
 Springhead2/src/tests/Physics/PHShapeGL/main.cpp

【概要】
  剛体Solidに形状を持たせたテストプログラム（位置を出力、面の頂点座標を出力、GL表示）
　・ペナルティ法による凸多面体同士の接触判定と接触力を確認する。
  ・剛体を自然落下させ、床の上に2個のブロックを積み上げる。
  ・頂点座標をデバッグ出力させ、OpenGLでシミュレーションを行う。
  
【終了基準】
  ・自由落下させた剛体が床の上で5秒間静止したら正常終了とする。
  ・自由落下させた剛体が床の上で静止せず、-500m地点まで落下した場合、異常終了とする。
 
【処理の流れ】
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  　剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。  
  ・与えられた条件により⊿t秒後の位置の変化を積分し、OpenGLでシミュレーションする。
　・デバッグ出力として、多面体の面(三角形)の頂点座標を出力する。   
 
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <gl/glut.h>
#pragma hdrstop
using namespace Spr;

#define ESC				27
#define FALL_DISTANCE	-500		// 落下距離(異常終了時)

GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

PHSdkIf* phSdk;
PHSceneIf* scene;
PHSolidIf* soFloor, *soBlock;

// 光源の設定 
static GLfloat light_position[] = { 15.0, 30.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// 材質の設定
static GLfloat mat_red[]        = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat mat_blue[]       = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[]  = { 120.0 };

static clock_t starttime, endtime, count;
static bool timeflag = false;

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void display(){
	/*	バッファクリア
	 *	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);　を内部でコール. */
	grDevice->ClearBuffer();


	glMaterialf(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (1.f,1.f,1.f,1.f));
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	Affined ad;
#if 0	
	Affinef view;
	view.Pos() = Vec3f(5.0, 5.0, 10.0);	                        // eye
	view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	// ビュー行列の逆行列をワールド行列とする
	view = view.inv();	
	render->SetViewMatrix(view);
#endif
	/********************************
	 *     下の赤い剛体(soFloor)    *
	 ********************************/
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
	render->PushModelMatrix();		// 行列スタックをプッシュ
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);

	// MultModelMatrix(ad); は、glMultMatrixd(ad); と同等
	//grDevice->MultModelMatrix(ad);		// 現在のmodelView * model
	grDevice->SetModelMatrix(ad);

	render->DrawSolid(soFloor);
	render->PopModelMatrix();		// 行列スタックをポップ

	/********************************
	 *     上の青い剛体(soBlock)    *
	 ********************************/
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);
	render->PushModelMatrix();
	pose = soBlock->GetPose();
	ad = Affined(pose);

	// 内部でglMultMatrixd(ad); をコール.
	//grDevice->MultModelMatrix(ad); としても同様.
	grDevice->SetModelMatrix(ad);		// view * model

	render->DrawSolid(soBlock);
	render->PopModelMatrix();

	/* ダブルバッファモード時、カレントウィンドウのバッファ交換を行い、
	   sceneのレンダリングを終了. glutSwapBuffers();を内部でコール. */
	render->EndScene();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

/**
 brief     	初期化処理
 param	 	なし
 return 	なし
 */
#if 1
void initialize(){
/*	Vec3f eye(0.0, 0.0, 3.0);
	Vec3f center(0.0, 0.0, 0.0);
	Vec3f up(0.0, 1.0, 0.0);
*/	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 3.0, 9.0, 
		      0.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	setLight();
}
#endif

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		 なし
 */
void reshape(int w, int h){
	// Viewportと射影行列を設定
	grDevice->Resize(Vec2f(w,h));

	/*glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);*/
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
	soBlock = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成

	Posed p = Posed::Rot(Rad(0.0), 'z');
	soBlock->SetPose(p);

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
		meshBlock = ICAST(CDConvexMeshIf, scene->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 3;
			md.vertices[i].z *= 3;
		}
		meshFloor = ICAST(CDConvexMeshIf, scene->CreateShape(md));
	}

	soFloor->AddShape(meshFloor);
	soBlock->AddShape(meshBlock);
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	soBlock->SetFramePosition(Vec3f(-0.5,5,0));
	soBlock->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(480, 360);
	int window = glutCreateWindow("PHShapeGL");
	grSdk = CreateGRSdk();
	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);

	// 初期設定
	// initialize();
	grDevice->Init();

	setLight();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);


	render->SetDevice(grDevice);	

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete phSdk;
}
