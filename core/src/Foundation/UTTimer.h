#ifndef UTTimer_H
#define UTTimer_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/Env.h>
#include <Foundation/Object.h>
#include <Foundation/SprUTTimer.h>

namespace Spr {

class UTTimerProvider;


/**	統合タイマー
	・（危険かもしれないが）1ms 程度の精度を持つマルチメディアタイマー
	・スレッドを使用した擬似タイマー
	・GLUTなどのフレームワークに依存したメッセージパッシング型のタイマー
	・アイドル処理
	などを統合したタイマー	*/

class SPR_DLL UTTimer : public Object{
public:
	SPR_OBJECTDEF(UTTimer);

protected:
	unsigned					interval;			///<	タイマー呼び出し間隔(ms)
	unsigned					resolution;			///<	タイマー呼び出しの解像度(ms)
	UTTimerIf::TimerFunc		func;				///<	時間が来るたびに呼ばれるコールバック関数．
	void*						arg;				///<	コールバック関数に渡す引数
	volatile UTTimerIf::Mode	mode;				///<	タイマーの動作モード
	volatile bool				bStarted;			///<	タイマーがON（定期的にコールバックを呼び出す状態）なら true
	volatile bool				bRunning;			///<	コールバック呼び出し中ならば true

public:
	unsigned					timerId;			///<	UTTimerの通し番号
	unsigned					timerIdImpl;		///<	モード別に使われるタイマID．通し番号ではない
	UTTimerProvider*			provider;			///<	フレームワークタイマーが動いているとき、タイマーを提供しているフレームワークを覚えておくためのポインタ
	volatile bool				bStopThread;		///<	スレッドモードのスレッドに停止を指示するためのフラグ

public:
	UTTimer();
	~UTTimer();

	unsigned GetID(){return timerId; }

	unsigned GetResolution(){return resolution; }
	bool SetResolution(unsigned r);
	
	unsigned GetInterval(){ return interval;}
	bool SetInterval(unsigned i);
	
	bool SetCallback(UTTimerIf::TimerFunc f, void* arg=NULL);
	UTTimerIf::TimerFunc GetCallback();
	void* GetCallbackArg();
	
	UTTimerIf::Mode GetMode() { return mode; }
	bool SetMode(UTTimerIf::Mode m);

	bool IsStarted(){return bStarted;}
	bool IsRunning(){return bRunning;}
	
	bool Start();
	bool Stop();
	void Call();

protected:
	friend class UTTimerProvider;
	friend unsigned long SPR_STDCALL UTTimer_ThreadCallback(void* arg);
	friend void SPR_STDCALL UTTimer_MMTimerCallback(unsigned int uID, unsigned int, ulong_ptr dwUser, ulong_ptr, ulong_ptr);
};

///	フレームワークによるタイマー処理のためのコールバッククラス
class SPR_DLL UTTimerProvider{
public:
	UTTimerProvider();
	virtual ~UTTimerProvider();
	///	フレームワークをUTTimerに登録
	void Register();
	///	フレームワークをUTTiemrから登録解除
	void Unregister();
	
	///	タイマーの設定
	virtual bool StartTimer(UTTimer* timer)=0;
	///	タイマーの削除
	virtual bool StopTimer(UTTimer* timer)=0;
	///	Idle処理の呼び出し口。mode == IDLEのタイマーのコールバックを一通り呼び出す。
	static void CallIdle();
};


}	//	namespace Spr
#endif
