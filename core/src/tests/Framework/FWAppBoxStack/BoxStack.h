#ifndef BOXSTACK_H
#define BOXSTACK_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>

using namespace Spr;
using namespace PTM;
using namespace std;



class BoxStack : public FWApp{
public:
	FWWinIf* window;

	double dt;
	Vec3d gravity;
	double nIter;
	bool bGravity;
	// 剛体に使う変数
	PHSolidDesc desc;
	PHSolidIf* soFloor;
	PHSolidIf* soPointer;
	vector<PHSolidIf*> soBox;
	CDConvexMeshIf* meshFloor;
	CDConvexMeshIf* meshConvex;
	CDBoxIf* meshBox;
	CDSphereIf* meshSphere;
	CDCapsuleIf* meshCapsule;
	//デバック表示に使う変数
	bool bDebug;
	bool bStep;
	bool bOneStep;

	BoxStack();	
	void Init(int argc, char* argv[]);		
	void Reset();
	void BuildScene();
	void Step();
	void Keyboard(int key, int x, int y);
}; 
extern BoxStack bstack;

#endif