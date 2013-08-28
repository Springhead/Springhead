/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Collision/ContGJK/main.cpp 

【概要】
  CCDGJKのテストプログラム。
  
【終了基準】
  強制終了。 
 
 */

#include <Springhead.h>		//	Springheadのインタフェース
#include <Collision/CDDetectorImp.h>
#include <ctime>
#include <string>
#include <GL/glut.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;

#define ESC					27			// ESC key
#define STAY_COUNTER		300			// 静止判定カウント
#define TOTAL_IDLE_COUNTER	10000		// 静止しない場合に利用


UTRef<PHSdkIf> sdk;
PHSceneIf* scene;
PHSolidIf* soFloor, *soBlock;

// 光源の設定 
static GLfloat light_position[] = { 15.0, 30.0, 20.0, 1.0 };
static GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; 
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

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
void __cdecl display(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glDisable(GL_ALPHA);
	glDisable(GL_BLEND);
	{
		PHSolidIf* solid[2] = {soFloor, soBlock};
		CDConvex* mesh[2];
		Posed pose[2];
		for(int i=0; i<2; ++i){
			mesh[i] = DCAST(CDConvex, solid[i]->GetShape(0));
			pose[i] = solid[i]->GetPose();
		}
		Vec3d normal;
		Vec3d pos[2];
		double dist=0;
		Vec3d dir(0,-1,1);
		int res = ContFindCommonPoint(mesh[0], mesh[1], pose[0], pose[1], dir, 0, 1, normal, pos[0], pos[1], dist);
		DSTR << "res:"  << res << " normal:" << normal << " dist:" << dist;
		DSTR << " p:" << pose[0]*pos[0] << " q:" << pose[1]*pos[1] << std::endl;
//		pose[1].Ori() = Quaterniond::Rot('z', Rad(5)) * pose[1].Ori();
//		solid[1]->SetPose(pose[1]);
	}

	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 材質の設定
	static GLfloat mat_floor[]      = { 1.0, 0.7, 0.7, 0.8 };
	static GLfloat mat_block[]      = { 0.7, 0.7, 1.0, 0.8 };
	static GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 0.8 };
	static GLfloat mat_shininess[]  = { 120.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	Affined ad;
	
	// 下の赤い剛体(soFloor)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_floor);
	glPushMatrix();
	Posed pose = soFloor->GetPose();
	pose.ToAffine(ad);
	glMultMatrixd(ad);	

	Vec3f normal;
	for(int i=0; i<soFloor->NShape(); ++i){
		CDShapeIf* shape = soFloor->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		if (mesh){
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
		CDSphereIf* sphere = DCAST(CDSphereIf, shape);
		if (sphere){
			glutSolidSphere(sphere->GetRadius(), 8, 8);
		}
	}
	glPopMatrix();

	
	// 上の青い剛体(soBlock)
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_block);
	glPushMatrix();
	pose = soBlock->GetPose();
	ad = Affined(pose);
	glMultMatrixd(ad);
		for(int i=0; i<soBlock->NShape(); ++i){
		CDShapeIf* shape = soBlock->GetShape(i);
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		if (mesh){
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
		CDSphereIf* sphere = DCAST(CDSphereIf, shape);
		if (sphere){
			glutSolidSphere(sphere->GetRadius(), 8, 8);
		}
	}
	glPopMatrix();
	
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
	if (key == 'q') exit(0);
}	

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void __cdecl idle(){
	static int total;
	total ++;
	if (total > TOTAL_IDLE_COUNTER){
		//exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
#if 0
	Vec3d prepos, curpos;	// position
	prepos = soBlock->GetFramePosition();

	scene->Step();

	curpos = soBlock->GetFramePosition();

	static int total=0;
	static int stay=0;
	total++;
	if (total > TOTAL_IDLE_COUNTER){
		DSTR << "\nPHShapeGL failure." << std::endl;
		exit(EXIT_FAILURE);
	} else {
		if (approx(prepos, curpos)){
			stay++;
			if (stay > STAY_COUNTER){				// 静止判定カウント	
			DSTR << "\nPHShapeGL success." << std::endl;
				exit(EXIT_SUCCESS);
			}
		} else {
			stay = 0;
		}
	}
#endif
	glutPostRedisplay();
}

/**
 brief 		多面体の面(三角形)の頂点座標をデバッグ出力させる。
 param 		<in/--> solidID　　 solidのID
 return 	なし
 */
void dstrSolid(const std::string& solidName) {
	PHSolidIf* solid = NULL;
	if (solidName == "soFloor")			solid = soFloor;
	else if (solidName == "soBlock")	solid = soBlock;
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

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int __cdecl main(int argc, char* argv[]){
	sdk = PHSdkIf::CreateSdk();					// SDKの作成　
	scene = sdk->CreateScene();				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	soBlock = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	
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

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= sqrt(2.0);
			md.vertices[i].z *= sqrt(2.0);
		}
		meshFloor = DCAST(CDConvexMeshIf, sdk->CreateShape(md));
	}
	soBlock->AddShape(meshBlock);
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	soFloor->SetOrientation( Quaternionf::Rot(Rad(30), 'x') );
	soBlock->SetFramePosition(Vec3f(-0.5,5,0));
#else
	CDSphereDesc sd;
	sd.radius = 1.0;
	CDSphereIf* sphere = DCAST(CDSphereIf, sdk->CreateShape(sd));

	soFloor->AddShape(sphere);
	soBlock->AddShape(sphere);
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	soBlock->SetFramePosition(Vec3f(0, 0.6,0));
#endif

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定

	// デバッグ出力
	dstrSolid("soFloor");
	dstrSolid("soBlock");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("PHShapeGL");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
}
