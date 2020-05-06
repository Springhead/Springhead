// FWAppGLtest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#ifndef FWAPPGLTEST_H
#define FWAPPGLTEST_H


#include <Springhead.h>
#include <Framework/SprFWApp.h>
using namespace Spr;

#define TEST_CASE 4

//人間モデルのロード
#if defined(TEST_CASE) && (TEST_CASE == 0)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "Walk"
#define FRAME_NUMBER 72

#elif defined(TEST_CASE) && (TEST_CASE == 1)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "Run"
#define FRAME_NUMBER 48

#elif defined(TEST_CASE) && (TEST_CASE == 2)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "WaveOnesArm"
#define FRAME_NUMBER 48

#elif defined(TEST_CASE) && (TEST_CASE == 3)
#define FILE_NAME "test2.x"
#define ANIMATION_SET_NAME "RunGround"
#define FRAME_NUMBER 72

#elif defined(TEST_CASE) && (TEST_CASE == 4)
#define FILE_NAME "testkobito.x"
#define ANIMATION_SET_NAME "push"
#define FRAME_NUMBER 60

#endif

class MyApp;
class MyApp: public FWApp{
public:
	int texSize;
	unsigned char	bits[256][256][4];
	unsigned int	texName;

	virtual void Init(int argc, char* argv[]);
	virtual void Display();
	virtual void TimerFunc(int id);
	virtual void Keyboard(int key, int x, int y);
	//void TexInit();
	//void DrawTexQuad();
};

#endif