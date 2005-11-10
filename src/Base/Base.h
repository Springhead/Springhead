#ifndef SBASE_H
#define SBASE_H
/**	@page Base Baseライブラリ
	<a href = "../../html/index.html"> Springhead </a> の中で，
	特に基本的なユーティリティを持つ部分を集め，
	Baseライブラリとしました．Baseライブラリは @ref PTM を含みます．<br>
	Baseライブラリは，
	- @ref Affine "アフィン行列・2/3次元ベクトル" (Spr::TVec2, Spr::TVec3, Spr::TAffine)
	- @ref PTM "n×m行列・n次元ベクトル" (PTM::TMatrix, PTM::TVector)
	- デバッグ支援機能(Debug.h)
	- コンパイラ対応マクロ(Env.h)
	- @ref UTRef
	- その他のユーティリティー(BaseUtilitiy.h)
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
