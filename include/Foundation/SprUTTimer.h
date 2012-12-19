/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** @file SprUTTimer.h
	統合タイマ
 */

#ifndef SPR_UTTIMER_H
#define SPR_UTTIMER_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct UTTimerIf : ObjectIf{
	SPR_IFDEF(UTTimer);

	///	タイマーのモード
	enum Mode{
		/// 1ms 程度の精度を持つマルチメディアタイマー
		MULTIMEDIA,
		/// スレッドを使用した擬似タイマー
		THREAD,
		/// GLUTなどのフレームワークに依存したメッセージパッシング型のタイマー
		FRAMEWORK,
		/// アイドル処理
		IDLE
	};

	///	コールバック関数の型
	typedef void (SPR_CDECL *TimerFunc)(int id, void* arg);
	///	タイマの数を取得
	static unsigned		SPR_CDECL NTimers();
	/// タイマを取得
	static UTTimerIf*	SPR_CDECL Get(unsigned id);
	/// タイマを作成
	static UTTimerIf*	SPR_CDECL Create();

	/// タイマIDを取得
	unsigned GetID();

	/// MULTIMEDIA時のタイマー精度の取得
	unsigned GetResolution();
	/// MULTIMEDIA時のタイマー精度の設定（MULTIMEDIAがサポートしない場合、設定した値から書き換わることがある。
	bool SetResolution(unsigned r);					
	
	/// タイマーの時間間隔(ms)
	unsigned GetInterval();
	/// タイマーの時間間隔(ms)の設定
	bool SetInterval(unsigned i);
	
	/// コールバック関数の設定
	bool SetCallback(TimerFunc f, void* arg=NULL);
	///	
	UTTimerIf::TimerFunc GetCallback();
	///
	void* GetCallbackArg();
	/// タイマーの動作モードの取得
	Mode GetMode();
	/// タイマーの動作モードの設定
	bool SetMode(Mode m);

	/// タイマーがON（定期的にコールバックを呼び出す状態）なら true
	bool IsStarted();
	/// コールバック呼び出し中ならば true
	bool IsRunning();
	
	/// タイマー動作開始
	bool Start();
	/// タイマーの停止
	bool Stop();
	/// コールバック関数を呼び出す
	void Call();

};

}	//	namespace Spr
#endif
