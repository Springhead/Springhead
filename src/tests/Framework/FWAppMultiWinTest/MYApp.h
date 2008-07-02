#ifndef MYAPP_H
#define MYAPP_H

#include <Framework/SprFWAppGLUT.h>
#include <Springhead.h>
#include <cstdlib>

using namespace Spr;

class MYApp : public FWAppGLUT, public UTRefCount{
	
private:
	double					dt;					//< 積分幅
	double					nIter;				//< LCP反復回数

	FWWin*					window[2];			//< 作ったウィンドウ[0]:上画面，[1]:下画面
	FWSceneIf*				fwScene[2];			//< Physicsのシーングラフ[0]:上画面，[1]:下画面
	const char*				winName[2];			//< Windowの名前[0]:上画面，[1]:下画面

	float					camAngle[2];		//< カメラの角度[0]:上画面，[1]:下画面
	float					camZoom[2];			//< カメラの倍率[0]:上画面，[1]:下画面
	Affinef					view[2];			//< カメラ位置の回転・ズームを行う時の行列用の変数[0]:上画面，[1]:下画面


public:
	
	// コンストラクタ
	MYApp();

	// このクラス内で新しく宣言する関数
	void MyRenderInit(FWWin* window, int winNumber);

	// インタフェース関数
	double	GetTimeStep()					{return dt;}
	void	SetTimeStep(double data)		{dt = data;}
	double	GetNumIteration()				{return nIter;}
	void	SetNumIteration(double data)	{nIter = data;}
	
	// 基底のクラスで宣言された関数のうちオーバーロードする関数
	void Init(int argc, char* argv[]);
	void Keyboard(int key, int x, int y);
	//void Display();
	
};

#endif