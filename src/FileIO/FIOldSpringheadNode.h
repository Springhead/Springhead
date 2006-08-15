/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIOLDSPRINGHEADNODE_H
#define FIOLDSPRINGHEADNODE_H

namespace SprOldSpringhead{;
using namespace Spr;

///	\defgroup gpFIOldSpringheadNode 旧(Springhead1)ファイルロード用互換ノード
//@{
/// Xファイル形式のバージョン情報
struct Header{
	unsigned int major;		
	unsigned int minor;		
	unsigned int flags;
};
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
    Vec4f specular;		///<	鏡面光
    Vec4f ambient;		///<	環境光
    Vec3f position;		///<	光源の位置
    Vec3f direction;	///<	光源の位置
    float range;		///<	光が届く範囲
    float falloff;		///<	減衰の早さ(大きいほど急峻)		0..∞
    ///@name	減衰率．Atten = 1/( att0 + att1 * d + att2 * d^2)
	//@{
	float attenuation0;	///<	att0	0..∞
    float attenuation1;	///<	att1	0..∞
    float attenuation2;	///<	att2	0..∞
	//@}
	float spotInner;	///<	スポットライトの中心部分		0..spotCutoff
	float spotCutoff;	///<	スポットライトの光が当たる範囲	0..π
};

///	DirectXのマテリアル．GRMateiralに対応．
struct Material{
	Vec4f face;				// face color
	float power;			// shininess power
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

/// DirectXのテクスチャファイル名（Materialの内部タグ)
struct TextureFilename{
	std::string filename;
};

///	DirectXのMeshのテクスチャ座標．
struct MeshTextureCoords{
	int nTextureCoords;
	std::vector<Vec2f> textureCoords;
};

///	Springhead1のScene
struct Scene{
};
///	Springhead1のCamera
struct Camera{
	Affinef view;
	float width;
	float height;
	float offsetX;
	float offsetY;
	float front;
	float back;
};

///	Springhead1のSolid
struct Solid{
	float mass;				///<	質量
	Matrix3f inertia;		///<	慣性モーメント
	Vec3f velocity;			///<	速度
	Vec3f angularVelocity;	///<	角速度
	Vec3f center;			///<	中心位置
};
struct PhysicalMaterial{
	float ns;
	float nd;
	float fs;
	float fd;
	float s;
	float d;
};

///
struct SolidContainer{
};
///	Springhead1のGravityEngine
struct GravityEngine{
	Vec3f gravity;
};

///	Springhead1のGravityEngine
struct ContactEngine{
};

///
struct JointEngine{
};
///
struct Joint{
};


//@}

}
#endif
