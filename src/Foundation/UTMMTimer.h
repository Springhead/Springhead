#ifndef UTMMTIMER_H
#define UTMMTIMER_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
namespace Spr {

/**	マルチメディアタイマー.
	タイマー動作中にデバッガを使うとOSが落ちるので，デバッグ用に
	スレッドによる擬似動作モードを用意してある．	*/
class SPR_DLL UTMMTimer{
	static int count;
	int tick;
public:
	typedef void MMTimerFunc(void* arg);

protected:
	static unsigned resolution;		///<	タイマーの時間精度
	unsigned interval;				///<	タイマーの時間間隔
	MMTimerFunc* func;			///<	時間が来るたびに呼ばれるコールバック関数．
	void* arg;					///<	コールバック関数に渡す引数
	unsigned timerID;
	volatile bool bCreated;		///<	タイマーが動作しているかどうか
	volatile bool bThread;		///<	タイマーがスレッドとして擬似動作しているかどうか
    volatile bool bRun;			///<	コールバック関数が実行中かどうか
	void* hThread;				///<	擬似動作用スレッド
	static void __stdcall TimerCallback(unsigned uID, unsigned, unsigned long dwUser, unsigned long, unsigned long);
	static unsigned long __stdcall ThreadCallback(void* arg);
	void BeginPeriod();
	void EndPeriod();

	public:
	volatile int heavy;

	UTMMTimer();					///<	コンストラクタ
	~UTMMTimer();					///<	デストラクタ
	unsigned Resolution();				///<	タイマーの時間精度
	void Resolution(unsigned res);		///<	タイマーの時間精度の設定
	unsigned Interval();				///<	タイマーの時間間隔
	void Interval(unsigned i);			///<	タイマーの時間間隔の設定
	void Set(MMTimerFunc* f, void* arg);
									///<	コールバック関数の設定
	bool Create();					///<	タイマー動作開始
	bool Thread();					///<	タイマー擬似動作開始
	bool IsCreated(){return bCreated;}
									///<	タイマーが動作しているかどうか
	bool IsThread(){return bThread;}
									///<	タイマーが擬似動作しているかどうか
	void Release();					///<	タイマーの停止
};

}	//	namespace Spr
#endif
