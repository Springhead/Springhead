#ifndef PH_FRICTIONTYPE_H
#define PH_FRICTIONTYPE_H

namespace Spr {

	//���C�̎�ނ��`���Ă���
	enum class PHFrictionType {

		None, //���C�Ȃ� or ��ޖ���
		Coulomb, //�N�[�������C
		TimeVaryFriction, //���Ԉˑ�������ꂽ����
		LuGre //LuGre���f��

	};

}

#endif