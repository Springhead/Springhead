#include "Collision.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDCutRing.h"

namespace Spr{;

void CDCutRing::MakeRing(){
	//	Mullar•Preparata‚ÅCŒ`‚ð‹‚ß‚é
	//	cutLines->CreateHull();
}
void CDCutRing::Print(std::ostream& os){
	DSTR << lines.size() << " lines" << std::endl;
	for(unsigned i=0; i<lines.size(); ++i){
		Vec3d n(lines[i].normal.x, lines[i].normal.y, 0);
		n = local.Ori() * n;
		DSTR << n << " d=" << lines[i].dist << std::endl;
	}
};

}
