

/*
	3������World���W�n�ƁA���C�������镽�ʏ�̍��W�n�̕ϊ����s��
*/
#include <Springhead.h>

namespace Spr {

	//�ڐG�ʂ̖@����^����ƁAWorld���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂�Pose��Ԃ�
	//����:
	//    normal: �ڐG�ʂ̖@��������World���W�n�ŕ\��������
	//    origin: ���ʏ�̍��W�n�̌��_�̍��W��World���W�n�ł̕\��
	//�߂�l:
	//    �ڐG���ʏ�̍��W�n��\��Pose�^�̕ϐ�
	Posed getWorldToPlanePose(Vec3d normal, Vec3d origin);

	//�ڐG�ʂ̖@���̕������ω������Ƃ��ɁA
	//����ɉ�����World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂̃N�H�[�^�j�I�����X�V����֐�
	//����:
	//    oldPose: �ȑO�̏�Ԃł̐ڐG���ʏ�̍��W�n��\��Pose�^�ϐ�
	//    newNormal: ���݂̐ڐG�ʂ�World���W�n������@������
	//    newOrigin: ���݂̐ڐG�ʂł̕��ʏ�̍��W�n�̌��_��\��World���W�n�ł̍��W
	//�߂�l:
	//    �ڐG���ʏ�̍��W�n��\��Pose�^�ϐ�
	Posed updateWorldToPlanePose(Posed oldPose, Vec3d newNormal, Vec3d newOrigin);

	//World���W�n�̍��W����A�ڐG���ʏ�̍��W�ɕϊ�����֐�
	//����:
	//    r: World���W�n�ɂ�������W 
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    �ڐG���ʏ�ɂ�������W (z�����͖@����������)
	Vec3d convertWorldToPlane(Vec3d r, Posed p);

	//�ڐG���ʏ�̍��W����AWorld���W�n�̍��W�ɕϊ�����֐�
	//����:
	//    s: �ڐG���ʏ�ɂ�������W
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    World���W�n�ɂ�������W
	Vec3d convertPlaneToWorld(Vec3d s, Posed p);
}