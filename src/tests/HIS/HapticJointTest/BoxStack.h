#ifndef BOXSTACK_H
#define BOXSTACK_H

#include "Synchronization.h"
#include <Framework/SprFWAppGLUT.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHContactDetector.h>
#include <Physics/PHContactPoint.h>
#include <Base/Combination.h>
#include<Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>

using namespace Spr;
using namespace PTM;
using namespace std;

struct PHSceneSolid{
	PHSolidIf* phSolidIf;
	bool bneighbor;
	bool blocal;
	bool bfirstlocal;
};

class BoxStack : public FWAppGLUT, public UTRefCount{
public:
	bool naga;
	// xFileのロードに使う変数
	const char* filePath;		//< xFileが置いてあるフォルダへの相対パス
	const char* fileName;		//< ロードするファイル名
	const char* hPointerName;	//< 力覚ポインタの名前

	//　プロセス間の同期に使う変数
	volatile bool bsync;
	bool calcPhys;
	volatile int hapticcount;
	FWWin* window;
	PHSceneIf* phscene;		
	GRRenderIf* render;		
	double dt;
	Vec3d gravity;
	double nIter;
	bool bGravity;

	// 剛体に使う変数
	PHSolidIf* soPointer;
	vector<PHSolidIf*> soBox;
	PHSolid phpointer;
	/*CDBoxIf* meshBox;*/
	//Floor
	PHSolidIf*			soFloor;
	CDShapeIf*			shapeFloor;
	//Object
	PHSolidDesc			desc;
	PHSolidIf*			soBox2[3];
	PHJointIf*			jointBox;
	vector<PHSolidIf*>	soObject;
	CDBoxIf*			shapeBox;
	CDSphereIf*			shapeSphere;

	//ポインタによるつかみ
	PHNeighborObject	grabObject;
	Vec3d				grabForce;
	bool				grabObjectFlag;
	double				grabForceRate;

	//キーボード用のカウント変数
	double countKey;
	// 近傍物体探索に使う変数
	double range;
	vector<PHNeighborObject> neighborObjects;	///<近傍物体を格納する
	vector<PHSceneSolid> sceneSolids;
	// 予測シミュレーションに使う変数
	UTRef<ObjectStatesIf> states, states2;
	//デバック表示に使う変数
	bool bStep;
	bool bOneStep;
	bool bDebug;
	vector<PHSolid> hapticsolids;

	struct Edge{
		float edge;				///<	端の位置(グローバル系)
		int	index;				///<	元の solidの位置
		bool bMin;				///<	初端ならtrue
		bool operator < (const Edge& s) const { return edge < s.edge; }
	};
	typedef std::vector<Edge> Edges;
	static BoxStack* GetInstance(){return (BoxStack*)instance;}
	BoxStack();	
	void DesignObject();
	void CreateObject();
	bool CopyObject(const char* phName, const char* grName);

	void Init(int argc, char* argv[]);		
	void SetHapticPointer();
	void InitCameraView();		
	void Start();
	void Idle();
	void PhysicsStep();
	void Display();		
	void UpdateHapticPointer();
	void FindNearestObject();
	void PredictSimulation();
	void FindNearestPoint();
	void DisplayContactPlane();
	void DisplayLineToNearestPoint();
	void DrawHapticSolids();
//	bool CopyObject(const char* phName, const char* grName);
	void Keyboard(unsigned char key);

	void GrabSpring(double dt);
}; 
extern BoxStack bstack;


#endif