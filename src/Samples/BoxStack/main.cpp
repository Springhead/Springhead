/**
 Springhead2/src/Samples/BoxStack/main.cpp

【概要】
  ・ユーザのキー(スペースキー)入力に対してボックスを生成し、積み上げる。
  
【終了基準】
　・Escキーで強制終了。
	
【処理の流れ】
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  　剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。
  ・接触エンジンを拘束エンジンに設定する。
  ・与えられた条件により⊿t(=0.1)秒後の位置の変化を積分し、OpenGLでシミュレーションする。  
　・ユーザのキー入力に対しSolidを発生させる。
 
*/

#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif
#include <sstream>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC		27

PHSdkIf* sdk;
PHSolidDesc desc;
PHSceneIf* scene;
CDConvexMeshIf* meshFloor=NULL;
CDConvexMeshIf* meshBox=NULL;
PHSolidIf* soFloor;
std::vector<PHSolidIf*> soBox;

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


int frame=0,timepassed=0,timebase=0;


int boxStep = 0;
void fps(){
	frame++; 
	timepassed = glutGet(GLUT_ELAPSED_TIME); 
	if (1000 < timepassed - timebase) {
		std::cout << "   FPS:" << frame*1000.0/(timepassed - timebase);
		timebase = timepassed;		
		frame = 0;
	}
}

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
		fps();
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

	//gluLookAt(0.0,10.0,25.0, 
		      
	gluLookAt(0.0,10.0,100.0, 
		0.0,10.0, 0.0,
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
		case ESC:		
		case 'q':
			exit(0);
			break;
		case ' ':
			{
				/*
				soBox.push_back(scene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				if (soBox.size() < 30) {
					soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				} else {
					soBox.back()->SetFramePosition(Vec3f(0.5,100,0));
				}
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				std::ostringstream os;
				os << "box" << soBox.size();
				soBox.back()->SetName(os.str().c_str());
				*/
				
			} 
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
	float DT = 0.05f;
	int times = (int)(DT / scene->GetTimeStep());
	for(int i=0; i<times; ++i) scene->Step();
	glutPostRedisplay();
	glutTimerFunc((unsigned int)1000*DT, timer, 0);
	//boxStep++;
	
	//if (boxStep > 5) {
						soBox.push_back(scene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				if (soBox.size() < 30) {
					soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				} else {
					soBox.back()->SetFramePosition(Vec3f(0.5,100,0));
				}
				soBox.back()->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
				std::ostringstream os;
				os << "box" << soBox.size();
				soBox.back()->SetName(os.str().c_str());

	//			boxStep = 0;
	//}

}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();					// SDKの作成　
	PHSceneDesc dscene;
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;	// 接触エンジンを選ぶ
	//dscene.timeStep = 0.01;
	dscene.timeStep = 0.033;
	scene = sdk->CreateScene(dscene);				// シーンの作成

	// soFloor用のdesc
	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	
	// soBox用のdesc
	desc.mass = 2.0;
	desc.inertia = 2.0 * Matrix3d::Unit();

	//	形状の作成
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
		meshBox = ICAST(CDConvexMeshIf, sdk->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 300;
			md.vertices[i].z *= 200;
		}
		meshFloor = ICAST(CDConvexMeshIf, sdk->CreateShape(md));
	}
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3f(0,-2,0));


	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("BoxStack");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;
}
