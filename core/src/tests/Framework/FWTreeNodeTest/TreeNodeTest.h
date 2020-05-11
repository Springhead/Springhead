#ifndef TreeNodeTest_H
#define TreeNodeTest_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>

using namespace Spr;


class TreeNodeTest : public FWApp{
public:
	FWWinIf* window;

	double dt;
	Vec3d gravity;
	double nIter;
	bool bGravity;
	// 剛体に使う変数
	PHSolidIf* soFloor;
	PHSolidIf* soPointer;
	std::vector<PHSolidIf*> links[2];
	std::vector<PHBallJointIf*> joints[2];
	CDBoxIf* meshFloor;
	//デバック表示に使う変数
	bool bDebug;
	bool bStep;
	bool bOneStep;

	TreeNodeTest();	
	void Init(int argc, char* argv[]);		
	void Reset();
	void BuildScene();
	void UserFunc();
	void Keyboard(int key, int x, int y);
}; 
extern TreeNodeTest ttest;

#endif