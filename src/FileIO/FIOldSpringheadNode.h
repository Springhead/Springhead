#ifndef FIOLDSPRINGHEADNODE_H
#define FIOLDSPRINGHEADNODE_H

namespace Spr{;
namespace SprOldSpringehead{;

struct Frame{
};
struct FrameTransformMatrix{
	Affinef matrix;
};

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
struct Material{
	Vec4f face;
	float power;
	Vec3f specular;
	Vec3f emissive;
};



struct MeshFace{
	int nFaceVertexIndices;
	std::vector<int> faceVertexIndices;
};
struct Mesh{
	int nVertices;
	std::vector<Vec3f> vertices;
	int nFaces;
	std::vector<MeshFace> faces;
	
};
struct MeshNormals{
	int nNormals;
	std::vector<Vec3f> normals;
	int nFaceNormals;
	std::vector<MeshFace> faceNormals;
};
struct MeshMaterialList{
	int nMaterials;
	int nFaceIndexes;
	std::vector<int> faceIndexes;
};
struct MeshTextureCoords{
	int nTextureCoords;
	std::vector<Vec2f> textureCoords;
};


}}
#endif
