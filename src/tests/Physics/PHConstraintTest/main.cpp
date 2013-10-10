/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Physics/PHConstraintTest.cpp

【概要】
  ・接触モードに LCP法 を選択して、シミュレーションする。
  
【終了基準】
　・生成したブロックすべてが、数ステップ間、床の上に静止したら正常終了。　
   
 */
#include <Springhead.h>		//	Springheadのインタフェース
#include <string>
#include <GL/glut.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC					27		// ESC key
#define STAY_COUNTER		10		// 静止判定カウント
#define TOTAL_IDLE_COUNTER	1000	// 静止しない場合に利用	
#define NUM_BLOCK			6//6	// 生成するボックス数

const float boxStride = (const float)(2+0.1);
float boxpos[][3] = {
/*	{0,boxStride*1,0},
	{0,boxStride*2,0},
	{0,boxStride*3,0},
	{0,boxStride*4,0},
	{0,boxStride*5,0},
*/
	{-2.4f, 1.1f, 0}, {0, 1.1f, 0}, {2.4f, 1.1f, 0}, {-1.2f, 4.2f, 0}, {1.2f, 4.2f, 0},
	{0, 7.4f, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}
};

UTRef<PHSdkIf> sdk;
PHSceneIf* scene;
PHSolidIf* soFloor, *soBlock[NUM_BLOCK];
static Vec3d prepos[NUM_BLOCK];				// previous position
static Vec3d curpos[NUM_BLOCK];				// current position

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

static int elapse = 100;	//timer周期[ms]
static double dt = 0.05;	//積分ステップ[s]

/**
 brief     多面体の面(三角形)の法線を求める
 param	   <in/out> normal　　  法線
 param     <in/-->   base　　　 meshの頂点
 param     <in/-->   face　　　 多面体の面
 return     なし
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
 param		なし
 return 	なし
 */
void SPR_CDECL display(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	Affined ad;
	
	// 下の赤い剛体(soFloor)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_red);
	glPushMatrix();
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);
	glMultMatrixd(ad);	

	Vec3f normal;
	for(int i=0; i<soFloor->NShape(); ++i){
		CDShapeIf* shape = soFloor->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		Vec3f* base = mesh->GetVertices();
		for(int f=0; f<mesh->NFace();++f){
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

	
	for(int n = 0; n < NUM_BLOCK; n++){
		// 上の青い剛体(soBlock)
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blue);
		glPushMatrix();
		pose = soBlock[n]->GetPose();
		ad = Affined(pose);
		glMultMatrixd(ad);
			for(int i=0; i<soBlock[n]->NShape(); ++i){
			CDShapeIf* shape = soBlock[n]->GetShape(i);
			CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
			Vec3f* base = mesh->GetVertices();
			for(int f=0; f<mesh->NFace();++f){
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
	}	
	glutSwapBuffers();
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
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 5.0, 20.0, 
		      0.0, 3.0, 0.0,
		 	  0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	setLight();
}

/**
 brief		glutReshapeFuncで指定したコールバック関数
 param		<in/--> w　　幅
 param		<in/--> h　　高さ
 return		 なし
 */
void SPR_CDECL reshape(int w, int h){
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
void SPR_CDECL keyboard(unsigned char key, int x, int y){
	if (key == ESC) exit(0);
}	

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL idle(){
#if 1
	static int stepCnt = 0;						// 静止したステップカウント
	static int totalStep = 0;					// 全実行ステップ数（終了判定で使用）
	
	int iCnt = 0;
	for (iCnt = 0; iCnt < NUM_BLOCK; iCnt++){
		prepos[iCnt] = soBlock[iCnt]->GetFramePosition();
	}
	scene->Step();
	for (iCnt = 0; iCnt < NUM_BLOCK; iCnt++){
		curpos[iCnt] = soBlock[iCnt]->GetFramePosition();
	}
	iCnt = 0;
	bool flag = true;
	int boxIndex = 0;
	// preposとcurposが一致しないboxがあるか
	while(iCnt < NUM_BLOCK){
		if (!approx(prepos[iCnt], curpos[iCnt])) {	
			flag = false;
			boxIndex = iCnt;
			break;
		}
		iCnt++;
	}
	// 静止ステップ数のカウント
	if (flag == true){
		stepCnt++;
		std::cout << "true" << std::endl;
	} else {
		std::cout << boxIndex << "               false    " << prepos[boxIndex]-curpos[boxIndex] <<  std::endl;
		stepCnt = 0;
	}
	// 終了判定
	if (stepCnt > STAY_COUNTER){
		DSTR << "\nPHConstraintTest success." << std::endl;
		exit(EXIT_SUCCESS);
	}
	totalStep++;
	if (totalStep > TOTAL_IDLE_COUNTER){
		DSTR << "\nPHShapeGL failure." << std::endl;
		exit(EXIT_FAILURE);
	}
	glutPostRedisplay();
#else
	scene->Step();
	glutPostRedisplay();
	static int count;
	count++;
	if (count > TOTAL_IDLE_COUNTER) exit(0);	
#endif	
}

/**
 brief  	glutTimerFuncで指定したコールバック関数
 param	 	<in/--> id　　 タイマーの区別をするための情報
 return 	なし
 */
void SPR_CDECL timer(int id){
	int n = (int)((double)(elapse) / 1000.0 / dt);
	for(int i = 0; i < n; i++)
		idle();
	glutTimerFunc(elapse, timer, 0);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */

int SPR_CDECL main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();							// SDKの作成　
	PHSceneDesc dscene;
	dscene.numIteration = 100;
	dscene.timeStep = dt;
	scene = sdk->CreateScene(dscene);				// シーンの作成
	scene->SetContactMode(PHSceneDesc::MODE_LCP);	// 接触モードに LCP法で選択

	PHSolidDesc dsolid;
	dsolid.mass = 2.0;
	dsolid.inertia *= 2.0;
	for(int i = 0; i < NUM_BLOCK; i++)
		soBlock[i] = scene->CreateSolid(dsolid);	// 剛体をdescに基づいて作成
	
	dsolid.mass = 1e20f;
	dsolid.inertia *= 1e20f;
	soFloor = scene->CreateSolid(dsolid);			// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	soFloor->SetDynamical(false);
	
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
		meshBlock = DCAST(CDConvexMeshIf, sdk->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 20;		
			md.vertices[i].z *= 10;
		}
		meshFloor = DCAST(CDConvexMeshIf, sdk->CreateShape(md));
	}

	soFloor->AddShape(meshFloor);
	for(int i = 0; i < NUM_BLOCK; i++)
		soBlock[i]->AddShape(meshBlock);
	
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	for(int i = 0; i < NUM_BLOCK; i++)
		soBlock[i]->SetFramePosition(*(Vec3f*)&boxpos[i]);
//	soBlock[NUM_BLOCK-1]->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("PHConstraintTest");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
}
