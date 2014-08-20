#ifndef SPR_PHSKELETON_H
#define SPR_PHSKELETON_H

#include <SprFoundation.h>
#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <vector>

namespace Spr{;

class PHBone : public SceneObject{

	PHSolidIf* solid;
	PHSolidIf* proxySolid;
	PHJointIf* joint;
	PHBoneIf*  parent;

	Posed      lastPose;

	std::vector<PHBoneIf*> children;

public:
	SPR_OBJECTDEF(PHBone);

	/** @brief デフォルトコンストラクタ
	*/
	PHBone(){ solid=NULL; proxySolid=NULL; joint=NULL; parent=NULL; lastPose = Posed(); }

	/** @brief Solidを設定する
	 */
	void SetSolid(PHSolidIf* solid) { this->solid = solid; }

	/** @brief Solidを取得する
	 */
	PHSolidIf* GetSolid() { return solid; }

	/** @brief Proxy Solidを設定する
	 */
	void SetProxySolid(PHSolidIf* solid) { this->proxySolid = solid; }

	/** @brief Proxy Solidを取得する
	 */
	PHSolidIf* GetProxySolid() { return proxySolid; }

	/** @brief Jointを設定する
	 */
	void SetJoint(PHJointIf* joint) { this->joint = joint; }

	/** @brief Jointを取得する
	 */
	PHJointIf* GetJoint() { return joint; }

	/** @brief 親Boneを設定する
	 */
	void SetParent(PHBoneIf* parent) { this->parent = parent; }

	/** @brief 親Boneを取得する
	 */
	PHBoneIf* GetParent() { return parent; }

	/** @brief ボーンの長さをセットする
	 */
	void SetLength(double length);

	/** @brief ボーンの向きをセットする（子ボーンのPositionも同時にセットする）
	 */
	void SetDirection(Vec3d dir);

	/** @brief ボーンの位置をセットする
	 */
	void SetPosition(Vec3d pos);

	// ----- ----- ----- ----- -----

	virtual bool        AddChildObject(ObjectIf* o) {
		if (DCAST(PHBoneIf,o)) { children.push_back(o->Cast()); return true; }
		return false;
	}
	virtual ObjectIf*   GetChildObject(size_t pos)  { return children[pos];   }
	virtual size_t      NChildObject() const        { return children.size(); }
	
};

class PHSkeleton : public SceneObject{
public:

	std::vector< UTRef<PHBone> > bones;

	float grabStrength;

	SPR_OBJECTDEF(PHSkeleton);

	/** @brief デフォルトコンストラクタ
	*/
	PHSkeleton(){ }

	/** @brief Boneの数を取得する
	 */
	int NBones() { return (int)(bones.size()); }

	/** @brief Boneを取得する
	 */
	PHBoneIf* GetBone(int i) { return bones[i]->Cast(); }

	/** @brief Boneを作成する
	 */
	PHBoneIf* CreateBone(PHBoneIf* parent, const PHBoneDesc& desc = PHBoneDesc()) {
		PHBone* bone = DBG_NEW PHBone();
		bone->SetDesc(&desc);
		bones.push_back(bone);
		bone->SetScene(GetScene());
		return bone->Cast();
	}

	/** @brief 掴んでる度を取得する
	 */
	float GetGrabStrength() { return grabStrength; }
};

}

#endif
