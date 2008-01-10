/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef MYGLOBALVALUE_H
#define MYGLOBALVALUE_H

#include <Springhead.h>
#include <vector>
#include <cstdlib>
#include <gl/glui.h>
//
// --- --- --- --- ---
// using
using namespace Spr;
using namespace std;

// --- --- --- --- ---
// special definition
#define ESC 27
#define SO_NSOLIDS 27
#define JO_NJOINTS 26
#define JO_NBALLJOINTS 14
#define JO_NHINGEJOINTS 8

// --- --- --- --- ---
// グローバル変数定義

/// --- Springhead SDK
UTRef<PHSdkIf>   phSdk;								//物理シミュレータを用いるための変数
PHSceneIf*       phScene;							//物理シミュレータ上にオブジェクト（シーングラフ）を登録するための変数			

UTRef<GRSdkIf>   grSdk;								//描画をするための変数
GRDebugRenderIf* grRender;							//描画関数をいじるための変数

GRDeviceGLIf*    grDevice;							//OpenGLデバイスをいじるための変数

/// --- Springhead 設定
const double simulationPeriod = 8.0;				//glutTimerFuncを呼ぶ周期（msec）
const double dt               = 0.01;				//シミュレーションの積分幅
const int    niter            = 200;				//LCPはんぷくかいすう
/// --- Creatureクラス用の変数
UTRef<CRCreatureIf>                creature;		//クリーチャー生成用の変数
UTRef<CRFourLegsAnimalBodyIf>	   animalBody1;		//動物の体をいじるための変数
UTRef<CRTryStandingUpControllerIf> standUpCtrl;		//立とうとするコントローラをいじるための変数

std::vector<PHSolidIf*>			solids;				//GUIに剛体の情報を持たせるための変数
std::vector<CDBoxIf*>			box;				//GUIに剛体の箱の情報を持たせるための変数
std::vector<PHBallJointIf*>		ballJoints;			//GUIにボールジョイントの情報を持たせるための変数
std::vector<PHHingeJointIf*>	hingeJoints;		//GUIにヒンジジョイントの情報を持たせるための変数

/// --- GLUI
std::vector<int>	solidsSpace;
std::vector<int>	ballJointsSpace;
std::vector<int>	hingeJointsSpace;
int		nowSolid			  = 0;									//< 現在編集できるSolidの番号
int		nowBallJoint		  = 0;									//< 現在編集できるBallJointの番号
int		nowHingeJoint		  = 0;									//< 現在編集できるHingeJointの番号
Vec3f	boxsize				  = Vec3f(0.0, 0.0, 0.0);				//< 編集してリアルタイムで値の変わる箱の大きさ(breadth, height, thickness)
Vec3f	defaultBoxsize;												//< default****[i]に****[i]を代入してResetするときのために取っておく
float	ballJointData[5]	  = {0.0, 0.0, 0.0, 0.0, 0.0};			//< spinnerに入っている値のコピー、リアルタイムに値が変わる（spring, damper, limitSwing.upper, limitTwist.lower, limitTwist.upper)
float	defaultBallJointData[5];									//< ロードした時の↑の要素の値を保存しておくための配列
float	rotationBallJoint[16] = {1.0, 0.0, 0.0, 0.0,				//< 関節のQuaternionをglMultiMatrix()に代入できるAffine行列に直すための変数。gluiから変更する可能性があるため要素数[16]の一次元配列になっている
								 0.0, 1.0, 0.0, 0.0,
								 0.0, 0.0, 1.0, 0.0,
								 0.0, 0.0, 0.0, 1.0};				
Affinef				matMadeByQuaterinon;							//< Euler系でのBallJointのRotation Matrix（Quaternion goalをEuler系の回転行列に変換したものを保存しておく変数）
Affinef				goalEulerMatrix;								//< 設定する制御目標のEuler系の行列

float				hingeJointData[4]	  = {0.0, 0.0, 0.0, 0.0};	//< spinnerに入っている値のコピー、リアルタイムに値が変わる（spring, damper, lower, upper, origin）
float				defaultHingeJointData[4];						//< ロードした時の↑の要素の値を保存しておくための配列
float				hingeJointGoal;									//< HingeJointの制御目標値(rad)
float				defaultHingeJointGoal;							//< ロードした時の↑の要素の値を保存しておくための変数
float				mass = 0;										//< 現在編集できるSolidの質量
float				defaultMass;									//< ロードした時の↑の要素の値を保存しておくための変数
int					dynamicalValue		= 0;						//< Horse Body Simulatorの腰が力の情報に対して動くかどうか
int					counterKeyframe		= 0;						//< 今までに登録したkeyframeの数
int					counterStep	= 0;

GLUI*			glui;												//< gluiのインスタンス
GLUI_Rollout*	menu;												//< menuのインスタンス
GLUI_Panel*		settingModel;										//< menu内のモデルの設定部分をまとめるインスタンス

//Solid
GLUI_Rollout*	rollSolid;											//< settingModel内のSolid部分のロールアウトを管理するインスタンス
GLUI_Panel*		solidDataArea;										//< rollSolid内のSolid情報を表示する領域を管理するインスタンス
GLUI_Listbox*	listBoxSolid;										//< rollSolid内でどのSolidを編集するかを選ぶリストボックスを管理するインスタンス
GLUI_Spinner*	spinnerMass;										//< rollSolid内のspinnerに表示される質量情報を管理するインスタンス
GLUI_Spinner*	spinnerBoxsize[3];									//< rollSolid内のspinnerに表示される箱の大きさを管理するためのインスタンス(Breath,Height,Thickness)

//Ball Joint
GLUI_Rollout*	rollBallJoint;										//< settingModel内のBallJoint部分のロールアウトを管理するインスタンス
GLUI_Panel*		ballJointDataArea;									//< rollBallJoint内のBallJoint情報を表示する領域を管理するインスタンス
GLUI_Listbox*	listBoxBallJoint;									//< rollBallJoint内でどのBallJointを編集するかを選ぶリストボックスを管理するインスタンス
GLUI_Spinner*	spinnerDataBallJoint[5];							//< rollBallJoint内のspinnerに表示される情報を管理するインスタンス(spring, damper, swingUpper, twistLower, twistUpper)


//Hinge Joint
GLUI_Rollout*	rollHingeJoint;										//< settingModel内のBallJoint部分のロールアウトを管理するインスタンス
GLUI_Panel*		hingeJointDataArea;									//< rollHingeJoint内のBallJoint情報を表示する領域を管理するインスタンス
GLUI_Listbox*	listBoxHingeJoint;									//< rollHingeJoint内でどのHingeJointを編集するかを選ぶリストボックスを管理するインスタンス
GLUI_Spinner*	spinnerDataHingeJoint[4];							//< rollHingeJoint内のspinnerに表示される情報を管理するインスタンス(spring, damper, lower, upper)
GLUI_Spinner*	spinnerDataHingeJointGoal;							//< rollHingeJoint内のspinnerに表示される情報を管理するインスタンス(origin)

int mainWindow	= 0;							//< Horse Body Simulatorのウィンドウを管理するインスタンス

/// --- グローバルフラグ
bool bTimer = true;								//	タイマーでシミュレーションを進めるかどうか

/// --- 視点
float   camAngle  = 0.0f;						//< Horse Body Simulatorのカメラの角度
float   camZoom   = 2.0f;						//< Horse Body Simulatorのカメラの倍率
Affinef view;									//< Horse Body Simulatorのカメラ位置の回転・ズームを行う時の行列用の変数

////////////////////////////////////////////////////////////////////////////
// Springheadとは関係ないポインタのメモリ開放は手動で行わなければならない //
// 手動で行うのは動的確保した自分でポインタだけ,下のPressQuit()に書く	  //
////////////////////////////////////////////////////////////////////////////

//Keyboardから呼ばれる関数
void PressQuit(){
	
	//////////////////////////
	//						//
	// ここでメモリ開放する //
	//						//
	//////////////////////////

	exit(0);
}

// GUIから呼ばれる関数
void PressQuit(int control){
	PressQuit();
}

#endif