/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef SPR_CRTIMESERIESIF_H
#define SPR_CRTIMESERIESIF_H

#include <Foundation/SprObject.h>

namespace Spr {

	struct CRBoneIf;

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	// CRJointTimeSliceのどの変数にデータが入っているかを示すフラグの指定に使う
	enum CRJointTimeSliceFlag {
		CRJT_SPRING, CRJT_DAMPER, CRJT_ANGLE, CRJT_VELOCITY, CRJT_ACCELERATION, CRJT_TORQUE
	};

	// ある時刻における関節の状態（または目標値）を示す構造体
	struct CRJointTimeSlice {
		// 対応するボーン（単なるデータとして使いたい場合はNULLでもいい）
		CRBoneIf* bone;

		// 時刻
		double time;

		// どの変数にデータが入っているかを示すフラグ
		bool enable[6];

		// バネ係数の標準値に対する比率
		double springRatio;

		// ダンパ係数の標準値に対する比率
		double damperRatio;
	};

	// -- ヒンジジョイントの場合
	struct CRHingeJointTimeSlice : CRJointTimeSlice {
		// 角度
		double angle;

		// 角速度
		double velocity;

		// 角加速度
		double acceleration;

		// トルク
		double torque;
	};

	// -- ボールジョイントの場合
	struct CRBallJointTimeSlice : CRJointTimeSlice {
		// 姿勢
		Quaterniond orientation;

		// 角速度
		Vec3d velocity;

		// 角加速度
		Vec3d acceleration;

		// トルク
		Vec3d torque;
	};

	// ----- ----- ----- ----- -----

	// 関節の集合に対するJointTimeSlice
	// （関節の集合：　全身や、体の一部など）
	struct CRJointsTimeSlice {
		// 各関節ごとのTimeSliceデータ
		std::vector<CRJointTimeSlice> jointTimeSlice;
	};

	// JointsTimeSliceの時系列データ
	struct CRJointsTimeSeries {
		std::vector<CRJointsTimeSlice> jointsTimeSlice;
	};

	// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

	// CRSolidTimeSliceのどの変数にデータが入っているかを示すフラグの指定に使う
	enum CRSolidTimeSliceFlag {
		CRST_POSE, CRST_VELOCITY, CRST_ACCELERATION, CRST_SPRING, CRST_DAMPER
	};

	// ある時刻における剛体の状態（または目標値）を示す構造体
	struct CRSolidTimeSlice {
		// 対応するボーン（単なるデータとして使いたい場合はNULLでもいい）
		CRBoneIf* bone;

		// 時刻
		double time;

		// どの変数にデータが入っているかを示すフラグ
		bool enable[5];

		// 位置・姿勢
		Posed pose;

		// 速度・角速度
		Vec6d velocity;

		// 加速度・角加速度
		Vec6d acceleration;

		// 手先PD制御のP係数の標準値に対する比率
		double springRatio;

		// 手先PD制御のD係数の標準値に対する比率
		double damperRatio;
	};

	// ----- ----- ----- ----- -----

	// 剛体の集合に対するSolidTimeSlice
	// （剛体の集合：　全身や、体の一部など）
	struct CRSolidsTimeSlice {
		// 各剛体ごとのTimeSliceデータ
		std::vector<CRSolidTimeSlice> solidTimeSlice;
	};

	// SolidsTimeSliceの時系列データ
	struct CRSolidsTimeSeries {
		std::vector<CRSolidsTimeSlice> solidsTimeSlice;
	};

}

#endif//SPR_CRTIMESERIESIF_H