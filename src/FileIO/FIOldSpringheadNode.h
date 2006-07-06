/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIOLDSPRINGHEADNODE_H
#define FIOLDSPRINGHEADNODE_H

namespace Spr{;
namespace SprOldSpringehead{;

///	\defgroup gpFIOldSpringheadNode 旧(Springhead1)ファイルロード用互換ノード
//@{
///	DirectXのFrame． GRFrameに対応
struct Frame{
};
///	DirectXのFrameTransformMatrix．フレームの変換行列を表す．GRFrame::transformに対応．
struct FrameTransformMatrix{
	Affinef matrix;
};

///	Springehead1の光源．GRLightに対応．
struct Light8{
	int type;
    Vec4f diffuse;		///<	拡散光
    Vec4f specular;		///<	
    Vec4f ambient;
    Vec4f position;		///<	光源の位置，w=0とすると無限遠(方向光源)になる．
    float range;		///<	光が届く範囲
    ///@name	減衰率．Atten = 1/( att0 + att1 * d + att2 * d^2)
	//@{
	float attenuation0;	///<	att0	0..∞
    float attenuation1;	///<	att1	0..∞
    float attenuation2;	///<	att2	0..∞
	//@}
	Vec3f spotDirection;///<	スポットライトの場合の向き
    float spotFalloff;	///<	減衰の早さ(大きいほど急峻)		0..∞
	float spotInner;	///<	スポットライトの中心部分		0..spotCutoff
	float spotCutoff;	///<	スポットライトの光が当たる範囲	0..π
};

///	DirectXのマテリアル．GRMateiralに対応．
struct Material{
	Vec4f face;				// face color
	float power;			
	Vec3f specular;			// specular color
	Vec3f emissive;			// emissive color
};


///	DirectXのMeshの面．
struct MeshFace{
	int nFaceVertexIndices;
	std::vector<int> faceVertexIndices;
};

///	DirectXのMesh．GRMeshに対応．
struct Mesh{
	int nVertices;
	std::vector<Vec3f> vertices;
	int nFaces;
	std::vector<MeshFace> faces;
	
};

///	DirectXのMeshの法線ベクトル
struct MeshNormals{
	int nNormals;
	std::vector<Vec3f> normals;
	int nFaceNormals;
	std::vector<MeshFace> faceNormals;
};
///	DirectXのMeshのマテリアルリスト．
struct MeshMaterialList{
	int nMaterials;
	int nFaceIndexes;
	std::vector<int> faceIndexes;
};
///	DirectXのMeshのテクスチャ座標．
struct MeshTextureCoords{
	int nTextureCoords;
	std::vector<Vec2f> textureCoords;
};
//@}

}}
#endif
