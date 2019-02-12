/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/
#ifndef SPRPHOPSPHASHAGENT_H
#define SPRPHOPSPHASHAGENT_H

#include <Collision/SprCDSpHash.h>
#include <Physics/SprPHOpObj.h>


namespace Spr{;




struct PHOpSpHashColliAgentDesc{
	bool useDirColli;
	float collisionCstrStiffness;
};

struct PHOpSpHashColliAgentIf: public ObjectIf{
	SPR_IFDEF(PHOpSpHashColliAgent);
	void SetCollisionCstrStiffness(float alpha);
	float GetCollisionCstrStiffness();
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
