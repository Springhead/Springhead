#ifndef SPRPHOPSPHASHAGENT_H
#define SPRPHOPSPHASHAGENT_H

#include <Collision/SprCDSpHash.h>
#include <Physics/SprPHOpObj.h>

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
	void Initial(float cellSize, CDBounds bounds);
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
