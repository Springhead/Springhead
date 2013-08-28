#include <Base/TQuaternion.h>

using namespace Spr;

int __cdecl main(){
	Quaterniond q = Quaterniond::Rot(Rad(30), 'y');
	Vec3d r1 = q.RotationHalf();
	std::cout << q << r1 << std::endl;	
	q = -q;
	Vec3d r2 = q.RotationHalf();
	std::cout << q << r2 << std::endl;	
	if (r1 == r2) return 0;
	return -1;
}
