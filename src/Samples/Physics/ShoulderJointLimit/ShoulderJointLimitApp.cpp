#include <windows.h>
#include <Springhead.h>
#include <HumanInterface/SprHIDRUsb.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <fstream>
#include "ShoulderJointLimitApp.h"
std::fstream Data("data/posnew.txt",std::ios::in); //読み込みでファイルを開く

MyApp app;

MyApp::MyApp(){
	//初期化
	Floor		= 0;
	bLeftButton = false, bRightButton = false;
	CameraZoom = 1.9;
	CameraRotX =  0.0;
	CameraRotY =  -2.50;
	cam = Vec3f(0.0,0.0,0.0);
	Step = true;

	count[jointsNum]=0;
	count2 = 0;
	Save = false;
	OneStep = false;
	Start = false;
	twistview = false;
	St = 0;
	cal = Vec3d(0,0,0);
	count[jointsNum] = 0;
//	spidar = false;
	jointsNum = 2;
	//文字の描画
	tn = 0;
}

bool MyApp::LoadPos(char* filename){
	double AA;
	if(Data.eof()){
		std::cout<< "fin" << std::endl;
	}
	else{
		Data >> position[0] >> position[1] >> position[2] >> AA;
		DSTR << position << std::endl;
		position /= 700;//剛体の中心は100になるはず
		AA = position[0] * position[0] + position[2]*position[2];
		position[0] = -position[0];// / AA;
		position[2] = position[2];// / AA;
		position += pospos;
	}
	return true;
}

void MyApp::Init(int argc, char* argv[]){
	//SDKの生成
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter( &begin );

	//シーングラフの生成
	PHSceneDesc dScene;
	dScene.timeStep = 0.05;		//積分時間幅
	dScene.numIteration = 20;	//繰り返し計算回数
	phScene = phSdk->CreateScene(dScene);

	CreateObj();



	//GL関係	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(800,0);

	grRender = grSdk->CreateRender();
	grDevice = grSdk->CreateDeviceGL();
	grDevice -> Init();

	fwSdk = FWSdkIf::CreateSdk();
	fwScene = fwSdk->CreateScene();
	fwScene->SetPHScene(phScene);
}

void MyApp::SetWindow1(){
	glutInitWindowSize(600,400);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	Win1 = glutCreateWindow("Test");
}
void MyApp::SetWindow2(){
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(300, 300);
	glEnable(GL_DEPTH_TEST);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	Win2 = glutCreateWindow("jointlimit");

//	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void MyApp::Display() {
	// 視点の設定
	Affinef view;
	view.Pos() = CameraZoom * Vec3f( cos(CameraRotX) * cos(CameraRotY),
									 sin(CameraRotX),
									 cos(CameraRotX) * sin(CameraRotY) ) + cam;
	view.LookAtGL(Vec3f(cam), Vec3f(0.0f, 100.0f, 0));

	grRender->SetViewMatrix(view.inv());
	grRender->ClearBuffer();
	fwScene->DrawPHScene(grRender);	
	grRender->EndScene();
	Constline();

	glEnd();
	glutSwapBuffers();
}

void MyApp::Display2() {
	static int r = 10;
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(!twistview){
		gluLookAt(0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glRotated(0.0, 0.0, 0.0, 1.0);
	}
	else{
		gluLookAt(0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glRotated((double)r, 0.0, 0.0, 1.0);
	}

	Vec2d pa;
	glLineWidth(2);
	glColor3d(0,0,0);
	//円
	if(!twistview){
		double rad = 90.0;
		glBegin(GL_LINES);
		glColor3d(0.8,0.8,0.8);
		glLineWidth(1);

		for(int j=0;j < 3;j++){
			glVertex3d(rad * cos(Rad(30.0*j)),rad * sin(Rad(30.0*j)),0.0);
			glVertex3d(-rad * cos(Rad(30.0*j)),-rad * sin(Rad(30.0*j)),0.0);
			glVertex3d(rad * cos(Rad(30.0*j)),-rad * sin(Rad(30.0*j)),0.0);
			glVertex3d(-rad * cos(Rad(30.0*j)),rad * sin(Rad(30.0*j)),0.0);
		}	glLineWidth(2);
		glEnd();
		rad = 90.0;

		for(int j=0;j<6;j++){
			glBegin(GL_LINE_LOOP);
			for(int i=0;i<18;i++){
				pa.x = rad * cos(Rad(20*i));
				pa.y = rad * sin(Rad(20*i));
				glVertex3d(pa.x,pa.y,0.0);
			}
			glEnd();
			rad -= 15.0;
			glLineWidth(1);
			glColor3d(0.5,0.5,0.5);
		}
	}
	Vec3d A;
	A = joints[jointsNum]->GetAngle();
	Vec2d para;
	para[0] = (Deg(A.x))/180.0 * 90.0 * cos(A.y);
	para[1] = (Deg(A.x))/180.0 * 90.0 * sin(A.y);
	aaaaa = Deg(A.x);
	HermiteSpline(para);
	//座標軸を書く
	glBegin(GL_LINES);
	glColor3d(0.8,0.8,0.8);
	glLineWidth(1);

	glLineWidth(2);
	glColor3d(0.0,0.0,0.0);
	glVertex3d(100,-0,0.0);
	glVertex3d(-100,-0,0.0);
	glVertex3d(-0,-100,0.0);
	glVertex3d(-0,100,0.0);
	glVertex3d(-0,0,-100.0);
	glVertex3d(-0,0,100.0);
	glEnd();
	//円
	if(!twistview){
		glColor3d(0,0,0);
		glBegin(GL_LINE_LOOP);
		for(int i=0;i<18;i++){
			pa.x = 90 * cos(Rad(20*i));
			pa.y = 90 * sin(Rad(20*i));
			glVertex3d(pa.x,pa.y,0.0);
		}
		glEnd();
	}
	//いらないカモ
	//if(twistview){
	//	double rad = 90.0;
	//	glBegin(GL_LINES);
	//	glColor3d(0.8,0.8,0.8);
	//	glLineWidth(1);

	//	for(int j=0;j < 3;j++){
	//		glVertex3d(rad * cos(Rad(30.0*j)),rad * sin(Rad(30.0*j)),0.0);
	//		glVertex3d(-rad * cos(Rad(30.0*j)),-rad * sin(Rad(30.0*j)),0.0);
	//		glVertex3d(rad * cos(Rad(30.0*j)),-rad * sin(Rad(30.0*j)),0.0);
	//		glVertex3d(-rad * cos(Rad(30.0*j)),rad * sin(Rad(30.0*j)),0.0);
	//	}	glLineWidth(2);
	//	glEnd();
	//	rad = 90.0;

	//	for(int j=0;j<6;j++){
	//		glBegin(GL_LINE_LOOP);
	//		for(int i=0;i<18;i++){
	//			pa.x = rad * cos(Rad(20*i));
	//			pa.y = rad * sin(Rad(20*i));
	//			glVertex3d(pa.x,pa.y,0.0);
	//		}
	//		glEnd();
	//		rad -= 15.0;
	//		glLineWidth(1);
	//		glColor3d(0.5,0.5,0.5);
	//	}
	//}

	glTranslated(para[0], -para[1], 0.0);
//	glTranslated(para[0], -para[1], Deg(A.z)/2);
	//SwingDir,Swing.Twist
	glColor3d(0,0,1);
	glutSolidSphere(3, 10, 10);
	glEnd();
//	char t_char[20];
//	sprintf_s(t_char, "twist:%.1f", Deg(A.z));//文字列の代入
//	DISPLAY_TEXT(0, 95, t_char  );
	glutSwapBuffers();

	////文字の描画
	//char t_char[20];
	//char t_char2[20];
	//sprintf_s(t_char2, "%d:", minute);//文字列の代入
	//strcat_s(t_char2, t_char);//文字列の合成
}

void MyApp::Reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void MyApp::Reshape2(int w, int h){
	width = w;
	hight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100, 100, 100, -100 ,-100,100);
	glMatrixMode(GL_MODELVIEW);
}

void MyApp::Timer(int id){
	//シミュレーションのステップ
	phScene->ClearForce();
	//制御機構
	// シミュレーションのステップの続き
	if(Step)
	{
//		if(spidar){
			//SPIDAR
//			spg->Update(0.001f);
//			Vec3f spgpos = spg->GetPosition();
//			Vec3d center;
//			center = (spgpos - cal);
//			Pointer[0]->SetCenterPosition(center);
//		}
		phScene->GenerateForce();
		phScene->Integrate();
		if(OneStep){
			OneStep = false;
			Step = false;
		}
		if(abc){
			Pointer[0]->SetCenterPosition(position);
		}
		Pointer[0]->SetVelocity(Vec3d(0.0,0.0,0.0));
	}
}

void MyApp::Keyboard(unsigned char key, int x, int y){
	char* fileName;
	static double vel = 1;
	Vec3f spgpos = Vec3f(0.0f,0.0f,0.0f);
	switch (key) {
		//ポインタの移動
		case 'a':
			Pointer[0]->SetVelocity(Vec3d(0.0,0.0,vel));
			break;
		case 's':
			Pointer[0]->SetVelocity(Vec3d(0.0,-vel,0.0));
			break;
		case 'd':
			Pointer[0]->SetVelocity(Vec3d(0.0,0.0,-vel));
			break;
		case 'w':
			Pointer[0]->SetVelocity(Vec3d(0.0,vel,0.0));
			break;
		case 'z':
			Pointer[0]->SetVelocity(Vec3d(vel,0.0,0.0));
			break;
		case 'e':
			Pointer[0]->SetCenterPosition(Vec3d(0.0,0.0,0.0));
			break;
		case 'x':
			Pointer[0]->SetVelocity(Vec3d(-vel,0.0,0.0));
			break;
		case ' ':
			Spring->Enable((!Spring->IsEnabled()));
			break;		
		//可動域の設定に使う
		case '+':
			if(points[0][1] < 175){
				points[0][1] += 5;
				points[count[jointsNum]-1][1] += 5;
			}
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[0][1]));
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[count[jointsNum]-1][1]));
			break;
		case '-':
			if(points[0][1] > 5){
				points[0][1] -= 5;
				points[count[jointsNum]-1][1] -= 5;
			}
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[0][1]));
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[count[jointsNum]-1][1]));
			break;
		//可動域の呼び出し
		case ';':
			if(jointsNum == 0)
				fileName = "rightShoulder.txt";
			else if(jointsNum == 2)
				fileName = "leftShoulder.txt";
			else fileName = "ConData.txt";
			joints[jointsNum]->SetConstLine(fileName, 1);
			loadData(fileName);
			joints[jointsNum]->SetConstraintMode(PHBallJointDesc::Spline);
			break;
		case ':':
			fileName = "circle.txt";
			joints[jointsNum]->SetConstLine(fileName, 1);
			loadData(fileName);
			joints[jointsNum]->SetConstraintMode(PHBallJointDesc::Spline);
			break;
		case 'i':
			fileName = "Clear.txt";
			joints[jointsNum]->SetConstLine(fileName, 1);
			loadData(fileName);
			joints[jointsNum]->SetConstraintMode(PHBallJointDesc::Spline);
			break;
		case 'o':
			SaveData();
			break;
		case '@':
			twistFlag = !twistFlag;
			break;
		//可動域を指定する関節を決める。
		case '0':
			jointsNum = 0;
			GetSpline();
			break;
		case '1':
			jointsNum = 1;
			GetSpline();
			break;
		case '2':
			jointsNum = 2;
			GetSpline();
			break;
		case '3':
			jointsNum = 3;
			GetSpline();
			break;
		case '9':
			fileName = "pos.txt";
				LoadPos(fileName);
				abc = true;
			break;
		case 'Z':
			twistview = !twistview;
			break;
		case 'c':
//			spidar = true;
//			cal = spgpos;
			break;
		case '\033':  //\033はESCのASCII コード
		exit(0);
		default:
		break;
	}
};

void MyApp::mouse(int button, int state, int x, int y){
	xlast = x, ylast = y;
	if(button == GLUT_LEFT_BUTTON)
		bLeftButton = (state == GLUT_DOWN);
	if(button == GLUT_RIGHT_BUTTON)
		bRightButton = (state == GLUT_DOWN);
}

void MyApp::mouse2(int button, int state, int x, int y){
	xlast = x, ylast = y;
	if(button == GLUT_LEFT_BUTTON)
		bLeftButton = (state == GLUT_DOWN);
	if(button == GLUT_RIGHT_BUTTON)
		bRightButton = (state == GLUT_DOWN);
	if(button == GLUT_MIDDLE_BUTTON)
		bMiddleButton = (state == GLUT_DOWN);
	if(bLeftButton){
		double xrel,yrel;
		xrel = (x*200.0/width-100.0)*2.0;
		yrel = -(y*200.0/hight-100.0)*2.0;
		double swing,swingdir;
		swing = sqrt(xrel*xrel + yrel*yrel);
		swingdir = atan2(yrel,xrel);
		if(yrel < 0) swingdir += Rad(360);
		swingdir = Deg(swingdir);
//		DSTR << x << ',' << y << std::endl;
//		DSTR << xrel << "," << yrel << std::endl;
//		DSTR << swing << "," << swingdir << std::endl << std::endl;
		if(count[jointsNum]==0){
			for(int j=0;j<14;j++){
				points[j][0] = 0;
				points[j][1] = 0;
				points[j][2] = 50;
				points[j][3] = 10;
				Vec2d twist;
				joints[jointsNum]->GetTwistRange(twist);
				points[j][4] = Deg(twist.y);
				points[j][5] = Deg(twist.x);
				for(int i = 0;i<6;i++)
				joints[jointsNum]->SetConstPoint(j,i,Rad(points[j][i]));
			}
			poleTwist = Vec2d(-90,0);
			joints[jointsNum]->SetTwistPole(Vec2d(Rad(poleTwist[0]),Rad(poleTwist[1])));
			points[1][0] = 360;
			points[0][1] = 90;
			points[1][1] = 90;
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[1][0]));
			joints[jointsNum]->SetConstPoint(0,1,Rad(points[0][1]));
			joints[jointsNum]->SetConstPoint(1,1,Rad(points[1][1]));
			count[jointsNum] = 2;
		}
		if(swingdir > 0 && swingdir < 360 && swing > 0 && swing < 180 && count[jointsNum] < 14){
			bool flag = false;
			for(int i = 0;i < count[jointsNum]-1;i++){
				if((points[i][0]-swingdir) > -5&&(points[i][0]-swingdir < 5)) {flag = true; pointnum=i;}
			}
			if(!flag){
				for(pointnum = 0;pointnum < count[jointsNum];pointnum++){
					if(points[pointnum][0]>swingdir) break;
				}
				for(int i = count[jointsNum];i > pointnum ;i--){
					for(int j=0;j < 6;j++){
						points[i][j] = points[i-1][j];
						joints[jointsNum]->SetConstPoint(i,j,Rad(points[i][j]));
					}
				}
				if(pointnum == 0 && count[jointsNum]!=2)	points[count[jointsNum]-1][0] = 0;
				else	points[pointnum][0] = swingdir;
				points[pointnum][1] = swing;
				joints[jointsNum]->SetConstPoint(pointnum,0,Rad(points[pointnum][0]));
				joints[jointsNum]->SetConstPoint(pointnum,1,Rad(points[pointnum][1]));
				joints[jointsNum]->SetConstraintMode(PHBallJointDesc::Spline);
				count[jointsNum]++;
			}
			else{
				points[pointnum][0] = swingdir;
				points[pointnum][1] = swing;
				joints[jointsNum]->SetConstPoint(pointnum,0,Rad(points[pointnum][0]));
				joints[jointsNum]->SetConstPoint(pointnum,1,Rad(points[pointnum][1]));
				if(pointnum == 0){
					points[count[jointsNum]-1][0] = 360;
					points[count[jointsNum]-1][1] = swing;
					joints[count[jointsNum]-1]->SetConstPoint(count[jointsNum]-1,0,Rad(points[count[jointsNum]-1][0]));
					joints[count[jointsNum]-1]->SetConstPoint(count[jointsNum]-1,1,Rad(points[count[jointsNum]-1][1]));
				}
			}
		}
	}
	if(bMiddleButton){
		double xrel,yrel;
		xrel = (x*200.0/width-100.0)*2.0;
		yrel = -(y*200.0/hight-100.0)*2.0;
		double swing,swingdir;
		swing = sqrt(xrel*xrel + yrel*yrel);
		swingdir = atan2(yrel,xrel);
		if(yrel < 0) swingdir += Rad(360);
		swingdir = Deg(swingdir);
//		DSTR << x << ',' << y << std::endl;
//		DSTR << xrel << "," << yrel << std::endl;
//		DSTR << swing << "," << swingdir << std::endl << std::endl;
		if(count[jointsNum]==0){
			for(int j=0;j<14;j++){
				points[j][0] = 0;
				points[j][1] = 0;
				points[j][2] = 50;
				points[j][3] = 10;
				Vec2d twist;
				joints[jointsNum]->GetTwistRange(twist);
				points[j][4] = Deg(twist.y);
				points[j][5] = Deg(twist.x);
				for(int i = 0;i<6;i++)
				joints[jointsNum]->SetConstPoint(j,i,Rad(points[j][i]));
			}
			poleTwist = Vec2d(-90,0);
			joints[jointsNum]->SetTwistPole(Vec2d(Rad(poleTwist[0]),Rad(poleTwist[1])));
			points[1][0] = 360;
			points[0][1] = 90;
			points[1][1] = 90;
			joints[jointsNum]->SetConstPoint(1,0,Rad(points[1][0]));
			joints[jointsNum]->SetConstPoint(0,1,Rad(points[0][1]));
			joints[jointsNum]->SetConstPoint(1,1,Rad(points[1][1]));
			count[jointsNum] = 2;
		}
		if(swingdir > 0 && swingdir < 360 && swing > 0 && swing < 180 && count[jointsNum] < 14){
			bool flag = false;
			for(int i = 0;i < count[jointsNum]-1;i++){
				if((points[i][0]-swingdir) > -5&&(points[i][0]-swingdir < 5)) {flag = true; pointnum=i;}
			}
			if(!flag){
				for(int j=0;j < 6;j++){
					points[count[jointsNum]][j] = points[count[jointsNum]-1][j];
					joints[jointsNum]->SetConstPoint(count[jointsNum],j,Rad(points[count[jointsNum]][j]));
				}
				if(pointnum == 0 && count[jointsNum]!=2)	points[count[jointsNum]-1][0] = 0;
				else	points[count[jointsNum]-1][0] = swingdir;
				points[count[jointsNum]-1][1] = swing;
				joints[jointsNum]->SetConstPoint(count[jointsNum]-1,0,Rad(points[count[jointsNum]-1][0]));
				joints[jointsNum]->SetConstPoint(count[jointsNum]-1,1,Rad(points[count[jointsNum]-1][1]));
				joints[jointsNum]->SetConstraintMode(PHBallJointDesc::Spline);
				count[jointsNum]++;
				pointnum = count[jointsNum]-2;
			}
			else{
				points[pointnum][0] = swingdir;
				points[pointnum][1] = swing;
				joints[jointsNum]->SetConstPoint(pointnum,0,Rad(points[pointnum][0]));
				joints[jointsNum]->SetConstPoint(pointnum,1,Rad(points[pointnum][1]));
				if(pointnum == 0){
					points[count[jointsNum]-1][0] = 360;
					points[count[jointsNum]-1][1] = swing;
					joints[count[jointsNum]-1]->SetConstPoint(count[jointsNum]-1,0,Rad(points[count[jointsNum]-1][0]));
					joints[count[jointsNum]-1]->SetConstPoint(count[jointsNum]-1,1,Rad(points[count[jointsNum]-1][1]));
				}
			}
		}
	}
}
void MyApp::motion2(int x,int y){
	static bool bFirst = true;
	xlast = x;
	ylast = y;
	double xrel,yrel;
	xrel = (x*200.0/width-100.0)*2.0;
	yrel = -(y*200.0/hight-100.0)*2.0;
	double swing,swingdir;
	swing = sqrt(xrel*xrel + yrel*yrel);
	swingdir = atan2(yrel,xrel);
	if(yrel < 0) swingdir += Rad(360);
	swingdir = Deg(swingdir);

	if(bFirst){
		bFirst = false;
		return;
	}
	// 左ボタン
	if(bLeftButton||bMiddleButton){
		if(swingdir > 0 && swingdir < 360){
			if(pointnum == 0)	points[pointnum][0] = 0;
			else	points[pointnum][0] = swingdir;
			joints[jointsNum]->SetConstPoint(pointnum,0,Rad(points[pointnum][0]));
		}
		if(swing > 0 && swing < 180){
			points[pointnum][1] = swing;
			joints[jointsNum]->SetConstPoint(pointnum,1,Rad(points[pointnum][1]));
		}
		if(pointnum == 0){
			points[count[jointsNum]-1][1] = points[pointnum][1];
			joints[jointsNum]->SetConstPoint(count[jointsNum]-1,1,Rad(points[count[jointsNum]-1][1]));
		}
	}
	// 右ボタン
	if(bRightButton){
		CameraZoom *= exp(yrel/10.0);
	}
}
void MyApp::motion(int x,int y){
	static bool bFirst = true;
	int xrel = x - xlast, yrel = y - ylast;
	xlast = x;
	ylast = y;
	if(bFirst){
		bFirst = false;
		return;
	}
	// 左ボタン
	if(bLeftButton){
		CameraRotY += xrel * 0.01;
		CameraRotX += yrel * 0.01;
		if(CameraRotY < Rad(-180)) CameraRotY += Rad(360);
		else if(CameraRotY > Rad(180)) CameraRotY -= Rad(360);
	}
	// 右ボタン
	if(bRightButton){
		CameraZoom *= exp(yrel/10.0);
	}
}
void MyApp::CreateObj(){
	//デスクリプタを用いて剛体オブジェクトを生成

	double bodyMass = 1.0f;  /// デフォルトがこれなのはどうも変だが(mitake)

	// 高さに関するデータ
	double hBody			= 1.7219; // 最大身長
	double hCervicale		= 1.4564; // 頸椎高
	double hSubsternale		= 1.2253; // 胸骨下縁高
	double hWaist			= 1.0281; // 胴囲高
	double hIliocristal		= 1.0142; // 腸骨稜高
	double hOmphalion		= 1.0036; // 臍高
	double hTrochanterion	= 0.8757; // 転子高
	double hTibiale			= 0.4420; // 脛骨上縁高
	double hCalf			= 0.3150; // 下腿最大囲高

	// 幅に関するデータ
	double wBideltoid		= 0.4562; // 肩幅
	double wChest			= 0.2887; // 胸部横径
	double wWaist			= 0.2611; // 胴部横径
	double wBicristal		= 0.2722; // 腸骨稜幅
	double wBitrochanteric	= 0.3067; // 大転子間幅

	double waistBreadth    = wBicristal;
	double waistLength     = hOmphalion - (hIliocristal - (waistBreadth/2));
	double abdomenBreadth  = wWaist;
	double abdomenLength   = hSubsternale - hOmphalion;
	double chestBreadth    = wChest;
	double chestLength     = hCervicale - hSubsternale - (chestBreadth/2);

	double bideltoidBreadth = wBideltoid;

	double headBreadth = 0.1619; // 0.5699 / 3.1415;
	double headHeight  = 0.2387;
	
	double neckLength   = 1.5796 - 1.4564; // 耳の高さ - 頸椎高  // 1.7219 - 1.3882 - 0.2387;
	double neckDiameter = 0.3563 / 3.1415;

	double occiputToNeckFront  = 0.1301;
	double neckPosZ     = headBreadth/2 - (occiputToNeckFront - neckDiameter/2);
	double neckPosY     = -neckDiameter/2;

	double bigonialBreadth = 0.1117;

	double upperArmLength   = 0.3406;
	double upperArmDiameter = 0.2808 / 3.1415;

	double lowerArmLength   = 0.2766;
	double lowerArmDiameter = 0.1677 / 3.1415;

	double handLength    = 0.1908;
	double handBreadth   = 0.1018;
	double handThickness = 0.0275;

	double upperLegLength   = 0.4092;
	double upperLegDiameter = 0.5289 / 3.1415; //  0.1682;
	double interLegDistance = wBitrochanteric - upperLegDiameter;
	double legPosY = -(hIliocristal - hTrochanterion - waistBreadth/2);

	double lowerLegLength   = 0.3946;
	double lowerLegDiameter = 0.1109;
	double kneeDiameter     = 0.1076;
	double ankleDiameter    = 0.2194 / 3.1415;
	double calfDiameter     = 0.3623 / 3.1415;
	double calfPosY         = hTibiale - hCalf;
	double calfPosZ         = (calfDiameter/2 - kneeDiameter/2)*0.9;

	double footLength       = 0.2544;
	double footBreadth      = 0.0994;
	double footThickness    = 0.0619;
	double ankleToeDistance = 0.2010;

	double vertexToEyeHeight     = 0.1255;
	double occiputToEyeDistance  = 0.1688;
	double eyeDiameter           = 0.0240;
	double interpupillaryBreadth = 0.0619;

	double springWaistAbdomen = -1;        double damperWaistAbdomen = -1;
	double springAbdomenChest = -1;        double damperAbdomenChest = -1;
	double springChestNeck    = -1;        double damperChestNeck    = -1;
	double springNeckHead     = -1;        double damperNeckHead     = -1;
	double springShoulder     = -1;        double damperShoulder     = -1;
	double springElbow        = -1;        double damperElbow        = -1;
	double springWrist        = -1;        double damperWrist        = -1;
	double springWaistLeg     = -1;        double damperWaistLeg     = -1;
	double springKnee         = -1;        double damperKnee         = -1;
	double springAnkle        = -1;        double damperAnkle        = -1;
	double springEye          = 100000.0;  double damperEye          =  2000.0;

	{
		PHSolidDesc        solidDesc;
		PHBallJointDesc    ballDesc;
		Posed              pose1,pose2,pose3;
		// Solid
		solidDesc.mass   = 0.252 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[0] = phScene->CreateSolid(solidDesc);
		// Shape
		int sn = 0;
		CDRoundConeDesc    rcDesc;
		// 中心～首付根
		rcDesc.radius[0] = chestBreadth/2;
		rcDesc.radius[1] = neckDiameter/2;
		rcDesc.length    = chestBreadth/2;
		solids[0]->AddShape(phSdk->CreateShape(rcDesc));
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[0]->SetShapePose(sn,pose2*pose1);
		sn++;
		// 中心～腹
		rcDesc.radius[0] = abdomenBreadth/2;
		rcDesc.radius[1] = chestBreadth/2;
		rcDesc.length    = chestLength;
		solids[0]->AddShape(phSdk->CreateShape(rcDesc));
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[0]->SetShapePose(sn,pose2*pose1);
		sn++;
		// 中心～肩
		double sX = bideltoidBreadth/2 - upperArmDiameter/2;
		double sY = chestBreadth/2 - upperArmDiameter/2;
		double theta = atan(sX/sY);
		rcDesc.radius[0] = chestBreadth/2;
		rcDesc.radius[1] = upperArmDiameter/2;
		rcDesc.length    = sqrt(sX*sX + sY*sY);
		pose1=Posed(); pose1.Pos() = Vec3d(0,0,rcDesc.length/2);
		pose2=Posed(); pose2.Ori() = Quaterniond::Rot(Rad(-90),'x');
		/// -- 左肩
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(theta,'z');
		solids[0]->AddShape(phSdk->CreateShape(rcDesc));
		solids[0]->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
		// -- 右肩
		pose3=Posed(); pose3.Ori() = Quaterniond::Rot(-theta,'z');
		solids[0]->AddShape(phSdk->CreateShape(rcDesc));
		solids[0]->SetShapePose(sn,pose3*pose2*pose1);
		sn++;
		// Joint -- [p]Abdomen-[c]Chest
		ballDesc                  = PHBallJointDesc();
		ballDesc.posePlug.Pos()   = Vec3d(0,  abdomenLength/2, 0);
		ballDesc.poseSocket.Pos() = Vec3d(0, -chestLength, 0);
//		joints[0]  = CreateJoint(solids[0], solids[SO_ABDOMEN], ballDesc);
		solids[0]->SetDynamical(false);
	}
	for(int lr = -1;lr<2;lr+=2){
		PHSolidDesc        solidDesc;
		PHBallJointDesc    ballDesc;

		int soNUpperArm   = 1;
		int joNShoulder   = 0;
		int soNLowerArm = 2;
		int joNElbow = 0;
		int soNHand = 3;
		int joNWrist = 1;
		if (lr==-1) {
			soNUpperArm   = 4;
			joNShoulder   = 2;
			soNLowerArm = 5;
			joNElbow = 1;
			soNHand = 6;
			joNWrist = 3;
		}
		// Solid
		solidDesc.mass      = 0.04 * bodyMass;
		solidDesc.inertia   = Matrix3d::Unit() * solidDesc.mass;
		solids[soNUpperArm] = phScene->CreateSolid(solidDesc);

		// Shape
		int sn = 0;
		CDRoundConeDesc    rcDesc;
		rcDesc.radius[0]    = lowerArmDiameter / 2.0;
		rcDesc.radius[1]    = upperArmDiameter / 2.0;
		rcDesc.length       = upperArmLength;
		solids[soNUpperArm]->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-lr*90),'y');
		solids[soNUpperArm]->SetShapePose(0,pose);
		sn++;
		// Joint -- Shoulder ([p]Chest-[c]UpperArm)
		ballDesc                    = PHBallJointDesc();
		ballDesc.poseSocket.Pos() = Vec3d(lr*(bideltoidBreadth/2.0 - upperArmDiameter/2.0), chestBreadth/2.0-upperArmDiameter/2.0, 0);
		ballDesc.posePlug.Pos() = Vec3d(-lr*upperArmLength/2.0, 0, 0);
		ballDesc.posePlug.Ori() = Quaterniond::Rot(Rad(-90),'y');
		ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90),'y');
		ballDesc.spring = 0;
		ballDesc.damper = 1;
//		ballDesc.limitSwing = Vec2d(Rad(0),Rad(5));
		ballDesc.limitTwist = Vec2d(Rad(-90),Rad(0));
		ballDesc.poleTwist  = Vec2d(Rad(-90),Rad(0));
//		ballDesc.limitSwingDir = Vec2d(Rad(190),Rad(220));
		joints[joNShoulder]       = (PHBallJointIf*) phScene->CreateJoint(solids[0],solids[soNUpperArm], ballDesc);
		joints[joNShoulder]->Enable(false);
			PHSpringDesc DSpring;
			DSpring.poseSocket.Pos() = Vec3d(lr*(bideltoidBreadth/2.0 - upperArmDiameter/2.0), chestBreadth/2.0-upperArmDiameter/2.0, 0);
			DSpring.posePlug.Pos() = Vec3d(-lr*upperArmLength/2.0, 0, 0);
			DSpring.posePlug.Ori() = Quaterniond::Rot(Rad(-90),'y');
			DSpring.poseSocket.Ori() = Quaterniond::Rot(Rad(-90),'y');
			DSpring.spring = Vec3d(1000.0,1000.0,1000.0);
			DSpring.damper = Vec3d(10.0,10.0,10.0);
			spring[joNShoulder] = (PHSpringIf*) phScene->CreateJoint(solids[0],solids[soNUpperArm], DSpring);

		phScene->SetContactMode(solids[soNUpperArm], solids[0], PHSceneDesc::MODE_NONE);
		PHHingeJointDesc   hingeDesc;

		// Solid
		solidDesc.mass      = 0.02 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[soNLowerArm] = phScene->CreateSolid(solidDesc);
		// Shape
		sn = 0;
		rcDesc.radius[0]    = lowerArmDiameter / 2.0;
		rcDesc.radius[1]    = lowerArmDiameter / 2.0;
		rcDesc.length       = lowerArmLength;
		solids[soNLowerArm]->AddShape(phSdk->CreateShape(rcDesc));

		pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-lr*90),'y');
		solids[soNLowerArm]->SetShapePose(0,pose);
		sn++;
		// Joint -- Elbow ([p]UpperArm-[c]LowerArm)
		hingeDesc                  = PHHingeJointDesc();
		hingeDesc.posePlug.Pos()   = Vec3d(lr*upperArmLength/2.0, 0, 0);
		hingeDesc.posePlug.Ori()   = Quaterniond::Rot(Rad(lr*90), 'x');
		hingeDesc.poseSocket.Pos() = Vec3d(-lr*lowerArmLength/2.0, 0, 0);
		hingeDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(lr*90), 'x');
		hingeDesc.spring = 0;
		hingeDesc.damper = 5;
		hingeDesc.targetPosition           = Rad(0);
		hingeDesc.lower			   = Rad(5);
		hingeDesc.upper			   = Rad(145);
		hjoints[joNElbow] = (PHHingeJointIf*) phScene->CreateJoint(solids[soNLowerArm], solids[soNUpperArm], hingeDesc);

		phScene->SetContactMode(solids[soNLowerArm], solids[soNUpperArm], PHSceneDesc::MODE_NONE);

		CDBoxDesc          boxDesc;
		// Solid
		solidDesc.mass     = 0.05 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[soNHand]    = phScene->CreateSolid(solidDesc);

		boxDesc.boxsize    = Vec3f(handLength, handThickness, handBreadth);
		solids[soNHand]->AddShape(phSdk->CreateShape(boxDesc));

		// Joint -- Wrist ([p]LowerArm-[c]Hand)
		ballDesc                  = PHBallJointDesc();
		ballDesc.posePlug.Pos()   = Vec3d(lr*lowerArmLength/2.0, 0, 0);
		ballDesc.poseSocket.Pos() = Vec3d(-lr*handLength/2.0, 0, 0);
		ballDesc.posePlug.Ori() = Quaterniond::Rot(Rad(-90),'y');
		ballDesc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90),'y');
		ballDesc.damper			  = 5;
		ballDesc.spring			  = 10;
		ballDesc.limitTwist		  = Vec2d(Rad(1),Rad(5));
		joints[joNWrist]          = (PHBallJointIf*) phScene->CreateJoint(solids[soNHand], solids[soNLowerArm], ballDesc);
		phScene->SetContactMode(solids[soNHand], solids[soNLowerArm], PHSceneDesc::MODE_NONE);

	}
	{//頭//solid7,8 balljoint4,5
		PHSolidDesc        solidDesc;
		PHBallJointDesc    ballDesc;

		// Solid
		solidDesc.mass  = 0.028 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[7] = phScene->CreateSolid(solidDesc);
		// Shape
		int sn = 0;
		CDCapsuleDesc      capsuleDesc;

		capsuleDesc.radius = neckDiameter/2;
		capsuleDesc.length = neckLength;
		solids[7]->AddShape(phSdk->CreateShape(capsuleDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(90),'x');
		solids[7]->SetShapePose(0,pose);
		sn++;
		// Joint -- [p]Chest-[c]Neck
		ballDesc                  = PHBallJointDesc();
		ballDesc.posePlug.Pos()   = Vec3d(0, chestBreadth / 2.0, 0);
		ballDesc.poseSocket.Pos() = Vec3d(0, -neckLength / 2.0, 0);
		ballDesc.spring			  = 7;
		ballDesc.damper			  = 5;
		joints[4] = (PHBallJointIf*)phScene->CreateJoint(solids[7], solids[0], ballDesc);

		phScene->SetContactMode(solids[7], solids[0], PHSceneDesc::MODE_NONE);

		// Solid
		solidDesc.mass    = 0.07 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[8]   = phScene->CreateSolid(solidDesc);
		// Shape
		sn = 0;
		CDRoundConeDesc   rcDesc;

		rcDesc.radius[0] = bigonialBreadth / 2.0;
		rcDesc.radius[1] = headBreadth / 2.0;
		double Z = headBreadth/2 - bigonialBreadth/2 ;
		double Y = headHeight - headBreadth/2 - bigonialBreadth/2;
		double theta = atan(Y/Z);
		rcDesc.length = sqrt(Z*Z + Y*Y);
		solids[8]->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose1=Posed(); pose1.Pos() = Vec3d(0,0,-rcDesc.length/2);
		Posed pose2=Posed(); pose2.Ori() = Quaterniond::Rot(-theta,'x');
		solids[8]->SetShapePose(0,pose2*pose1);
		sn++;
		// Joint -- [p]Neck-[c]Head
		ballDesc                  = PHBallJointDesc();
		ballDesc.posePlug.Pos()   = Vec3d(0, neckLength / 2.0, 0);
		ballDesc.poseSocket.Pos() = Vec3d(0, neckPosY, neckPosZ);
		ballDesc.spring			  = 7;
		ballDesc.damper			  = 5;
		joints[5] = (PHBallJointIf*)phScene->CreateJoint(solids[8], solids[7], ballDesc);

		phScene->SetContactMode(solids[8], solids[7], PHSceneDesc::MODE_NONE);

	}
	PHSolidDesc Dfloor;
	PHSolidDesc dPointer;	Dfloor.pose.Pos() = Vec3d(0,1,0);
	Dfloor.mass = 0.001;
	dPointer.pose.Pos() = Vec3d(-0.5,-0.5,0);
	dPointer.pose.Pos() = Vec3d(1,1,1);
	dPointer.mass = 0.5;
	Pointer[0] = phScene->CreateSolid(dPointer);
	{
		PHSolidDesc        solidDesc;
		PHBallJointDesc    ballDesc;

		// Solid
		solidDesc.mass     = 0.028 * bodyMass;
		solidDesc.inertia  = Matrix3d::Unit() * solidDesc.mass;
		solids[9] = phScene->CreateSolid(solidDesc);
		// Shape
		CDRoundConeDesc    rcDesc;
		rcDesc.radius[0]   = abdomenBreadth/2;
		rcDesc.radius[1]   = abdomenBreadth/2;
		rcDesc.length      = abdomenLength;
		solids[9]->AddShape(phSdk->CreateShape(rcDesc));
		Posed pose=Posed(); pose.Ori() = Quaterniond::Rot(Rad(-90),'x');
		solids[9]->SetShapePose(0,pose);

		// Joint -- [p]Abdomen-[c]Chest
		ballDesc                  = PHBallJointDesc();
		ballDesc.posePlug.Pos()   = Vec3d(0,  abdomenLength/2, 0);
		ballDesc.poseSocket.Pos() = Vec3d(0, -chestLength, 0);
		ballDesc.spring			  = 70;
		ballDesc.damper			  = 5;
		joints[6]  = (PHBallJointIf*)phScene->CreateJoint(solids[0], solids[9], ballDesc);
		phScene->SetContactMode(solids[0], solids[9], PHSceneDesc::MODE_NONE);
	}
	Dfloor.pose.Pos() = Vec3d(0,1,0);
	Dfloor.mass = 0.001;
	dPointer.pose.Pos() = Vec3d(-0.5,-0.5,0);
	dPointer.pose.Pos() = Vec3d(1,1,1);
	dPointer.mass = 0.5;
	Pointer[0] = phScene->CreateSolid(dPointer);

	//関節可動域
	PHSpringDesc DSpring;
	DSpring.poseSocket.Pos() = Vec3d(0,0,0);
	DSpring.posePlug.Pos() = Vec3d(0,0.0,0);
	DSpring.spring = Vec3d(1000.0,1000.0,1000.0);
	DSpring.damper = Vec3d(10.0,10.0,10.0);
//	Spring = (PHSpringIf*) phScene->CreateJoint(Pointer[0], solids[6], DSpring);
	Spring = (PHSpringIf*) phScene->CreateJoint(Pointer[0], solids[4], DSpring);
	Spring->Enable(false);
//	phScene->SetContactMode(Pointer[0],PHSceneDesc::MODE_NONE);

	//デスクリプタを用いて箱形状オブジェクトを生成
	CDSphereDesc sPoint;
	sPoint.radius = 0.05;
	shapePoint  = phSdk->CreateShape(sPoint);
	//剛体オブジェクトに箱オブジェクトを割り当てる
	Pointer[0]->AddShape(shapePoint);
	Pointer[0]->SetGravity(false);




	phScene->SetGravity(Vec3d(0.0,0.0,0.0));
	pospos = Vec3d(-(bideltoidBreadth/2.0 - upperArmDiameter/2.0), chestBreadth/2.0-upperArmDiameter/2.0, 0);
}

//描画関係
void MyApp::Constline(){
	if(points[0][0] <= 360 && points[0][0] >= 0){
		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(2);
		if(points[count[jointsNum]-1][0] != 360 && count[jointsNum] != 0){
			points[count[jointsNum]][0] = 360;
			points[count[jointsNum]][1] = points[0][1];
			points[count[jointsNum]][2] = points[0][2];
			points[count[jointsNum]][3] = points[0][3];
			points[count[jointsNum]][4] = points[0][4];
			count[jointsNum]++;
		}
		
		GRMaterialDesc DescR;
		DescR.ambient = Vec4d(1,0,0,0);
		DescR.specular = Vec4d(1,0,0,0);
		DescR.emissive = Vec4d(0,0,1,0);
		grRender->SetMaterial(DescR);

		glBegin(GL_LINE_STRIP);
		Posed BJPose,soPose;
		joints[jointsNum]->GetSocketPose(BJPose);
		soPose = (joints[jointsNum]->GetSocketSolid())->GetPose();
//		DSTR << BJPose << std::endl;
		Vec3d DConst;
		for(int i = 1 ; i < count[jointsNum]; i++){
			int HerWidth;
			HerWidth = points[i][0] - points[i-1][0];

			for(double j = 0; j <= 20;j++){
				double Herx,Hery;
				Vec3d Pj;
				Herx = j / 20;
				Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][4] + points[i-1][4]) * Herx * Herx * Herx
					+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][4] - points[i][4]) * Herx * Herx
					+ points[i-1][4] * Herx + points[i-1][1];
				Herx *= HerWidth;
				Herx += points[i-1][0];
				Pj.x = sin(Rad(Hery)) * cos(Rad(Herx)) * 0.2;
				Pj.y = sin(Rad(Hery)) * sin(Rad(Herx)) * 0.2;
				Pj.z = cos(Rad(Hery)) * 0.2;

				if(jointsNum==0||jointsNum == 1){
					Pj *= -1;
				}
				Pj = soPose * BJPose * Pj;
				glVertex3d(Pj.x,Pj.y,Pj.z);/**/
			}
		}
		glEnd();
		if(count[jointsNum] > 1){
			//DescR.ambient = Vec4d(0.2,0,0,0);
			//DescR.specular = Vec4d(0.2,0,0,0);
			//DescR.emissive = Vec4d(0.2,0.2,0.2,0);
			//grRender->SetMaterial(DescR);
			//Vec3d Pose = BJPose.Pos();
			////glRotated((double)r, 0.0, 1.0, 0.0);
			//glTranslated(Pose.x,Pose.y,Pose.z);
			//glColor3d(0.2,0.2,0.2);
			//glutWireSphere(0.18, 10, 10);
			//glEnd();
		}
	}
}
void MyApp::HermiteSpline(Vec2d para){
	if(count[jointsNum] > 1){

		if(twistview){
			//twist角可動域の描画
			for(int i = 1 ; i < count[jointsNum] ; i++){
				glBegin(GL_LINE_STRIP);
					double Herx,Hery;
					double para = 0 / 30;
					Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Vec2d now;
					now[0] = Hery/2.0 * cos(Rad(Herx));
					now[1] = -Hery/2.0 * sin(Rad(Herx));
					glVertex3d(now[0],now[1],points[i-1][4]/2.0);
					glVertex3d(now[0],now[1],points[i-1][5]/2.0);
				glEnd();
			}
			for(int i = 1 ; i < count[jointsNum] ; i++){
				glColor4d(1.0, 0.5, 0.8,0.3);
				glColor4d(0.2, 0.2, 0.2,0.3);
				glBegin(GL_POLYGON);
				for(double j = 0; j <= 29;j++){

					double Herx,Hery,Herz;
					double Herx2,Hery2,Herz2,Herz3,Herz4;
					double para = j / 30;
					Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz = (1-para) * points[i-1][4] + (para) * points[i][4];
					Herz3 = (1-para) * points[i-1][5] + (para) * points[i][5];
					para = (j+1) / 30;
					Herx2 = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery2 = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz2 = (1-para) * points[i-1][4] + (para) * points[i][4];
					Herz4 = (1-para) * points[i-1][5] + (para) * points[i][5];
					Vec4d now;
					now[0] = Hery/2.0 * cos(Rad(Herx));
					now[1] = -Hery/2.0 * sin(Rad(Herx));
					now[2] = Hery2/2.0 * cos(Rad(Herx2));
					now[3] = -Hery2/2.0 * sin(Rad(Herx2));
					glVertex3d(now[0],now[1],Herz/2.0);
					glVertex3d(now[2],now[3],Herz2/2.0);
					glVertex3d(now[2],now[3],Herz4/2.0);
					glVertex3d(now[0],now[1],Herz3/2.0);
				}
			}
			glEnd();
			for(int k =0;k<2;k++){
				int a,b;
				a=1;b=count[jointsNum];
				if(k==1){
					a=2;
					b--;
				}
			for(int i = a ; i < b ; i++){
				glColor3d(0.5, 0.0, 0.5);
				glLineWidth(2);
				glBegin(GL_LINE_STRIP);
				for(double j = 0; j <= 30;j++){
					double Herx,Hery,Herz;
					double para = j / 30;
					Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz = (1-para) * points[i-1][4+k] + (para) * points[i][4+k];
					Vec2d now;
					now[0] = Hery/2.0 * cos(Rad(Herx));
					now[1] = -Hery/2.0 * sin(Rad(Herx));
					glVertex3d(now[0],now[1],Herz/2.0);
				}
			}
			glEnd();
			}
			//縦線を書く
			for(int i = 2 ; i < count[jointsNum] ; i++){
				glColor3d(0.5, 0.0, 0.5);
				glLineWidth(2);
				glBegin(GL_LINES);
				double Herx,Hery,Herz,Herz2;
				double para = 0 / 30;
				Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
					+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
					+ points[i-1][2] * para + points[i-1][0];
				Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
					+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
					+ points[i-1][3] * para + points[i-1][1];
				Herz = (1-para) * points[i-1][4] + (para) * points[i][4];
				Herz2 = (1-para) * points[i-1][5] + (para) * points[i][5];
				Vec2d now;
				now[0] = Hery/2.0 * cos(Rad(Herx));
				now[1] = -Hery/2.0 * sin(Rad(Herx));
				glVertex3d(now[0],now[1],Herz/2.0);
				glVertex3d(now[0],now[1],Herz2/2.0);
			}
			//天井の面
			for(int i = 1 ; i < count[jointsNum] ; i++){
				glColor4d(0.5, 0.0, 0.5,0.3);
		//		glBegin(GL_POLYGON);
				for(double j = 0; j < 1;j++){

					double Herx,Hery,Herz;
					double Herx2,Hery2,Herz2,Herz3,Herz4;
					double para = j / 10;
					Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz = (1-para) * points[i-1][4] + (para) * points[i][4];
					Vec4d now;
					now[0] = Hery/2.0 * cos(Rad(Herx));
					now[1] = -Hery/2.0 * sin(Rad(Herx));
					glColor4d(1.0, 0.5, 0.8,0.3);
					glColor4d(0.2, 0.2, 0.2,0.3);
					glBegin(GL_POINTS);
					glVertex3d(now[0],now[1],Herz/2.0);
					glEnd();
					glColor4d(0.5, 0.0, 0.5,0.3);
					glBegin(GL_LINE_STRIP);
					for(int k = 0;k < 11; k++){
						Herz3 = (1-0.01*k*k)*90 + (0.01*k*k) * Herz ;
						glVertex3d(now[0]*0.1*k,now[1]*0.1*k,Herz3/2.0);
//						DSTR << k * now[0] << "," << k * now[1] << "," << Herz3 << std::endl;
//						glVertex3d(now[2],now[3],Herz2);
					}
					glEnd();
				}
			}
			for(int i = 1 ; i < count[jointsNum] ; i++){
				for(double j = 0; j < 6;j++){

					double Herx,Hery,Herz;
					double Herx2,Hery2,Herz2,Herz3,Herz4;
					double para = j / 5;
					Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz = (1-para) * points[i-1][4] + (para) * points[i][4];
					//Herz3 = (1-para) * points[i-1][5] + (para) * points[i][5];
					para = (j+1) / 5;
					Herx2 = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
						+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
						+ points[i-1][2] * para + points[i-1][0];
					Hery2 = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
						+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
						+ points[i-1][3] * para + points[i-1][1];
					Herz2 = (1-para) * points[i-1][4] + (para) * points[i][4];
					//Herz4 = (1-para) * points[i-1][5] + (para) * points[i][5];

					Vec4d now;
					now[0] = Hery/2.0 * cos(Rad(Herx));
					now[1] = -Hery/2.0 * sin(Rad(Herx));
					now[2] = Hery2/2.0 * cos(Rad(Herx2));
					now[3] = -Hery2/2.0 * sin(Rad(Herx2));
					glColor4d(0.5, 0.0, 0.5,0.3);
					glBegin(GL_LINES);
					for(int k = 0;k < 11; k++){
						Herz3 = (1-0.01*k*k)*90 + (0.01*k*k) * Herz ;
						Herz4 = (1-0.01*k*k)*90 + (0.01*k*k) * Herz2 ;
						glVertex3d(now[0]*0.1*k,now[1]*0.1*k,Herz3/2.0);
						glVertex3d(now[2]*0.1*k,now[3]*0.1*k,Herz4/2.0);
					}
					glEnd();

				}
			}
		}
		for(int i = 1 ; i < count[jointsNum] ; i++){
			glColor3d(0.5, 0.0, 0.5);
			glLineWidth(2);
			glBegin(GL_LINE_STRIP);
			for(double j = 0; j <= 30;j++){
				double Herx,Hery;
				double para = j / 30;
				Herx = (2 * points[i-1][0] - 2 * points[i][0] + points[i][2] + points[i-1][2]) * para * para * para
					+ (3 * points[i][0] - 3 * points[i-1][0] - 2 * points[i-1][2] - points[i][2]) * para * para
					+ points[i-1][2] * para + points[i-1][0];
				Hery = (2 * points[i-1][1] - 2 * points[i][1] + points[i][3] + points[i-1][3]) * para * para * para
					+ (3 * points[i][1] - 3 * points[i-1][1] - 2 * points[i-1][3] - points[i][3]) * para * para
					+ points[i-1][3] * para + points[i-1][1];
				Vec2d now;
				now[0] = Hery/2.0 * cos(Rad(Herx));
				now[1] = -Hery/2.0 * sin(Rad(Herx));
				glVertex3d(now[0],now[1],0.0);
			}
		}
		glEnd();
		glColor3d(0.7, 0.3, 0.8);
		glPointSize(5);
		char t_char[20] = {"twist"};
		char tu_char[20];
		char tl_char[20];
		for(int i = 0 ; i < count[jointsNum]-1 ; i++){
			Vec2d now;
			now[0] = (points[i][1])/2.0 * cos(Rad(points[i][0]));
			now[1] = -(points[i][1])/2.0 * sin(Rad(points[i][0]));
			glBegin(GL_POINTS);
			glVertex3d(now[0], now[1],0.0);
			glEnd();
			if(twistFlag){
			sprintf_s(tu_char, "upper:%.1f", points[i][4]);//文字列の代入
			sprintf_s(tl_char, "lower:%.1f", points[i][5]);//文字列の代入
			sprintf_s(tl_char, "%.1f~%.1f", points[i][5],points[i][4]);//文字列の代入
			DISPLAY_TEXT(now[0]/2+45, -now[1]/2 + 60, t_char  );
//			DISPLAY_TEXT(now[0]/2+50, -now[1]/2 + 60, tu_char  );
			DISPLAY_TEXT(now[0]/2+50, -now[1]/2 + 55, tl_char  );
			glColor3d(0.7, 0.3, 0.8);
			}
		}
		Vec2d pole;
		joints[jointsNum]->GetTwistRange(pole);
//		sprintf_s(tl_char, "%.1f~%.1f", Deg(pole[0]),Deg(pole[1]));//文字列の代入
//		DISPLAY_TEXT(para[0]/2+45, para[1]/2 + 60, t_char  );
//		DISPLAY_TEXT(para[0]/2+50, para[1]/2 + 55, tl_char  );
		glColor3d(0.7, 0.3, 0.8);

	}
	else{
		glColor3d(1.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3d(0,1,0);
		glVertex3d(Rad(360),1,0);
		glEnd();
	}
}
void MyApp::GetSpline(){
	count[jointsNum] = 0;
	for(int i = 0;i < 15;i++){
		for(int j=0;j<5;j++){
			points[i][j] = Deg(joints[jointsNum]->GetConstLine(i,j));
		}
		if(points[i][0] == 360)
			count[jointsNum] = i;
	}
}

bool MyApp::SaveData(){
	char* a;
	if(jointsNum == 0)
		a = "rightShoulder.txt";
	else if(jointsNum == 2)
		a = "leftShoulder.txt";
	else a = "ConData.txt";
	
	std::fstream ConstData( a, std::ios::out );    // 書き込みでファイルを開く
	if( ConstData.fail() )
		return false;	// ファイルを開けないとき
	ConstData << "拘束座標" << std::endl; //書き込み
	ConstData << poleTwist[0] << ' ' << poleTwist[1] << std::endl; 
	for(int i = 0; i < count[jointsNum] ;i++){
		ConstData << points[i][0] << ' ' << points[i][1] << ' ' << points[i][2] << ' ' << points[i][3] << ' ' << points[i][4] << ' ' << points[count[jointsNum]][5] << std::endl;
	}
	return true;
}

bool MyApp::loadData(char* filename){
	std::fstream ConstData(filename,std::ios::in); //読み込みでファイルを開く
	if( ConstData.fail()) return false;
	char txt[10];
	count[jointsNum] = 0;
	ConstData >> txt;
	ConstData >> poleTwist[0] >> poleTwist[1];
	while( !ConstData.eof()){
		ConstData >> points[count[jointsNum]][0] >> points[count[jointsNum]][1] >> points[count[jointsNum]][2] >> points[count[jointsNum]][3] >> points[count[jointsNum]][4] >> points[count[jointsNum]][5];
		count[jointsNum]++;
	}
	count[jointsNum]--;
	return true;
}

void MyApp::DRAW_STRING(int x, int y, char *string, void *font){
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++){
	    glutBitmapCharacter(font, string[i]);
	}
}

void MyApp::DISPLAY_TEXT(int x, int y, char *string){
	//glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushAttrib(GL_ENABLE_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3d(0.0, 0.0, 1.0);	//文字の色
	glCallList(app.list);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	list=glGenLists(1);
	glNewList(list,GL_COMPILE);

	DRAW_STRING(x, y, string , GLUT_BITMAP_9_BY_15);
	glEndList();

	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}