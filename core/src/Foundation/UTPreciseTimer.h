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

/** Precise timer using CPU clock
    Modified from PreciseTimer.h written by Kajimoto in Tachi Lab. at Univ. of Tokyo. */

// マルチスレッドでの計測の場合は，UTQPTimerを使う

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
	unsigned long stopWatch;		///<計測時間us単位
	bool startFlag;
public:
	UTPreciseTimer();
	void Init(int period = 100);	///< カウンタの初期化
	void WaitUS(int time);			///< us単位で待つ
	int CountUS();					///< 前回からの経過時間をus単位で計測
	int CountNS();					///< 前回からの経過時間をns単位で計測
	void CountAndWaitUS(int time);	///< 前回からの経過時間をus単位で計測し，全体としてus単位で待つ（一定ループ生成用）
	
	/// ストップウォッチ機能.
	unsigned long Start();	///< 計測開始，開始時間（stopWatchの値）をus単位で返す
	unsigned long Stop();	///< 計測停止，経過時間（stopWatchの値）をus単位で返す
	unsigned long Clear();	///< クリア，stopWatchを0にする
	static unsigned long CPUFrequency(){	///< CPUの周波数を返す
		return freq;
	}
};

}

#endif
