//#ifndef OPVERTEX_H							// ヘッダファイルを作る時のおまじない
//#define OPVERTEX_H								// 同上

#include <Springhead.h>								// SpringheadのIf型を使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル

#include <set>
#include <iostream>
#include <sstream>
#include <GL/glut.h> 
#include <Windows.h> 


#define ESC 27	
using namespace std;
using namespace Spr;

class Vertex{
public:
	bool deleted;
	Vec3d pos;		//position
	Vec3d vel;		//velocity
	Vec3d force;
	Vec3d normal;
	double area;	//	頂点の担当面積
	void AddForce(Vec3d f);
	Vertex():deleted(false){}
};

class OpVertex: public Vertex
{
public:
	//CDQHPlane<QhullVertex>* horizon;
	Vec3f orientation;
	///  頂点の位置
	Vec3f GetPos() const{ return pos;}
	//@}
	///	位置の設定
	void SetPos(Vec3d p){ pos = p; }
};