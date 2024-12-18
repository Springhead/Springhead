/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

/*
Convex���̂ɑ΂��Ă̐ڐG�͂��v�Z���邽�߂̃v���O���~���O�ƂȂ�܂�
*/

/*
made by KIM
*/

#ifndef PHCONTACTENGINE_H
#define PHCONTACTENGINE_H

#include <Physics/PHContactPoint.h>

typedef long long int ll;
typedef unsigned int ui;

namespace Spr {

	class PHShapePairForLCP;
	enum CONTACT { UNKNOWN = -1, POINT = 1, LINE, FACE };
	class PHContactEngine : public PHContactPoint {
	public:
		SPR_OBJECTDEF(PHContactEngine);

		struct ContactPartInfo {
			/// �T�|�[�g�^�O�ɂ��_�̌��
			std::vector<Vec3d> vectorGroup;
			/// �T�|�[�g�^�O�ɂ��l�i�ǂ̂Ƃ��낪�ڐG���Ă��邩�������ϐ��F0,1,2�ŋ敪�����j
			CONTACT supportTag;
			ContactPartInfo() : vectorGroup(NULL), supportTag(CONTACT::UNKNOWN) {};
		};

		struct ObjectInfo {
			PHSolid* PHSptr;
			CDConvex* CDCptr;
			Posed posed;
		};


		struct NecessaryInfo {
			/// ZMP�X�V�O
			Vec3d OldCoP = Vec3d();
			/// ZMP�X�V��
			Vec3d NewCoP = Vec3d();

			double  frictionMargin;   ///< �ő喀�C�͂ƍ�p���Ă��門�C�͂Ƃ̃}�[�W��
			double  copMargin;        ///< CoP����ڐG���p�`���E�܂ł̃}�[�W��
									  /// ZMP�ʒu�Z�o�p
			Vec3d ContPoint;
			Matrix3d ContLocal;
			Quaterniond w2x;
		};

		struct Info {
			NecessaryInfo necessaryInfo;
			ContactPartInfo contactpartInfo[2];
			ObjectInfo objectInfo[2];
		};

		Info phceInfo;

		// GJK�̃o���G�[�V����
		bool			GJKIteration(int st_0, int st_1, const Vec3f& z_0, const Vec3f& z_1, Vec3f& tmp);
		// Euclid �����v�Z
		void			EuclideanProjection(CONTACT st_0, CONTACT st_1, const Vec3f& z_0, Vec3f& tmp);
		// �ǂ̂悤�ɐڐG���Ă��邩�𔻒f������̂ł���AInteger�^�Ƃ��Ă��̐ڐG�̌`�𕪗ނ���
		CONTACT			HowToContact();
		// �_Pt��c0�ɑ����Ă��邩�ǂ����𔻒f���ABoolean�l��Ԃ�
		inline bool		isPointInObject0(const Vec3d& pt);
		// �_Pt��c1�ɑ����Ă��邩�ǂ����𔻒f���ABoolean�l��Ԃ�
		inline bool		isPointInObject1(const Vec3d& pt);
		// CoP��c0�ɑ����Ă��邩�ǂ����𔻒f���ABoolean�l��Ԃ�
		inline bool		isCoPInObject0();
		// CoP��c1�ɑ����Ă��邩�ǂ����𔻒f���ABoolean�l��Ԃ�
		inline bool		isCoPInObject1();
		// 2D��œ_���ʂɑ����Ă��邩�ǂ����𔻕ʂ���֐��ł���A���ɂ����true�A�Ȃ����false��Ԃ�
		bool			IsInside2D(const std::vector<Vec3d>& c_vec, CDConvex* c, PHSolid* s, const Vec3d& pt);

	public:

		/// �R���X�g���N�^
		PHContactEngine();
		PHContactEngine(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1);

		// ----- PHConstraint�̋@�\���I�[�o�[���C�h
		virtual bool Iterate();

		// Projection and Update
		void			updateWithProjection(unsigned s, unsigned e, bool& updated);

		// ----- ���̃N���X�Ŏ�������@�\
		/// �ڐG���(cts)�𔻒f���A����_�iold_cop�j�����ԋ߂��_�icp�j��o�^���A�X�V���ꂽ��true�ŕԂ�
		bool	FindClosestPoint(Vec3f& cp, const Vec3d& old_cop, CONTACT cts, bool copOnObject0, bool copOnObject1);
		void	Projection(SpatialVector& fnew, const int i, bool& updat);
		bool	GJK2D(const std::vector<Vec3d>& c_vec, CDConvex* c, PHSolid* s, Vec3f& tmp, const Vec3d& old_cop);
		/// ���镨�̏��Ƃ��̕��̏�̓_����(cdc0, phs0, pt0)�ʂ̕���(cdc1, phs1)�ɑ΂��Ĉ�ԋ߂��_���Z�o��pt1�ɃZ�[�u����
		bool	GJK3D(CDConvex* cdc0, PHSolid* phs0, Vec3d& pt0, CDConvex* cdc1, PHSolid* phs1, Vec3d& pt1);
		// �C�ӂ�2������Convex�Ԃ̋��ʋ�Ԃɑ΂��Ă���_�����ԋ߂��_��T��
		void	DykstraProjection(Vec3f& v, const Vec3d& pt);
		virtual void CompBias();
			
	};

	// Preprocess for InOut2D
	Vec3d			calculateMid3D(const std::vector<Vec3d>& vec_Group);
	// �����t���ʐρi>0�Ȃ��pqr�͔����v���j
	inline double	area2D(const Vec2d& p, const Vec2d& q, const Vec2d& r);
	// 2D��Convex hull�Ƃ���2D�̂���_����͂Ƃ��A���̓_��Convex hull�̒��ɓ����Ă��邩�ǂ����𔻒f��boolean�l��Ԃ�
	bool			isPointIn2DConvexHull(const std::vector<Vec3d>& vec_group, const Vec3d& pt);
	Vec2d			calculateMid2D(const std::vector<Vec3d>& vec_Group);
	// 3�����̐��Ɉ�ԋ߂��_��T�����̂ł���A�o�͂Ƃ��ĂR�����̍��W�ƂȂ�
	Vec3d			ClosestPtPointLine(const Vec3d& old_ZMP, const std::vector<Vec3d>& vecGroup, double& t);
	// 3�����̐��Ɉ�ԋ߂��_��T�����̂ł���A�o�͂Ƃ��ĂR�����̍��W�ƂȂ�
	Vec3d			ClosestPtPointLine(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, double& t);
	// �R�����̎O�p�`�ɑ΂��Ĉ�ԋ߂��_�������o���A�o�͂Ƃ��ĂR�����̍��W�Ƃ���
	Vec3d			ClosestPtPointTriangle(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, const Vec3d& tmp_3);
	// 3�����̐��Ɉ�ԋ߂��_��T���A�o�͂Ƃ��āA��ԋ߂��_������͂Ƃ��Ă��ꂽ�_�܂ł̃x�N�g����Ԃ�
	Vec3d			NormalPt2ClosestPt(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2);
	// 2D��Convex hull�ƁA����x�N�g�������邱�ƂŃT�|�[�g�|�C���g�̓_�̍��W�����o���A�o�͂͂��̍��W�̔ԍ��ƂȂ�
	ll				PointSupport(const std::vector<Vec3d>& tmp_vec, const Vec3d& normal);
	// 3D��̓�̓_�ɑ΂���2D��̍��W�ɕϊ����Ă��炻�̋�����Ԃ�
	inline double	Norm2D(const Posed& pose_1, const Vec3d& tmp_1, const Posed& pose_2, const Vec3d& tmp_2);
	// 3D��̓�̓_�ԋ���
	inline double	Norm3D(const Vec3d& pt1, const Vec3d& pt2);
	// ���镨�̂̓_�̌�₽�������ԋ߂��_�������o���A���̓_��Ԃ��֐�
	Vec3d			ClosestPtFromGroup(const Vec3d& old_ZMP, const std::vector<Vec3d>& c_vec);

}

#endif