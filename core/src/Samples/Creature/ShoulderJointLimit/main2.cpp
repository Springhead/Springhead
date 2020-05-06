#include <Springhead.h>
//
#include <stdlib.h>
//#include "TMatrix.h"//行列ライブラリをインクルードする。
#include <iostream>
//
#include <GL/glut.h>

using namespace PTM;//  行列クラスはPTM名前空間の中で宣言されている。

using namespace Spr;
UTRef< GRDebugRenderIf > grRender   = NULL;
UTRef< PHSceneIf >       phScene    = NULL;
UTRef< PHSolidIf >		 solid[5]	= {0,0,0,0,0};
UTRef< PHJoint1DIf > joint[2];

// カメラ位置
double CameraZoom = 30.0;
double CameraRotX =  0.0;
double CameraRotY =  4.6;
//関節の角度
double q1 = 90;
double q2 = 10;
//クリックした位置
double point[2][2];				// 座標を記憶する配列 
int w=800;//ウィンドウのサイズ
int h=600;


void Display() {
	// 視点の設定
	Affinef view;
	double yoffset = 0.0;
	view.Pos() = CameraZoom * Vec3f( cos(CameraRotX) * cos(CameraRotY),
									 sin(CameraRotX),
									 cos(CameraRotX) * sin(CameraRotY) );
	view.PosY() += yoffset;
	view.LookAtGL(Vec3f(0.0f, yoffset, 0.0f), Vec3f(0.0f, 100.0f, 0.0f));
	grRender->SetViewMatrix(view.inv());
//	glViewport(0, 0, w, h);

	grRender->ClearBuffer();
	grRender->DrawScene(phScene);
	grRender->EndScene();
	glutSwapBuffers();
}

void Timer(int id){
	double x,y,fai;
	glutTimerFunc(20, Timer,0);
	//シミュレーションのステップ
	phScene->ClearForce();
	//制御機構
	//中心位置、400,300// 400,255
	//point[1][0] = CameraZoom*(8*cos(Rad(-q1))+7*cos(Rad(-q2)))+400;
	//point[1][1] = -CameraZoom*(8*sin(Rad(-q1))+7*sin(Rad(-q2)))-255;
	//std::cout << point[0][0];
//
	point[1][0] = 8*cos(Rad(q1))+7*cos(Rad(q1+q2));//現在の位置
	point[1][1] = 8*sin(Rad(q1))+7*sin(Rad(q1+q2));
	
	fai= acos(point[0][0]/sqrt(point[0][0]*point[0][0]+point[0][1]*point[0][1]));
	TVector<2,double> dX;

	x = (point[0][0]-point[1][0])/200;//現在値と目標値の差
	y = (point[0][1]-point[1][1])/200;

	point[0][0] = point[1][0];
	point[0][1] = point[1][1];

	TVector<2,double> dQ; 
	TMatrixRow<2,2,double> jacobien;//2×2の行列を宣言


	for(int count=0;count<200;count++){
		point[1][0] = 8*cos(Rad(q1))+7*cos(Rad(q1+q2));//現在の位置の計算
		point[1][1] = 8*sin(Rad(q1))+7*sin(Rad(q1+q2));

		point[0][0] += x;
	    point[0][1] += y;

		dX[0] = point[0][0]-point[1][0];//現在値と目標値の差
		dX[1] = point[0][1]-point[1][1];

		jacobien[0][0] = -8*sin(Rad(q1))-7*sin(Rad(q1+q2)); jacobien[0][1] = -7*sin(Rad(q1+q2));
		jacobien[1][0] = 8*cos(Rad(q1))+7*cos(Rad(q1+q2));  jacobien[1][1] = 7*cos(Rad(q1+q2));

		dQ = jacobien.inv()*dX; //逆運動学モデルにより、角度の変化を求める
		//std::cout << jacobien;
		if(dQ[0]<-0&&-90<q1){ q1 = q1-1;}
		else if(dQ[0]>0&&q1<270){ q1 = q1+1;}
		if(dQ[1]<-0&&q2>-180){ q2 = q2-1;}
		else if(dQ[1]>0&&q2<180){ q2 = q2+1;}
	}
		printf("%f,%f\n",point[0][0],point[0][1]);
	    printf("%f,%f\n",q1,q2);
	
	printf("%f,%f\n",q1,q2);
	joint[0]->SetSpringOrigin(Rad(90.0-q1));
	joint[1]->SetSpringOrigin(Rad(-q2));
	// シミュレーションのステップの続き
	phScene->GenerateForce();
	phScene->Integrate();
	glutPostRedisplay();
}
void Reshape(int w, int h){
	grRender->Reshape(Vec2f(), Vec2f(w,h));

}


void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'x': 
			CameraRotX+=0.2;
			break;
		case 'y': 
			CameraRotY+=0.2;
			break;
		case '1':
			point[0][0]=14.0;
			point[0][1]=1.0;
			printf("%f,\n",point[0][0]);
			break;
		case '2':
			point[0][0]=1.0;
			point[0][1]=12.0;
			printf("%f,\n",point[0][0]);
			
			break;
		case '3':
			point[0][0]=-10.0;
			point[0][1]=5.0;
			printf("%f,\n",point[0][0]);
			
			break;
		case 'q':
		case 'Q':
		case '\033':  //\033はESCのASCII コード
		exit(0);
		default:
		break;
	}
}
void mouse(int button, int state, int x, int y){

	switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("Left");
			//クリックを押すとき
			if(state==GLUT_DOWN){
				point[0][0] = (x-w/2)/30.0;
				point[0][1] = (h/2-y)/30.0;
				printf("%f,%f\n",point[0][0],point[0][1]);
			//クリックが離れるとき
			}
			else{
				point[0][0] = (x-w/2)/20.0;
				point[0][1] = (h/2-y)/20.0;

			}
			break;
		case GLUT_MIDDLE_BUTTON:
			printf("Middle");
			break;
		case GLUT_RIGHT_BUTTON:
			printf("Right");
			break;
		default:
			break;
	}

	printf(" button is ");

	switch (state) {
		case GLUT_UP:
			printf("up");
			break;
		case GLUT_DOWN:
			printf("down");
			break;
		default:
			break;
	}
	solid[3]->SetCenterPosition(Vec3d(-point[0][0],point[0][1]+1.5,0.0));
	//printf(" at (%d, %d)\n", x, y);
}

int main(int argc,char* argv[]){
	point[0][0] = 8*cos(Rad(q1))+7*cos(Rad(q1+q2));//400;//
	point[0][1] = 8*sin(Rad(q1))+7*sin(Rad(q1+q2));//195;//
	//printf("%f,%f\n",point[0][0],point[0][1]);

	//SDKの生成
	UTRef< PHSdkIf > phSdk = PHSdkIf::CreateSdk();
	UTRef< GRSdkIf > grSdk = GRSdkIf::CreateSdk();
	
	//シーングラフの生成
	PHSceneDesc dScene;
	dScene.timeStep = 0.05;		//積分時間幅
	dScene.numIteration = 20;	//繰り返し計算回数
	phScene = phSdk->CreateScene(dScene);

	//デスクリプタを用いて剛体オブジェクトを生成
	PHSolidDesc sDesc;
	PHSolidDesc dSolid0;
	//剛体を空中に固定する
	dSolid0.dynamical = false;

	//UTRef< PHSolidIf > solid[3];
	solid[0] = phScene->CreateSolid(dSolid0);
	solid[1] = phScene->CreateSolid(sDesc);
	solid[2] = phScene->CreateSolid(sDesc);
	solid[3] = phScene->CreateSolid(sDesc);

	//デスクリプタを用いて箱形状オブジェクトを生成
	CDBoxDesc bDesc;
	CDBoxDesc sBase;
	CDSphereDesc sPoint;
	bDesc.boxsize = Vec3f(1.0f,6.0f,1.0f);		//Vec3f(縦，横，高さ)を書き換えている
	sBase.boxsize = Vec3f(1.0f,1.0f,1.0f);
	sPoint.radius = 0.5;

	UTRef< CDShapeIf > shapeBox = phSdk->CreateShape(bDesc);
	UTRef< CDShapeIf > BaseBox  = phSdk->CreateShape(sBase);
	UTRef< CDShapeIf > shapePoint  = phSdk->CreateShape(sPoint);
	//剛体オブジェクトに箱オブジェクトを割り当てる
	solid[0]->AddShape(BaseBox);
	solid[1]->AddShape(shapeBox);
	solid[2]->AddShape(shapeBox);
	solid[3]->AddShape(shapePoint);
	phScene->SetContactMode(solid[1],solid[2],PHSceneDesc::MODE_NONE);
	phScene->SetContactMode(solid[3],PHSceneDesc::MODE_NONE);

	solid[3]->SetGravity(false);
	solid[3]->SetCenterPosition(Vec3d(-point[0][0],point[0][1]+1.5,0.0));
	//solid[3]->SetPose(Vec3f(point[0][0],point[0][1],0.0));
	//デスクリプタと2つの剛体を用いて関節を生成
	PHHingeJointDesc hDesc;
	hDesc.poseSocket.Pos() = Vec3d(0.0f, 2.0f, 0.0f);
	//hDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90.0f),'x');
	hDesc.posePlug.Pos()   = Vec3d(0.0f, -4.0f, 0.0f);
	//hDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-q1),'x');
	hDesc.spring		   = 1000;
	hDesc.damper		   = 100;
	joint[0] = (PHJoint1DIf*) phScene->CreateJoint(solid[0], solid[1], hDesc);

	//デスクリプタと2つの剛体を用いて関節を生成
	hDesc.poseSocket.Pos() = Vec3d(0.0f, 4.0f, 0.0f);
	//hDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90.0f),'x');
	hDesc.posePlug.Pos()   = Vec3d(0.0f, -4.0f, 0.0f);
	//hDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(-180+q1-q2),'x');
	hDesc.spring		   = 1000;
	hDesc.damper		   = 100;
	joint[1] = (PHJoint1DIf*) phScene->CreateJoint(solid[1], solid[2], hDesc);

	//GL関係
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w,h);
	int window = glutCreateWindow("Springhead Application");

	grRender = grSdk->CreateDebugRender();
	UTRef< GRDeviceGLIf > grDevice = grSdk->CreateDeviceGL();
	grDevice -> Init();
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(20,Timer,0);
	glutMouseFunc(mouse);

	grRender->SetDevice(grDevice);//Springhead2の描画機能にOpenGLの使用登録をする。

	// 照明の設定
	GRLightDesc light0, light1;
	light0.position = Vec4f(10.0, 20.0, 20.0, 1.0);
	light1.position = Vec4f(-10.0, 10.0, 10.0, 1.0);
	grRender->PushLight(light0);
	grRender->PushLight(light1);

	glutMainLoop();

	return 0;
}