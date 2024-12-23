/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

/* made by KIM SAE YOUNG */
/* �~���̎����̂��߂ɁA�~���̃f�B�X�N���v�^�ƂȂ�܂� */

#include <Collision/CDCylinder.h>

namespace Spr {
	const double weak_epsilon = 1.0e-4;
	//----------------------------------------------------------------------------
	//	CDCylinder
	CDCylinder::CDCylinder(const CDCylinderDesc& desc) {
		material = desc.material;
		radius = desc.radius;
		length = desc.length;
	}

	// CDCylinder��Volume�v�Z
	float CDCylinder::CalcVolume() {
		return  CDShape::CalcCylinderVolume(radius, length);
	}

	Matrix3f CDCylinder::CalcMomentOfInertia() {

		Matrix3f I0 = CDShape::CalcCylinderInertia(radius, length);

		return I0;
	}

	// �T�|�[�g�|�C���g�����߂�
	int CDCylinder::Support(Vec3f& w, const Vec3f& v) const {
		// ���S������W�n���o�Ă���Ƃ��Čv�Z
		// Cylinder��Center���猩���Ƃ��ɍ��W�n�iz����Length�ɉ����āj
		Vec3f m = Vec3f(0.0f, 0.0f, 0.0f);

		//�T�|�[�g�T���Ƃ��ɂ�x,y���W�����ōl����
		//z�͌�ŁA���m�͈̔͂����đ���
		m.X() = v.X();
		m.Y() = v.Y();
		float p = v.Z();
		float n = sqrt(Square(v.X()) + Square(v.Y()));
		float sign = 0.0f;
		sign = v.Z() > 0.0f ? 1.0f : -1.0f;

		if (n < 1.0e-10f) {
			if (p <= -1.0e-10f) {
				w = Vec3f(0.0f, 0.0f, 0.0f);
				w.Z() = -length * 0.5f;
			}
			else if (p < 1.0e-10f){
				w = Vec3f(0.0f, 0.0f, 0.0f);
			}
			else{
				w = Vec3f(0.0f, 0.0f, 0.0f);
				w.Z() = length * 0.5f;
			}
		}
		else {
			if (p <= -1.0e-10f) {
				w = (radius / n) * m;
				w.Z() = (-1) * 0.5f * length;
			}
			else if (p < 1.0e-10f) {
				w = (radius / n) * m;
			}
			else {
				w = (radius / n) * m;
				w.Z() = 0.5f * length;
			}
		}

		return -1;

	}

	// �T�|�[�g�|�C���g���ǂ��ɂ��邩�����߂�
	int CDCylinder::SupportTag(const Vec3f& v, std::vector<Vec3d>& c_vec) const {
		// ���S������W�n���o�Ă���Ƃ��Čv�Z
		// Cylinder��Center���猩���Ƃ��ɍ��W�n�iz����Length�ɉ����āj
		Vec3f m = Vec3f(0.0f, 0.0f, 0.0f);
		Vec3d w = Vec3d();

		//�T�|�[�g�T���Ƃ��ɂ�x,y���W�����ōl����
		//z�͌�ŁA���m�͈̔͂����đ���
		m.X() = v.X();
		m.Y() = v.Y();
		float p = v.Z();
		float n = m.norm();
		float sign = 0.0f;
		sign = v.Z() > 0.0f ? 1.0f : -1.0f;

		if (n < 1.0e-2f) {
			if (p <= -1.0e-10f) {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				w.Z() = -length * 0.5f;
				c_vec.push_back(w);
				return 3;
			}
			else if (p < 1.0e-10f) {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				c_vec.push_back(w);
				return 0; // NONE
			}
			else {
				w = Vec3d(0.0f, 0.0f, 0.0f);
				w.Z() = length * 0.5f;
				c_vec.push_back(w);
				return 3; // CIRCLE_FACE
			}
		}
		else {
			if (p <= -1.0e-10f) {
				w = (radius / n) * m;
				w.Z() = (-1) * 0.5f * length;
				c_vec.push_back(w);
				return 1; // EDGE
			}
			else if (p < 1.0e-10f) {
				w = Vec3f(0.0f, 0.0f, 0.0f);
				c_vec.push_back(w);
				return 2; // Z-EDGE
			}
			else {
				w = (radius / n) * m;
				w.Z() = 0.5f * length;
				c_vec.push_back(w);
				return 1;
			}
		}
	}

	// �؂�������߂�. �ڐG��͂��s��.
	IntersectionType CDCylinder::FindCutRing(CDCutRing& ring, const Posed& toW) {
		//	�؂��(ring.local)�n�ł̉~���̌���
		Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0, 0, 1); // �~�Ղ�z���Ƃ��Ē�`����Ă��邽��
		Vec3f center = ring.localInv * toW.Pos();
		float sign = center.X() > 0.0f ? 1.0f : -1.0f;
		if (dir.X()*sign < 0) dir = -dir;
		center -= sign*dir * length / 2;

		if (dir.X()*sign < 0.1f) {		//	�~�����ڐG�ʂɑ�̕��s�ȏꍇ
			float shrink = sqrt(1 - dir.X()*dir.X());	//	�X���Ă��邽�߂ɋ������k�ފ���
			float start = -0.0f*length*shrink;
			float end = 1.0f*length*shrink;
			if (dir.X()*sign > 1e-4) {	//	���S�ɕ��s�łȂ��ꍇ
				float is = (radius / shrink - sign*center.X()) / (sign*dir.X()) * shrink;	//	�ڐG�ʂƒ��S���𔼌a���炵�����Ƃ̌�_
				if (start <= is && is <= end) end = is;
				else return SEC_POINT;
			}
			//	ring�ɐ�����ǉ�
			float lenInv = 1 / sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());
			ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z())*lenInv, -start));
			ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z())*lenInv, end));
			ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y())*lenInv, 0));
			ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y())*lenInv, 0));
			return SEC_POLYGON;
		}
		else if (dir.X() * sign > 0.8f) {		//	�~�����ڐG�ʂɑ�̐����ȏꍇ
			return SEC_CURVETURE;
		}
		else {
			return SEC_POINT;
		}
	}

	int CDCylinder::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset) {
		const float eps = 1.0e-10f;
		Vec3f p;
		int num = 0;
		Vec3f sCenter[2];	//���̒��S�ʒu
		sCenter[0] = Vec3f(0.0f, 0.0f, length);
		sCenter[1] = Vec3f(0.0f, 0.0f, -length);

		//�������̔���
		for (int i = 0; i<2; i++) {
			const Vec3f n = sCenter[i] - origin;		 //�ʂ̖@�� = �J�����Ƌ��̌��_�����ԃx�N�g��
			float tmp = n * dir;						 //�ʂ̖@���ƃ|�C���^�̃x�N�g���Ƃ̓���
			if (abs(tmp) < eps)							 //���ς��������ꍇ�͔��肵�Ȃ�
				continue;
			float s = ((sCenter[i] - origin) * n) / tmp; //�J�����Ɩʂ̋��� 
			if (s < 0.0)
				continue;
			p = origin + dir * s;						 //�����Ɩʂ̌�_p = �J�������W�n�̌��_+�J�������W�n����ʂւ̃x�N�g��*���� (Shape���W�n)
			Vec3f po = p - sCenter[i];					 //���̒��S�����_�Ƃ������̌�_�̈ʒu
														 // �~�̓����ɂ��邩
			if (po.norm()<GetRadius()) {
				result[num] = p;
				offset[num] = s;
				num++;
			}
		}
		//�~�������̔���
		const Vec3f n = origin;						 //�J���������ւ̐����Ȓf�ʂ̖@�� = �J������shape�̌��_�����ԃx�N�g��
		float tmp = n * dir;							 //�ʂ̖@���ƃ|�C���^�̃x�N�g���Ƃ̓���

		for (int i = 0; i<1; i++) {
			if (abs(tmp) < eps)							 //���ς��������ꍇ�͔��肵�Ȃ�
				continue;
			float s = ((-origin) * n) / tmp;			 //�J�����Ɩʂ̋��� 
			if (s < 0.0)
				continue;
			p = origin + dir * s;						 //�����Ɩʂ̌�_p = �J�������W�n�̌��_+�J�������W�n����ʂւ̃x�N�g��*���� (Shape���W�n)

			Vec2f pr = Vec2f(p.x, p.y);					 //xy���ʂ�p
														 // �~���̓����ɂ��邩
			if (pr.norm()<GetRadius() && abs(p.z) <= GetLength()*0.5) {
				result[num] = p;
				offset[num] = s;
				num++;
			}
		}
		return num;
	}

	Vec3d CDCylinder::GetPointA()
	{
		Vec3d w = Vec3d();
		w.Z() = -length * 0.5;
		return w;
	}

	Vec3d CDCylinder::GetPointB()
	{
		Vec3d w = Vec3d();
		w.Z() = length * 0.5;
		return w;
	}

	bool CDCylinder::IsInside(const Vec3f& p) {
		if (-0.5 * (length + weak_epsilon) <= p.z && p.z <= 0.5 * (length + weak_epsilon))
			if (p.x * p.x + p.y * p.y <= (radius + weak_epsilon) * (radius + weak_epsilon))
				return true;

		return false;
	}

}	//	namespace Spr
