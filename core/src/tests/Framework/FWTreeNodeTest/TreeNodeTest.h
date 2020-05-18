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
	std::vector<PHSolidIf*> links[2];		//	リンクのSolid
	std::vector<PHBallJointIf*> joints[2];	//	リンクをつなぐジョイント

	bool bRun;	//	タイマーを走らせるかどうか
	//デバック表示に使う変数
	void Init(int argc, char* argv[]);		
	void BuildScene();
	void UserFunc();
	void TimerFunc(int id);
	void Step();
	void Keyboard(int key, int x, int y);
}; 
extern TreeNodeTest ttest;

#endif