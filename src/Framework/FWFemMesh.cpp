#include "Framework.h"
#include <Framework/SprFWFemMesh.h>
#include <Graphics/GRFrame.h>
#include <Physics/PHConstraint.h>
#include "FWFemMesh.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWFemMesh::FWFemMesh(const FWFemMeshDesc& d):FWFemMeshDesc(d), grMesh(NULL){
}
size_t FWFemMesh::NChildObject() const{
	return FWObject::NChildObject() + (grMesh ? 1 : 0);
}
ObjectIf* FWFemMesh::GetChildObject(size_t pos){
	if (pos < FWObject::NChildObject()){
		return FWObject::GetChildObject(pos);
	}
	if (grMesh && pos < NChildObject()) return grMesh;
}

bool FWFemMesh::AddChildObject(ObjectIf* o){
	if (DCAST(GRMesh, o)){
		GRMesh* mesh = (GRMesh*)o;
		//	tetgen‚Æ‚©‚â‚Á‚ÄPH‚ðì‚é
	}
	FWObject::AddChildObject(o);
}
}
