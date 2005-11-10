#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include <Base/BaseUtility.h>
#include <SceneGraph/SGScene.h>
#include <Collision/CDMesh.h>
#include <vector>

//	FRMを使う場合は，USE_FRMを定義	FRMのソースは非公開．
//	#define USE_FRM

#ifdef USE_FRM
#include "PHWaterResistanceMap.h"
#endif

namespace Spr{;

//この辺の機能は出来る限りCollisionとマージしたいところ...
//PHWサフィックスはPHWater内部クラスの意

class CDConvex;
class CDMesh;
//typedef std::vector<UTRef<CDConvex> > CDConveces;

//ジオメトリが持つ凸多面体をリストアップしたもの
class PHWaterContactEngine;
class PHWGeometry : public UTRefCount{
public:
	PHWGeometry();
	UTRef<SGFrame>	frame;			//このジオメトリが属する子フレーム
	CDMesh*			mesh;
#ifdef USE_FRM
	PHWaterResistanceMap* frm;		///
#endif
	CDGeometries	conveces;		//このジオメトリを構成する凸多面体
	void Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

class PHSolid;
class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//剛体
	UTRef<SGFrame>		frame;		//剛体フレーム
	//UTRef<PHWResistanceMap> frm;
//	Affinef				posture;	//剛体フレームのワールドフレームに対するposture
	PHWGeometries		geometries;	//剛体のフレームの形状データ
	void Init(PHWaterContactEngine* e);
	void EnumGeometries(SGFrame*, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWSolid> >		PHWSolids;

/**	剛体と流体との接触力計算エンジン.
	現状ではPHWater 1 対 PHSolid 多
	もちろんPHSolid同士の接触は別途PHContactEngineでやってください．
 */
class PHWater;
class PHWaterContactEngine : public SGBehaviorEngine{
public:
	PHSolid* soPaddle;
	Vec3f paddleForceK;
	Vec3f paddleForceB;
	Vec3f paddleTorqueK;
	Vec3f paddleTorqueB;
public:
	SGOBJECTDEF(PHWaterContactEngine);
	bool bUseFrm;

	UTRef<PHWater>	water;			///水
	PHWSolids		solids;			///水に接触する剛体
#ifdef USE_FRM
	PHWaterResistanceMaps	frms;	///
#endif

	std::vector<Vec3f> tris;		///<	デバッグ用:水面下の3角形
	std::vector<Vec3f> points;		///<	デバッグ用:物体と重なっている水

	///
	PHWaterContactEngine();

	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	ロード完了時の処理
	virtual void Loaded(SGScene* scene);
	///	初期化処理、Loadedが呼ぶ
	virtual void Init(SGScene* scene);
	///
	int GetPriority() const { return SGBP_WATERCONTACTENGINE; }
	///	時間を dt 進める．
	virtual void Step(SGScene* s);
	///	
	virtual void Clear(SGScene* s);
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
	///
	virtual void Render(GRRender* r, SGScene* s);
};


}
#endif

