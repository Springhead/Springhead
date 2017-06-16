/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CDDETECTORIMP_H
#define CDDETECTORIMP_H

#include "CDQuickHull3DImp.h"
#include "CDConvex.h"

namespace Spr {;

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

class CDFace;
class CDConvex;
struct PHSdkIf;

/// Shapeの組の状態
struct CDShapePairSt{
	Vec3d normal;				///<	衝突の法線(0から1へ) (Global)
	double depth;				///<	衝突の深さ：最近傍点を求めるために，2物体を動かす距離．
	unsigned lastContactCount;	///<	最後に接触した時刻
	CDShapePairSt():depth(0), lastContactCount((unsigned)-2){}
};
class CDShapePair: public CDShapePairSt, public Object{
	SPR_OBJECTDEF(CDShapePair);
public:
	enum State{		//	接触があった場合だけ値が有効なフラグ。接触の有無は lastContactCountとscene.countを比較しないと分からない。
		NONE,		//	接触していないにも関わらず、列挙された。
		NEW,		//	前回接触なしで今回接触
		CONTINUE,	//	前回も接触
	} state;

	CDConvex* shape[2];			// 判定対象の形状
	Posed shapePoseW[2];		// World系での形状の姿勢のキャッシュ

	//	接触判定結果
	Vec3d	closestPoint[2];		///< 最近傍点（ローカル座標系）
	Vec3d	commonPoint;			///< 交差部分の内点（World座標系）
	Vec3d	center;					///< 2つの最侵入点の中間の点
	Vec3d   iNormal;				///< 積分による法線

public:
	CDShapePair():state(NONE){}
	void SetState(const CDShapePairSt& s){
		(CDShapePairSt&)*this = s;
	}
	///	接触判定．接触が見つかった場合に接触状態を更新
	virtual bool Detect(unsigned ct, const Posed& pose0, const Posed& pose1);
	//	接触判定時に使う、法線計算。
	void CalcNormal();

	/**	連続接触判定．同時に法線/中心なども計算．
		- ct	ステップカウント
		- pose0	Shape0の姿勢
		- pose1 Shape1の姿勢
		- delta0 Shape0の並進移動量
		- delta1 Shape1の並進移動量		*/
	bool ContDetect(unsigned ct, const Posed& pose0, const Posed& pose1, const Vec3d& delta0, const Vec3d& delta1, double dt);

	/// 形状を取得する
	CDShapeIf* GetShape(int i){ return shape[i]->Cast(); }
	/// closestpointをワールド座標系で取得する
	Vec3d GetClosestPointOnWorld(int i){ return shapePoseW[i] * closestPoint[i]; }
};

///	BBox同士の交差判定．交差していれば true．
bool FASTCALL BBoxIntersection(Posed postureA, Vec3f centerA, Vec3f extentA,
					  Posed postureB, Vec3f centerB, Vec3f extentB);

///	GJKで共有点を見つける
bool FASTCALL FindCommonPoint(const CDConvex* a, const CDConvex* b,
					 const Posed& a2w, const Posed& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb);

/**	GJKで共有点を見つける．連続版
	startからendの間に接触点があるか調べる。
	@return 0:まったく接触なし。-2:startより前に接触あり。-1: endより先に接触あり。
	1: 接触あり。
	@param a     凸形状 a
	@param b     凸形状 b
	@param a2w   World系から見た凸形状aの座標系
	@param b2w   World系から見た凸形状bの座標系
	@param dir   ２物体の相対速度の方向ベクトル。World系。単位ベクトルでなければならない。
	@param start 区間の始点：a2w.pos + dir*start に aがあり、 b2wの点にbがある状態から、
	@param end   区間の終点：a2w.pos + dir*end に aがあり、b2wの点にbがある状態までを判定。
	@param normal 接触の法線（出力）
	@param pa	物体 a 上の接触点。物体a系。（出力）
	@param pb	物体 b 上の接触点。物体b系。（出力）
	@param dist 衝突位置：a2w.pos + dir*dist に aがあり、 b2w.pos にbがあるときに衝突が起こった。
*/
int FASTCALL ContFindCommonPoint(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end, 
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist);

///	デバッグ用のツール。ファイルに引数を保存する。
void FASTCALL ContFindCommonPointSaveParam(const CDConvex* a, const CDConvex* b,
	const Posed& a2w, const Posed& b2w, const Vec3d& dir, double start, double end, 
	Vec3d& normal, Vec3d& pa, Vec3d& pb, double& dist);
///	デバッグ用のツール。
void ContFindCommonPointCall(std::istream& file, PHSdkIf* sdk);

namespace GJK{
	extern Vec3f p[4];			///<	Aのサポートポイント(ローカル系)
	extern Vec3f q[4];			///<	Bのサポートポイント(ローカル系)
	extern int p_id[4];			///<	Aのサポートポイントの頂点番号（球など頂点がない場合は -1）
	extern int q_id[4];			///<	Bのサポートポイントの頂点番号（球など頂点がない場合は -1）
	extern Vec3d w[4];			///<	ContFindCommonPointで使用する速度向き座標系でのサポートポイント
	extern Vec3d v[4];			///<	ContFindCommonPointで使用するv
	extern int nSupport;		///<	何点のsupportから最近傍点を計算したかを表す。(ContFindCommonPoint専用)
	extern Vec3d dec;			///<	内分の割合
	extern int ids[4];			///<	4つの p, q, p_id, q_id, w, v のうちどれを使うか
		//	pa = dec[0]*p[ids[0]] + dec[1]*p[ids[1]] + dec[2]*p[ids[2]];
		//	pb = dec[0]*q[ids[0]] + dec[1]*q[ids[1]] + dec[2]*q[ids[2]];
		//	で最近傍点（local系）が求まる
		//	例えば、nSupportが3の時、Aの頂点番号は、p_id[ids[0]], p_id[ids[1]], p_id[ids[2]] の3つ
	extern Vec3d p_q[4];		///<	ミンコスキー和上でのサポートポイント(ワールド系) ContでないFindXXで使用
}	//	namespace GJK


#if 1
/// GJKで最近傍点対を見つける
double FASTCALL FindClosestPoints(const CDConvex* a, const CDConvex* b,
					   const Posed& a2w, const Posed& b2w,
					   Vec3d& v, Vec3d& pa, Vec3d& pb);
#endif

class CDFace;


///	ContactAnalysisが使用する凸多面体の面を表す．
class CDContactAnalysisFace{
public:
	class DualPlanes: public std::vector<CDQHPlane<CDContactAnalysisFace>*>{};

	CDFace* face;	///<	面を双対変換した頂点でQuickHullをするので，CDFaceがVtxs.
	int id;			///<	どちらのSolidの面だか表すID．

	//@group CDContactAnalysis が交差部分の形状を求める際に使う作業領域
	//@{
	Vec3f normal;	///<	面の法線ベクトル
	float dist;		///<	原点からの距離
	/**	QuickHullアルゴリズム用ワークエリア．
		一番遠い頂点から見える面を削除したあと残った形状のエッジ部分
		を一周接続しておくためのポインタ．
		頂点→面の接続．	面→頂点は頂点の並び順から分かる．	*/
	CDQHPlane<CDContactAnalysisFace>* horizon;
	//@}

	/** コンストラクタ
		変数の初期化をしておかないと、Floating-point invalid operationが起こる
		2012.12.06 susa
	**/
	CDContactAnalysisFace(){
		face = NULL;
		id = -1;
		normal = Vec3d();
		dist = 0.0f;
		horizon = NULL;
	}
	
	///	QuickHullにとっての頂点．この面を双対変換してできる頂点
	Vec3f GetPos() const { return normal / dist; }
	/**	双対変換を行う．baseに渡す頂点バッファは，双対変換が可能なように
		双対変換の中心が原点となるような座標系でなければならない．	*/
	bool CalcDualVtx(Vec3f* base);

	/**	この面を双対変換してできる頂点を含む面．
		つまり，交差部分の形状を構成する頂点のうち，
		この面にあるもの	*/
	DualPlanes dualPlanes;
	///	交差部分の形状を構成する頂点のうちこの面にあるものの数.
	size_t NCommonVtx(){ return dualPlanes.size(); }
	///	交差部分の形状を構成する頂点のうちこの面にあるもの.
	Vec3f CommonVtx(int i);
	///	デバッグ用表示
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const CDContactAnalysisFace& f){
	f.Print(os);
	return os;
}

/**	交差部分の解析をするクラス．(交差部分の形状を求める/初回の法線を積分で求める)	*/
class CDContactAnalysis{
public:
	/// \defgroup quickHull QuickHullのための頂点と平面
	//@{
	
	typedef std::vector<CDContactAnalysisFace*> Vtxs;	
	static Vtxs vtxs;					///<	QuickHullの頂点
	typedef std::vector<CDContactAnalysisFace> VtxBuffer;	
	static VtxBuffer vtxBuffer;

	static CDQHPlanes<CDContactAnalysisFace> planes;	///<	面
	bool isValid;						///<	交差部分のポリゴンは有効？
	//@}

	//Vec3d correctionCommonPoint;		///<	補正CommonPoint(World座標系) CDContactAnalysis::CorrectCommonPoint()で更新される
	//std::vector<Vec3f> tvtxs[2];		///<	対象の2つの凸多面体のCommonPoint系での頂点の座標
	///** 共通部分に含まれる共有点を求める.
	//	返り値は共有点.
	//	cp の shapePoseW に shape[0], shape[1]の頂点をWorld系に変換する変換行列が入っていなければならない.
	//	また、GJKで求めたcommon pointを入れておくと収束が早くなるかも。
	//**/
	//void CalcDistance(const CDFaces::iterator it, const Vec3f* base, const Vec3d common, Vec3f &normal, float &dist);
	///** 共通部分の共有点を求める。
	//	戻り値は共有点があるとtrue, ないとfalseを返す
	//	ある場合はcorrectionCommonPointが更新される。
	//*/
	//bool CorrectCommonPoint(CDShapePair* cp);
	/**	共通部分の形状を求める．
		結果は，共通部分を構成する面を vtxs.begin() から返り値までに，
		共通部分を構成する頂点を， planes.begin から planes.end のうちの
		deleted==false のものに入れて返す．
		cp の shapePoseW に shape[0], shape[1]の頂点をWorld系に変換する
		変換行列が入っていなければならない．	*/
	CDContactAnalysisFace** FindIntersection(CDShapePair* cp);
	/**	交差部分の形状の法線を積分して，衝突の法線を求める．
		物体AとBの衝突の法線は，交差部分の面のうち，Aの面の法線の積分
		からBの面の法線の積分を引いたものになる．	*/
	void IntegrateNormal(CDShapePair* cp);
	/**	法線の計算．前回の法線の向きに物体を動かし，
		物体を離して最近傍点を求める．	*/
	void CalcNormal(CDShapePair* cp);
};

}
#endif
