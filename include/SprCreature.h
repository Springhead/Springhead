/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprCreature.h
 *	@brief クリーチャのヘッダファイル群
*/
#ifndef SPR_CREATURE_H
#define SPR_CREATURE_H

// クリーチャのSdk
#include "Creature/SprCRSdk.h"

// クリーチャ（１体）
#include "Creature/SprCRCreature.h"

// ボディ : クリーチャの身体
#include "Creature/SprCRBody.h"
#include "Creature/SprCRBodyPart.h"

// CREngine : クリーチャを動かすアルゴリズム群（感覚・運動・行動決定など）の基本クラス
#include "Creature/SprCREngine.h"

// 個々のコントローラ
#include "Creature/SprCRGazeController.h"
// #include "Creature/SprCRBalanceController.h"



// ... Will Be Obsolete

// 個々のエンジン
#include "Creature/SprCRReachController.h"
#include "Creature/SprCRTrajectoryController.h"

#include "Creature/SprCRTouchSensor.h"
#include "Creature/SprCRVisualSensor.h"

// CRScene : クリーチャの記憶構造
#include "Creature/SprCRScene.h"

#endif
