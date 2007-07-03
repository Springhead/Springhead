/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/*
 Springhead2/src/Samples/ASHIGARU/main.cpp

【概要】３脚モジュラーロボットASHIGARUの歩容生成シミュレータ
 
【仕様】

*/

#include <windows.h>	// Window関連
#include <winuser.h>
#include "resource1.h"
#include <stdio.h>
#include <math.h>
#include <Springhead.h>		//	Springheadのインタフェース
#include <GL/glut.h>
#include "robot.h"
#include "myheader.h"	// 自作関数，構造体

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#define CN 9 
#define CK 4 
typedef unsigned int set; 
#define first(n) ((set) ((1U << (n)) - 1U))

using namespace Spr;

#define ESC		27
#define module_max 4

UTRef<PHSdkIf> phSdk;			// SDK
UTRef<GRSdkIf> grSdk;
UTRef<PHSceneIf> scene;			// Scene
UTRef<GRDebugRenderIf> render;
UTRef<GRDeviceGLIf> device;

INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);	// メインダイアログ

// メインダイアログのハンドル
HWND		g_hDlgMain;		// ハンドル
HINSTANCE	g_hInstMain;	// インスタンス

Robot robot[50];//とりあえずASHIGARU最大１０体

double zoom = 0.2;

//CPG関連
double Tr=0.1;//uの時定数
double ae=1.5;//２つのニューロンの結合係数
double af=1.5;//２つのニューロンの結合係数
double s=1.0;//外部入力
double b=2.5;//疲労係数
double Ta=0.5;//vの時定数
double U[10];//ニューロンの内部状態
double V[10];//ニューロンの自己抑制度
double WY[10];//重みと出力の積
double w[10][10];//結合係数
double Y[10], Y_Past[10];//ＣＰＧ出力
double smp[10];//最大振幅
double ue[10], ye[10], ve[10], sume[10];//eニューロンに関する出力
double uf[10], yf[10], vf[10], sumf[10];//fニューロンに関する出力

double para_X1 = 0.11, para_a1 = 0.03, para_b1 = 0.05, para_H1 = 0.17;
double	L1 = 0.067, L2 = 0.067, L3 = 0.125;	// 関節間長さ
double   direction_theta = 0.0;  //進行方向

double			NowTime = 0.0;	// 時間
double			s_time = 0.0;	// サンプリングタイム初期化
double			CycleTime = 1.0;
double			Phase = 0.0;

bool stepOnTimer = false;
int turn_flag = 0;


//CPG計算用関数たち////////////////////////////////////////////////////////////////////////////////////////////
double _max(double x) {return ((x>0) ? x : 0);}//xが０より大きいかどうかを判別する関数

double CalcU(int cpgnum, double u, double a, double y, double v, double sum)//ニューロンの内部状態を計算する関数
{
	U[cpgnum] = ( - u - a*y + s - b*v - sum ) / Tr;

	return U[cpgnum];
}

double CalcV(int cpgnum, double v, double y)//ニューロンの自己抑制度を計算する関数
{
	V[cpgnum] = ( - v + y ) / Ta;
	return V[cpgnum];
}

double CalcWY(int cpgnum, int othercpg, double y)//重みと出力の積を計算する関数
{	
	WY[cpgnum] = w[othercpg][cpgnum] * y;
	return WY[cpgnum];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateFloor(){
	PHSolidDesc sd;
	PHSolidIf* soFloor = scene->CreateSolid(sd);
	CDBoxDesc bd;
	bd.boxsize = Vec3f(3.0f, 0.3f, 3.0f);
	soFloor->AddShape(phSdk->CreateShape(bd));
	soFloor->SetFramePosition(Vec3f( 0, -0.15, 0));
	soFloor->SetDynamical(false);					// 床は外力によって動かないようにする
}

/* brief     	glutDisplayFuncで指定するコールバック関数．画面の描画を行う．
   param	 	なし
   return 	    なし*/
void Display(){
	Affinef af;
	af.Pos() = Vec3f(0, 3.5, 4)*zoom;
	af.LookAtGL(Vec3f(0.6,0,0.1), Vec3f(0.1,10,0.1));
	render->SetViewMatrix(af.inv());	//	視点の設定
	
	render->ClearBuffer();
	render->DrawScene(scene);
	render->EndScene();
	glutSwapBuffers();
}

/*  brief     	光源の設定
   param	 	なし
   return 	    なし*/
void setLight() {
	GRLightDesc light0, light1;
	light0.position = Vec4f(20.0, 40.0, 40.0, 0.1);
	//light1.position = Vec4f(-20.0, 20.0, 20.0, 0.1);
	render->PushLight(light0);
	//render->PushLight(light1);
}

/* brief     	初期化処理
   param	 	なし
   return 	    なし*/
void initialize(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	setLight();
}

/* brief		glutReshapeFuncで指定したコールバック関数
   param		<in/--> w　　幅
   param		<in/--> h　　高さ
   return		なし*/
void Reshape(int w, int h){
	// Viewportと射影行列を設定
	render->Reshape(Vec2f(), Vec2f(w,h));
}

/* brief 		glutKeyboardFuncで指定したコールバック関数 
  param		    <in/--> key　　 ASCIIコード
  param 		<in/--> x　　　 キーが押された時のマウス座標
  param 		<in/--> y　　　 キーが押された時のマウス座標
  return 	なし	*/
void Keyboard(unsigned char key, int x, int y){
	unsigned int i = 0;
	switch (key) {
	//終了
	case ESC:	
		exit(0);
		break;
	case 'q':
		exit(0);
		break;
	case 'a':
		NowTime += 0.05;
		break;
	case 's':
		NowTime -= 0.05;
		break;
	case 'w':
		turn_flag++ ;
		break;
	case 'z':
		zoom -= 0.01;
		//glutPostRedisplay();
		break;
	case 'x':
		zoom += 0.01;
		//glutPostRedisplay();
		break;
	case 't':
		stepOnTimer = !stepOnTimer;
		break;
	case 'p':
		stepOnTimer = false;
		scene->Step();
		glutPostRedisplay();
		break;
	case 'o':
		for(int i=0; i<module_max; i++){
			robot[i].soBody->SetDynamical(true);
		}
		break;
	default:
		break;
	}
}	

/* brief  	    glutTimerFuncで指定したコールバック関数
   param	 	<in/--> id　　 タイマーの区別をするための情報
   return 	    なし */
void timer(int id){

	static LARGE_INTEGER	freq, start, now, past;			// 周波数，開始時間，現在時間，前回時間

	// 時間計測準備
	start.QuadPart = now.QuadPart = past.QuadPart = 0;
	QueryPerformanceFrequency( &freq );	// 計算機の周波数計測

	glutTimerFunc(50, timer, 0);

	/// 時刻のチェックと画面の更新を行う
	if (stepOnTimer){
		//for(int i = 0; i < 1; i++)
		scene->Step();

		// 位相生成
		QueryPerformanceCounter( &now );
		NowTime = (double)( now.QuadPart - start.QuadPart ) / (double)freq.QuadPart;	// 経過時間の計算
		s_time = (double)( now.QuadPart - past.QuadPart ) / (double)freq.QuadPart;
		past.QuadPart = now.QuadPart;

		Phase = (2 * PI / CycleTime) * NowTime;
	
	//直列結合
		for(int module_num=0; module_num<module_max; module_num++){
			for(int leg_num = 1; leg_num < 3; leg_num++){
			double	PhaseTemp;
			PhaseTemp = Phase + leg_num * 2 * PI / 2.0 + module_num * 2 * PI / 2.0;
			//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num];
			//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num] + leg_num * 2 * PI / 2.0 + module_num * 2 * PI / 2.0;
			//PhaseTemp[module_num * 2 + leg_num]= Phase[module_num * 2 + leg_num] + leg_num * 2 * PI / 2.0;

			// 足先軌道の計算（楕円軌道）
			double	xx = para_X1;
			//double	yy = pow(-1, leg_num) * para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
			double	yy; 
					if(turn_flag%2 == 0)yy = pow(-1, leg_num) * para_a1 * cos(PhaseTemp);
			        else if(turn_flag%2 == 1)yy = para_a1 * cos(PhaseTemp);
			//double	yy = pow(-1, module_num) * para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
			//double	yy = para_a1 * cos(PhaseTemp[module_num * 2 + leg_num]);
			double	zz;
				if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
				//if(sin(PhaseTemp[module_num * 2 + leg_num]) < 0.0)	zz = - para_H1;
				else zz = para_b1 * sin(PhaseTemp) - para_H1;
				//else zz = para_b1 * sin(PhaseTemp[module_num * 2 + leg_num]) - para_H1;

			// 各関節角度の計算
			double	theta1 = CalcTheta1(xx, yy);
			if(leg_num)theta1 += pow(-1, leg_num) * PI / 6.0;
			double	theta2 = CalcTheta2(xx, yy, zz, L1, L2, L3);
			double	theta3 = CalcTheta3(xx, yy, zz, L1, L2, L3);

			// グラフィック表示のために各値をセット
			robot[module_num].leg[leg_num].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
			robot[module_num].leg[leg_num].jntDX2  ->  SetSpringOrigin(-theta2);
			robot[module_num].leg[leg_num].jntFoot ->  SetSpringOrigin(-theta3);
			}
		}

	//バトルドロイド
		// module0（脚）
			/*for(int leg_num = 1; leg_num < 3; leg_num++){

				double	PhaseTemp = Phase + leg_num * 2 * PI / 2.0;

				// 足先軌道の計算（楕円軌道）
				double	zz = - para_a1 * cos(PhaseTemp);
				double	yy = 0.0;
				double	xx;
					if(sin(PhaseTemp) < 0.0)	xx = - para_H1;
					else						xx = para_b1 * sin(PhaseTemp) - para_H1;

				// 各関節角度の計算
				double	theta1 = 0.0;
				if(leg_num)
					theta1 -= pow(-1, leg_num) * PI / 3.0;
				double	theta2 = atan(zz / xx) + acos( (L2*L2 + xx*xx + zz*zz - L3*L3) / (2 * L2 * sqrt(xx*xx + zz*zz)) );
				double	theta3 = - PI + acos( (L2*L2 + L3*L3 - xx*xx - zz*zz) / (2 * L2 * L3) );

				// グラフィック表示のために各値をセット
				robot[0].leg[leg_num].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
				robot[0].leg[leg_num].jntDX2  ->  SetSpringOrigin(-theta2);
				robot[0].leg[leg_num].jntFoot ->  SetSpringOrigin(-theta3);

			}

			// module1（腕，顔）
			double	theta0 = -20.0 * PI / 180.0 * sin(Phase);
			robot[1].leg[0].jntDX1  ->  SetSpringOrigin(-theta0-Rad(90));

			for(int leg_num = 1; leg_num < 3; leg_num++){

				double	PhaseTemp = Phase + leg_num * 2 * PI / 2.0 + 2 * PI / 2.0;

				// 手軌道の計算（楕円軌道）
				double	xx = para_X1;
				double	yy = pow(-1, leg_num) * para_a1 * cos(PhaseTemp);
				double	zz = para_b1 * sin(PhaseTemp) - 0.10;
					//if(sin(PhaseTemp) < 0.0)	zz = - 0.12;
					//else						zz = para_b1 * sin(PhaseTemp) - 0.12;

				// 各関節角度の計算
				double	theta1 = 0.0;
				//if(leg_num)
				//	theta1 += pow(-1, leg_num) * PI / 3.0;
				double	theta2 = CalcTheta2(xx, yy, zz, L1, L2, L3);
				double	theta3 = CalcTheta3(xx, yy, zz, L1, L2, L3);

				// グラフィック表示のために各値をセット
				robot[1].leg[leg_num].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
				robot[1].leg[leg_num].jntDX2  ->  SetSpringOrigin(-theta2);
				robot[1].leg[leg_num].jntFoot ->  SetSpringOrigin(-theta3);

			}*/

	//３モジュール環状
		//module0//leg2
		/*double	PhaseTemp = Phase;

		// 足先軌道の計算（楕円軌道）
		double	xx = 0.0;
		double	yy = pow(-1, 2) * para_a1 * cos(PhaseTemp);
		double	zz;
			if(sin(PhaseTemp) < 0.0)	zz =  - para_H1;
			else						zz =  para_b1 * sin(PhaseTemp) - para_H1;
		double  YY = xx * cos(direction_theta + PI/2.0) - yy * sin(direction_theta + PI/2.0);
		double  XX = xx * sin(direction_theta + PI/2.0) + yy * cos(direction_theta + PI/2.0) + para_X1;

		// 各関節角度の計算
		double	theta1 = CalcTheta1(XX, YY);
		double	theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		double	theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[0].leg[2].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[0].leg[2].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[0].leg[2].jntFoot ->  SetSpringOrigin(-theta3);

		//leg1
		PhaseTemp = Phase + 1 * 2 * PI / 2.0;

		// 足先軌道の計算（楕円軌道）
		xx = 0.0;
		yy = pow(-1, 2) * para_a1 * cos(PhaseTemp);
		zz;
			if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
			else						zz = para_b1 * sin(PhaseTemp) - para_H1;
		YY = xx * cos(direction_theta - PI/6) - yy * sin(direction_theta - PI/6);
		XX = xx * sin(direction_theta - PI/6) + yy * cos(direction_theta - PI/6) + para_X1;

		// 各関節角度の計算
		theta1 = CalcTheta1(XX, YY);
		theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[0].leg[1].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[0].leg[1].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[0].leg[1].jntFoot ->  SetSpringOrigin(-theta3);

	//module1//leg2
		PhaseTemp = Phase;

		// 足先軌道の計算（楕円軌道）
			xx = 0.0;
			yy = pow(-1, 1) * para_a1 * cos(PhaseTemp);
			zz;
				if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
				else						zz = para_b1 * sin(PhaseTemp) - para_H1;
			YY = xx * cos(direction_theta + PI/6) - yy * sin(direction_theta + PI/6);
			XX = xx * sin(direction_theta + PI/6) + yy * cos(direction_theta + PI/6) + para_X1;

		// 各関節角度の計算
		theta1 = CalcTheta1(XX, YY);
		theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[1].leg[2].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[1].leg[2].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[1].leg[2].jntFoot ->  SetSpringOrigin(-theta3);

		//leg1
		PhaseTemp = Phase + 1 * 2 * PI / 2.0;

		// 足先軌道の計算（楕円軌道）
		xx = 0.0;
		yy = pow(-1, 0) * para_a1 * cos(PhaseTemp);
		zz;
			if(sin(PhaseTemp) < 0.0)	zz =  - para_H1;
			else						zz =  para_b1 * sin(PhaseTemp) - para_H1;
		YY = xx * cos(direction_theta + PI/2.0) - yy * sin(direction_theta + PI/2.0);
		XX = xx * sin(direction_theta + PI/2.0) + yy * cos(direction_theta + PI/2.0) + para_X1;

		// 各関節角度の計算
		theta1 = CalcTheta1(XX, YY);
		theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[1].leg[1].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[1].leg[1].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[1].leg[1].jntFoot ->  SetSpringOrigin(-theta3);

	// module2//leg2
		PhaseTemp = Phase;

		// 足先軌道の計算（楕円軌道）
			xx = 0.0;
			yy = pow(-1, 1) * para_a1 * cos(PhaseTemp);
			zz;
				if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
				else						zz = para_b1 * sin(PhaseTemp) - para_H1;
			YY = xx * cos(direction_theta - PI/6) - yy * sin(direction_theta - PI/6);
			XX = xx * sin(direction_theta - PI/6) + yy * cos(direction_theta - PI/6) - para_X1;

		// 各関節角度の計算
		theta1 = CalcTheta1(XX, YY);
		theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[2].leg[2].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[2].leg[2].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[2].leg[2].jntFoot ->  SetSpringOrigin(-theta3);

	//leg1
		PhaseTemp = Phase + 2 * PI / 2.0;

		// 足先軌道の計算（楕円軌道）
		xx = 0.0;
		yy = pow(-1, 1) * para_a1 * cos(PhaseTemp);
		zz;
			if(sin(PhaseTemp) < 0.0)	zz = - para_H1;
			else						zz = para_b1 * sin(PhaseTemp) - para_H1;
		YY = xx * cos(direction_theta + PI/6) - yy * sin(direction_theta + PI/6);
		XX = xx * sin(direction_theta + PI/6) + yy * cos(direction_theta + PI/6) + para_X1;

		// 各関節角度の計算
		theta1 = CalcTheta1(XX, YY);
		theta2 = CalcTheta2(XX, YY, zz, L1, L2, L3);
		theta3 = CalcTheta3(XX, YY, zz, L1, L2, L3);

		robot[2].leg[1].jntDX1  ->  SetSpringOrigin(-theta1-Rad(90));
		robot[2].leg[1].jntDX2  ->  SetSpringOrigin(-theta2);
		robot[2].leg[1].jntFoot ->  SetSpringOrigin(-theta3);*/

		glutPostRedisplay();
	}
}

/* brief		メイン関数 */
int main(int argc, char* argv[]){


	//CPG系初期化
	for(int i=1; i<7; i++){
		ue[i]=0.0; uf[i]=0.0;
		ve[i]=ue[i]; vf[i]=uf[i];
		sume[i]=0.0; sumf[i]=0.0;
	}

	ue[1]=0.5;	//CPG活性化エネルギー？これがないと振動しません。

	for(int i=1; i<7; i++){//結合係数を初期化
		for(int j=1; j<7; j++){
			w[i][j]=0.0;
		}
	}

	for(int i=1; i<7; i++)smp[i] = 0;
	//for(int i=1; i<7; i++)n[i] = 0;

	////結合係数を設定　２個目の数字が影響を受けるCPGの番号
	w[1][3] = 0.5; w[3][1] = 0.5;
	w[2][4] = 0.5; w[4][2] = 0.5;
	w[1][2] = 0.5; w[2][1] = 0.5;
	w[3][4] = 0.5; w[4][3] = 0.5;

	w[3][5] = 0.5; w[5][3] = 0.5;
	w[4][6] = 0.5; w[6][4] = 0.5;
	w[6][5] = 0.5; w[5][6] = 0.5;

	w[1][6] = 0.5; w[6][1] = 0.5;
	w[2][5] = 0.5; w[5][2] = 0.5;

	// SDKの作成　
	phSdk = PHSdkIf::CreateSdk();
	grSdk = GRSdkIf::CreateSdk();
	// シーンオブジェクトの作成
	PHSceneDesc dscene;
	dscene.timeStep = 0.05;
	dscene.numIteration = 5;
	scene = phSdk->CreateScene(dscene);			// シーンの作成
	scene->SetGravity(Vec3f(0.0, -9.8, 0.0));	//	重力を設定

	// シーンの構築
	CreateFloor();								//	床

	Posed pose;
	pose.Ori() = Quaterniond::Rot(Rad(120.0), 'y');
	for(int i=0; i<module_max; i++){			//ASHIGARU構築
		pose.Pos() = Vec3d(0.3*i, 0.17, 0.0);
		robot[i].Build_root(pose, scene, phSdk);
	}

	//３モジュール環状のときはこの初期描画で
	/*pose.Pos() = Vec3d(0.0, 0.17, 0.0);
	robot[0].Build_root(pose, scene, phSdk);
	pose.Ori() = Quaterniond::Rot(Rad(240.0), 'y');
	pose.Pos() = Vec3d(0.2, 0.17, 0.0);
	robot[1].Build_root(pose, scene, phSdk);
	pose.Ori() = Quaterniond::Rot(Rad(360.0), 'y');
	pose.Pos() = Vec3d(0.1, 0.17, -0.173);
	robot[2].Build_root(pose, scene, phSdk);*/

	PHHingeJointDesc jdConnect;

	double K = 10.0, D = 0.0;//結合部分をガッチガチに

	std::vector<PHSolidIf*> allSolids;
	for(int i=0; i<module_max; i++){
		allSolids.push_back(robot[i].soBody);
		for(int j=0; j<3; ++j){
			allSolids.push_back(robot[i].leg[j].soDX1);
			allSolids.push_back(robot[i].leg[j].soDX2);
			allSolids.push_back(robot[i].leg[j].soFoot);
		}
	}
	scene->SetContactMode(&allSolids[0], allSolids.size(), PHSceneDesc::MODE_NONE);

	for(int i=0; i<module_max; i++){
		robot[i].leg[0].jntDX1  -> SetSpringOrigin(Rad(-90.0));//結合脚(leg[0])の姿勢を設定
		robot[i].leg[0].jntDX1  -> SetSpring(K);
		robot[i].leg[0].jntDX1  -> SetDamper(D);
		robot[i].leg[0].jntDX2  -> SetSpringOrigin(Rad(-90.0));
		robot[i].leg[0].jntDX2  -> SetSpring(K);
		robot[i].leg[0].jntDX2  -> SetDamper(D);
		robot[i].leg[0].jntFoot -> SetSpringOrigin(Rad(180.0));
		robot[i].leg[0].jntFoot -> SetSpring(K);
		robot[i].leg[0].jntFoot -> SetDamper(D);
	}
	/*robot[1].leg[0].jntDX2  -> SetSpringOrigin(Rad(0.0));
	robot[1].leg[0].jntDX2  -> SetSpring(K);
	robot[1].leg[0].jntDX2  -> SetDamper(D);
	robot[1].leg[0].jntFoot -> SetSpringOrigin(Rad(120.0));
	robot[1].leg[0].jntFoot -> SetSpring(K);
	robot[1].leg[0].jntFoot -> SetDamper(D);*/

//////モジュール直列結合構築/////////////////////////////////////////////////////////////////////////

	for(int i=0; i<module_max; i++){
		robot[i].leg[1].jntDX1 -> SetSpringOrigin(Rad(-60.0));
		robot[i].leg[2].jntDX1 -> SetSpringOrigin(Rad(-120.0));
	}

	//結合部分構築
	jdConnect.poseSocket.Ori() = Quaterniond();
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	for(int i=0; i<module_max-1; i++){
		//robot[i].leg[0].jntConnect[0] = scene->CreateJoint(robot[i+1].soBody, robot[i].soBody, jdConnect)->Cast();
		//robot[i].leg[0].jntConnect[0]->SetSpring(K);
		//robot[i].leg[0].jntConnect[0]->SetDamper(D);
		//robot[i].leg[0].jntConnect[0]->SetSpringOrigin(Rad(0.0));
		robot[i].jntConnect = scene->CreateJoint(robot[i+1].soBody, robot[i].soBody, jdConnect)->Cast();
		robot[i].jntConnect->SetSpring(K);
		robot[i].jntConnect->SetDamper(D);
		robot[i].jntConnect->SetSpringOrigin(Rad(0.0));
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////３モジュール環状結合////////////////////////////////////////////////////////////////////////////////////////////

	//結合部分構築
	/*jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[0].jntConnect = scene->CreateJoint(robot[1].soBody, robot[0].soBody, jdConnect)->Cast();
	robot[0].jntConnect->SetSpring(K);
	robot[0].jntConnect->SetDamper(D);
	robot[0].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);
	robot[1].jntConnect = scene->CreateJoint(robot[2].soBody, robot[1].soBody, jdConnect)->Cast();
	robot[1].jntConnect->SetSpring(K);
	robot[1].jntConnect->SetDamper(D);
	robot[1].jntConnect->SetSpringOrigin(Rad(0.0));
	jdConnect.poseSocket.Ori() = Quaterniond::Rot(Rad(-120.0), 'y');
	jdConnect.poseSocket.Pos() = Vec3d(0.0, 0.0, 0.0);
	jdConnect.posePlug.Pos() = Vec3d(-0.1, 0.0, 0.173);*/
	/*robot[2].jntConnect = scene->CreateJoint(robot[0].soBody, robot[2].soBody, jdConnect)->Cast();
	robot[2].jntConnect->SetSpring(K);
	robot[2].jntConnect->SetDamper(D);
	robot[2].jntConnect->SetSpringOrigin(Rad(0.0));*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	GLUTの初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	int window = glutCreateWindow("ASHIGARU");

	render = grSdk->CreateDebugRender();
	device = grSdk->CreateDeviceGL();
	device->Init();
	render->SetDevice(device);	// デバイスの設定

	glutTimerFunc(100, timer, 0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	initialize();

	glutMainLoop();
}
