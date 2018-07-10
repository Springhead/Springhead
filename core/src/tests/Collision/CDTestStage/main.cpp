/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Collision/CDTestStage/main.cpp 

【概要】
  CCDのテストプログラム。
  判定手法，オブジェクトを切り替えてテストできる
  左上に現在の衝突時間が表示される
  colcounterはサポート探索回数

【終了基準】
  強制終了。 

【操作】
3,4,5　衝突判定メソッド切り替え（3=Springhead，4＝加速，5＝GJKRaycast）
1,2 操作オブジェクト切り替え
z,x,c,v,b,n 形状切り替え
w,a,s,d,q,e オブジェクト移動
t,g,f,h オブジェクト回転
p　csv定義の組み合わせでの計測開始
l　今の組み合わせで計測開始
o　今の衝突情報を記録
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <Collision/CDDetectorImp.h>
//#include <Physics/SprPHEngine.h>
#include <ctime>
#include <string>
#include <numeric>
//#include <GL/glut.h>
#include<Foundation/UTQPTimer.h>
#include <iostream>
#include <fstream>
#include <tests/Collision/CDTestStage/teststage.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC					27			// ESC key
#define LEFT				37			//←
#define UP					38			//↑
#define RIGHT				39			//→
#define DOWN				40			//↓
#define STAY_COUNTER		300			// 静止判定カウント
#define COLTIME_AVE_FRAME 10	//衝突判定時間の平均を何フレームごとに出すか

const double epsilon = 1e-16;	//1e-8
const float testHeight = 10;
const float moverate = 0.05f;
const float rotaterate = (float) M_PI/180; //1°刻み

bool automode = false;
bool superAuto = false; //csv分全部通しでやるときにはtrue

UTRef<PHSdkIf> sdk;
TestStage stage;
PHSceneIf* scene;
TestObj obj[2];
ShapeID idFloor = ShapeID::SHAPE_BOX;
ShapeID idBlock = ShapeID::SHAPE_SPHERE;
int selectObj = 0;
int colMethod = 0;
int coltimeDisp[3];
double colCountAve = 0;
int aveCount = 0;
Vec3d cameraPos = Vec3d(0,3.0,9.0);
double camTheta;
double camPhi;

//record
string filename;
string hitFilename;
int rotateCount = 0;
int transCount = 0;
double hitTimePool[3] = { 0,0,0 };
double outTimePool[3] = { 0,0,0 };
double colCountHit = 0;
double colCountOut = 0;
int hitCount = 0;
int outCount = 0;
bool recordHit=false;

vector<int> testShapes;
const string testCSVPath = "testsetting.csv";
int caseCount = 0;

Vec2d lastMouse;

UTLongLong& coltimePhase1 = UTPerformanceMeasure::GetInstance("Collision")->Count("P1");
UTLongLong& coltimePhase2 = UTPerformanceMeasure::GetInstance("Collision")->Count("P2");
UTLongLong& coltimePhase3 = UTPerformanceMeasure::GetInstance("Collision")->Count("P3");

namespace Spr {
	extern int		colcounter;
	extern double	biasParam;
}

// 光源の設定 
static GLfloat light_position[] = { 15.0, 30.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

enum mouseMode {
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};
mouseMode mouseButton;

void __cdecl camera(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vec3d camRot;
	camRot.z = cos(camTheta)*cos(camPhi);
	camRot.y = sin(camPhi);
	camRot.x = sin(camTheta)*cos(camPhi);
	Vec3d newPos = cameraPos - camRot;
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
		newPos.x, newPos.y, newPos.z,
		0, 1 , 0);
}

void SetFileName() {
	time_t timer;
	time(&timer);
	tm lTimer;
#ifdef _MSC_VER
	localtime_s(&lTimer, &timer);	// この引数並びは正しい？
#else
	localtime_r(&timer, &lTimer);
#endif
	filename = "CDTest_" + to_string(lTimer.tm_mon + 1) + to_string(lTimer.tm_mday) + to_string(lTimer.tm_hour) + to_string(lTimer.tm_min) + ".csv";
	hitFilename = "CDHit_" + to_string(lTimer.tm_mon + 1) + to_string(lTimer.tm_mday) + to_string(lTimer.tm_hour) + to_string(lTimer.tm_min) + ".csv";
}

void StartAutomode(bool sameFile) {
	automode = true;
	rotateCount = 0;
	transCount = 0;
	memset(hitTimePool, 0, sizeof(hitTimePool));
	memset(outTimePool, 0, sizeof(outTimePool));
	hitCount = 0;
	outCount = 0;
	colCountHit = 0;
	colCountOut = 0;
	if(!sameFile) SetFileName();
	Quaternionf quat = Quaternionf();
	obj[0].SetRot(quat);
	obj[0].SetPos(Vec3f(-3, testHeight, -3));
	if (!superAuto) {
		ofstream ofs(filename, ios::app);
		ofs << colMethod << "," << obj[0].m_shapeID << "," << obj[1].m_shapeID << "," << biasParam << std::endl;
	}
}

Vec3d hitPos, hitPos2;
Vec3d vecPos, vecPos2;
int res;
double dist;

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param		なし
 return 	なし
 */
void __cdecl display(){
//	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera();
	PHSolidIf* solid[2] = { obj[1].GetSolid(), obj[0].GetSolid() };

	glEnable(GL_LIGHTING);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 材質の設定
	static GLfloat mat_floor[]      = { 1.0f, 0.7f, 0.7f, 1.0f };
	static GLfloat mat_block[]      = { 0.7f, 0.7f, 1.0f, 1.0f };
	static GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat mat_shininess[]  = { 120.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	Affined ad;
	
	// 下の赤い剛体(objB)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_floor);
	glPushMatrix();
	Posed pose = solid[0]->GetPose();

	Vec3f normal;
	for(int i=0; i<solid[0]->NShape(); ++i){
		SetGLMesh(solid[0]->GetShape(i), obj[1].m_shapeID, pose);
	}

	
	// 上の青い剛体(objA)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_block);
	pose = solid[1]->GetPose();
	for(int i=0; i<solid[1]->NShape(); ++i){
		SetGLMesh(solid[1]->GetShape(i), obj[0].m_shapeID, pose);
	}

	
	//衝突情報描画
	glDisable(GL_ALPHA);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);


	BeginRend2D();
	RendText(0, 0.95f, (char*)"res:%d normal:%.3f,%.3f,%.3f deist:%.4f", res, normal.x, normal.y, normal.z, dist);
	RendText(0, 0.9f, (char*)"coltime phase1:%d phase2:%d phase3:%d phase3 loop:%d", coltimeDisp[0], coltimeDisp[1], coltimeDisp[2], colcounter);
	RendText(0, 0.85f, (char*)"bias param:%.2f", biasParam);
	glPointSize(5.0);					// 点の太さ
	glBegin(GL_POINTS);					// 点の座標を記述開始
	glColor4f(0.3f, 1.0f, 0.3f, 0);	// 点の色(RGBA)
	glVertex2d(hitPos.X(), hitPos.Y());			// 点2つ分の座標
	glVertex2d(hitPos2.X(), hitPos2.Y());
	glEnd();							// 座標の記述終了
	glLineWidth(3.0);

	glBegin(GL_LINES);		//線
	glColor4f(0.3f, 0.3f, 1.0f, 0);	// 線の色(RGBA)
	glVertex2d(vecPos.X(), vecPos.Y());			// 線の座標
	glVertex2d(vecPos2.X(), vecPos2.Y());
	glEnd();

	EndRend2D();


	glutSwapBuffers();
}

int colcountAcc = 0;
void collisionTest() {
	CDConvex* mesh[2];
	Posed pose[2];
	PHSolidIf* solid[2] = { obj[1].GetSolid(), obj[0].GetSolid() };
	for (int i = 0; i<2; ++i) {
		mesh[i] = DCAST(CDConvex, solid[i]->GetShape(0));
		pose[i] = solid[i]->GetPose();
	}
	Vec3d normal;
	Vec3d pos[2];
	dist = 0;
	Vec3d dir(0, -1, 0);
	double dirLength = testHeight * 2;
	res = 0;
	colcounter = 0;
	float maxSurf = mesh[0]->GetMaxSurf();
	switch (colMethod)
	{
	case 0:
		res = ContFindCommonPointGino(mesh[0], mesh[1], pose[0], pose[1], dir, -DBL_MAX, dirLength, normal, pos[0], pos[1], dist);
		break;
	case 1:
		res = ContFindCommonPointAccel(mesh[0], mesh[1], pose[0], pose[1], dir, -DBL_MAX, dirLength, normal, pos[0], pos[1], dist);
		break;
	case 2:
		res = ContFindCommonPoint(mesh[0], mesh[1], pose[0], pose[1], dir, -DBL_MAX, dirLength, normal, pos[0], pos[1], dist);
		break;
	default:
		assert("Not selected collision method");
		break;
	}
	colcountAcc += colcounter;

	//coltimeの平均化
	if (aveCount >= COLTIME_AVE_FRAME) {
		coltimeDisp[0] = (int)((double)coltimePhase1 / (double)COLTIME_AVE_FRAME);
		coltimeDisp[1] = (int)((double)coltimePhase2 / (double)COLTIME_AVE_FRAME);
		coltimeDisp[2] = (int)((double)coltimePhase3 / (double)COLTIME_AVE_FRAME);
		colCountAve = (double)colcountAcc / (double)COLTIME_AVE_FRAME;
		colcountAcc = 0;
		aveCount = 0;
		coltimePhase1 = 0;
		coltimePhase2 = 0;
		coltimePhase3 = 0;
		//自動テスト時の処理
		if (automode) {
			//衝突してるかどうかで分ける
			if (res == 1) {
				for (int i = 0; i < 3; ++i) {
					hitTimePool[i] += coltimeDisp[i];
				}
				colCountHit += colCountAve;
				hitCount++;
			}
			else {
				for (int i = 0; i < 3; ++i) {
					outTimePool[i] += coltimeDisp[i];
				}
				colCountOut += colCountAve;
				outCount++;
			}
			rotateCount++;
			obj[0].Rotate(Quaternionf::Rot(rotaterate * 15, Vec3f(1, 0, 0)));
			if (rotateCount >= 24) { //回転が終わったら
				rotateCount = 0;
				transCount++;
				obj[0].SetRot(Quaternionf());
				obj[0].SetPos(Vec3f(transCount % 6 - 3, testHeight, transCount / 6 - 3)); //6*6のグリッドを移動
				if (!superAuto) { //一回だけならここで記録
					ofstream ofs(filename, ios::app);
					if (hitCount > 0) {
						for (int i = 0; i < 3; ++i) {
							hitTimePool[i] /= hitCount;
						}
						colCountHit /= hitCount;
					}
					if (outCount > 0) {
						for (int i = 0; i < 3; ++i) {
							outTimePool[i] /= outCount;
						}
						colCountOut /= outCount;
					}
					ofs << hitTimePool << "," << std::flush;
					if (transCount % 6 == 0) ofs << std::endl;
					hitCount = 0;
					outCount = 0;
					assert(0);
					memset(hitTimePool, 0, sizeof(hitTimePool));
					memset(outTimePool, 0, sizeof(outTimePool));
					colCountHit = 0;
					colCountOut = 0;
				}
				if (transCount >= 36) { //全グリッド測ったら
					ofstream ofs(filename, ios::app);
					if (hitCount > 0) {
						for (int i = 0; i < 3; ++i) {
							hitTimePool[i] /= hitCount;
						}
						colCountHit /= hitCount;
					}
					if (outCount > 0) {
						for (int i = 0; i < 3; ++i) {
							outTimePool[i] /= outCount;
						}
						colCountOut /= outCount;
					}
					if (colMethod == 0) {
						ofs << obj[0].m_shape->GetName() << "-" << obj[1].m_shape->GetName();
						ofs << "," << obj[0].m_shapeID << "," << obj[1].m_shapeID << ",";
						for (int i = 0; i < 3; ++i) ofs << hitTimePool[i]  << ",";
						for (int i = 0; i < 3; ++i) ofs << outTimePool[i] << ",";
						ofs << hitCount << "," << outCount << ",";
						ofs << std::accumulate(hitTimePool, hitTimePool + 3, 0.0) << ",";
						ofs << std::accumulate(outTimePool, outTimePool + 3, 0.0) << ",";
						ofs << colCountHit << "," << colCountOut;
					}
					else {
						ofs << ",,";
						for (int i = 0; i < 3; ++i) ofs << hitTimePool[i] << ",";
						for (int i = 0; i < 3; ++i) ofs << outTimePool[i] << ",";
						ofs << hitCount << "," << outCount << ",";
						ofs << std::accumulate(hitTimePool, hitTimePool + 3, 0.0) << ",";
						ofs << std::accumulate(outTimePool, outTimePool + 3, 0.0) << ",";
						ofs << colCountHit << "," << colCountOut << std::endl;
					}
					automode = false;
					if (superAuto && caseCount < testShapes.size()) { //形状切り替え，終了判定
						if (colMethod == 2) {//次の形状へ
							caseCount += 2;
							obj[0].SetShape(stage.GetShape((ShapeID)testShapes[caseCount]), (ShapeID)testShapes[caseCount]);
							obj[1].SetShape(stage.GetShape((ShapeID)testShapes[caseCount + 1]), (ShapeID)testShapes[caseCount + 1]);
						}
						colMethod = colMethod + 2; //メソッド切り替え
						if (colMethod >= 3) colMethod = 0;
						StartAutomode(true);
					}
					else { //終了
						if (superAuto) exit(0);
						automode = false;
						superAuto = false;
					}
				}
			}
		}
	}
	else
	{
		aveCount++;
	}
	//VSのログに出すとき
	//DSTR << "res:" << res << " normal:" << normal << " dist:" << dist;
	//DSTR << " p:" << pose[0] * pos[0] << " q:" << pose[1] * pos[1] << std::endl;
	hitPos = ObjtoScreenPos(pose[0] * pos[0]);
	hitPos2 = ObjtoScreenPos(pose[1] * pos[1]);
	vecPos = hitPos2;
	vecPos2 = ObjtoScreenPos(pose[1] * pos[1] + dir*dirLength);
	if (recordHit)
	{
		ofstream ofs(hitFilename, ios::app);
		ofs << colMethod << "," << res << "," << normal.x << "," << normal.y << "," << normal.z << "," << dist << ","
			<< obj[0].m_position.x << "," << obj[0].m_position.y << "," << obj[0].m_position.z << ","
			<< obj[1].m_position.x << "," << obj[1].m_position.y << "," << obj[1].m_position.z << ","
			<< coltimeDisp[0] << "," << coltimeDisp[1] << "," << coltimeDisp[2] << "," << coltimeDisp[0] + coltimeDisp[1] + coltimeDisp[2] << "," << colcounter << std::endl;
		recordHit = false;
	}
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

	gluLookAt(0.0, 3.0, 9.0, 
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
 return		 なし
 */
void __cdecl reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	width = (float) w;
	height = (float) h;
}

void __cdecl RecordHit()
{
	SetFileName();
	recordHit = true;
}


/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void __cdecl keyboard(unsigned char key, int x, int y){
	if (key == ESC) exit(0);
	if (key == '1') selectObj = 0;
	if (key == '2') selectObj = 1;
	if (key == '3') colMethod = 0;
	if (key == '4') colMethod = 1;
	if (key == '5') colMethod = 2;
	if (key == 'w') obj[selectObj].Translate(Vec3f(0, moverate, 0));
	if (key == 'a') obj[selectObj].Translate(Vec3f(-moverate, 0, 0));
	if (key == 's') obj[selectObj].Translate(Vec3f(0, -moverate, 0));
	if (key == 'd') obj[selectObj].Translate(Vec3f(moverate, 0, 0));
	if (key == 'e') obj[selectObj].Translate(Vec3f(0, 0, moverate));
	if (key == 'q') obj[selectObj].Translate(Vec3f(0, 0, -moverate));
	if (key == 't') obj[selectObj].Rotate(Quaternionf::Rot(rotaterate, Vec3f(1,0,0)));
	if (key == 'g') obj[selectObj].Rotate(Quaternionf::Rot(-rotaterate, Vec3f(1, 0, 0)));
	if (key == 'f') obj[selectObj].Rotate(Quaternionf::Rot(rotaterate, Vec3f(0, 0, 1)));
	if (key == 'h') obj[selectObj].Rotate(Quaternionf::Rot(-rotaterate, Vec3f(0, 0, 1)));
	if (key == 'z') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_BOX), ShapeID::SHAPE_BOX);
	if (key == 'x') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_SPHERE), ShapeID::SHAPE_SPHERE);
	if (key == 'c') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_CAPSULE), ShapeID::SHAPE_CAPSULE);
	if (key == 'v') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_ROUNDCONE), ShapeID::SHAPE_ROUNDCONE);
	if (key == 'b') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_POLYSPHERE), ShapeID::SHAPE_POLYSPHERE);
	if (key == 'n') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_DODECA), ShapeID::SHAPE_DODECA);
	if (key == 'm') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_LONGCAPSULE), ShapeID::SHAPE_LONGCAPSULE);
	if (key == ',') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_LONGPOLYSPHERE), ShapeID::SHAPE_LONGPOLYSPHERE);
	if (key == '.') obj[selectObj].SetShape(stage.GetShape(ShapeID::SHAPE_ELLIPSOID), ShapeID::SHAPE_ELLIPSOID);
	if (key == 'r') {
		static int count = 0;
		if (count == 0) {
			testShapes = LoadTestCSV(testCSVPath);
			count = 0;
			obj[0].SetPos(Vec3d(100, 0, 0));
			obj[1].SetPos(Vec3d(0, 3, 0));
			obj[1].SetRot(Quaterniond::Rot(Rad(-60), 'y'));
		}
		obj[1].SetShape(stage.GetShape((ShapeID)testShapes[count]), (ShapeID)testShapes[count]);
		count++;
		if (count >= testShapes.size()) count = 0;
	}
	if (key == 'p') {
			colMethod = 0;
		caseCount = 0;
		superAuto = true;
		StartAutomode(false);
		testShapes = LoadTestCSV(testCSVPath);
		obj[0].SetShape(stage.GetShape((ShapeID)testShapes[caseCount]), (ShapeID)testShapes[caseCount]);
		obj[1].SetShape(stage.GetShape((ShapeID)testShapes[caseCount + 1]), (ShapeID)testShapes[caseCount + 1]);
	}
	if (key == 'l') StartAutomode(false);
	if (key == 'o') RecordHit();
	if (key == '9') biasParam -= 0.1;
	if (key == '0') biasParam += 0.1;
}	

void __cdecl mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			mouseButton = mouseMode::MOUSE_LEFT;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			mouseButton = mouseMode::MOUSE_RIGHT;
		}
		break;
	default:
		break;
	}
	lastMouse = Vec2d(x, y);
}

void __cdecl motion(int x, int y) {
	Vec2d delta = Vec2d(x, y) - lastMouse;
	switch (mouseButton)
	{
	case mouseMode::MOUSE_LEFT:
			camTheta += delta.x*0.001;
			camPhi -= delta.y*0.001;
		break;
	case mouseMode::MOUSE_RIGHT:
			cameraPos.x += (delta.x*cos(camTheta) +delta.y*sin(camPhi)*sin(camTheta))*0.01;
			cameraPos.y += delta.y*cos(camPhi)*0.01;
			cameraPos.z -= (delta.x*sin(camTheta)+delta.y*sin(camPhi)*cos(camTheta))*0.01;
		break;
	default:
		break;
	}
	lastMouse = Vec2d(x, y);
}



/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl idle(){
	if (automode || superAuto) {
		collisionTest();
	}
	else {
		glutPostRedisplay();
	}
}

/**
 brief 		多面体の面(三角形)の頂点座標をデバッグ出力させる。
 param 		<in/--> solidID　　 solidのID
 return 	なし
 */
void dstrSolid(const std::string& solidName) {
	PHSolidIf* solid = NULL;
	if (solidName == "objB")			solid = obj[1].GetSolid();
	else if (solidName == "objA")	solid = obj[0].GetSolid();
	std::cout << "***  " << solidName << "   ***\n";

	for(int i=0; i<solid->NShape(); ++i){
		CDShapeIf* shape = solid->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		if (mesh){
			Vec3f* base = mesh->GetVertices();
			for(int f=0; f<mesh->NFace();++f){
				CDFaceIf* face = mesh->GetFace(f);
				for(int v=0; v<face->NIndex(); ++v){
					std::cout << base[face->GetIndices()[v]];
				}
				std::cout << std::endl;
			}
		}
		CDSphereIf* sphere = DCAST(CDSphereIf, shape);
		if (sphere){
			std::cout << "radius: " << sphere->GetRadius() << std::endl;
		}
		std::cout << "mass: " << solid->GetMass() << std::endl;
		std::cout << "inertia: " << std::endl << solid->GetInertia() << std::endl;
	}
}


namespace Spr {
	void setGjkThreshold(double th, double e);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int __cdecl main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();					// SDKの作成　
	scene = sdk->CreateScene();				// シーンの作成
	stage = TestStage();
	stage.Init(sdk);

	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;

	obj[0].Init(scene->CreateSolid(desc), stage.GetShape(idBlock), idBlock);
	obj[1].Init(scene->CreateSolid(desc), stage.GetShape(idFloor), idFloor);
	
	setGjkThreshold(1e-6, 1e-6);

	
	//	形状の作成
#if 0
	CDConvexMeshIf* meshBlock;
	CDConvexMeshIf* meshFloor;
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

		// objB(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= sqrt(2.0);
			md.vertices[i].z *= sqrt(2.0);
		}
		meshFloor = DCAST(CDConvexMeshIf, sdk->CreateShape(md));
	}
	objA->AddShape(meshBlock);
	objB->SetFramePosition(Vec3f(0,-1,0));
	objB->SetOrientation( Quaternionf::Rot(Rad(30), 'x') );
	objA->SetFramePosition(Vec3f(-0.5,5,0));
#else
	CDSphereDesc sd;
	sd.radius = 1.0;
	CDSphereIf* sphere = DCAST(CDSphereIf, sdk->CreateShape(sd));

	obj[1].SetPos(Vec3f(0,-1,0));
	obj[0].SetPos(Vec3f(0, 0.6,0));
#endif

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	if (automode) {
		SetFileName();
	}

	// デバッグ出力
	dstrSolid("objB");
	dstrSolid("objA");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize((int)width, (int)height);
	glutCreateWindow("PHShapeGL");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
}
