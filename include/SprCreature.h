/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprCollision.h
 *	@brief 衝突判定のヘッダファイル群
*/
#ifndef SPR_CREATURE_H
#define SPR_CREATURE_H

// そのうちファイル名を下記分類に従って整理しようかと思う（070829,mitake）

// HumanModel
#include "Creature/SprCRHingeHuman.h"

// Sensor
#include "Creature/SprCRSensor.h"

// Controller
#include "Creature/SprCRAttentionController.h"
#include "Creature/SprCREyeController.h"
#include "Creature/SprCRReachingMovement.h"

// InternalModel
#include "Creature/SprCRAttentionList.h"

#endif
