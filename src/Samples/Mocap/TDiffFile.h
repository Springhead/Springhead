#ifndef TDIFFFILE_H
#define TDIFFFILE_H

#include <Graphics/GRRender.h>
#include <Base/Affine.h>
#include <vector>
#include "THuman.h"

///	DIFFファイルの一行
class TDiffRecord{
public:
	std::string type;
	float time;
	typedef std::vector<Spr::Vec3f> TData;
	TData data;
	TDiffRecord():time(0.0f){}
	bool Load(std::istream& is);
	float LoadField(std::istream& is);
	void Print(std::ostream& os);
};

///	Diffファイル１つに対応するクラス
class TDiffFile {
public:
	class TRecords:public std::vector<TDiffRecord>{
	public:
		void Print(std::ostream& os);
		void Draw(Spr::GRRender* render, float time);
		bool GetRecord(TDiffRecord& pos, TDiffRecord& vel, float time);
		int curIndex;
		TRecords():curIndex(0){}
	};
	TRecords g1[2];	///<	G1L, G1R
	TRecords g2[2];	///<	G2L, G2R
	TRecords frf;	///<	FRF
	bool bLoaded;
	TDiffFile(): bLoaded(false){}
	bool IsLoaded(){ return bLoaded; }
	bool Load(std::istream& is);
	void Print(std::ostream& os);
	void Draw(Spr::GRRender* render, THuman& human, float time, bool bDrawLine=true);
	void GetRecPos(TRecords*& rec, int &pos, int id, bool bLeft);
	///	通し番号で計測点の位置を取得する．全14点
	bool GetRecord(Spr::Vec3f* pos, Spr::Vec3f* vel, bool bLeft, float time);
	///	i1とi2の距離を求める．
	float GetLinkLength(TRecords& r1, int i1, TRecords& r2, int i2);
	float GetLinkLength(int id1, bool bLeft1, int id2, bool bLeft2);
	float GetLinkLength(int id1, int id2);
	void SetHumanScale(THuman& human);
	bool AddSpringForce(THuman& human, float time, float dt);
};

#endif