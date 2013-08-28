#ifndef UTMMTIMER_H
#define UTMMTIMER_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/Env.h>
#include <iostream>

#if _MSC_VER >= 1700
//  #include <windows.h>
typedef	unsigned int UINT;
#if defined(_WIN64)
typedef	unsigned __int64 DWORD_PTR;
#else
typedef __w64 unsigned long DWORD_PTR;
#endif
#endif // _MSC_VER >= 1700

using namespace std; 

namespace Spr {

/**	マルチメディアタイマー.
	タイマー動作中にデバッガを使うとOSが落ちるので，デバッグ用に
	スレッドによる擬似動作モードを用意してある．	*/
typedef void MMTimerFunc(void* arg);
typedef void  SPR_CDECL MMTimerIdFunc(int id);

class SPR_DLL UTMMTimer{
	static int count;
	int tick;
public:
	//typedef void MMTimerFunc(void* arg);

protected:
	static unsigned resolution;		///<	タイマーの時間精度
	unsigned interval;					///<	タイマーの時間間隔
	MMTimerFunc* func;				///<	時間が来るたびに呼ばれるコールバック関数．
	MMTimerIdFunc* idFunc;				///<	時間が来るたびに呼ばれるコールバック関数．
	void* arg;								///<	コールバック関数に渡す引数
	int id;								///<	コールバック関数に渡すint型引数
	unsigned timerID;
	volatile bool bCreated;				///<	タイマーが動作しているかどうか
	volatile bool bThread;				///<	タイマーがスレッドとして擬似動作しているかどうか
    volatile bool bRun;					///<	コールバック関数が実行中かどうか
	void* hThread;						///<	擬似動作用スレッド
#if (_MSC_VER >= 1700)
	static void SPR_STDCALL TimerCallback(UINT uID, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR);
#else
	static void SPR_STDCALL TimerCallback(unsigned uID, unsigned, unsigned long dwUser, unsigned long, unsigned long);
#endif
	static unsigned long SPR_STDCALL ThreadCallback(void* arg);
	void BeginPeriod();
	void EndPeriod();

	public:
	volatile int heavy;

	UTMMTimer();							///<	コンストラクタ
	~UTMMTimer();							///<	デストラクタ

	unsigned Resolution();					///<	タイマーの時間精度
	void Resolution(unsigned res);		///<	タイマーの時間精度の設定
	unsigned Interval();						///<	タイマーの時間間隔
	void Interval(unsigned i);				///<	タイマーの時間間隔の設定
	void Set(MMTimerFunc* f, void* arg);
	void Set(MMTimerIdFunc* f, int i);
													///<	コールバック関数の設定
	void Init(unsigned int r, unsigned int i, MMTimerFunc* f, void* arg);				///< 時間精度，間隔，コールバック関数を一度で設定 
	bool Create();								///<	タイマー動作開始
	bool Thread();							///<	タイマー擬似動作開始
	bool IsCreated(){return bCreated;}
													///<	タイマーが動作しているかどうか
	bool IsThread(){return bThread;}
													///<	タイマーが擬似動作しているかどうか
	void Release();							///<	タイマーの停止
};

}	//	namespace Spr
#endif
