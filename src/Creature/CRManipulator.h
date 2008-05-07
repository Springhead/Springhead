#ifndef CRMANIPULATOR_H
#define CRMANIPULATOR_H

#include <Springhead.h>
#include <Foundation/Object.h>

#include <vector>

#include "CRBody.h"

namespace Spr{;

//@{
class CRManipulator : public CRBody, public CRManipulatorDesc {

private:
	PHSceneIf*									phScene;		// 手を置くシーンへのポインタ
	std::vector< PHSolidIf* >					soFingersTop;	// 指先の剛体へのポインタの配列
	std::vector< std::vector< PHSolidIf* > >	soFingers;		// 各指の剛体へのポインタを5本指に対して適用したもの
	std::vector< std::vector< PHJointIf* > >	joFingers;		// 各指の関節へのポインタを5本指に対して適用したもの
	std::vector< PHBallJointIf* >				joFingerTips;	// 各指の付け根関節へのポインタを5本指に対して適用したもの

public:
	SPR_OBJECTDEF(CRManipulator);
	ACCESS_DESC(CRManipulator);

	CRManipulator(){}
	CRManipulator(const CRManipulatorDesc& desc, CRCreatureIf* c = NULL)
		: CRManipulatorDesc(desc),
		CRBody((const CRManipulatorDesc&)desc, c)
	{
		solids.resize(CRManipulatorDesc::SO_NSOLIDS);
		joints.resize(CRManipulatorDesc::JO_NJOINTS);

		CreateHand();

		Init();

	}

	int	 NSolids();
	int	 NJoints();
	int  NBallJoints();
	int  NHingeJoints();

	bool CreateHand();					// 手を作成する
	bool CreateThumb();					// 親指系を作成する(指番号 0 )
	bool CreateIndexFinger();			// 人差し指系を作成する(指番号 1 )
	bool CreateLongFinger();			// 中指系を作成する(指番号 2 )
	bool CreateAnnularFinger();			// 薬指系を作成する(指番号 3 )
	bool CreateDigitusFinger();			// 子指系を作成する(指番号 4 )

	void Grip();						// 握る動作をさせる
	void Release();						// 脱力したポーズをさせる（握っているものを放す）
	void Rock();						// グーのポーズをさせる
	void Paper();						// パーのポーズをさせる
	void Scissors();					// チョキのポーズをさせる

};

}

#endif