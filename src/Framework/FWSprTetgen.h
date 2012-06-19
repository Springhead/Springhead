#ifndef FWSPRTETGEN_H
#define FWSPRTETGEN_H
#ifndef SWIG

namespace Spr{;

struct GRMeshFace;

bool __cdecl sprTetgen(int& nVtxsOut, Spr::Vec3d*& vtxsOut, int& nTetsOut, int*& tetsOut, int nVtxsIn, Spr::Vec3d* vtxsIn, int nFacesIn, Spr::GRMeshFace* facesIn, char* option);
void __cdecl sprTetgenFree(Spr::Vec3d* vtxOut, int* tetsOut);

};

#endif
#endif
