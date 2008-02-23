/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef MYAPP_H
#define MYAPP_H


#include <Springhead.h>
#include <SprFramework.h>
#include <Framework/SprFWAppGLUT.h>

#include <GL/glut.h>
#include <vector>
#include <windows.h>

using namespace Spr;

/** @brief MYAppクラスはFWAppGLUTまでで定義されている関数たちを
		   オーバーライドして使うためのクラス
*/
class MYApp : public FWAppGLUT, public UTRefCount{

public:
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//メンバ変数
	FWWin*								window;			//< 表示するウィンドウのハンドル
	FWSceneIf*							fwScene;		//< FWScene(= PHScene + GRScene)のインスタンス
	PHSceneIf*							phScene;		//< PHSceneのインスタンス
	GRRenderIf*							grRender;		//< GRRenderのインスタンス
	GRDeviceGLIf*						deviceGL;		//< GLのデバイスハンドル
	float								camHeight;		//< カメラの高さ(TopView)
	float								camTopAngle;	//< TopViewのカメラ角度
	double								dt;				//< 積分幅
	double								nIter;			//< LCP反復回数
	bool								bStep;			//< シミュレーションを行うかどうか
	bool								flagDebugMode;	//< シーンをデバッグモードで描画するかのフラグ
	const char*							filePass;		//< xFileが置いてあるフォルダへの相対パス
	const char*							fileName;		//< ロードするファイル名
	const char*							phCookieName;	//< クッキーの衝突判定形状の名前
	const char*							grCookieName;	//< クッキーの見た目の名前
	std::vector< UTRef< FWObjectIf > >	copiedCookies;	//< コピーされたクッキーを格納する配列

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//コンストラクタ・デストラクタ
	MYApp();

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//新しく定義した関数の宣言
	static MYApp* GetInstance(){
		return (MYApp*) instance;
	}
	bool CopyTheCookie(const char* phName, const char* grName);		//< //引数の名前の剛体を探してきて，同じデータのものを複製する関数
	void ResetScene();
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//オーバーライド
	//FWに関する関数
	void Init(int argc, char* argv[]);				//< 初期化関数のオーバーライド宣言
	void Step();									//< 毎ステップシミュレーションとして呼ばれる関数のオーバーライド宣言
	//GLUT周りの関数
	void Keyboard(unsigned char key, int x, int y);	//< キーボードイベントを処理する関数のオーバーライド宣言
	void Display();									//< 描画イベントを処理する関数のオーバーライド宣言
};

#endif