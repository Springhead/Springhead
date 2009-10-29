#ifndef FWGLUT_H
#define FWGLUT_H
#include <vector>
#include <GL/glut.h>
#include <Framework/SprFWGraphicsAdaptee.h>
#include <Framework/SprFWApp.h>

using namespace std;

namespace Spr{;

class FWWinGLUT : public FWWin{
public:
	FWWinGLUT(int wid, const FWWinDesc& d, GRRenderIf* r):
	  FWWin(wid, d, r){}
	void FullScreen();
	void Position(int left, int top);
	void Reshape(int width, int height);
	void SetTitle(UTString t);
};


class FWGLUTDesc{
public:
	FWGLUTDesc();
};

class FWGLUT : public FWGraphicsAdaptee, public FWGLUTDesc{
protected:
	/** コールバック関数*/
	static FWGLUT* instance;
	static void SPR_CDECL GlutDisplayFunc();
	static void SPR_CDECL GlutReshapeFunc(int w, int h);
	static void SPR_CDECL GlutTimerFunc(int id);
	static void SPR_CDECL GlutIdleFunc();
	static void SPR_CDECL GlutKeyboardFunc(unsigned char key, int x, int y);
	static void SPR_CDECL GlutSpecialFunc(int key, int x, int y);
	static void SPR_CDECL GlutMouseFunc(int button, int state, int x, int y);
	static void SPR_CDECL GlutMotionFunc(int x, int y);
	static void SPR_CDECL GlutJoystickFunc(unsigned int buttonMask, int x, int y, int z);
	static void SPR_CDECL AtExit();
public:	
	FWGLUT();
	~FWGLUT();

	///	GLUTの初期化を行う。最初にこれを呼ぶ必要がある。
	virtual void Init(int argc, char* argv[]);
	
	/** タイマ */
	///GLUTによるIdleFuncをスタートする
	virtual void StartIdleFunc();
	///GLUTによるTimerをスタートする
	virtual void StartMainLoop();

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	virtual FWWin* CreateWin(const FWWinDesc& d=FWWinDesc());
	///	ウィンドウを破棄する
	virtual void DestroyWin(FWWin* w);
	///	カレントウィンドウを設定する
	virtual void SetCurrentWin(FWWin* w);
	///	カレントウィンドウを返す。
	virtual FWWin* GetCurrentWin();	
	///	カレントウィンドウを返す。
	virtual int GetWinFromId();
	///カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	virtual void PostRedisplay();
	/// Shift,Ctrl,Altのステートを返す
	virtual int Modifiers();

};

}





#endif