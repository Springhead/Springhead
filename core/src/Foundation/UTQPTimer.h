/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef UTQTTIMER_H
#define UTQTTIMER_H
#pragma once
#include <Foundation/UTPreciseTimer.h> //UTLargeIntegerの読み込み
#include <vector>
#include <sstream>
namespace Spr{;

//梶本先生のソースコード
//http://kaji-lab.jp/ja/index.php?people/kaji
//QueryPerformanceCounterを使った例．(windows.h内で宣言された関数）
//以前のRDTSCを使ったバージョンで生じていた問題を
//QueryPerformanceCounterを使い，
//スレッドをロックする
//ことによって，たぶん，ほぼ解消．
//これ以上を求めるなら今のところはインタフェースボードのタイマカウンタを使うしかない．

class UTQPTimer{
private:
	UTLargeInteger freq;
	UTLargeInteger lasttime;//前回の状態を保存する
	unsigned long stopWatch;		///<計測時間us単位
	bool startFlag;

public:
	UTQPTimer();
	void WaitUS(int time); //μs単位で待つ
	int CountUS();//前回からの経過時間をμs単位で計測
	void CountAndWaitUS(int time);//前回からの経過時間をμs単位で計測し，全体としてus単位で待つ（一定ループ生成用）
	int Freq(); //タイマーの周波数を出力

	/// ストップウォッチ機能.
	unsigned long Start();		///< 計測開始，開始時間（stopWatchの値）をus単位で返す
	unsigned long Stop();		///< 計測停止，経過時間（stopWatchの値）をus単位で返す
	unsigned long GetTime();	///< 現在の時間（stopWatchの値）をus単位で返す
	unsigned long Clear();		///< クリア，stopWatchを0にする
};

/// UTQPTimerを使い、プログラムの計算時間を測定するためのクラス
class UTQPTimerFileOut : private UTQPTimer{
private:
	double unit;	// file出力時の単位(1e-6でsec)
	std::vector< std::vector< unsigned long > > data;	// 計測データ
	// 名前でデータの場所を管理
	struct Name{
		int id;					// dataの位置
		std::string name;		// 計測場所の名前
		unsigned long lastTime;	// 計測開始時の時間
	};
	std::vector< Name > names;
public:
	UTQPTimerFileOut(double u = 1e-6);
	void Init();
	void StartPoint(std::string name);	// 計測開始点
	void EndPoint(std::string name);	// 計測終了点
	void Interval(std::string start, std::string name);	// 計測開始点からのインターバル
	void FileOut(std::string filename);		// データの吐き出し.xlsでしてい
private:
	int FindIdByName(std::string name);		// 名前からidを取得する
	int ResizeDataArea(std::string name);	// データ領域のリサイズ
};

}
#endif

