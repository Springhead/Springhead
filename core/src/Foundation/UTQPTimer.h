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

#include <Foundation/SprUTQPTimer.h>
#include <Foundation/Object.h>

#include <vector>
#include <sstream>
#include <string>

namespace Spr{;

//梶本先生のソースコード
//http://kaji-lab.jp/ja/index.php?people/kaji
//QueryPerformanceCounterを使った例．(windows.h内で宣言された関数）
//以前のRDTSCを使ったバージョンで生じていた問題を
//QueryPerformanceCounterを使い，
//スレッドをロックする
//ことによって，たぶん，ほぼ解消．
//これ以上を求めるなら今のところはインタフェースボードのタイマカウンタを使うしかない．

union UTLargeInteger {
	struct {
		unsigned long lowPart;
		long highPart;
	};
	UTLongLong quadPart;
};


class UTQPTimer{
private:
	static UTLargeInteger freq;
	UTLargeInteger lasttime;		///<	前回の時刻
	unsigned long stopWatch;		///<	ストップウォッチ機能の計測時間us単位
	bool startFlag;

public:
	UTQPTimer();
	static int Freq();				///<	タイマーの周波数を出力
	void WaitUS(int time);			///<	μs単位で待つ
	void Accumulate(UTLongLong& l);	///<	前回からの経過時間をカウント値で追加
	UTLongLong Count();				///<	前回からの経過時間をカウント値で計測
	int CountNS();					///<	前回からの経過時間をns単位で計測
	int CountUS();					///<	前回からの経過時間をμs単位で計測
	void CountAndWaitUS(int time);	///<	前回からの経過時間をμs単位で計測し，全体としてus単位で待つ（一定ループ生成用）

	/// ストップウォッチ機能.
	unsigned long Start();		///< 計測開始，開始時間（stopWatchの値）をus単位で返す
	unsigned long Stop();		///< 計測停止，経過時間（stopWatchの値）をus単位で返す
	unsigned long GetTime();	///< 現在の時間（stopWatchの値）をus単位で返す
	unsigned long Clear();		///< クリア，stopWatchを0にする
};

/// UTQPTimerを使い、アルゴリズムの所要時間を測定するためのクラス
class SPR_DLL UTPerformanceMeasure : public Object{
public:
	SPR_OBJECTDEF(UTPerformanceMeasure);
private:
	std::string name;
	double unit;	// 出力時の単位(1でsec, 1e-3でmsec)
	enum { MAXCOUNTS = 100 };
	UTLongLong counts[MAXCOUNTS];	// 計測データ
	struct Name {
		int id;					// dataの位置
		std::string name;		// 計測場所の名前
	};
	std::vector< Name > names;
	UTPerformanceMeasure(const char* n) :name(n), unit(1e-3) {}

public:
	UTPerformanceMeasure() :name(""), unit(1e-3) {
		assert(0);
		//	should not create independently.
	}
	int NCounter() {
		return (int)names.size();
	}
	const char* GetNameOfCounter(int id) {
		if (0<= id && id < names.size()) return names[id].name.c_str();
		return NULL;
	}
	const char* GetName() { return name.c_str(); }
	std::string PrintAll();
	std::string Print(std::string name);
	std::string Print(int id);
	double Time(const char* n) {
		int id = FindId(n);
		return Time(id);
	}
	double Time(int id) {
		double t = (double)counts[id] / (double)UTQPTimer::Freq();
		return t / unit;
	}
	UTLongLong& Count(const char* name) {
		int id = GetId(name);
		return counts[id];
	}
	UTLongLong& Count(int id) {
		return counts[id];
	}
	int GetId(std::string name);		///< 名前からIDを取得する。
	int FindId(std::string name);		///< 名前からIDを検索する。
	int CreateId(std::string name);		///< 名前に新しいIDを割り当てる。
	void SetUnit(double u) { unit = u; }
	double GetUnit() { return unit; }
	void ClearCounts() {
		for (int i = 0; i < names.size(); ++i) {
			counts[i] = 0;
		}
	}
	friend class UTPerformanceMeasureIf;
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

