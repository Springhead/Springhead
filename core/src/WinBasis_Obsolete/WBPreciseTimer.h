/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef WBPRECISETIMER_H
#define WBPRECISETIMER_H
#pragma once
#include <WinBasis/WBWin.h>


namespace Spr{;
/**	CPUクロックを利用した正確なタイマー	
	東大舘研の梶本さんが作られた PreciseTimer.h を改変しました．	*/

class WBPreciseTimer{
private:
	static DWORD freq;
	LARGE_INTEGER cycles2;
	DWORD stopWatch;
	bool startFlag;
public:
	WBPreciseTimer();
	void Init(int period = 100);	///<	カウンタの初期化
	void WaitUS(int time);			///<	μs単位で待つ
	int CountUS();					///<	前回からの経過時間をμs単位で計測
	void CountAndWaitUS(int time);	///<	前回からの経過時間をμs単位で計測し，全体としてus単位で待つ（一定ループ生成用）
	//ストップウォッチ機能	
	DWORD Start();					///<	計測開始
	DWORD Stop();					///<	計測停止
	DWORD Clear();					///<	クリア
	static DWORD CPUFrequency(){	///<	CPUの周波数を返す．
		return freq;
	}
};

}

#endif
