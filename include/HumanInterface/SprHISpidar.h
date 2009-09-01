/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHISpidar.h
 *	@brief ヒューマンインタフェースの基本クラス
*/


#ifndef SPR_HISpidar_H
#define SPR_HISpidar_H
#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

struct DRRealDeviceIf;

///	@name	モータについての定数
struct HISpidarMotorDesc{
	//@{
	///	最大出力(N)
	float maxForce;
	///	最小出力(N)
	float minForce;
	///	D/Aの出力電圧 / モータの出力  (V/N)
	float voltPerNewton;
	///	モータに流れる電流 / D/Aの出力電圧
	float currentPerVolt;
	///	糸の長さ / パルス数
	float lengthPerPulse;
	///	モータが取り付けられている位置(糸の出口)の座標
	Vec3f pos;
	///	糸がグリップに取り付けられている位置
	Vec3f jointPos;
};
struct HISpidarMotorIf: public HIBaseIf{
	SPR_IFDEF(HISpidarMotor);

	void SetForce(float f);
	///	実際に出力した力の取得
	float GetForce();
	///	電流値の取得
	float GetCurrent();
	///	現在の糸の長さの設定
	void SetLength(float l);
	///	現在の糸の長さ
	float GetLength();
	///	速度の計測
	float GetVelocity();
	///	モータ定数のキャリブレーションのための計測 (100回くらいこれを呼んでから)
	void GetCalibrationData(float dt);
	///	モータ定数のキャリブレーション (こっちを呼んでキャリブレーション)
	void Calibrate(bool bUpdate);
	///	電源電圧の計測
	void GetVdd();
	///	 エンコーダのカウント値を返す
	int GetCount();

};

/**	@brief	SpidarG6の基本クラス	*/
struct HISpidarGIf: public HIForceInterface6DIf{
	SPR_VIFDEF(HISpidarG);
	///	デバイスの実際の提示トルクを返す
	Vec3f GetTorque();
	///	デバイスの実際の提示力を返す
	Vec3f GetForce();
	///	デバイスの目標出力とトルク出力を設定する
	void SetForce(const Vec3f& f, const Vec3f& t);

	///	モータ
	HISpidarMotorIf* GetMotor(size_t i);
	///	モータの数
	size_t NMotor() const;
};

	///	SpidarG6のデスクリプタ
struct HISpidarGDesc{
	SPR_DESCDEF(HISpidarG);
	std::vector<HISpidarMotorDesc> motors;
	HISpidarGDesc();
	HISpidarGDesc(char* type){Init(type);}
	HISpidarGDesc(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF=0.5f, float maxF=10.0f){
		Init(nMotor, motorPos, knotPos, vpn, lpp, minF, maxF);
	}
	///	パラメータによる初期化
	void Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF=0.5f, float maxF=10.0f);
	///	SPIDARのタイプ指定による初期化
	void Init(char* type);
};

/**	@brief	Spidar4の基本クラス	*/
struct HISpidar4If: public HIForceInterface3DIf{
	SPR_VIFDEF(HISpidar4);
	/////	デバイスの状態を更新する.
	//void Update(float dt);

};

	///	Spidar4のデスクリプタ
struct HISpidar4Desc{
	SPR_DESCDEF(HISpidar4);
	std::vector<HISpidarMotorDesc> motors;
	Vec4i port;
	HISpidar4Desc();
	HISpidar4Desc(char* type){Init(type);}
	HISpidar4Desc(char* type , Vec4i portNum){
		Init(type);
		port=portNum;
	}
	HISpidar4Desc(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF=0.5f, float maxF=10.0f){
		Init(nMotor, motorPos, knotPos, vpn, lpp, minF, maxF);
	}
	///	パラメータによる初期化
	void Init(int nMotor, Vec3f* motorPos, Vec3f* knotPos, float vpn, float lpp, float minF=0.5f, float maxF=10.0f);
	///	SPIDARのタイプ指定による初期化
	void Init(char* type);
	void InitSpidarG(char* type);
	void InitSpidarBig(char* type);
};


//@}
}
#endif
