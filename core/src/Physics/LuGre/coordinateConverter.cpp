
/*
	3������World���W�n�ƁA���C��������ڐG���ʏ�̍��W�n�̕ϊ����s��
	�����ł́A�ڐG���ʏ�̍��W�n��\��Pose�^�̕ϐ��𐶐����Ă��邪�A����́A
	1 �ʒu�́A�ڐG���ʏ�̍��W�n�̌��_��World���W�n��ł̕\��
	2 ������\���N�H�[�^�j�I���́AWorld���W�n����ڐG�ʏ�̍��W�n�ւ̉�]�̕ϊ����s�����߂̃N�H�[�^�j�I��
	�̈ʒu�E�����̏������ϐ��ƂȂ��Ă���
*/

#include <Springhead.h>
#include <cmath>
#include "coordinateConverter.h"

namespace Spr{

	//�ڐG�ʂ̖@����^����ƁAWorld���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂�Pose��Ԃ�
	//����:
	//    normal: �ڐG�ʂ̖@��������World���W�n�ŕ\��������
	//    origin: ���ʏ�̍��W�n�̌��_�̍��W��World���W�n�ł̕\��
	//�߂�l:
	//    �ڐG���ʏ�̍��W�n��\��Pose�^�̕ϐ�
	Posed getWorldToPlanePose(Vec3d normal, Vec3d origin) {

		//�߂�l�̂��߂̕ϐ���p��
		Posed p;

		//���_���W�̐ݒ�
		p.Pos() = origin;

		//�ȉ��́AWorld���W�n����ڐG�ʏ�̍��W�n�ւ̉�]�̕ϊ����s�����߂̃N�H�[�^�j�I�������߂Ă�
		//�O�̂��߁Anorm�̒�����1�ɕ␳���Ă���
		normal = normal.unit();

		//�ڐG�ʏ��x�������̒P�ʃx�N�g��ex�ƁA�ڐG�ʏ��y�������̒P�ʃx�N�g��ey�����߂�
		Vec3d ex, ey;
		if (normal[1] == 0 && normal[2] == 0) {
			ey = Vec3d(0, 1, 0);
			if (normal[0] < 0) {
				ex = Vec3d(0, 0, 1);
			}
			else {
				ex = Vec3d(0, 0, -1);
			}

		} else {
			//ey���v�Z���� ey = (0, eyy, eyz)�Ƃ���
			double denominator = std::sqrt(normal[1] * normal[1] + normal[2] * normal[2]);
			double eyy = normal[2] / denominator;
			double eyz = -normal[1] / denominator;
			ey = Vec3d(0, eyy, eyz);
			
			//ex���v�Z����
			ex = ey % normal;//%�͊O��
		}

		//ex, ey, normal��p���āA�ڐG�ʏ�̍��W�n����World���W�n�ւ̕ϊ����s�����W�ϊ��s������߂�
		Matrix3d matrix = Matrix3d(ex, ey, normal);

		//���W�ϊ��s�񂩂�N�H�[�^�j�I���ɕϊ�
		Quaterniond q;
		q.FromMatrix(matrix);//�ڐG�ʏ�̍��W�n����World���W�n�ւ̕ϊ����s���N�H�[�^�j�I��

		//World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂̃N�H�[�^�j�I����ݒ�
		p.Ori() = q.Inv();

		return p;
	}

	//�ڐG�ʂ̖@����x�������A���_���W��^����ƁAWorld���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂�Pose��Ԃ�
	//����:
	//    normal: World���W�n�ɂ����錻�݂̐ڐG�ʂ̖@������
	//	  xAxis: World���W�n�ɂ����錻�݂̐ڐG�ʂ�x������(�܂���x���̌����ɂ���������)
	//           normal�ƒ������Ȃ��ꍇ�ɂ́Anormal�ɒ�������悤�Ɏˉe�x�N�g�����Ƃ��Ďg�p���邱�ƂƂȂ�
	//           �܂��AxAxis�Ƀ[���x�N�g����������ꂽ�ꍇ��AxAxis��normal�ƑS�����������̏ꍇ�ɂ�xAxis�̒l�͖�������A
	//           getWorldToPlanePose(Vec3d normal, Vec3d origin)�Ɠ������ʂ�Ԃ�
	//    origin: ���݂̐ڐG�ʂł̕��ʏ�̍��W�n�̌��_��\��World���W�n�ł̍��W
	//�߂�l:
	//    �ڐG���ʏ�̍��W�n��\��Pose�^�ϐ�
	Posed getWorldToPlanePose(Vec3d normal, Vec3d xAxis, Vec3d origin) {

		//�߂�l�̂��߂�Pose�^�ϐ��̗p��
		Posed newPose;

		//�V�������ʏ�̍��W�n�ł̌��_�̐ݒ�
		newPose.Pos() = origin;

		//xAxis���[���x�N�g���̏ꍇ���l������
		if (xAxis == Vec3d()) {
			return getWorldToPlanePose(normal, origin);
		}

		//newNormal, newXAxis�̒�����1�ɕ␳���Ă���
		normal = normal.unit();
		xAxis = xAxis.unit();

		//normal��xAxis����������悤�ɁAxAxis��␳
		//�␳���@�ɂ���
		//�������� <=> dot(normal, xAxis) = 0    (dot�͓��ςƂ���)
		//�����ŁA����萔a��p���āA
		//xAxis <- xAxis - a * normal
		//�Ƃ��邱�ƂŁA���������Ԃɂ��邱�Ƃ��l����
		//�����ŁAnormal���P�ʃx�N�g���Ȃ̂ŁAdot(normal, normal) = 1�ł��邱�Ƃɒ��ӂ����
		//dot(normal, xAxis - a * normal) = dot(normal, xAxis) - a * dot(normal, normal) = dot(normal, xAxis) - a
		//�ƂȂ�̂ŁA�����0�ɂ��邽�߂ɂ́Aa = dot(normal, xAxis)
		//���̂��߁A
		//xAxis <- xAxis - dot(normal, xAxis) * normal
		//�ɂ��␳���邱�ƂƂ���
		xAxis = xAxis - dot(normal, xAxis) * normal;

		//xAxis��normal�Ɠ��������̏ꍇ(���̎��_��xAxis���[���x�N�g���ƂȂ��Ă���ꍇ)���l������
		if (xAxis == Vec3d()) {
			return getWorldToPlanePose(normal, origin);
		}

		//�V����y���������v�Z
		Vec3d yAxis = normal % xAxis;//%�͊O��

		//�ڐG�ʏ�̍��W�n����World���W�n�ɕϊ������]�s���p��
		Matrix3d matrix = Matrix3d(xAxis, yAxis, normal);

		//���W�ϊ��s�񂩂�N�H�[�^�j�I���ɕϊ�
		Quaterniond q;
		q.FromMatrix(matrix);//�ڐG�ʏ�̍��W�n����World���W�n�ւ̕ϊ����s���N�H�[�^�j�I��

		//World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂̃N�H�[�^�j�I����ݒ�
		newPose.Ori() = q.Inv();

		return newPose;
	}

	//�ڐG�ʂ̖@���̕������ω������Ƃ��ɁA
	//����ɉ�����World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ����s�����߂̃N�H�[�^�j�I�����X�V����֐�
	//����:
	//    oldPose: �ȑO�̏�Ԃł̐ڐG���ʏ�̍��W�n��\��Pose�^�ϐ�
	//    newNormal: ���݂̐ڐG�ʂ�World���W�n������@������
	//    newOrigin: ���݂̐ڐG�ʂł̕��ʏ�̍��W�n�̌��_��\��World���W�n�ł̍��W
	//�߂�l:
	//    �ڐG���ʏ�̍��W�n��\��Pose�^�ϐ�
	Posed updateWorldToPlanePose(Posed oldPose, Vec3d newNormal, Vec3d newOrigin) {

		//�߂�l�̂��߂�Pose�^�ϐ��̗p��
		Posed newPose;

		//�V�������ʏ�̍��W�n�ł̌��_�̐ݒ�
		newPose.Pos() = newOrigin;

		//�ȉ��́AWorld���W�n����ڐG�ʏ�̍��W�n�ւ̉�]�̕ϊ����s�����߂̃N�H�[�^�j�I�������߂Ă�
		//�O�̂��߁AnewNormal�̒�����1�ɕ␳���Ă���
		newNormal = newNormal.unit();

		//�ȑO�̏�Ԃł�World���W�n�ł̖@������
		Vec3d oldNormal = convertPlaneToWorldVec(Vec3d(0, 0, 1), oldPose);
		
		//�ȑO�̏�Ԃł̖@���������猻�݂̏�Ԃł̖@�������ɕϊ����邽�߂̃N�H�[�^�j�I��
		Quaterniond transformQuaternion;
		transformQuaternion.RotationArc(oldNormal, newNormal);
		
		//�ȑO�́u�ڐG�ʏ�̍��W�n�ւ̉�]��\���N�H�[�^�j�I���v����A
		//�V�����u�ڐG�ʏ�̍��W�n�ւ̉�]��\���N�H�[�^�j�I���v( = newQuaternion)�����߂�B
		//�v�Z�̎d�g��:
		//transformQuaternion * oldNormal = newNormal   <- transformQuaternion�̒�`
		//oldPose.Ori().Inv() * Vector3(0, 0, 1) = oldNormal   <- World���W�ł�oldNormal���ȑO�̐ڐG�ʏ�̍��W�n�ł�(0, 0, 1)�ɂ�����
		//newQuaternion.Inv() * Vector3(0, 0, 1) = newNormal   <- World���W�ł�newNormal���ȑO�̐ڐG�ʏ�̍��W�n�ł�(0, 0, 1)�ɂ�����
		//���̂R�̎��𖞂���newQuaternion��1�Ƃ��āAnewQuaternion = oldPose.Ori() * transformQuaternion.Inv()���������B
		Quaterniond newQuaternion = oldPose.Ori() * transformQuaternion.Inv();

		//�V�����ڐG�ʏ�̍��W�n�ւ̉�]��\���N�H�[�^�j�I����Pose�^�ϐ��ɐݒ�
		newPose.Ori() = newQuaternion;
		
		return newPose;
	}

	//World���W�n�̍��W����A�ڐG���ʏ�̍��W�ɕϊ�����֐�
	//����:
	//    r: World���W�n�ɂ�������W 
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    �ڐG���ʏ�ɂ�������W (z�����͖@����������)
	Vec3d convertWorldToPlanePos(Vec3d r, Posed p) {
		Vec3d s = p.Ori() * (r - p.Pos());//�ڐG���ʏ�ɂ�������W(z�����́A�@������������\��)
		return s;
	}

	//World���W�n�̃x�N�g������A�ڐG���ʏ�̃x�N�g���ɕϊ�����֐�
	//���x�Ȃǌ��_�̈ʒu���l�����Ȃ��ϊ�������ꍇ�ɗp����
	//����:
	//    r: World���W�n�ł̃x�N�g�� 
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    �ڐG���ʏ�ł̃x�N�g�� (z�����͖@����������)
	Vec3d convertWorldToPlaneVec(Vec3d r, Posed p) {
		Vec3d s = p.Ori() * r;//�ڐG���ʏ�ł̃x�N�g��(z�����́A�@������������\��)
		return s;
	}

	//�ڐG���ʏ�̍��W����AWorld���W�n�̍��W�ɕϊ�����֐�
	//����:
	//    s: �ڐG���ʏ�ɂ�������W
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    World���W�n�ɂ�������W
	Vec3d convertPlaneToWorldPos(Vec3d s, Posed p) {
		//p.Ori()��World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ�������N�H�[�^�j�I���Ȃ̂ŁA
		//�ڐG�ʏ�̍��W�n����World���W�n�ւ̕ϊ����s�����߂ɂ͋t�ɂ��Ȃ��Ă͂Ȃ�Ȃ����Ƃɒ���
		return p.Ori().Inv() * s + p.Pos();
	}

	//�ڐG���ʏ�̃x�N�g������AWorld���W�n�̃x�N�g���ɕϊ�����֐�
	//���x�Ȃǌ��_�̈ʒu���l�����Ȃ��ϊ�������ꍇ�ɗp����
	//����:
	//    s: �ڐG���ʏ�ł̃x�N�g��
	//    p: ���ʏ�̍��W�n��\��Pose�^�ϐ�(�֐�GetWorldToPlanePose()�Ő�������)
	//�߂�l:
	//    World���W�n�ł̃x�N�g��
	Vec3d convertPlaneToWorldVec(Vec3d s, Posed p) {
		//p.Ori()��World���W�n����ڐG�ʏ�̍��W�n�ւ̕ϊ�������N�H�[�^�j�I���Ȃ̂ŁA
		//�ڐG�ʏ�̍��W�n����World���W�n�ւ̕ϊ����s�����߂ɂ͋t�ɂ��Ȃ��Ă͂Ȃ�Ȃ����Ƃɒ���
		return p.Ori().Inv() * s;
	}
}


