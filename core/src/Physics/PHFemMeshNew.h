/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef PH_FEMMESH_NEW_H
#define PH_FEMMESH_NEW_H

#include "Physics/SprPHFemMeshNew.h"
#include "Foundation/Object.h"
#include "PHScene.h"

namespace Spr{;
using namespace PTM;

/* 四面体を表すためのクラス、構造体の宣言 */
//	頂点
class FemVertex{
public:
	Vec3d pos;
	Vec3d initialPos;
	Vec3d vel;
	std::vector<int> tetIDs;
	std::vector<int> edgeIDs;
	std::vector<int> faceIDs;
	bool bUpdated;

	//For multiple FEM objects
	double centerDist;    //The distance to the closest neighbour
};
//	四面体
class FemTet{
public:
	int vertexIDs[4];	///< 頂点ID																																	  
	int faceIDs[4];		///< 表面４つ
	int edgeIDs[6];		///< 対応する辺のID。0:辺01, 1:辺12, 2:辺20, 3:辺03, 4:辺13, 5:辺23
	PTM::TVector<4,double> vecf[4];		//>	{f1}:vecf[0],{f2(熱流束)}:vecf[1],{f3(熱伝達)}:vecf[2],{f4(熱輻射)}:vecf[3]
	int& edge(int i, int j);
};

// 面
class FemFace{
	int sorted[3];		///< 比較するための、ソート済みの頂点id。Update()で更新。
public:
	int vertexIDs[3];	///<頂点ID。順番で面の表裏を表す。
	//（*表から見て時計回り。ただし全体形状nSurfaceFaceの表面のみ正しい。内部は2つの四面体が共有するため、表裏がいえない。）
	void Update();
	bool operator < (const FemFace& f2);	///< 頂点IDで比較
	bool operator == (const FemFace& f2);	///< 頂点IDで比較

	//added for the FEM implementation
	Vec3d centroid;  /// Face centroid
	double area;	/// Face area
	Vec3d normal;	//Face normal
	int tetraId;    //Saves the FaceId
};
//	辺
struct FemEdge{
	int vertexIDs[2];
	bool operator < (const FemEdge& e2); 	///< 頂点IDで比較
	bool operator == (const FemEdge& e2);	///< 頂点IDで比較
	FemEdge(int v1=-1, int v2=-1);
};

//Data structures for the multiple FEM implementation
//This structure is used for the KDTree Search
struct data {
    int faceId;
    Vec3d cpoint;
	Vec3d median;
	Vec3d fnormal;
	int vtxIds[3];
	int tetraId;
    data() {}
};

//class to define the KDTree data structure
class KDVertex{
public:
	KDVertex *left;
	KDVertex *right;
	data val;

	KDVertex() {
		left = NULL;
		right = NULL;
	}
};

//This structure is used to match Tetrahedra and vertex in a collision
struct FemFVPair {
	int vertexId;
	int faceId;
	int tetraId;
	int femIndex;
	Vec3d projection;
	double dist;
	Vec3d initialPos[2];

	Vec3d debugVertex; //DEBUG
	int debugMaster;   //DEBUG
	int debugSlave;    //DEBUG

	FemFVPair () {}
};

class PHFemBase;
typedef std::vector< UTRef< PHFemBase > > PHFemBases;
class PHFemVibration;
class PHFemThermo;
class PHFemPorousWOMove;
class PHFemMeshNew : public SceneObject{//, public PHFemMeshNewDesc{
public:
	/* PHFemMeshNewのメンバ変数、関数群 */
	SPR_OBJECTDEF(PHFemMeshNew);
	//SPR_DECLMEMBEROF_PHFemMeshNewDesc;
protected:
	///	基本情報(生成時にデスクリプタから与えられる情報)
	PHSolidIf* solid;					///< 関連づけられている剛体
public:
	std::vector<FemVertex> vertices;	///< 頂点
	std::vector<FemTet> tets;			///< 四面体
	
	/// 追加情報	基本情報からSetDesc()が計算して求める。
	std::vector<int> surfaceVertices;	///< 物体表面の頂点のID
	int GetSurfaceVertex(int i) { return surfaceVertices[i];  }
	int NSurfaceVertices() { return (int)surfaceVertices.size();  }
	std::vector<FemFace> faces;	///< 面
	unsigned nSurfaceFace;		///< 物体表面に存在する面の数。表面：faces[0],..,faces[nSurfaceFace-1]、内面:faces[nSurfaceFace],..,faces[faces.size()]
	std::vector<FemEdge> edges;	///< 辺
	unsigned nSurfaceEdge;		///< 物体表面に存在する辺。表面:edges[0],..,edges[nSurfaceEdge-1]、内面:edges[nSurfaceEdge],..,edges[edges.size()]

	/// 計算モジュール
	PHFemBases femBases;
	UTRef< PHFemVibration > femVibration;
	UTRef< PHFemThermo > femThermo;
	UTRef< PHFemPorousWOMove > femPorousWOMove;

	///	デスクリプタ
	PHFemMeshNew(const PHFemMeshNewDesc& desc = PHFemMeshNewDesc(), SceneIf* s=NULL);
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const ;
	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* p) const ;
	///	デスクリプタの設定。ここで、頂点座標と四面体の4頂点のIDの情報から、面や辺についての情報を計算しておく。
	virtual void SetDesc(const void* p);
	/// 子オブジェクトの追加
	virtual bool AddChildObject(ObjectIf* o);

	/// 初期化
	virtual void Init();
	///	時刻をdt進める。PHFemEngineが呼び出す。
	virtual void Step(double dt);
	/// 剛体を関連づける
	void SetPHSolid(PHSolidIf* s);
	/// 関連付けされている剛体を返す
	PHSolidIf* GetPHSolid();
	/// PHFemVibrationIfを返す
	PHFemVibrationIf* GetPHFemVibration();
	/// PHFemThermoIfを返す
	PHFemThermoIf* GetPHFemThermo();
	/// PHFemPorousWOMoveIfを返す
	PHFemPorousWOMoveIf* GetPHFemPorousWOMove();

	/// 頂点の総数を返す
	int NVertices();
	///	面の総数を返す
	int NFaces();
	int* GetFaceVertexIds(int f) { return faces[f].vertexIDs; }
	Vec3d GetFaceNormal(int f) { return faces[f].normal; }
	/// 四面体の総数を返す
	int NTets();
	///	四面体の頂点のIDを返す
	int* GetTetVertexIds(int t) { return tets[t].vertexIDs; }
	///	vertices[i].updatedFlag
	void SetVertexUpdateFlags(bool flg);
	void SetVertexUpateFlag(int vid, bool flg);

	///////////////////////////////////////////////////////////////////////////////////////////
	//* 頂点に関する関数 */
	/// 頂点の初期位置を取得する（ローカル座標系）
	Vec3d GetVertexInitalPositionL(int vtxId);
	/// 頂点の位置を取得する（ローカル座標系）
	Vec3d GetVertexPositionL(int vtxId);
	/// 頂点の変位を取得する（ローカル座標系）
	Vec3d GetVertexDisplacementL(int vtxId);
	/// 頂点の速度を取得する（ローカル座標系）
	Vec3d GetVertexVelocityL(int vtxId);
	/// 頂点に変位を与える（ワールド座標系）
	bool AddVertexDisplacementW(int vtxId, Vec3d disW);
	/// 頂点に変位を与える（ローカル座標系）
	bool AddVertexDisplacementL(int vtxId, Vec3d disL);
	/// 頂点の位置を指定する（ワールド座標系）
	bool SetVertexPositionW(int vtxId, Vec3d posW);
	/// 頂点の位置を指定する（ローカル座標系）
	bool SetVertexPositionL(int vtxId, Vec3d posL);
	bool SetVertexVelocityL(int vtxId, Vec3d posL);

	///////////////////////////////////////////////////////////////////////////////////////////
	//* 四面体に関する関数 */
	/// 四面体の計算(対象によらずに必要になる形状関数のXXを計算する関数)

	/// 四面体の体積を返す
	double CompTetVolume(const Vec3d pos[4]);
	double CompTetVolume(const int& tetID, const bool& bDeform);

	/// 形状関数の係数を返す
	/*
		(下記のa, b, c, dの行列）
		|N0|			|a0 b0 c0 d0||1|
		|N1|=	1/6V *	|a1 b1 c1 d1||x|
		|N2|			|a2 b2 c2 d2||y|
		|N3|			|a3 b3 c3 d3||z|
	*/
	TMatrixRow< 4, 4, double > CompTetShapeFunctionCoeff(Vec3d pos[4]);
	TMatrixRow< 4, 4, double > CompTetShapeFunctionCoeff(const int& tetId, const bool& bDeform);

	/// 四面体内のある点における形状関数値を返す
	/// (四面体外の位置を指定すると負値がでるのfalseを返す）
	bool CompTetShapeFunctionValue(const TMatrixRow< 4, 4, double >& sf, const double& vol, const Vec3d& posL, Vec4d& value);
	bool CompTetShapeFunctionValue(const int& tetId, const Vec3d& posL, Vec4d& value, const bool& bDeform);

	/// 面から四面体を探す
	int FindTetFromFace(int faceId);

	///////////////////////////////////////////////////////////////////////////////////////////
	//* 面に関する関数 */
	///	Face辺の両端点の座標を返す?
	std::vector<Vec3d> GetFaceEdgeVtx(unsigned id);
	///	Face辺の両端点の座標を返す?
	Vec3d GetFaceEdgeVtx(unsigned id, unsigned vtx);

	/// 面積を返す
	double CompFaceArea(const Vec3d pos[3]);		// 任意の頂点
	double CompFaceArea(const int& faceId, const bool& bDeform);

	/// 面の法線を返す
	Vec3d CompFaceNormal(const Vec3d pos[3]);		// 任意の頂点
	Vec3d CompFaceNormal(const int& faceId, const bool& bDeform);
	int NSurfaceFace(){return nSurfaceFace;}

	//For multiple object implementation VARIABLES
	std::vector<bool> contactVector;//saves the contact configuration of the scene
	int femIndex;					//stores the fem loading Index
	KDVertex* root;					//stores the KDTree root  
	RingBuffer<Vec3d> ringVel;		//ring buffer to save the instant vel 
	void SetVelocity(Vec3d v) { ringVel.WriteNoLimit(v);  }
	bool spheric;					//indicates the shape of the model (from SPR file)
	int GetFemIndex() { return femIndex;  }
	

	//For DEBUG FEM multiple collision
	Vec3d debugFoundPoint;  //debug
	Vec3d debugSearchPoint;  //debug
	Vec3d debugClosestPoint;  //debug
	Vec3d debugContactFace; //debug
	Vec3d debugPivotPoint;  //debug
	std::vector<int> *debugVertexInside;  //debug
	std::vector<int> *debugFacesInside;  //debug
	std::vector<int> *debugFixedPoints;  //debug
	std::vector<FemFVPair> *debugPairs;   //debug

};

}

#endif
