#ifndef PH_FRICTIONTYPE_H
#define PH_FRICTIONTYPE_H

namespace Spr {

	//摩擦の種類を定義しておく
	enum class PHFrictionType {

		None, //摩擦なし or 種類未定
		Coulomb, //クーロン摩擦
		TimeVaryFriction, //時間依存性を入れたもの
		LuGre //LuGreモデル

	};

}

#endif