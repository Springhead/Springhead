/** 
 @file  Springhead2/src/tests/Physics/PHSimpleGL/main.cpp
 
@brief 2つの剛体の位置の変化を確認するテストプログラム（位置を出力,GL表示）
   
 <PRE>  
 <B>テスト内容</B>
  ・プログラムが正常終了したら0を返す。
  
 <B>プログラム概要</B>
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
    剛体の形状はOpenGLでティーポットを指定する。
  ・シミュレーションが動き始めたら、下記条件で剛体に力を加える。
      左の赤いティーポット：オブジェクトのローカル座標系を原点とし、重心の1m上を右に1Nの力を加える。
      右の青いティーポット：オブジェクトのローカル座標系の原点から1m右に重心をずらし、重心の1m上を右に1Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を積分し、剛体の位置情報を出力と、OpenGLレンダリングを行う。
 </PRE>
   
 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <gl/glut.h>
#pragma hdrstop
using namespace Spr;

#define ESC 27

PHSdkIf* sdk;
PHSceneIf* scene;
PHSolidIf* solid1, *solid2;
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

/**
 @brief     glutDisplayFuncで指定したコールバック関数
 @param	 	なし
 @return 	なし
 */
void display(){

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMaterialf(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (1.f,1.f,1.f,1.f));
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// 左の赤いティーポット
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);

	Affined ad;
	glPushMatrix();
	Posed pose = solid1->GetPose();
	pose.ToAffine(ad);
	//	solid1->GetOrientation().to_matrix(af);
	//	af.Pos() = solid1->GetFramePosition();
	// poseでは精度はdoubleなのでmatrixdにする
	glMultMatrixd(ad);
	glutSolidTeapot(1.0);
	glPopMatrix();

	// 右の青いティーポット
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);

	glPushMatrix();
	pose = solid2->GetPose();
	ad = Affined(pose);
	glMultMatrixd(ad);
	glutSolidTeapot(1.0);
	glPopMatrix();

	glutSwapBuffers();
}

/**
 @brief     光源の設定
 @param	 	なし
 @return 	なし
 */
void setLight() {
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);
}

/**
 @brief     初期化処理
 @param	 	なし
 @return 	なし
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(4.0, 3.0, 10.0, 
		      4.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	setLight();
}

/**
 @brief  glutReshapeFuncで指定したコールバック関数
 @param	 <in/--> w   幅
 @param  <in/--> h   高さ
 @return なし
 */
void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)(w/h), 1.0, 500);
	glMatrixMode(GL_MODELVIEW);
}

/**
 @brief glutKeyboardFuncで指定したコールバック関数 
 @param	<in/--> key     ASCIIコード
 @param <in/--> x       キーが押された時のマウス座標
 @param <in/--> y       キーが押された時のマウス座標
 @return なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == ESC) exit(0);
}

/**
 @brief  glutIdleFuncで指定したコールバック関数
 @param	 なし
 @return なし
 */
void idle(){
	//	剛体の重心の1m上を右に押す．
	solid1->AddForce( Vec3f(1,0,0), Vec3f(0,1,0)+solid1->GetCenterPosition());
	solid2->AddForce( Vec3f(1,0,0), Vec3f(0,1,0)+solid2->GetCenterPosition());
	scene->Step();
	glutPostRedisplay();
	std::cout << solid1->GetFramePosition();
	std::cout << solid2->GetFramePosition() << std::endl;
}

/**
 @brief  メイン関数
 @param	 <in/--> argc    コマンドライン入力の個数
 @param  <in/--> argv    コマンドライン入力
 @return  0 (正常終了)
 */
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();				//	SDKの作成
	scene = sdk->CreateScene();			//	シーンの作成
	
	PHSolidDesc desc;					// 左のteapot
	desc.mass = 2.0;					// 質量	
	desc.inertia *=2.0;					// 慣性テンソル
	desc.center = Vec3f(0,0,0);			// 質量中心の位置
	solid1 = scene->CreateSolid(desc);	// 剛体をdescに基づいて作成
	
	desc.center = Vec3f(1,0,0);			//	重心の位置をSolidの原点から1m右にずらす．
	desc.pose.pos = Vec3f(3.5, 0.0, 0.0);
	solid2 = scene->CreateSolid(desc);	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("PHSimpleGL");

	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	
	return 0;
}

