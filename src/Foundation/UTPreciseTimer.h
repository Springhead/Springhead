/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UTPRECISETIMER_H
#define UTPRECISETIMER_H
#pragma once

namespace Spr{;
/**	CPUクロックを利用した正確なタイマー	
	東大舘研の梶本さんが作られた PreciseTimer.h を改変しました．	*/

union UTLargeInteger{
	struct {
		unsigned long lowPart;
		long highPart;  
	};  
	long long quadPart;
};

class UTPreciseTimer{
private:
	static unsigned long freq;
	UTLargeInteger cycles2;
	unsigned long stopWatch;
	bool startFlag;
public:
	UTPreciseTimer();
	void Init(int period = 100);	///<	カウンタの初期化
	void WaitUS(int time);			///<	μs単位で待つ
	int CountUS();					///<	前回からの経過時間をμs単位で計測
	void CountAndWaitUS(int time);	///<	前回からの経過時間をμs単位で計測し，全体としてus単位で待つ（一定ループ生成用）
	//ストップウォッチ機能	
	unsigned long Start();					///<	計測開始
	unsigned long Stop();					///<	計測停止
	unsigned long Clear();					///<	クリア
	static unsigned long CPUFrequency(){	///<	CPUの周波数を返す．
		return freq;
	}
};

}

#endif
