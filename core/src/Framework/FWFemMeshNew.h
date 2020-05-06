/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FW_FEMMESH_NEW_H
#define FW_FEMMESH_NEW_H

#include "FWObject.h"
#include <Framework/SprFWFemMeshNew.h>
#include "../Physics/PHFemMeshNew.h"
#include "FrameworkDecl.hpp"

namespace Spr{;

class GRMesh;

/**	Framework上でのFEMメッシュ。
	毎ステップ、PHFemMeshNewのFEMシミュレーション結果をGRMeshに反映させる。
	初期化時には、GRMeshからPHFemMeshNewを生成し、さらにPHFemMeshNewからGRMeshを生成し、grMeshとgrFrameの下のGRMeshと置き換える。*/
class FWFemMeshNew: public FWObject{
	SPR_OBJECTDEF(FWFemMeshNew);		
	SPR_DECLMEMBEROF_FWFemMeshNewDesc;
public:
	//UTRef< PHFemMeshNew > phFemMesh;	///< 物理計算用のメッシュ
	PHFemMeshNew*  phFemMesh;
	UTRef< GRMesh > grFemMesh;			///< 描画用のメッシュ
	std::vector<int> vertexIdMap;		///< grFemMeshからphFemMeshへ
	//char* meshRoughness;			///< tetgenで生成するメッシュの粗さ
	bool drawflag;
	std::vector<float> texmode1Map;

	FWFemMeshNew(const FWFemMeshNewDesc& d=FWFemMeshNewDesc());		//コンストラクタ
	///	子オブジェクトの数
	virtual size_t NChildObject() const;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos);
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);
	/// phMeshを取得
	PHFemMeshNewIf* GetPHFemMesh(){ return phFemMesh->Cast();	};

	///	ロード後に呼ばれる。grMeshからphMeshを生成し、grMeshもphMeshに合わせたものに置き換える
	void Loaded(UTLoadContext* );
	///	grFemMeshから四面体メッシュを生成する。Tetgenを使いphFemMeshに格納する。
	virtual bool CreatePHFemMeshFromGRMesh();
	///	phFemMeshからをGRMeshを生成する。マテリアルなどはgrMeshから拾う。
	void CreateGRFromPH();

	///	グラフィクス表示前の同期処理
	void Sync();
	/// PHVibrationの情報を同期
	void SyncVibrationInfo();
	/// PHThermoの情報を同期
	void SyncThermoInfo();

	void DrawEdgeCW(Vec3d vtx0, Vec3d vtx1,float Red,float Green,float Blue);		// 2位置座標をつなぐ線を引く,Color,World coordinate

	Vec4f CompThermoColor(float value);

	void CreateTranslucentMesh(GRMeshIf* wetMesh,std::map<int,int> corFemWet,std::vector<int> femVtx,std::vector<bool> vWetFlag,GRRenderIf* render);

	void EnableDrawEdgeCW(bool flag){drawflag = flag;};

	void Settexmode1Map(float temp);
public:
	enum TEXTURE_MODE{
		BROWNED,
		MOISTURE,
		THERMAL,
	} texturemode;
	void SetTexmode(unsigned mode){texture_mode = mode;};
};
}

#endif
