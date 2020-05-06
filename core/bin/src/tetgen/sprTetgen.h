#include <Base/Affine.h>
#include <Graphics/SprGRMesh.h>

extern "C"{
	bool __declspec(dllexport) __cdecl sprTetgen(int& nVtxsOut, Spr::Vec3d*& vtxsOut, int& nTetsOut, int*& tetsOut, int nVtxIn, Spr::Vec3d* vtxIn, int nFacesIn, Spr::GRMeshFace* facesIn, char* option);
	void __declspec(dllexport) __cdecl sprTetgenFree(Spr::Vec3d* vtxOut, int* tetsOut);
}
