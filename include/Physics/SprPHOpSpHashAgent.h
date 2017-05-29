#ifndef SPRPHOPSPHASHAGENT_H
#define SPRPHOPSPHASHAGENT_H

//#include <Foundation/SprObject.h>
#include <Physics/SprPHOpObj.h>
//#include <Physics/PHOpObj.h>
//#include <Physics/PHOpSpHashColliAgent.h>

//class PHOpSpHashColliAgentDesc
//{
//
//}

namespace Spr{;




struct PHOpSpHashColliAgentDesc{
	bool useDirColli;
};

struct PHOpSpHashColliAgentIf: public ObjectIf{
	SPR_IFDEF(PHOpSpHashColliAgent);
	void EnableCollisionDetection(bool able = true);
	void Initial(float cellSize, Bounds bounds);
	void OpCollisionProcedure(int myTimeStamp);
	void OpCollisionProcedure();
	void OpDirCollision();
	
	void AddContactPlane(Vec3f planeP, Vec3f planeN);
	bool IsCollisionEnabled();
	void SetIsDirCollision(bool flag);
	bool GetIsDirCollision();
	void SetDebugMode(bool flag);
	bool GetDebugMode();
};


}
#endif
