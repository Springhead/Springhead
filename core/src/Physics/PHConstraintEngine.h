/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <Collision/CDDetectorImp.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHGear.h>
#include <Physics/PHContactDetector.h>
#include <Foundation/UTQPTimer.h>

namespace Spr{;

class PHSolid;
class PHJoint;
class PH1DJoint;
class PHRootNode;
class PHConstraintEngine;
class PHPath;

struct PHLuGreSt {
	Vec2d z;		// Average displacement of bristles. z[0]:x-axis, z[1]:y-axiss
	double T;		// Average sticking time
};

///	形状の組
class PHShapePairForLCP: public PHShapePair{
public:
	SPR_OBJECTDEF(PHShapePairForLCP);

	std::vector<Vec3d>	section;	///< 交差断面の頂点．個々がPHContactPointとなる．可視化のために保持

	///	接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．
	void  EnumVertex      (unsigned ct, PHSolid* solid0, PHSolid* solid1);
	int   NSectionVertexes(){return (int)section.size();}		//(sectionの数を返す）
	Vec3d GetSectionVertex(int i){return section[i];}	//(i番目のsectionを返す）

	///	接触面積．接触形状の頂点座標から面積を計算
	double GetContactDimension(){
		if((int)section.size() < 2)	return 0.0;	// 頂点が2以上ない場合は面積なし
		Vec3d area;
		for(unsigned int i = 0; i < section.size()-2; i++){
			Vec3d vec1 = section[i+1] - section[0];
			Vec3d vec2 = section[i+2] - section[0];
			area += cross(vec1, vec2);
		}
		return area.norm() / 2;
	}

	/// 接触面の単位法線ベクトル
	Vec3d GetNormalVector(){
		Vec3d normal;
		if((int)section.size() < 3){
			// 頂点が3以上ない場合は近傍点間の法線を返す
			normal = shapePoseW[1] * closestPoint[1] - shapePoseW[0] * closestPoint[0];
			return normal / normal.norm();
		}
		Vec3d vec1 = section[1] - section[0];
		Vec3d vec2 = section[2] - section[0];
		normal = cross(vec1, vec2);
		return normal / normal.norm();
	}

	void GetClosestPoints(Vec3d& pa, Vec3d& pb){
		pa = shapePoseW[0] * closestPoint[0];
		pb = shapePoseW[1] * closestPoint[1];
	}

	PHLuGreSt GetLuGreState() { return LuGreState; }
	PHLuGreSt LuGreState;
};

/// Solidの組
class PHConstraintEngine;

class PHSolidPairForLCP : public PHSolidPair{
public:
	SPR_OBJECTDEF(PHSolidPairForLCP);
	
	virtual PHShapePair* CreateShapePair(){ return DBG_NEW PHShapePairForLCP(); }
	virtual void OnDetect    (PHShapePair* cp, unsigned ct, double dt);
	virtual void OnContDetect(PHShapePair* cp, unsigned ct, double dt);

	int	     GetContactState    (int i, int j){return shapePairs.item(i, j)->state;}
	unsigned GetLastContactCount(int i, int j){return shapePairs.item(i, j)->lastContactCount;}
	Vec3d    GetCommonPoint     (int i, int j){return shapePairs.item(i, j)->commonPoint;}
	double   GetContactDepth    (int i, int j){return shapePairs.item(i, j)->depth;}
	PHShapePairForLCPIf* GetShapePair(int i, int j){return shapePairs.item(i, j)->Cast();}
};

struct PHConstraintsSt{
	std::vector<PHConstraintState> joints;
	std::vector<PHConstraintState> gears;
};


class PHConstraintEngine : public PHConstraintEngineDesc, public PHContactDetector{
	friend class PHConstraint;
	friend class PHShapePairForLCP;
	SPR_OBJECTDEF1(PHConstraintEngine, PHEngine);
	ACCESS_DESC(PHConstraintEngine);
public:

	typedef std::vector< UTRef<PHRootNode> >	PHRootNodes;
	typedef std::vector< UTRef<PHPath> >		PHPaths;
	typedef std::vector< PHConstraintBase* >	PHConstraintBases;
	
	PHConstraints	points;			///< 接触点の配列
	PHConstraints	joints;			///< 関節の配列
	
	PHRootNodes		trees;			///< Articulated Body Systemの配列
	PHGears			gears;			///< ギアの配列
	PHPaths			paths;			///< パスの配列

	PHConstraints		cons;		///< 有効な拘束の配列
	PHConstraintBases	cons_base;

	int count;

	/// Peformance counter
	UTQPTimer ptimer;
	UTLongLong& timeCollision;
	UTLongLong& timeSetup;
	UTLongLong& timeIterate;

public:
	PHConstraintEngine(UTPerformanceMeasureIf* pm = UTPerformanceMeasureIf::GetInstance("global"));
	~PHConstraintEngine();
	
	PHJoint*    CreateJoint(const IfInfo* ii, const PHJointDesc& desc, PHSolid* lhs = NULL, PHSolid* rhs = NULL);	///< 関節の追加する
	PHRootNode* CreateRootNode(const PHRootNodeDesc& desc, PHSolid* solid = NULL);	///< ツリー構造のルートノードを作成
	PHTreeNode* CreateTreeNode(const PHTreeNodeDesc& desc, PHTreeNode* parent = NULL, PHSolid* solid = NULL);	///< ツリー構造の中間ノードを作成
	PHGear*		CreateGear(const PHGearDesc& desc, PH1DJoint* lhs = NULL, PH1DJoint* rhs = NULL);	///< ギアを作成
	PHPath*		CreatePath(const PHPathDesc& desc);
	
	// Objectの仮想関数
	virtual bool	AddChildObject(ObjectIf* o);
	virtual bool	DelChildObject(ObjectIf* o);
	virtual void	Clear         ();
	virtual size_t	GetStateSize  () const;
	virtual void	ConstructState(void* m) const;
	virtual void	DestructState (void* m) const;
	virtual bool	GetState      (void* s) const;
	virtual void	SetState      (const void* s);

	// PHEngineの仮想関数
	virtual int  GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	virtual void Step();

	// PHContactDetectorの仮想関数
	virtual PHSolidPair* CreateSolidPair(){ return DBG_NEW PHSolidPairForLCP(); }
	
	void StepPart1();
	void StepPart2();
	void UpdateSolids(bool bVelOnly);	///< 結果をSolidに反映する. bVelOnly == trueならば結果の速度のみをSolidに反映させ，位置はそのまま．
	void CompResponseMatrix();
	void Setup();					///< 速度更新LCPの準備
	void SetupCorrection();			///< 誤差修正LCPの準備
	void Iterate();					///< 速度更新LCPの一度の反復
	void IterateCorrection();		///< 誤差修正LCPの一度の反復

	// インタフェースの実装
	PHConstraintsIf* GetContactPoints();
	void	SetVelCorrectionRate     (double value){velCorrectionRate = value;}
	double	GetVelCorrectionRate     (){return velCorrectionRate;}
	void	SetPosCorrectionRate     (double value){posCorrectionRate = value;}
	double	GetPosCorrectionRate     (){return posCorrectionRate;}
	void	SetContactCorrectionRate (double value){contactCorrectionRate = value;}
	double	GetContactCorrectionRate (){return contactCorrectionRate;}
	void	SetBSaveConstraints      (bool value){bSaveConstraints = value;}
	void	SetUpdateAllSolidState   (bool flag){bUpdateAllState = flag;}
	void	SetUseContactSurface     (bool flag){bUseContactSurface = flag;}
	void	SetShrinkRate            (double data){shrinkRate = data;}
	double	GetShrinkRate            (){return shrinkRate;}
	void	SetShrinkRateCorrection  (double data){shrinkRateCorrection = data;}
	double	GetShrinkRateCorrection  (){return shrinkRateCorrection;}
	void    SetRegularization        (double reg){regularization = reg;}
	double  GetRegularization        (){return regularization;}

	//	接触領域を表示するための情報を更新するかどうか
	virtual void	EnableRenderContact	(bool enable);
	//	表示情報を更新するかどうかのフラグ
	bool renderContact;
	//	接触領域の表示用
	struct ContactInfo{
		PHConstraints points;
		std::vector< std::vector<Vec3f> > sections;
		void Clear();
	};
	struct ContactInfoQueue{
		ContactInfo queue[3];
		volatile int reading;	//	読出中のindex(0..2) 0を読んでいる間、2に書き込む、1に読み進んでも大丈夫。
		volatile int wrote;		//	書込後のindex(0..2) 2に書き込み終わると、2になる。
		ContactInfoQueue();
	} contactInfoQueue;
	void UpdateContactInfoQueue();
};

}	//	namespace Spr
#endif
