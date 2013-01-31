#include <Physics/PHFemBase.h>
//#include <Physics/PHFemMeshNew.h>

namespace Spr{;

///////////////////////////////////////////////////////////////////
/* ŒvŽZƒ‚ƒWƒ…[ƒ‹‚Ì‹¤’Ê•”•ª */
void PHFem::SetPHFemMesh(PHFemMeshNew* m){ phFemMesh = m; }

PHFemMeshNew* PHFem::GetPHFemMesh(){ return phFemMesh; }

int PHFem::NVertices(){ return GetPHFemMesh()->NVertices(); }

}