#include "springhead.h"
#include "Object.h"
#include "Scene.h"
#pragma hdrstop
#include <sstream>

namespace Spr {;

//----------------------------------------------------------------------------
//	Object
OBJECTIMPBASE(Object);

void Object::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << ">" << std::endl;
	os.width(w);
}
void Object::SetName(const char* n){
	if (name.compare(n) == 0) return;
	assert(scene);
	if (name.length()) scene->names.Del(this);
	name = n;
	scene->names.Add(this);
}


}

