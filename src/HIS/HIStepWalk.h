#ifndef HID_STEPWALK_H
#define HID_STEPWALK_H
#include <WinBasis/WBWin.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Device/DVDeviceManager.h>
#include <Device/DVAdBase.h>
#include <Base/Affine.h>

 namespace Spr{;
using namespace std;

#define SENSOR4 //センサーが4つのとき

class SPR_DLL HIStepWalk{
	//	データを最大値と最小値で正規化するクラス．
	class SPR_DLL Sensor{
	public:
		float offsetMin;//キャリブレイション用に新たに作った
		float offsetMax;// 2002/6/18
//		float offset; //上記の前まで江島さんのプログラムで使っていたもの
		float scale;
		float rawCur;
		float cur;
	public:
		Vec2f pos;	//	センサの位置(メートル単位)
		DVAdBase* ad;
		Sensor(){
//			offset = 2.5f;
			offsetMin = 0.5f;//初期値（適当）
			offsetMax = 1.0f;//初期値（適当）
			scale = 2.0f;//初期値（適当?）
			rawCur = 0;
			cur = 0;
			ad = NULL;
		}
		void CalibMin(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur < offsetMin) offsetMin = rawCur;
		}
		void CalibMax(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur > offsetMax) offsetMax = rawCur;
		}
		///	正規化後の値を返す(0から1).
		float Get(){
			if (ad) rawCur = ad->Voltage();
			cur = (rawCur - offsetMin) / scale;
//			if (cur > 1) cur = 1; //1より大きくなってもいいんじゃないか？
//			if (cur < 0) cur = 0; //0より小さくなってもいいんじゃないか？
//			std::cout << "cur = " << cur << std::endl;
			return cur;
		}
		/*江島さんが作ったもの2002/6/18上記に変更
		void Calib(){
			if (ad) rawCur = ad->Voltage();
			if (rawCur < offset) offset = rawCur;
		}
		///	正規化後の値を返す(0から1).
		float Get(){
			if (ad) rawCur = ad->Voltage();
			cur = (rawCur - offset) / scale;
			if (cur > 1) cur = 1;
			if (cur < 0) cur = 0;
			return cur;
		}
		*/
	};

public:
	Vec2f cogAve, cog, direction;
	float inner, resultOfInner[3], memoryOfInner;

	HIStepWalk();
	virtual ~HIStepWalk();
	///	初期化
	bool Init(DVDeviceManager& devMan);
	///	本体．だいたい20msごとに呼び出す．	
	void Step(float);
	float GetVelocity(){ return walkVel; }
	float GetPitch(){ return pitch; }
	float GetPitchParam(){ return pitchParam; }
	void  SetPitchParam(float p){ pitchParam = p; }
	Vec2f GetDirection(){ return direction; }
	void SetDirection(Vec2f vector){ direction = vector; }
	int GetStep(){ return step; }
	void Print(std::ostream& os) const;
	float GetSensorVoltage(int n){ return sensor[n].Get(); }
	///キャリブレイション用
	void PSenRange();
	void PSenMax();
	void PSenMin();
	void SetCenterOfGravity(Vec2f& centerG);
	Vec2f GetCenterOfGravity(){ return centerOfGravity[0]; };
	void PSenCal();
	bool pSenCal; 
	bool printForCenterG;
	bool rotateSwitch;
	void SetThreshold(float t) { threshold = t; }
	void CloseSIGGRAPHFile();

	void DataOutput(float x, float y, float z);

	std::ofstream *dataFileSIGGRAPH;
	char* folderName;
	char folderNameBack[20];
	int fileNameTime;
	char fileTime[10];

	float jumpPower;
	void SetJump(float& jump);


protected:

	bool SideStep();				///サイドステップの検出
	Vec2f walkingDirection;			///圧力センサのみから検出された現在の足踏み方向
	Vec2f gravityDiff;				///現在の重心の（平均から見た）位置	
	int sideStep;					///サイドステップ数
	bool frontCheck;				///正面を向いているかのチェック
	float frontGap;					///チェックの際正面からどれくらいのズレを無視するか

	DWORD dwLastTime;               ///<    StepWalkが前回呼び出された時のカウント
	DWORD dwTime;                   ///<    StepWalkが今回呼び出された時のカウント
	int step;						///<	歩数
	int backStep;                   ///<    後ろに進む
	bool BackStep(float value);     ///<    後ろに進むかどうか判断する
	float walkVel;					///<	歩行速度	
	unsigned long dwInterval;		///<	足踏み間隔
	unsigned long dwStepTime;		///<	足踏みを検出した瞬間の時刻
	float pitch;					///<	dwIntervalの逆数
	float pitchParam;               ///<    pitchの移動距離調整用変数
	bool flag_SIGGRAPH;             ///<    SIGGRAPHデータ取りようフラグ 
	float threshold;                ///<    移動判定の閾値
	///	センサー数, 前，左，右で3つのセンサー
#ifndef SENSOR4
	enum {NSENSOR = 3};
#else
	enum {NSENSOR = 4};
#endif
	///	前，左，右のセンサー
	Sensor sensor[NSENSOR];

	///	履歴の数
	enum {CENTER_OF_GRAVITY_LEN = 6};
	///	重心の座標(履歴を含む)
	Vec2f centerOfGravity[CENTER_OF_GRAVITY_LEN];
	///	重心の移動速度
	float dCenterOfGravity;
};

}

#endif
