/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWFEMMESH_H
#define FWFEMMESH_H

#include <Framework/SprFWFemMesh.h>
#include "FWObject.h"
#include "../Physics/PHFemMesh.h"
//#include "..Physics/PHFemMeshThermo.h"
#include "FrameworkDecl.hpp"

namespace Spr{;

class GRMesh;

/**	Framework上でのFEMメッシュ。
	毎ステップ、PHFemMeshのFEMシミュレーション結果をGRMeshに反映させる。
	初期化時には、GRMeshからPHFemMeshを生成し、さらにPHFemMeshからGRMeshを生成し、grMeshとgrFrameの下のGRMeshと置き換える。*/
class FWFemMesh: public FWObject{

	SPR_OBJECTDEF(FWFemMesh);		
	SPR_DECLMEMBEROF_FWFemMeshDesc;	//	FWFemMeshDescのメンバとGetDesc(), GetDescSize()の実装
//protected:
	UTRef<GRMesh> grMesh;
	UTRef<PHFemMesh> phMesh;
	//UTRef<PHFemMeshThermo> phMeshThermo;
	std::vector<int> vertexIdMap;	//	grMeshからphMeshへの頂点の対応表
public:
	FWFemMesh(const FWFemMeshDesc& d=FWFemMeshDesc());		//コンストラクタ
	///	ロード後に呼ばれる。grMeshからphMeshを生成し、grMeshもphMeshに合わせたものに置き換える
	void Loaded(UTLoadContext* );
	///	グラフィクス表示前の同期処理
	void Sync();
	///	子オブジェクトの数
	virtual size_t NChildObject() const;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos);
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);
	/// phMeshを取得
	PHFemMeshIf* GetPHMesh(){return phMesh->Cast();	};

//protected:
	//	TetGenを用いて、GRMeshを四面体メッシュ化し、phMeshに格納する。
	virtual bool CreatePHFromGR();
	virtual bool CreatePHFromGRroughness();
	//	phMeshからGRMeshを生成する。マテリアルなどはgrMeshから拾う。
	void CreateGRFromPH();

	//	メッシュのface辺を描画	//辺以外も描画可能
	void DrawVtxLine(float length, float x, float y, float z);
	void DrawEdge(float x0, float y0, float z0, float x1, float y1, float z1);		// 2位置座標をつなぐ線を引く
	void DrawEdge(Vec3d vtx0, Vec3d vtx1);											// 2位置座標をつなぐ線を引く
	void DrawEdgeCW(Vec3d vtx0, Vec3d vtx1,float Red,float Green,float Blue);		// 2位置座標をつなぐ線を引く,Color,World coordinate
	void DrawNormal(Vec3d vtx0, Vec3d vtx1);		// vtx0:始点位置ベクトル vtx1:法線ベクトル
	//void DrawEdge(unsigned id0, unsigned id1);
	//	メッシュのface辺を描画
	void DrawFaceEdge();
	//	IH加熱領域の境界線を引く
	void DrawIHBorderXZPlane(bool sw);
	void DrawIHBorder(double xs, double xe);
	void DrawCompanion(Vec3d vtx); //小野原追加
	//void SetTextureMode();
public:
	enum TEXTURE_MODE{
		BROWNED,
		MOISTURE,
		THERMAL,
		THERMAL_CAM,
		//THERMAL_CAM2,
		DEBUG, //小野原追加
	} texturemode;
	//enum化する
	//void SetTexmode(TEXTURE_MODE){TEXTURE_MODE =  };
	unsigned texture_mode;
	void SetTexmode(unsigned mode){texture_mode = mode;};

};
}

#endif
