#ifndef SPRPHOPSPHASHAGENT_H
#define SPRPHOPSPHASHAGENT_H

#include <Foundation/SprObject.h>
//#include <Physics/PHOpObj.h>
//#include <Physics/PHOpSpHashColliAgent.h>

//class PHOpSpHashColliAgentDesc
//{
//
//}

namespace Spr{;

class Bounds
{
public:
	Bounds(){}
	inline Bounds(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

	inline void set(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

	void clamp(Vec3f &pos){//‹«ŠE‚ð’´‚¦‚½“_‚ð‚»‚Ì‚Ü‚Ü‹«ŠE‚Ì‚Ç‚±‚ëÝ’è‚·‚é
		if (isEmpty()) return;
		//pos.maximum(min);
		maximum(pos, min);
		minimum(pos, max);
	};
	inline bool isEmpty() const {
		if (min.x > max.x&&min.y > max.y&&min.z > max.z) return true;
		return false;
	}
	inline void minimum(Vec3f &self, Vec3f &other) {
		if (other.x < self.x) self.x = other.x;
		if (other.y < self.y) self.y = other.y;
		if (other.z < self.z) self.z = other.z;

	}
	inline void maximum(Vec3f &self, Vec3f &other) {
		if (other.x > self.x) self.x = other.x;
		if (other.y > self.y) self.y = other.y;
		if (other.z > self.z) self.z = other.z;
	}
	Vec3f min, max;
};


struct PHOpSpHashColliAgentDesc{

};

struct PHOpSpHashColliAgentIf: public ObjectIf{
	SPR_IFDEF(PHOpSpHashColliAgent);
	void EnableCollisionDetection(bool able = true);
	void Initial(float cellSize, Bounds bounds);
	void OpCollisionProcedure(int myTimeStamp);
	void OpCollisionProcedure();
	
	void AddContactPlane(Vec3f planeP, Vec3f planeN);
	bool CollisionEnabled();

};


}
#endif