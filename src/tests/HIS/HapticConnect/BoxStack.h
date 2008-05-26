#ifndef BOXSTACK_H
#define BOXSTACK_H

#include "Synchronization.h"
#include <Framework/SprFWAppGLUT.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHContactDetector.h>
#include <Physics/PHContactPoint.h>
#include<Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>
#include <Base/TMatrix.h>

using namespace Spr;
using namespace PTM;
using namespace std;

struct PHSceneSolid{
	PHSolidIf* phSolidIf;
	bool blocal;
	bool bfirstlocal;
};

class BoxStack : public FWAppGLUT, public UTRefCount{
public:
	FWWin* window;
	PHSceneIf* phscene;		
	GRDebugRenderIf* render;		
	double dt;
	Vec3d gravity;
	double nIter;			
	bool bStep;
	// 剛体に使う変数
	PHSolidDesc desc;
	PHSolidIf* soFloor;
	PHSolidIf* soPointer;
	vector<PHSolidIf*> soBox;
	PHSolid phpointer;
	CDConvexMeshIf* meshFloor;
	CDConvexMeshIf* meshConvex;
	//　プロセス間の同期に使う変数
	bool bsync;
	vector<PHNeighborObject> neighborObjects;	///<近傍物体を格納する
	vector<PHSceneSolid> sceneSolids;
	// 予測シミュレーションに使う変数
	UTRef<ObjectStatesIf> states;

	vector<PHSolid> hapticsolids;

	BoxStack();	
	void Init(int argc, char* argv[]);				
	void InitCameraView();										
	void Start();
	void DesignObject();
	void Step();
	void Display();		
	void UpdateHapticPointer();
	void FindNearestObject();
	void PredictSimulation();
	void FindNearestPoint();
	void DisplayLineToNearestPoint();
	void DrawHapticSolids();
	void Keyboard(unsigned char key);	
}; 

#endif