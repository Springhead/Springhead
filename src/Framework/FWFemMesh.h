/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWFEMMESH_H
#define FWFEMMESH_H

#include <Springhead.h>
#include <Framework/FWObject.h>
#include <Graphics/GRMesh.h>
#include <Physics/PHFemMesh.h>
#include "FrameworkDecl.hpp"
#include "tetgen.h"

//#include "tetgen.h"
//#include "ThermalFEM.h"

namespace Spr{;

/**	Framework上でのFEMメッシュ。
	毎ステップ、PHFemMeshのFEMシミュレーション結果をGRMeshに反映させる。
	初期化時には、GRMeshからPHFemMeshを生成する。*/
class FWFemMesh: public FWObject{
	SPR_OBJECTDEF(FWFemMesh);		
	SPR_DECLMEMBEROF_FWFemMeshDesc;	//	FWFemMeshDescのメンバとGetDesc(), GetDescSize()の実装
protected:
	GRMesh* grMesh;
	UTRef<PHFemMesh> phMesh;
public:
	FWFemMesh(const FWFemMeshDesc& d=FWFemMeshDesc());		//コンストラクタ
	///	子オブジェクトの数
	virtual size_t NChildObject() const;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos);
	///	子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);

protected:
	//	TetGenを用いて、GRMeshを四面体メッシュ化し、phMeshに格納する。
	virtual bool GeneratePHFemMesh();
/*	ThermalFEM FEM;
	tetgenio in,out;
	tetgenio::facet *f;
	tetgenio::polygon *p;
*/
};

}

#endif
