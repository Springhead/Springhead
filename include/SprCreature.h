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

// CREngine : クリーチャを動かすアルゴリズム群（感覚・運動・行動決定など）
#include "Creature/SprCREngine.h"

#include "Creature/SprCRGazeController.h"
#include "Creature/SprCRReachController.h"
#include "Creature/SprCRTrajectoryController.h"

#include "Creature/SprCRTouchSensor.h"
#include "Creature/SprCRVisualSensor.h"

// CRScene : クリーチャの記憶構造
#include "Creature/SprCRScene.h"

// ボディ : 剛体関節モデルの生成を補助するクラス
#include "Creature/SprCRBody.h"

#endif
