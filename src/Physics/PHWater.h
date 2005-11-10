#ifndef PHWATER_H
#define PHWATER_H

#include <Base/Affine.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRVisual.h>
#include <Physics/PHSolid.h>
#include <Collision/CDGeometry.h>

using namespace PTM;
namespace Spr{;

/*
	・設計によってはWaterContainerという命名が適当かも
	・PHWaterClearForceの対象はSolidに限らず力が加わるもの全般なので、
		PHClearForceとすべき

	PHWaterの座標の取り方：
	・Z軸は静水面に直交し、上向き．
	・格子はx軸方向に格子数mx + 1，幅mx * dh，y軸方向に格子数my + 1，幅my + dh．
	・座標原点は格子の中央．
	・今のところ水深depthはスカラ定数だがheightと同じく格子ごとに異なる値をとるように拡張したい
*/

class D3Mesh;
class GLMesh;
class GRMaterial;
class D3Material;
class D3Render;
class GLRender;

class PHWaterTrackTarget : public SGObject{
public:
	SGOBJECTDEF(PHWaterTrackTarget);

	bool		AddChildObject(SGObject* o, SGScene* s);
	size_t		NReferenceObjects(){return targets.size();}
	SGObject*	ReferenceObject(size_t i){return targets[i];}

	SGFrames targets;
};
#if 0
class PHWFRM : public UTRefCount{
public:
	class ForceSet{
	public:
		
	};
public:
	int id;
	float buo_scl, pres_scl, fric_scl, vel_scl, unit_mass, disp_scl;
    float wz, wa;
	float dlen;
	Affinef	posture;
	float v0;
	FIString	filename;
	Vec3f	d, s;

	float dthe, dphi;
	int nhsrc, ndata;
	float rate;
	int nthe, nphi, ntex, sym[3], n;
	vector<ForceSet> frc_set;

	bool Load(FIString);
};
#endif
class PHWater : public GRVisual{
public:
	SGOBJECTDEF(PHWater);
	bool		AddChildObject(SGObject* o, SGScene* s);///< ロード時に使用．
	size_t		NReferenceObjects();					///< 
	SGObject*	ReferenceObject(size_t i);				///< フレームを返す．
	void		Loaded(SGScene* scene);					///< ロード終了時の初期化
	void		Step(SGScene* s);						///< 時刻を進める．
	Affinef		GetPosture(){ return frame->GetWorldPosture(); }
	float edgeWidth;									///< 描画時に付けるふちの幅

	///	レンダリング
	virtual void Render(SGFrame* fr, GRRender* render);
	void CalcNormal();

	///力をクリアする
	void ClearForce(){}

	///格子中間の値を線形補完して返す
	double LerpHeight(double x, double y);
/*
	double LerpVelocityU(double x, double y);
	double LerpVelocityV(double x, double y);
	Vec2f LerpVelocity(double x, double y){
		return Vec2f(LerpVelocityU(x,y), LerpVelocityV(x,y));
	}
*/
	//パラメータ
	UTRef<SGFrame> frame;	//	親フレーム
	int		my, mx;			//分割数
	TVec2<int> bound;		//境界 このセルと次のセルが境界のセルになる．
	TVec2<int> texOffset;	//bound と同じだが，%mx しない．
	Vec2d	velocity;		//定常流速．定常的にこの速度で水が流れる．
	double	dh, dhinv;		//格子の幅
	double	depth;			//水深
	double	gravity;		//重力
	double	density;		//密度
	double	loss;			//損失係数

	PHWater();
	friend class PHWaterContactEngine;
	friend struct PHWConvexCalc;

//protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	//追従対象
	UTRef<PHWaterTrackTarget> targets;

	UTRef<GRMaterial> material;
	UTRef<D3Material> materialD3;

	int				mxy;	//mx * my
	double			rx;		//mx * dh / 2.0
	double			ry;		//my * dh / 2.0
	
	matrix_type		p;				//圧力
	matrix_type		u, utmp;		//x方向流速
	matrix_type		v, vtmp;		//y方向流速
	matrix_type		height, htmp;	//高さ
	v3matrix_type	normal;			//法線
	v3matrix_type	tvec;			//
	Vec3d			vlight;			//光の向き

	///初期化処理
	void Init(SGScene* scene);

	///境界条件を設定する
	void Bound();

	///積分する
	void Integrate(double dt);

	///屈折ベクトルの計算
	Vec3d calRefracVec(Vec3d n, Vec3d v, double ra);

	void RenderD3(SGFrame* fr, D3Render* render);
	void RenderGL(SGFrame* fr, GLRender* render);
	DWORD GetColor(float h);
};
class CDWater: public CDGeometry{
public:
	SGOBJECTDEF(CDWater);
	UTRef<PHWater> water;
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
	virtual int GeometryID();
};

typedef UTRefArray<PHWater> PHWaters;

class PHWaterEngine : public /*SGBehaviorEngine*/PHSolverBase{
public:
	SGOBJECTDEF(PHWaterEngine);

	PHWaters waters;
	
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
	int GetPriority() const {return SGBP_DYNAMICALSYSTEM;}
	virtual void Step(SGScene* s);
	virtual void ClearForce();
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s);
	virtual size_t NChildObjects(){ return waters.size(); }
	virtual SGObject* ChildObject(size_t i){ return (SGObject*)&*(waters[i]); }

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};

}

#endif