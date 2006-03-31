/**
 Springhead2/src/Samples/Joints/main.cpp

【概要】関節機能のデモ
【仕様】
  ・シーン番号0, 1, ...をタイプすると対応するシーンに切り替わる
  ・シーン0：鎖
		space : 鎖を伸ばす
  　シーン1：アクチュエータ
		a : 正転トルク
		s : 脱力
		d : 逆転トルク
*/

#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <win32/GL/glut.h>
#include <sstream>
#pragma hdrstop
using namespace Spr;

#define ESC		27

PHSdkIf* sdk;			// SDKインタフェース
PHSceneIf* scene;		// Sceneインタフェース

double lookAtY, lookAtZ;

int sceneNo;			// シーン番号

PHSolidDesc descFloor;					//床剛体のディスクリプタ
PHSolidDesc descBox;					//箱剛体のディスクリプタ
CDConvexMeshIf* meshBox;				//箱形状のインタフェース
PHSolidIf* soFloor;						//床剛体のインタフェース
std::vector<PHSolidIf*> soBox;			//箱剛体のインタフェース
std::vector<PHJointIf*> jntLink;		//関節のインタフェース

// 光源の設定 
static GLfloat light_position[] = { 25.0, 50.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// 材質の設定
static GLfloat mat_floor[]      = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat mat_box[]        = { 0.8, 0.8, 1.0, 1.0 };
static GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[]  = { 120.0 };

void BuildScene();
void BuildScene0();
void BuildScene1();
void OnKey(char key);
void OnKey0(char key);
void OnKey1(char key);

/**
 brief     	多面体の面(三角形)の法線を求める
 param	 	<in/out> normal　　 法線
 param     	<in/-->  base　　　 meshの頂点
 param     	<in/-->  face　　　 多面体の面
 return 	なし
 */
void genFaceNormal(Vec3f& normal, Vec3f* base, CDFaceIf* face){
	Vec3f edge0, edge1;
	edge0 = base[face->GetIndices()[1]] - base[face->GetIndices()[0]];
	edge1 = base[face->GetIndices()[2]] - base[face->GetIndices()[0]];
	normal = edge0^edge1;
	normal.unitize();	
}

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	Affined ad;
	
	// 下の床(soFloor)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_floor);
	glPushMatrix();
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);
	glMultMatrixd(ad);	

	Vec3f normal;
	for(int i=0; i<soFloor->NShape(); ++i){
		CDShapeIf** shapes = soFloor->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
		Vec3f* base = mesh->GetVertices();
		for(size_t f=0; f<mesh->NFace();++f){
			CDFaceIf* face = mesh->GetFace(f);
			
			glBegin(GL_POLYGON);
			genFaceNormal(normal, base, face);
			glNormal3fv(normal.data);
			for(int v=0; v<face->NIndex(); ++v){	
				glVertex3fv(base[face->GetIndices()[v]].data);
			}
			glEnd();
		}
	}
	glPopMatrix();
	
	// ボックス(soBox)
	for (unsigned int boxCnt=0; boxCnt<soBox.size(); ++boxCnt) {	
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_box);
		glPushMatrix();
		pose = soBox[boxCnt]->GetPose();
		ad = Affined(pose);
		glMultMatrixd(ad);
			for(int i=0; i<soBox[boxCnt]->NShape(); ++i){
				CDShapeIf** shapes = soBox[boxCnt]->GetShapes();
				CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
				Vec3f* base = mesh->GetVertices();
				for(size_t f=0; f<mesh->NFace();++f){
					CDFaceIf* face = mesh->GetFace(f);
				
					glBegin(GL_POLYGON);
					genFaceNormal(normal, base, face);
					glNormal3fv(normal.data);	
					for(int v=0; v<face->NIndex(); ++v){	
						glVertex3fv(base[face->GetIndices()[v]].data);
					}
					glEnd();
				}
			}
		glPopMatrix();
		std::cout << "\rBox count : " << static_cast<unsigned int>(soBox.size());
	}

	glutSwapBuffers();
}

/**
 brief     	光源の設定
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
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	lookAtY = 15.0;
	lookAtZ = 30.0;
	gluLookAt(0.0,15.0,40.0, 
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
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	switch (key) {
		//終了
		case ESC:		
		case 'q':
			exit(0);
			break;
		//シーン切り替え
		case '0': case '1':
			scene->Clear();
			soFloor = NULL;
			soBox.clear();
			jntLink.clear();
			sceneNo = key - '0';
			BuildScene();
			break;
		case 'x':
			{
				static bool bEnable = true;
				bEnable = !bEnable;
				for(int i = 0; i < jntLink.size(); i++)
					jntLink[i]->Enable(bEnable);
			}break;
		default:
			OnKey(key);
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
//	for(int i=0; i<10; ++i) 
		scene->Step();
	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	// SDKの作成　
	sdk = CreatePHSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;	// 接触エンジンを選ぶ
	dscene.timeStep = 0.05;
	scene = sdk->CreateScene(dscene);				// シーンの作成
	// シーンの構築
	sceneNo = 0;
	BuildScene();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Joints");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timer, 0);

	
	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;
}

void BuildScene(){
	switch(sceneNo){
	case 0: BuildScene0(); break;
	case 1: BuildScene1(); break;
	}
}

// CDConvexMeshDescを直方体に初期化
void InitBoxMesh(CDConvexMeshDesc& md, double x, double y, double z){
	md.vertices.clear();
	md.vertices.push_back(Vec3f(-x,-y,-z));
	md.vertices.push_back(Vec3f(-x,-y, z));	
	md.vertices.push_back(Vec3f(-x, y,-z));	
	md.vertices.push_back(Vec3f(-x, y, z));
	md.vertices.push_back(Vec3f( x,-y,-z));	
	md.vertices.push_back(Vec3f( x,-y, z));
	md.vertices.push_back(Vec3f( x, y,-z));
	md.vertices.push_back(Vec3f( x, y, z));
}

// シーン0 : 鎖のデモ。space keyで箱が増える
void BuildScene0(){	
	CDConvexMeshDesc md;
	
	// 床を作成
	InitBoxMesh(md, 30.0, 1.0, 20.0);
	soFloor = scene->CreateSolid(descFloor);
	soFloor->AddShape(sdk->CreateShape(md));
	soFloor->SetFramePosition(Vec3f(0,-2,0));
	soFloor->SetDynamical(false);			// 床は外力によって動かないようにする
	
	//鎖の根になる箱を作成
	InitBoxMesh(md, 1.0, 1.0, 1.0);
	meshBox = ICAST(CDConvexMeshIf, sdk->CreateShape(md));
	descBox.mass = 2.0;
	descBox.inertia = 2.0 * Matrix3d::Unit();
	soBox.push_back(scene->CreateSolid(descBox));
	soBox.back()->AddShape(meshBox);
	//空中に固定する
	soBox.back()->SetFramePosition(Vec3f(0.0, 20.0, 0.0));
	soBox.back()->SetOrientation(Quaterniond::Rot(-1.57, Vec3d(0.0, 0.0, 1.0)));
	soBox.back()->SetDynamical(false);

	// 重力を設定
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

// シーン1 : アクチュエータのデモ
void BuildScene1(){
	CDConvexMeshDesc md;
	
	// 床を作成
	InitBoxMesh(md, 30.0, 1.0, 20.0);
	soFloor = scene->CreateSolid(descFloor);
	soFloor->AddShape(sdk->CreateShape(md));
	soFloor->SetFramePosition(Vec3f(0,-2,0));
	soFloor->SetDynamical(false);			// 床は外力によって動かないようにする
	
	soBox.resize(3);
	
	InitBoxMesh(md, 0.5, 1.0, 0.5);
	soBox[0] = scene->CreateSolid(descBox);
	soBox[0]->AddShape(sdk->CreateShape(md));
	soBox[0]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	InitBoxMesh(md, 0.5, 2.5, 0.5);
	soBox[1] = scene->CreateSolid(descBox);
	soBox[1]->AddShape(sdk->CreateShape(md));
	soBox[1]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	InitBoxMesh(md, 0.5, 5.0, 0.5);
	soBox[2] = scene->CreateSolid(descBox);
	soBox[2]->AddShape(sdk->CreateShape(md));
	soBox[2]->SetFramePosition(Vec3f(0.0, 20.0, 0.0));

	PHHingeJointDesc jd;
	jntLink.resize(4);
	jd.poseJoint[0].Pos() = Vec3d(0.0, 10.0, 0.0);
	jd.poseJoint[1].Pos() = Vec3d(0.0, -1.0, 0.0);
	jntLink[0] = scene->CreateJoint(soFloor, soBox[0], jd);

	jd.poseJoint[0].Pos() = Vec3d(4.0, 10.0, 0.0);
	jd.poseJoint[1].Pos() = Vec3d(0.0, -2.5, 0.0);
	jntLink[1] = scene->CreateJoint(soFloor, soBox[1], jd);

	jd.poseJoint[0].Pos() = Vec3d(0.0, 1.0, 0.0);
	jd.poseJoint[1].Pos() = Vec3d(0.0, -5.0, 0.0);
	jntLink[2] = scene->CreateJoint(soBox[0], soBox[2], jd);

	jd.poseJoint[0].Pos() = Vec3d(0.0, 2.5, 0.0);
	jd.poseJoint[1].Pos() = Vec3d(0.0, 0.0, 0.0);
	jntLink[3] = scene->CreateJoint(soBox[1], soBox[2], jd);

	scene->EnableContact(soBox[0], soBox[1], false);
	scene->EnableContact(soBox[0], soBox[2], false);
	scene->EnableContact(soBox[1], soBox[2], false);

	// 重力を設定
	scene->SetGravity(Vec3f(0, -9.8, 0));
}

void OnKey(char key){
	switch(sceneNo){
	case 0: OnKey0(key); break;
	case 1: OnKey1(key); break;
	}
}	
void OnKey0(char key){
	switch(key){
	case ' ':{
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(meshBox);
		soBox.back()->SetFramePosition(Vec3f(0.0, 1.0, 0.0));
		PHHingeJointDesc jdesc;
		jdesc.poseJoint[0].Pos() = Vec3d( 1,  1,  0);
		jdesc.poseJoint[1].Pos() = Vec3d(-1, -1,  0);
		int n = soBox.size();
		jntLink.push_back(scene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		}break;
	}
}
void OnKey1(char key){
	PHHingeJointIf* hinge = ICAST(PHHingeJointIf, jntLink[0]);
	switch(key){
	case 'a': hinge->SetMotorTorque(1.0);	break;
	case 's': hinge->SetMotorTorque(0.0);	break;
	case 'd': hinge->SetMotorTorque(-1.0);	break;
	case 'f': hinge->SetDesiredVelocity(Rad(90.0));	break;
	case 'g': hinge->SetDesiredVelocity(Rad(0.0));	break;
	case 'h': hinge->SetDesiredVelocity(Rad(-90.0));	break;
	case 'j':
		hinge->SetSpring(30.0);
		hinge->SetSpringOrigin(1.0);
		//hinge->SetDamper(1.0);
		break;
	case 'k':
		hinge->SetSpring(30.0);
		hinge->SetSpringOrigin(0.0);
		//hinge->SetDamper(0.0);
		break;
	case 'l':
		hinge->SetSpring(30.0);
		hinge->SetSpringOrigin(-1.0);
		break;
	case 'z':{
		CDConvexMeshDesc md;
		InitBoxMesh(md, 1.0, 1.0, 1.0);
		soBox.push_back(scene->CreateSolid(descBox));
		soBox.back()->AddShape(sdk->CreateShape(md));
		soBox.back()->SetFramePosition(Vec3f(7.0, 40.0, 0.0));
		//soBox.back()->SetGravity(false);
		}break;	
	}
}