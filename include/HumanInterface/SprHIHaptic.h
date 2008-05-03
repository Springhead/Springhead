/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprHIHaptic.h
 *	@brief ヒューマンインタフェースの基本クラス
*/


#ifndef SPR_HIHaptic_H
#define SPR_HIHaptic_H
#include <HumanInterface/SprHIBase.h>

namespace Spr{;

/**	\addtogroup	gpHumanInterface	*/
//@{

struct DRRealDeviceIf;

/**	@brief	ヒューマンインタフェースの基本クラス	*/
struct HIForceInterface6DIf: public HIPoseIf{
	SPR_IFDEF(HIForceInterface6D);
	///	デバイスの実際の提示トルクを返す
	Vec3f GetTorque();
	///	デバイスの実際の提示力を返す
	Vec3f GetForce();
	///	デバイスの目標出力とトルク出力を設定する
	void SetForce(const Vec3f& f, const Vec3f& t);
};


//@}
}
#endif
