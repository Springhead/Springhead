/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SBASE_H
#define SBASE_H
/**	@page Base Baseライブラリ
	<a href = "../../html/index.html" target=_top> Springhead </a> の中で，
	特に基本的なユーティリティを持つ部分を集め，
	Baseライブラリとしました．Baseライブラリは，
	- 数学・ベクトル・行列ライブラリ
		- @ref pgPTM n×m行列・n次元ベクトル (PTM::TMatrix, PTM::TVector)
		- @ref pgAffine アフィン行列・2/3次元ベクトル (Spr::TVec2, Spr::TVec3, Spr::TAffine)
	- rttiを使わない実行時型情報 (BaseTypeInfo.h)
	- デバッグ支援機能 (BaseDebug.h)
	- コンパイラ対応マクロ (Env.h)
	- @ref gpBaseUtility その他のユーティリティー (BaseUtility.h)
		- @ref gpUTRef

から構成されます．
*/

namespace Spr{}
#include "Env.h"
#include "BaseDebug.h"
#include "BaseUtility.h"
#include "BaseTypeInfo.h"
#include "StrBufBase.h"
#include "TVector.h"
#include "TMatrix.h"
#include "TinyVec.h"
#include "TinyMat.h"
#include "TQuaternion.h"
#include "Affine.h"

#endif
