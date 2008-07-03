#ifndef MYAPP_H
#define MYAPP_H

#include <Framework/SprFWAppGLUT.h>
#include <Springhead.h>
#include <cstdlib>
#include <vector>

using namespace Spr;
using namespace std;

class MYApp : public FWAppGLUT, public UTRefCount{
	
private:
	double					dt;				//< 積分幅
	double					nIter;			//< LCP反復回数

	int						numWindow;		//< ウィンドウの数
	vector< FWWin* >		windows;		//< 作ったウィンドウ[0]:上画面，[1]:下画面
	vector< FWSceneIf* >	fwScenes;		//< Physicsのシーングラフ[0]:上画面，[1]:下画面
	vector< string >		winNames;		//< Windowの名前[0]:上画面，[1]:下画面

	vector< float >			camAngles;		//< カメラの角度[0]:上画面，[1]:下画面
	vector< float >			camZooms;		//< カメラの倍率[0]:上画面，[1]:下画面
	vector< Affinef	>		views;			//< カメラ位置の回転・ズームを行う時の行列用の変数[0]:上画面，[1]:下画面


public:
	
	// コンストラクタ
	MYApp();

	// このクラス内で新しく宣言する関数
	void MyRenderInit(FWWin* window, int num);

	// インタフェース関数
	double	GetTimeStep()					{return dt;}
	void	SetTimeStep(double data)		{dt = data;}
	double	GetNumIteration()				{return nIter;}
	void	SetNumIteration(double data)	{nIter = data;}
	
	// 基底のクラスで宣言された関数のうちオーバーロードする関数
	void Init(int argc, char* argv[]);
	void Keyboard(int key, int x, int y);
	
};

#endif