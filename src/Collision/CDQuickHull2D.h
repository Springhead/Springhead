/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDQUICKHULL2D_H
#define CDQUICKHULL2D_H

#include <Base/Affine.h>
#include <Base/TQuaternion.h>
#include <vector>

//#define HULL_DEBUG	//	デバッグ出力
#ifdef HULL_DEBUG
 #define HULL_DEBUG_EVAL(x) x
#else
 #define HULL_DEBUG_EVAL(x)
#endif

namespace Spr{;

///	QuickHullで作られる面
template <class TVtx>
class CDQHLine{
public:
	Vec2d normal;			///<	面の法線
	double dist;			///<	面の原点からの距離
	
	TVtx* vtx[2];			///<	面を構成する頂点
	CDQHLine<TVtx>* neighbor[2];	///<	隣の面 vtx[0] の隣が neighbor[0]
	bool deleted;			///<	削除された面はtrue
	void Clear();			///<	メモリクリア．使う前に呼ぶ．
	void Reverse();			///<	辺の裏表をひっくり返す．
	bool Visible(TVtx* p);	///<	頂点 v から表側が見えるかどうか
	///	vの頂点番号を返す(0..1を返す)．見つからなければ3を返す．
	int GetVtxID(TVtx* v);
	void CalcNormal();		///<	法線ベクトルと距離を計算する．
	///	点との距離を計算する．精度を考慮して一番近い点で計算する．
	double CalcDist(TVtx* v);
	///	デバッグ用表示
	void Print(std::ostream& os) const;
};

///	面のバッファ
template <class TVtx>
class CDQHLines{
public:
	double epsilon;
	double infinite;

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

	CDQHLine<TVtx>* buffer;	///<	バッファへのポインタ new する．
	int len;			///<	バッファの長さ
	CDQHLine<TVtx>* begin;	///<	最初の辺
	CDQHLine<TVtx>* end;		///<	最後の辺の次
	TVtx** vtxBegin;	///<	残っている頂点の先頭
	TVtx** vtxEnd;		///<	残っている頂点の最後の次
	int nLines;			///<	辺の数
	unsigned size();	///<	使用済みバッファのサイズ
	CDQHLines(int l);
	void Clear();
	~CDQHLines();
	/**	bからeまでの頂点から凸包を作る．使用した頂点はbからvtxBegin，
		使用しなかった頂点は，vtxBeginからeに移動する．	
		beginからendは頂点を3つ含む面になる．それらの面うち凸包に使われた面
		は CDQHLine::deleted が false になっている．	*/
	void CreateConvexHull(TVtx** b, TVtx** e);
	void Print(std::ostream& os) const;

private:
	/**	最初の凸多面体＝2本の辺(表裏)を作る．
		できるだけ大きい辺を作ると効率が良い．	*/
	bool CreateFirstConvex();
	/**	辺curと，その面から一番遠い頂点 top を受け取り，
		curとその周囲の辺を削除し，凸包にtopを含める．
		end[-1], end[-2]が新たに作られた辺になる．	*/
	void CreateCone(CDQHLine<TVtx>* cur, TVtx* top);
	/**	一番遠くの頂点を見つける．見つけたらそれを頂点リストからはずす	*/
	bool FindFarthest(CDQHLine<TVtx>* plane);
	/*	外側 内側 の順に並べる．
		外側の終わり＝内側の始まりが inner	*/
	TVtx** DivideByPlaneR(CDQHLine<TVtx>* plane, TVtx** start, TVtx** end);
	TVtx** DivideByPlane(CDQHLine<TVtx>* plane, TVtx** start, TVtx** end);
	/**	一つの面に対する処理を行う．一番遠くの頂点を見つけ，
		地平線を調べ，コーンを作り，内部の頂点をはずす．*/
	void TreatPlane(CDQHLine<TVtx>* cur);
};

///	頂点クラスの例
class CDQHVtx2DSample{
public:
	///@name QuickHullが使用するメンバ．必須．
	//@{
	///  頂点の位置
	Vec2f GetPos() const { return pos; }
	//@}
public:
	Vec2f pos;		///<	位置
	int id_;
	void Print(std::ostream& os) const;
};

}

#endif
