#ifndef CDBOX_H
#define CDBOX_H

#include <SprCollision.h>
#include <Foundation/Object.h>
#include "CDConvex.h"

namespace Spr{;



/**	立方体の面(四角形)を表すクラス．CDBoxが所有．
	CDContactAnalysis で使われる．
*/
class CDQuadFace: public CDFaceIf{
public:
	int vtxs[4];	///< 面の頂点ID
	Vec3f normal;	///< 面の法線

	/// CDQuadFaceの面のインデックス数
	virtual int NIndex(){ return 4; }
	/// CDQuadFaceインデックス配列を取得
	virtual int* GetIndices(){ return vtxs; }
};

class CDQuadFaces:public std::vector<CDQuadFace>{
};

/// 直方体
class CDBox: public InheritCDShape<CDBoxIf, CDConvex>{
public:
	
	OBJECTDEF(CDBox);
	
	Vec3f boxsize;				/// 直方体のサイズ（各辺の長さ）
	CDQuadFaces qfaces;			/// 面（四角形:quadrangular face）
	std::vector<Vec3f> base;	/// 頂点の座標(ローカル座標系)
	
	CDBox();
	CDBox(const CDBoxDesc& desc);
	
	virtual int ShapeType(){ return CDShapeDesc::BOX; }
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
	///	切り口を求める．接触解析に使う．
	/// ただし、球体に関しては、切り口は求めない。接触解析時には最近傍の１点さえあればいい。
	/// 球体に関してFindCutRing()が呼び出された場合には、assertionが発生する。
	virtual bool FindCutRing(CDCutRing& r, const Posed& toW);
	
	/// 直方体のサイズを取得
	virtual Vec3f GetBoxSize();
	Vec3f* GetVertices();
	CDFaceIf* GetFace(size_t i);
};
	

}	//	namespace Spr

#endif	// CDBOX_H
