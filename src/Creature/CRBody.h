/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRBODY_H
#define CRBODY_H

#include <Springhead.h>

#include <Foundation/Object.h>

//@{
namespace Spr{;
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディの構成要素
*/
class CRBodyPart : public SceneObject, public CRBodyPartDesc {
protected:
public:
	SPR_OBJECTDEF(CRBodyPart);
	// ACCESS_DESC(CRBodyPart);

	CRBodyPart(){}
	CRBodyPart(const CRBodyPartDesc& desc) 
		: CRBodyPartDesc(desc)
	{
	}

	/** @brief ラベル（役割を示す文字列：Handなど）の取得
	 */
	const char* GetLabel() const { return label.c_str(); }

	/** @brief ラベル（役割を示す文字列：Handなど）の設定
	 */
	void SetLabel(const char* str) { label = std::string(str); }
};

class CRSolid : public CRBodyPart, public CRSolidDesc {
protected:
	PHSolidIf* solid;

public:
	SPR_OBJECTDEF(CRSolid);
	ACCESS_DESC(CRSolid);

	CRSolid(){ solid = NULL; }
	CRSolid(const CRSolidDesc& desc) 
		: CRSolidDesc(desc), CRBodyPart(desc)
	{
		solid = NULL;
	}

	/** @brief PHSolidを取得
	 */
	PHSolidIf* GetPHSolid() { return solid; }

	/** @brief PHSolidを設定
	 */
	void SetPHSolid(PHSolidIf* so) { solid = so; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { if (solid) { return 1; } else { return 0; } }
	virtual ObjectIf* GetChildObject(size_t i) { if (i==0) { return solid; } else { return NULL; } }
	virtual bool AddChildObject(ObjectIf* o) { solid = o->Cast(); return(solid!=NULL); }
	virtual bool DelChildObject(ObjectIf* o) { if (o==solid) { solid = NULL; return true; } return false; }
};

class CRIKSolid : public CRSolid, public CRIKSolidDesc {
protected:
	typedef std::vector<PHIKControlPointIf*> PHIKControlPointIfs;
	PHIKControlPointIfs ikCtlPnts;

public:
	SPR_OBJECTDEF(CRIKSolid);
	ACCESS_DESC(CRIKSolid);

	CRIKSolid(){}
	CRIKSolid(const CRIKSolidDesc& desc) 
		: CRIKSolidDesc(desc), CRSolid(desc)
	{
	}

	/** @brief IK制御点の数を取得
	 */
	int NIKControlPoints() { return ikCtlPnts.size(); }

	/** @brief i番目のIK制御点を取得
	 */
	PHIKControlPointIf* GetIKControlPoint(int i) {
		return ikCtlPnts[i];
	}

	/** @brief IK制御点を追加
	 */
	void AddIKControlPoint(PHIKControlPointIf* ikCP) {
		ikCtlPnts.push_back(ikCP);
	}

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const {
		if (solid) {
			return 1 + ikCtlPnts.size();
		} else {
			return 0 + ikCtlPnts.size();
		}
	}
	virtual ObjectIf* GetChildObject(size_t i) {
		if (solid) {
			if (i==0) {
				return solid;
			} else {
				return ikCtlPnts[i-1];
			}
		} else {
			return ikCtlPnts[i];
		}
	}
	virtual bool AddChildObject(ObjectIf* o) {
		PHSolidIf* so = o->Cast();
		if (so) { solid = so; return true; }

		PHIKControlPointIf* ikcp = o->Cast();
		if (ikcp) { ikCtlPnts.push_back(ikcp); return true; }
		
		return false;
	}
	virtual bool DelChildObject(ObjectIf* o) {
		if (o==solid) { solid = NULL; return true; }

		PHIKControlPointIf* ikcp = o->Cast();
		if (ikcp) {
			PHIKControlPointIfs::iterator it = std::find(ikCtlPnts.begin(), ikCtlPnts.end(), ikcp);
			if(it != ikCtlPnts.end()){
				ikCtlPnts.erase(it);
				return true;
			}
		}

		return false;
	}
};

class CRJoint : public CRBodyPart, public CRJointDesc {
protected:
	PHJointIf* joint;

public:
	SPR_OBJECTDEF(CRJoint);
	ACCESS_DESC(CRJoint);

	CRJoint(){}
	CRJoint(const CRJointDesc& desc) 
		: CRJointDesc(desc), CRBodyPart(desc)
	{
	}

	/** @brief PHJointを取得
	 */
	PHJointIf* GetPHJoint() { return joint; }

	/** @brief PHJointを設定
	 */
	void SetPHJoint(PHJointIf* jo) { joint = jo; }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { if (joint) { return 1; } else { return 0; } }
	virtual ObjectIf* GetChildObject(size_t i) { if (i==0) { return joint; } else { return NULL; } }
	virtual bool AddChildObject(ObjectIf* o) { joint = o->Cast(); return(joint!=NULL); }
	virtual bool DelChildObject(ObjectIf* o) { if (o==joint) { joint = NULL; return true; } return false; }
};

class CRIKJoint : public CRJoint, public CRIKJointDesc {
protected:
	typedef std::vector<PHIKNodeIf*> PHIKNodeIfs;
	PHIKNodeIfs ikNodes;

public:
	SPR_OBJECTDEF(CRIKJoint);
	ACCESS_DESC(CRIKJoint);

	CRIKJoint(){}
	CRIKJoint(const CRIKJointDesc& desc) 
		: CRIKJointDesc(desc), CRJoint(desc)
	{
	}

	/** @brief IKノードの数を取得
	 */
	int NIKNodes() { return ikNodes.size(); }

	/** @brief i番目のIKノードを取得
	 */
	PHIKNodeIf* GetIKNode(int i) { return ikNodes[i]; }

	/** @brief IKノードを追加
	 */
	void AddIKNode(PHIKNodeIf* ikNd) { ikNodes.push_back(ikNd); }

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const {
		if (joint) {
			return 1 + ikNodes.size();
		} else {
			return 0 + ikNodes.size();
		}
	}
	virtual ObjectIf* GetChildObject(size_t i) {
		if (joint) {
			if (i==0) {
				return joint;
			} else {
				return ikNodes[i-1];
			}
		} else {
			return ikNodes[i];
		}
	}
	virtual bool AddChildObject(ObjectIf* o) {
		PHJointIf* jo = o->Cast();
		if (jo) { joint = jo; return true; }

		PHIKNodeIf* ikn = o->Cast();
		if (ikn) { ikNodes.push_back(ikn); return true; }
		
		return false;
	}
	virtual bool DelChildObject(ObjectIf* o) {
		if (o==joint) { joint = NULL; return true; }

		PHIKNodeIf* ikn = o->Cast();
		if (ikn) {
			PHIKNodeIfs::iterator it = std::find(ikNodes.begin(), ikNodes.end(), ikn);
			if(it != ikNodes.end()){
				ikNodes.erase(it);
				return true;
			}
		}

		return false;
	}
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/** @brief ボディ
*/
class CRBody : public SceneObject, public CRBodyDesc {
protected:
	/// ボディを構成する剛体
	typedef std::vector< UTRef<CRSolidIf> > CRSolids;
	CRSolids solids;	
	
	/// ボディを構成する関節
	typedef std::vector< UTRef<CRJointIf> > CRJoints;
	CRJoints joints;

public:
	SPR_OBJECTDEF(CRBody);
	ACCESS_DESC(CRBody);

	CRBody(){}
	CRBody(const CRBodyDesc& desc) 
		: CRBodyDesc(desc)
	{
	}

	/** @brief 剛体の数を得る
	*/
	virtual int	NSolids() { return solids.size(); }

	/** @brief i番目の剛体を得る
	*/
	virtual CRSolidIf* GetSolid(int i) { return solids[i]; }

	/** @brief 関節の数を得る
	*/
	virtual int NJoints() { return joints.size(); }

	/** @brief i番目の関節を得る
	*/
	virtual CRJointIf* GetJoint(int i) { return joints[i]; }

	/** @brief ボディの重心座標を得る
	*/
	virtual Vec3d GetCenterOfMass();
	
	/** @brief ボディの質量を得る
	*/
	virtual double GetSumOfMass();

	/** @brief 子要素の扱い
	*/
	virtual size_t NChildObject() const { return solids.size()+joints.size(); }
	virtual ObjectIf* GetChildObject(size_t i);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);

	virtual Matrix3d CalcBoxInertia(Vec3d boxsize, double mass);
};
}
//@}

#endif//CRBODY_H
