/**
 @file	Springhead2/src/tests/Physics/PHShapeGL/main.cpp

 @brief ユーザのキー入力(スペースキー)に対してボックスを生成し自然落下させる。
  
 <PRE>
 <B>概要：</B>
  ・ペナルティ法による凸多面体同士の接触判定と接触力を確認する。
  ・剛体を自然落下させ、床の上に2個のブロックを積み上げる。
  ・頂点座標をデバッグ出力させ、OpenGLでシミュレーションを行う。
  
 <B>終了基準：</B>
　・自由落下させた剛体が床の上で5秒間静止したら正常終了とする。
  ・自由落下させた剛体が床の上で静止せず、-500m地点まで落下した場合、異常終了とする。
	
 <B>処理の流れ：</B>
  ・シミュレーションに必要な情報(剛体の形状・質量・慣性テンソルなど)を設定する。
  　剛体の形状はOpenGLで指定するのではなく、Solid自体で持たせる。  
  ・与えられた条件により⊿t秒後の位置の変化を積分し、OpenGLでシミュレーションする。
　・デバッグ出力として、多面体の面(三角形)の頂点座標を出力する。   
 </PRE>
 
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <ctime>
#include <string>
#include <gl/glut.h>
#pragma hdrstop
using namespace Spr;

#define ESC				27

PHSdkIf* sdk;
PHSolidDesc desc;
PHSceneIf* scene;
PHSolidIf* soFloor;
CDConvexMeshIf* meshFloor=NULL;
CDConvexMeshIf* meshBlock=NULL;
//PHSolidIf* soBlock, *soBlock2;
std::vector<PHSolidIf*> soBlock;


// 光源の設定 
static GLfloat light_position[] = { 25.0, 50.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// 材質の設定
static GLfloat mat_red[]        = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat mat_blue[]       = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[]  = { 120.0 };


/**
 @brief     多面体の面(三角形)の法線を求める
 @param	 	<in/out> normal　　 法線
 @param     <in/-->  base　　　 meshの頂点
 @param     <in/-->  face　　　 多面体の面
 @return 	なし
 */
void genFaceNormal(float *normal, Vec3f* base, CDFaceIf* face){
		normal[0] = base[face->GetIndices()[0]].y * base[face->GetIndices()[1]].z
						- base[face->GetIndices()[1]].y * base[face->GetIndices()[0]].z
						+ base[face->GetIndices()[1]].y * base[face->GetIndices()[2]].z
						- base[face->GetIndices()[2]].y * base[face->GetIndices()[1]].z
						+ base[face->GetIndices()[2]].y * base[face->GetIndices()[0]].z
						- base[face->GetIndices()[0]].y * base[face->GetIndices()[2]].z;
		normal[1] = base[face->GetIndices()[0]].z * base[face->GetIndices()[1]].x
						- base[face->GetIndices()[1]].z * base[face->GetIndices()[0]].x
						+ base[face->GetIndices()[1]].z * base[face->GetIndices()[2]].x
						- base[face->GetIndices()[2]].z * base[face->GetIndices()[1]].x
						+ base[face->GetIndices()[2]].z * base[face->GetIndices()[0]].x
						- base[face->GetIndices()[0]].z * base[face->GetIndices()[2]].x;
		normal[2] = base[face->GetIndices()[0]].x * base[face->GetIndices()[1]].y
						- base[face->GetIndices()[1]].x * base[face->GetIndices()[0]].y
						+ base[face->GetIndices()[1]].x * base[face->GetIndices()[2]].y
						- base[face->GetIndices()[2]].x * base[face->GetIndices()[1]].y
						+ base[face->GetIndices()[2]].x * base[face->GetIndices()[0]].y
						- base[face->GetIndices()[0]].x * base[face->GetIndices()[2]].y;
}

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

	Affined ad;
	
	// 下の赤い剛体(soFloor)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
	glPushMatrix();
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);
	glMultMatrixd(ad);	

	float normal[3];
	for(int i=0; i<soFloor->GetNShapes(); ++i){
		CDShapeIf** shapes = soFloor->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
		Vec3f* base = mesh->GetVertices();
		for(size_t f=0; f<mesh->GetNFaces();++f){
			CDFaceIf* face = mesh->GetFace(f);
			
			glBegin(GL_POLYGON);
			genFaceNormal(normal, base, face);
			glNormal3fv(normal);
			for(int v=0; v<face->GetNIndices(); ++v){	
				glVertex3fv(base[face->GetIndices()[v]].data);
			}
			glEnd();
		}
	}
	glPopMatrix();
	
	// 上の青い剛体(soBlock)
	for (unsigned int blockCnt = 0; blockCnt < soBlock.size(); blockCnt++) {	
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);
		glPushMatrix();
		pose = soBlock[blockCnt]->GetPose();
		ad = Affined(pose);
		glMultMatrixd(ad);
			for(int i=0; i<soBlock[blockCnt]->GetNShapes(); ++i){
				CDShapeIf** shapes = soBlock[blockCnt]->GetShapes();
				CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[i]);
				Vec3f* base = mesh->GetVertices();
				for(size_t f=0; f<mesh->GetNFaces();++f){
					CDFaceIf* face = mesh->GetFace(f);
				
					glBegin(GL_POLYGON);
					genFaceNormal(normal, base, face);
					glNormal3fv(normal);	
					for(int v=0; v<face->GetNIndices(); ++v){	
						glVertex3fv(base[face->GetIndices()[v]].data);
					}
					glEnd();
				}
			}
		glPopMatrix();
	}

	glutSwapBuffers();
}

/**
 @brief     光源の設定
 @param	 	なし
 @return 	なし
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
 @brief     初期化処理
 @param	 	なし
 @return 	なし
 */
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,20.0,30.0, 
		      0.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);


	setLight();
}

/**
 @brief		glutReshapeFuncで指定したコールバック関数
 @param		<in/--> w　　幅
 @param		<in/--> h　　高さ
 @return	なし
 */
void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

/**
 @brief 	glutKeyboardFuncで指定したコールバック関数 
 @param		<in/--> key　　 ASCIIコード
 @param 	<in/--> x　　　 キーが押された時のマウス座標
 @param 	<in/--> y　　　 キーが押された時のマウス座標
 @return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case ESC:		
		case 'q':
			exit(0);
			break;
		case ' ':{
			/*
			soBlock.push_back(scene->CreateSolid(desc));
			size_t index;
			index = soBlock.size() - 1;
			soBlock[index]->AddShape(meshBlock);
			soBlock[index]->SetFramePosition(Vec3f(0.5,10,0));
			soBlock[index]->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));
			*/
			soBlock.push_back(scene->CreateSolid(desc));
			size_t index;
			index = soBlock.size() - 1;



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


			soBlock[index]->AddShape(meshBlock);
			soBlock[index]->SetFramePosition(Vec3f(0.5,3,0));
			soBlock[index]->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));

				/*desc.mass = 2.0;
				desc.inertia *= 2.0;
			soBlock.push_back(scene->CreateSolid(desc));
			index = soBlock.size() - 1;
			soBlock[index]->AddShape(meshBlock);
			soBlock[index]->SetFramePosition(Vec3f(0.5,10,0));
			soBlock[index]->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));
*/
				 }break;
	
		default:
			break;
	}
}	

/**
 @brief  	glutIdleFuncで指定したコールバック関数
 @param	 	なし
 @return 	なし
 */
void idle(){

	scene->Step();
	glutPostRedisplay();
}

/**
 @brief		メイン関数
 @param		<in/--> argc　　コマンドライン入力の個数
 @param		<in/--> argv　　コマンドライン入力
 @return	0 (正常終了)
 */
int main(int argc, char* argv[]){
	sdk = CreatePHSdk();					// SDKの作成　
	scene = sdk->CreateScene();				// シーンの作成

	// soFloor用のdesc
	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	
	// soBlock用のdesc
	desc.mass = 2.0;
	desc.inertia = 2.0*Matrix3d::Unit();

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
		meshBlock = ICAST(CDConvexMeshIf, scene->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 20;
			md.vertices[i].z *= 10;
		}
		meshFloor = ICAST(CDConvexMeshIf, scene->CreateShape(md));
	}

	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3f(0,-2,0));


	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("PHShapeGL");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	//	SDKは開放しなくても良い．しなくてもmainを抜けてから開放される．
	delete sdk;
}