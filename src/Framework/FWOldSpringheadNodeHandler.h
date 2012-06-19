/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWOLDSPRINGHEADNODEHANDLER_H
#define FWOLDSPRINGHEADNODEHANDLER_H

#if (!defined SWIG) || (defined SWIG_OLDNODEHANDLER)

#ifdef SWIG_OLDNODEHANDLER
#define VECTOR_LENGTH(x)	[x]		///	SWIGに配列の長さ情報を渡す
#else
#define VECTOR_LENGTH(x)
#endif


namespace SprOldSpringhead{;
using namespace Spr;

///	\defgroup gpFWOldSpringheadNode 旧(Springhead1)ファイルロード用互換ノード
//@{
/// Xファイル形式のバージョン情報．
struct Header{
	unsigned int major;		
	unsigned int minor;		
	unsigned int flags;
};
	
///	DirectXのFrame． GRFrameに対応．
struct Frame{
};
	
///	DirectXのFrameTransformMatrix．フレームの変換行列を表す．GRFrame::transformに対応．
struct FrameTransformMatrix{
	Affinef matrix;
};

///	Springehead1の光源．GRLightに対応．
struct Light8{
	enum XLightType{
		XLIGHT_POINT = 1,			///< 点光源
		XLIGHT_SPOT = 2,			/// スポットライト
		XLIGHT_DIRECTIONAL = 3,		///< 平行光源
	} type;
    Vec4f diffuse;		///<	拡散光
    Vec4f specular;		///<	鏡面光
    Vec4f ambient;		///<	環境光
    Vec3f position;		///<	光源の位置
    Vec3f direction;	///<	光源の方向
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

///	DirectXのMeshの法線ベクトル．
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

///	DirectXのMeshの頂点色．
struct IndexColor{
	int index;
	Vec4f indexColor;
};

///	DirectXのMeshの頂点色リスト．
struct MeshVertexColors{
	int nVertexColors;
	std::vector<IndexColor> vertexColors VECTOR_LENGTH(nVertexColors);
};

/// DirectXのテクスチャファイル名（Materialの内部タグ)．
struct TextureFilename{
	std::string filename;
};

///	DirectXのMeshのテクスチャ座標．
struct MeshTextureCoords{
	int nTextureCoords;
	std::vector<Vec2f> textureCoords;
};

//	DirectXのSkinメッシュのSkinWeight部のヘッダ
struct XSkinMeshHeader{
	int nMaxSkinWeightsPerVertex;
	int nMaxSkinWeightsPerFace;
	int nBones;
};
/*
struct XSkinMeshHeader{
	unsigned short nMaxSkinWeightsPerVertex;
	unsigned short nMaxSkinWeightsPerFace;
	unsigned short nBones;
};
*/

//	DirectXのSkinメッシュのSkinWeight
struct SkinWeights {
	std::string transformNodeName;
	unsigned int nWeights;
	std::vector<unsigned int> vertexIndices;
	std::vector<float> weights;
	Affinef matrixOffset;
};

//	アニメーション関係
struct Animation {
};

struct AnimationSet {
};

struct FloatKeys {
 unsigned int nValues;
 std::vector<float> values;
};

struct TimedFloatKeys {
	unsigned int time;
	FloatKeys tfkeys;
};

struct AnimationKey {
 unsigned int keyType;
 unsigned int nKeys;
 std::vector<TimedFloatKeys> keys;
};

struct AnimTicksPerSecond {
	unsigned int hz;
};


///	Springhead1のScene．
struct Scene{
};

///	Springhead1のSimulator．
struct Simulator{
	double timeStep;	///< 積分ステップ
	double decay;		///< 剛体の速度の減衰率	
};
	
///	Springhead1のCamera．
struct Camera{
	Affinef view;
	float width;
	float height;
	float offsetX;
	float offsetY;
	float front;
	float back;
};

///	Springhead1のSolid．
struct Solid{
	float mass;				///< 質量
	Matrix3f inertia;		///< 慣性モーメント
	Vec3f velocity;			///< 速度
	Vec3f angularVelocity;	///< 角速度
	Vec3f center;			///< 中心位置
};

/// Springhead1のSphere．GRSphereに対応．
struct Sphere{
	float radius;			///< 半径
	int slices;				///< 経線方向の分割数
	int stacks;				///< 緯線方向の分割数
};	
	
/// Springhead1の物理マテリアル
struct PhysicalMaterial{				
	float ns;				///< 抗力のばね係数
	float nd;				///< 抗力のダンパ係数
	float fs;				///< 摩擦力のばね係数
	float fd;				///< 摩擦力のダンパ係数
	float s;				///< 最大静止摩擦係数
	float d;				///< 動摩擦係数
};

/// Springhead1のSolidContainer．
struct SolidContainer{
};
	
///	Springhead1のGravityEngine．
struct GravityEngine{
	Vec3f gravity;
};

/// Springhead1のContactInactive．
struct ContactInactive{
	int nSolids;
	std::vector<int> solidIndexes;
};

///	Springhead1のContactEngine．
struct ContactEngine{
};
	
/// Springhead1のJointEngine．
struct JointEngine{
};
	
/// Springhead1のJoint．
struct Joint{
	int			nType;				///<	関節種類 0:蝶番, 1:スライド
	Vec3f		prj;				///<	親剛体のフレームから見た関節位置
	Matrix3f	pRj;				///<	親剛体のフレームから見た関節姿勢
	Vec3f		crj;				///<	子剛体のフレームから見た関節位置
	Matrix3f	cRj;				///<	子剛体のフレームから見た関節姿勢

	float		fPosition;			///<	変位
	float		fVelocity;			///<	速度
	float		fMaxTorque;			///<	最大トルク
	float		fMinPosition;		///<	可動範囲(最大値)
	float		fMaxPosition;		///<	可動範囲(最小値)
	int			nInputType;			///<	入力の種類 @see PHJointInputType
	float		fInput;				///<	入力値の初期値
	float		fPValue;			///<	PID制御のPの値
	float		fIValue;			///<	PID制御のIの値
	float		fDValue;			///<	PID制御のDの値
};

//@}

}
#endif
#endif
