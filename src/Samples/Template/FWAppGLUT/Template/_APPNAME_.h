#ifndef #{APPLICATIONNAME}_H
#define #{APPLICATIONNAME}_H

#include <Springhead.h>
#include <Framework/SprFWAppGLUT.h>

using namespace Spr;
using namespace PTM;
using namespace std;

class #{APPLICATIONNAME} : public FWAppGLUT, public UTRefCount{
public:
	FWWin* window;

	double dt;
	Vec3d gravity;
	double nIter;
	bool bGravity;

	// 剛体に使う変数

	//デバック表示に使う変数
	bool bDebug;
	bool bStep;
	bool bOneStep;

	#{APPLICATIONNAME}();	
	void Init(int argc, char* argv[]);		
	void Reset();
	void InitCameraView();										
	void DesignObject();
	void Step();
	void Display();		
	void Keyboard(int key, int x, int y);
}; 

extern #{APPLICATIONNAME} app;

#endif
