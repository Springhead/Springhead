/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDQUICKHULL3D_H
#define CDQUICKHULL3D_H

#include <Base/Affine.h>
#include <Base/TQuaternion.h>
#include <iosfwd>
#include <vector>

#define HULL_EPSILON 1e-6f	///<	QuickHullで扱う最小の数値
#define HULL_INFINITE 1e8f	///<	QuickHullで扱う最大の数値

//#define HULL_DEBUG		//	デバッグ出力
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif

namespace Spr{;

///	QuickHullで作られる面
template <class TVtx>
class CDQHPlane{
public:
	Vec3d normal;			///<	面の法線
	double dist;			///<	面の原点からの距離
	
	TVtx* vtx[3];			///<	面を構成する頂点
	CDQHPlane* neighbor[3];	///<	隣の面 vtx[0]-vtx[1] の隣が neighbor[0]
	bool deleted;			///<	削除された面はtrue
	void Clear();			///<	メモリクリア．使う前に呼ぶ．
	bool Visible(TVtx* p);	///<	頂点 v から表側が見えるかどうか
	///	vの頂点番号を返す(0..2を返す)．見つからなければ3を返す．
	int GetVtxID(TVtx* v);
	void CalcNormal();		///<	法線ベクトルと距離を計算する．
	void Reverse();			///<	表裏をひっくり返す．		
	///	点との距離を計算する．精度を考慮して一番近い点で計算する．
	double CalcDist(TVtx* v);
	///	デバッグ用表示
	void Print(std::ostream& os) const;
};

///	面のバッファ
template <class TVtx>
class CDQHPlanes{
public:
	TVtx** vtxBeginInput;	///<	残っている頂点の先頭
	TVtx** vtxEndInput;		///<	残っている頂点の最後の次
	///	頂点のVector
	class TVtxs: public std::vector<TVtx*>{
	public:
		void Print(std::ostream& os) const{
			for(typename TVtxs::const_iterator it = begin(); it != end(); ++it){
				(*it)->Print(os);
			}
		}
 	};

	CDQHPlane<TVtx>* buffer;		///<	バッファへのポインタ new する．
	int len;			///<	バッファの長さ
	CDQHPlane<TVtx>* begin;		///<	最初の面
	CDQHPlane<TVtx>* end;		///<	最後の面の次
	TVtx** vtxBegin;	///<	残っている頂点の先頭
	TVtx** vtxEnd;		///<	残っている頂点の最後の次
	int nPlanes;		///<	面の数
	unsigned size();	///<	使用済みバッファのサイズ
	CDQHPlanes(int l);
	void Clear();
	~CDQHPlanes();
	/**	bからeまでの頂点から凸包を作る．使用した頂点はbからvtxBegin，
		使用しなかった頂点は，vtxBeginからeに移動する．	
		beginからendは頂点を3つ含む面になる．それらの面うち凸包に使われた面
		は CDQHPlane::deleted が false になっている．	*/
	void CreateConvexHull(TVtx** b, TVtx** e);
	void Print(std::ostream& os) const;

private:
	/**	最初の凸多面体＝2枚の三角形(表裏)を作る．
		できるだけ大きい3角形を作ると効率が良いので，x最大，x最小，
		2点が作る直線から一番遠い頂点で3角形を作る．
		使用した頂点は，vtxBeginの手前に移動して，頂点リストからはずす．
	*/
	void CreateFirstConvex();
	/**	horizon を作る． cur が穴をあける面，vtx が新しい頂点．
	rv にhorizonを辺に持つ3角形を1つ返す．*/
	void FindHorizon(TVtx*& rv, CDQHPlane<TVtx>* cur, TVtx* vtx);
	/**	頂点とhorizonの間にコーンを作る．*/
	void CreateCone(TVtx* firstVtx, TVtx* top);
	/**	一番遠くの頂点を見つける．見つけたらそれを頂点リストからはずす	*/
	bool FindFarthest(CDQHPlane<TVtx>* plane);
	/*	外側 内側 の順に並べる．
		外側の終わり＝内側の始まりが inner	*/
	TVtx** DivideByPlaneR(CDQHPlane<TVtx>* plane, TVtx** start, TVtx** end);
	TVtx** DivideByPlane(CDQHPlane<TVtx>* plane, TVtx** start, TVtx** end);
	/**	一つの面に対する処理を行う．一番遠くの頂点を見つけ，
		地平線を調べ，コーンを作り，内部の頂点をはずす．*/
	void TreatPlane(CDQHPlane<TVtx>* cur);
};

///	頂点クラスの例
class CDQHVtx3DSample{
public:
	///@name QuickHullが使用するメンバ．必須．
	//@{
	/**	QuickHullアルゴリズム用ワークエリア．
		一番遠い頂点から見える面を削除したあと残った形状のエッジ部分
		を一周接続しておくためのポインタ．
		頂点→面の接続．	面→頂点は頂点の並び順から分かる．	*/
	CDQHPlane<CDQHVtx3DSample>* horizon;
	///  頂点の位置
	Vec3f GetPos() const;
	//@}
	///	位置の設定
	void SetPos(Vec3f p);
public:
	Vec3f dir;		///<	原点からの向き
	float dist;		///<	原点からの距離
public:
	int id_;

	///	デバッグ用表示
	void Print(std::ostream& os) const;
};

}

#endif
