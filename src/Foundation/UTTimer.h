#ifndef UTTimer_H
#define UTTimer_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/Env.h>

namespace Spr {

class UTTimerProvider;


/**	統合タイマー
	・（危険かもしれないが）1ms 程度の精度を持つマルチメディアタイマー
	・スレッドを使用した擬似タイマー
	・GLUTなどのフレームワークに依存したメッセージパッシング型のタイマー
	・アイドル処理
	などを統合したタイマー	*/

class SPR_DLL UTTimer{
public:
	///	タイマーのモード
	enum Mode{
		MULTIMEDIA,
		THREAD,
		FRAMEWORK,
		IDLE
	};
	///	コールバック関数の型
	typedef void TimerFunc(void* arg);

protected:
	unsigned interval;								///<	タイマー呼び出し間隔(ms)
	unsigned resolution;							///<	タイマー呼び出しの解像度(ms)
	TimerFunc* func;								///<	時間が来るたびに呼ばれるコールバック関数．
	void* arg;										///<	コールバック関数に渡す引数
	volatile Mode mode;								///<	タイマーの動作モード
	volatile bool bStarted;							///<	タイマーがON（定期的にコールバックを呼び出す状態）なら true
	volatile bool bRunning;							///<	コールバック呼び出し中ならば true
	volatile int heavy;								///<	処理落ちフラグ。処理が時間をオーバーした場合に数値が増える

	//	以下、実装用作業領域
public:
	unsigned timerId;								///<	タイマーを識別するためのId
	UTTimerProvider* provider;						///<	フレームワークタイマーが動いているとき、タイマーを提供しているフレームワークを覚えておくためのポインタ
	volatile bool bStopThread;						///<	スレッドモードのスレッドに停止を指示するためのフラグ

public:
	UTTimer();										///<	コンストラクタ
	~UTTimer();										///<	デストラクタ

	unsigned GetResolution(){return resolution; }	///<	MULTIMEDIA時のタイマー精度の取得
	bool SetResolution(unsigned r);					///<	MULTIMEDIA時のタイマー精度の設定（MULTIMEDIAがサポートしない場合、設定した値から書き換わることがある。）
	unsigned GetInterval(){ return interval;}		///<	タイマーの時間間隔(ms)
	bool SetInterval(unsigned i);					///<	タイマーの時間間隔(ms)の設定
	bool SetCallback(TimerFunc* f, void* arg);		///<	コールバック関数の設定
	Mode GetMode() { return mode; }					///<	タイマーの動作モードの取得
	bool SetMode(Mode m);							///<	タイマーの動作モードの設定
	bool IsStarted(){return bStarted;}				///<	タイマーがON（定期的にコールバックを呼び出す状態）なら true
	bool IsRunning(){return bRunning;}				///<	コールバック呼び出し中ならば true
	
	bool Start();									///<	タイマー動作開始
	bool Stop();									///<	タイマーの停止
	void Call();

protected:
	friend class UTTimerProvider;
	friend unsigned long SPR_STDCALL UTTimer_ThreadCallback(void* arg);
	friend void SPR_STDCALL UTTimer_MMTimerCallback(unsigned uID, unsigned, unsigned long dwUser, unsigned long, unsigned long);

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
	void CallIdle();
};


}	//	namespace Spr
#endif
