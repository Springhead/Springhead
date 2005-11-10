#ifndef PHPENALTY_H
#define PHPENALTY_H
#include "Physics.h"

namespace Spr{;

/**	2つのフレーム間の衝突を検出し，ペナルティを求め，力を加える．
	PHPenaltyEngine が持つ．	*/
class PHPenalty:public SGObject, public CDFramePairWithNormal{
	SGOBJECTDEF(PHPenalty);
	float convertedMass;			///<	2剛体の換算質量．バネ係数を決めるために使用．
	float area;						///<	2つのフレーム間の接触領域の面積
	float dt;						///<	delta t
protected:
public:
	UTRef<PHSolid> solid[2];		///<	結果を反映させる剛体

public:
	///
	PHPenalty(){ Init(); }
	///
	PHPenalty(PHSolid* s1, PHSolid* s2){ Init(); Set(s1, s2); }
	~PHPenalty(){
		solid[0] = NULL;
		solid[1] = NULL;
	}
	///	detector を初期化する．
	void Init();
	///	ロード後，参照が解決してから，convertedMass を計算する．
	void Loaded(SGScene* scene);
	///	Solidを設定
	void Set(PHSolid* s1, PHSolid* s2);
	///	dt だけシミュレーションを進める．
	void Step(SGScene* s);
	///	摩擦のバネの表示
	void Draw(SGScene* s);
	///	換算質量を計算
	void CalcConvertedMass();
	///	solidやdetectorをNULLでクリアする．メモリリーク対策．
	void Clear();
	///
	bool operator < (const PHPenalty& p) const {
		if (solid[0] < p.solid[0]) return true;
		if (p.solid[0] < solid[0]) return false;
		if (solid[1] < p.solid[1]) return true;
		return false;
	}
	bool AddChildObject(SGObject* o, SGScene* s);
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new PHConvexPair;
	}
	virtual CDGeometryPair* CreateGeometryPair(){
		return new PHGeometryPair;
	}
	friend class PHPenaltyLoader;
	friend class PHPenaltySaver;
};
///	PHPenaltyの配列
class PHPenalties:public std::vector< UTRef<PHPenalty> >{
};
/**	ペナルティ法のエンジン．
	衝突判定を行い，ペナルティを求め，PHSolidに力を加える．	*/
class PHPenaltyEngine:public SGBehaviorEngine{
	SGOBJECTDEF(PHPenaltyEngine);
public:
	///	2つの剛体のペナルティ計算
	PHPenalties penalties;
	bool AddChildObject(SGObject* o, SGScene* s);
	int GetPriority() const { return SGBP_PENALTYENGINE; }
	///	時間を dt 進める
	virtual void Step(SGScene* s);
	///	ペナルティをクリアする．
	virtual void Clear(SGScene* s);
	virtual void Loaded(SGScene* scene){
		for(PHPenalties::iterator it = penalties.begin(); it != penalties.end(); ++it){
			(*it)->Loaded(scene);
		}
	}
};


}
#endif
