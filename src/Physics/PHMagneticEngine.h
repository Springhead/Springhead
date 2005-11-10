// PHMagneticEngine.h: PHMagneticEngine クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_)
#define AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_

#include "PHSolid.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>
#include <Graphics/GRMesh.h>


namespace Spr{;

//データを入れる構造体
typedef Vec3f		Vector;
DEF_RECORD(XMagnet,{
	GUID Guid(){ return WBGuid("F99E7A61-47F2-4960-A430-F0E45C1B552B"); }
	Vector	sPos;
	Vector	nPos;
	FLOAT	magPow;
});

//磁石クラス
class PHMagnet : public SGObject
{
public:
	void Clear();
	XMagnet		magn;
	PHSolid	*	solid;
	SGOBJECTDEF(PHMagnet);
	bool AddChildObject(SGObject* o,SGScene* s);	
};

class PHMagnets:public std::vector< UTRef<PHMagnet> >{
public:
	UTRef<PHMagnet>* Find(const UTRef<PHMagnet>& s){
		iterator it = std::find(begin(), end(), s);
		if (it == end()) return NULL;
		else return &*it;
	}
	UTRef<PHMagnet>* Find(const UTRef<PHMagnet>& s) const {
		return ((PHMagnets*)this)->Find(s);
	}
};

/////////////////////////////////////////////////////////
//MagneticEngine
/////////////////////////////////////////////////////////
DEF_RECORD(XMagneticEngine,{
	GUID Guid(){ return WBGuid("A6217078-938D-4bdc-9AA8-59738B5C0CC6"); }
	Vector earthMagnet;
});

class PHMagneticEngine : public SGBehaviorEngine  
{
public:
	void LineDraw(Vec3f center,SGScene *s);
	void EarthPower(PHMagnet *one);
	void MagnetForce(PHMagnet *one,PHMagnet *two, SGScene *s);
//	void ForceCom(int one_kyoku,PHMagnet *one,SGScene *s);
//	void ApplyForce(int a_kyoku,int b_kyoku,SGScene *s);
	void AddForce(PHMagnet *one, PHMagnet *two,int flag);
	bool AddChildObject(SGObject* o,SGScene* s);
	XMagneticEngine	mag;
	PHMagnets		mags;
	UTRef<GRMesh>	mesh;
	void Clear(SGScene* s){
		for(PHMagnets::iterator it = mags.begin(); it != mags.end(); ++it){
			(*it)->Clear();
		}
		mags.clear();
	}
	virtual void Step(SGScene *s);
	SGOBJECTDEF(PHMagneticEngine);
	PHMagneticEngine();
	
private:
	//void Draw(SGScene *s,Vec3f *point);
};

}
#endif // !defined(AFX_PHMAGNETICENGINE_H__EB4D369F_EA8F_42D5_94C1_36C5FD8B8E02__INCLUDED_)
