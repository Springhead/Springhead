/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRMesh_H
#define GRMesh_H

#include <Graphics/SprGRMesh.h>
#include "GRFrame.h"
#include <GL/glew.h>

namespace Spr{;

class GRMaterial;

class GRSkinWeight: public GRVisual, public GRSkinWeightDesc{
public:
	SPR_OBJECTDEF(GRSkinWeight);
	ACCESS_DESC(GRSkinWeight);

	/// 子オブジェクト
	GRFrame* frame;

	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const ;
	virtual ObjectIf* GetChildObject(size_t pos) ;

	GRSkinWeight(const GRSkinWeightDesc& desc = GRSkinWeightDesc());
};

/**	@class	GRMesh
    @brief	グラフィックスシーングラフでの座標系を表す． */
class GRMesh: public GRVisual, public GRMeshDesc{
protected:
	UTRef<GRRenderIf> render;			///< レンダラー
	unsigned int list;					///< ディスプレイリストの識別子

	///	頂点フォーマットに合った頂点バッファ
	std::vector<float>	vtxs;
	std::vector<float>	blendedVtxs;
	const GRVertexElement* vtxFormat;
	int stride;			///< 1頂点のバイト数
	int positionOffset;	///< 頂点座標までのオフセット
	int normalOffset;	///< 法線までのオフセット
	int texOffset;		///< テクスチャ座標までのオフセット
	Vec3f& Pos(std::vector<float>& v, int i){ return (Vec3f&)v[stride*i + positionOffset]; }
	Vec3f& Normal(std::vector<float>& v, int i){ return (Vec3f&)v[stride*i + normalOffset]; }
	/// 頂点バッファの作成
	void MakeBuffer();
	/// 頂点バッファの描画
	void DrawBuffer(void* vtx);

	/// 3角形分割
	void DecomposeFaces();
	/// 法線自動生成
	void GenerateNormals();
	/// ���_����
	void DuplicateVertices();

	virtual void AfterSetDesc();
public:
	SPR_OBJECTDEF(GRMesh);
	ACCESS_DESC(GRMesh);

	bool tex3d;		///< 3次元テクスチャの場合 true
	bool alwaysCreateBuffer;

	/// 子オブジェクト
	std::vector< UTRef<GRMaterial> >	material;			///< マテリアル
	std::vector< UTRef<GRSkinWeight> >	skinWeights;		///< スキンMeshの場合のskinWeights

	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け \n
	  （Xファイルからロードされた面は、すべて三角形分割されているため、
	   どの面がどのマテリアルを適用するのか判断するためのもの）
	 */
	std::vector<GLuint>	triFaces;		///< 三角形分割後の面の頂点インデックス
	std::vector<GLuint>	triML;			///< 三角形分割後のマテリアルインデックス
	//std::vector<int>	orgFaces;		///< 三角形分割前の面に頂点インデックス
	//std::vector<int>	orgFaceIds;		///< 三角形に対応する分割前の面の番号

public:
	/// ディスプレイリストを作成する．
	void CreateList(GRRenderIf* r);

	//	頂点バッファ（への転送元バッファ）
	float* GetVertexBuffer(){								///< 頂点バッファ（への転送元バッファ）を返す
		if (vtxs.size()) return &vtxs[0];
		else return NULL;
	}
	int GetNVertex(){ return (int)vtxs.size()/stride; }			///< 頂点数
	const GRVertexElement* GetVertexFormat(){				///< 頂点フォーマットを返す。
		return vtxFormat;
	}
	int GetStride(){ return stride; }						///< 1頂点のデータがfloat何個分か
	int GetNormalOffset(){ return normalOffset; }			///< 法線のオフセット(float何個分)
	int GetPositionOffset(){ return positionOffset; }		///< 位置のオフセット(float何個分)
	int GetTexOffset(){ return texOffset; }					///< テクスチャ座標のオフセット(float何個分)

	void	EnableTex3D(bool on = true){ tex3d = on; }
	bool	IsTex3D(){ return tex3d; }						///< 3次元テクスチャなら true
	void	EnableAlwaysCreateBuffer(bool on = true){ alwaysCreateBuffer = on; }

	int		NVertex(){ return (int)vertices.size(); }
	int		NFace(){ return (int)faces.size(); }
	int		NTriangle(){ return (int)triFaces.size()/3; }
	
	Vec3f*		GetVertices(){ return &vertices[0]; }
	Vec3f*		GetNormals(){ return &normals[0]; }
	int			NColors(){	return (int)colors.size(); }
	Vec4f*		GetColors(){ return &colors[0]; }
	Vec2f*		GetTexCoords(){ return &texCoords[0]; }
	GRMeshFace*	GetFaces(){ return &faces[0]; }
	GRMeshFace*	GetFaceNormals(){ return &faceNormals[0]; }
	GLuint*		GetTriangles(){ return &triFaces[0]; }
	int*		GetMaterialIndices(){ return &materialList[0]; }
	void		SwitchCoordinate();

	GRSkinWeightIf* CreateSkinWeight(const GRSkinWeightDesc& desc);
	
	void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff = Affinef());
	
	// Objectの仮想関数
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const ;
	virtual ObjectIf* GetChildObject(size_t pos) ;
	virtual void Print(std::ostream& os) const ;

	// GRVisualの仮想関数
	virtual void Render(GRRenderIf* r);
	virtual void Rendered(GRRenderIf* r);

	GRMesh(const GRMeshDesc& desc=GRMeshDesc());
	~GRMesh();
	
};

}
#endif
