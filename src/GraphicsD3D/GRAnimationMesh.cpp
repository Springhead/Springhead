/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "GraphicsD3D.h"
#include "GRAnimationMesh.h"
#include <string>
#include <algorithm>


namespace Spr{;

//----------------------------------------------------------------------------
//	GRAnimationMesh::AllocateHierarchy
class GRAnimationMesh::AllocateHierarchy : public ID3DXAllocateHierarchy{
public:
	AllocateHierarchy(const std::string& filename="");
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ 
	                               LPCSTR Name, 
	                               CONST D3DXMESHDATA *pMeshData,
	                               CONST D3DXMATERIAL *pMaterials, 
	                               CONST D3DXEFFECTINSTANCE *pEffectInstances, 
	                               DWORD NumMaterials, 
	                               CONST DWORD *pAdjacency, 
	                               LPD3DXSKININFO pSkinInfo, 
	                               LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameBase);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
private:
	template<typename T> T* AllocateArray(size_t count, const T *src);
	LPDIRECT3DTEXTURE9*     AllocateTextures(DWORD NumMaterials, CONST D3DXMATERIAL *pMaterials, LPDIRECT3DDEVICE9 device);
protected:
	std::string directory;
};

GRAnimationMesh::AllocateHierarchy::AllocateHierarchy(const std::string& filename){
	directory = (filename.find_last_of("/\\")==std::string::npos) ? "" : filename.substr(0,filename.find_last_of("/\\")+1);
}

HRESULT GRAnimationMesh::AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame){
	Frame *result = new Frame();
	ZeroMemory(result, sizeof(Frame));
	try{
		result->Name							= (Name==NULL) ? NULL : AllocateArray(strlen(Name)+1, Name);
		result->TransformationMatrix			= D3DXMATRIX(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		result->pMeshContainer					= NULL;
		result->pFrameSibling					= NULL;
		result->pFrameFirstChild				= NULL;
		result->CombinedTransformationMatrix	= D3DXMATRIX(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		result->overrideWeight					= 0;
	}
	catch(...){
		return E_FAIL;
	}
	*ppNewFrame = result;
	return D3D_OK;
}

HRESULT GRAnimationMesh::AllocateHierarchy::CreateMeshContainer(LPCSTR Name,
                                                               CONST D3DXMESHDATA *pMeshData,
                                                               CONST D3DXMATERIAL *pMaterials,
                                                               CONST D3DXEFFECTINSTANCE *pEffectInstances,
                                                               DWORD NumMaterials,
                                                               CONST DWORD *pAdjacency,
                                                               LPD3DXSKININFO pSkinInfo,
															   LPD3DXMESHCONTAINER *ppNewMeshContainer){
	if(pMeshData->Type != D3DXMESHTYPE_MESH){
		DSTR << "Loading meshes of any type except D3DXMESHTYPE_MESH is not implemented." << std::endl;
		return E_FAIL;
	}
	for(DWORD i=0; i<NumMaterials; i++){
		if(pEffectInstances[i].pEffectFilename != NULL){
			DSTR << "Loading meshes using effects is not implemented." << std::endl;
			return E_FAIL;
		}
	}

	MeshContainer *result;
	result = new MeshContainer();
	ZeroMemory(result, sizeof(MeshContainer));

	CComPtr<IDirect3DDevice9> device;
	pMeshData->pMesh->GetDevice(&device);

	try{
		result->Name				= (Name==NULL) ? NULL : AllocateArray(strlen(Name)+1, Name);
		result->MeshData.Type		= pMeshData->Type;
		result->MeshData.pMesh		= pMeshData->pMesh;  if(pMeshData->pMesh) pMeshData->pMesh->AddRef();
		result->pMaterials			= AllocateArray(NumMaterials, pMaterials);
		result->pEffects			= NULL;
		result->NumMaterials		= NumMaterials;
		result->pAdjacency			= AllocateArray(pMeshData->pMesh->GetNumFaces()*3, pAdjacency);
		result->pSkinInfo			= pSkinInfo;  if(pSkinInfo) pSkinInfo->AddRef();
		result->boneOffsetMatrices	= NULL;
		result->boneFrameMatrices	= NULL;
		result->ppTextures			= AllocateTextures(NumMaterials, pMaterials, device);
		
		for(DWORD i=0; i<result->NumMaterials; i++){
			if(result->pMaterials[i].pTextureFilename != NULL){
				LPSTR newTextureFilename = new char[ strlen(result->pMaterials[i].pTextureFilename)+1 ];
				strcpy( newTextureFilename, result->pMaterials[i].pTextureFilename );
				result->pMaterials[i].pTextureFilename = newTextureFilename;
			}

			if(result->pMaterials[i].MatD3D.Ambient.r==0
			&& result->pMaterials[i].MatD3D.Ambient.g==0
			&& result->pMaterials[i].MatD3D.Ambient.b==0){
				result->pMaterials[i].MatD3D.Ambient = result->pMaterials[i].MatD3D.Diffuse;
			}
		}
	}
	catch(...){
		return E_FAIL;
	}

	*ppNewMeshContainer = result;
	return D3D_OK;
}

HRESULT GRAnimationMesh::AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameBase){
	Frame *pFrame = (Frame*)pFrameBase;
	delete [] pFrame->Name;
	delete    pFrame;
	return D3D_OK;
}

HRESULT GRAnimationMesh::AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase){
	MeshContainer *pMeshContainer = (MeshContainer*)pMeshContainerBase;
	delete [] pMeshContainer->Name;
	if(pMeshContainer->MeshData.pMesh) pMeshContainer->MeshData.pMesh->Release();
	for(DWORD i=0; i<pMeshContainer->NumMaterials; ++i)  delete [] pMeshContainer->pMaterials[i].pTextureFilename;
	delete [] pMeshContainer->pMaterials;
	delete [] pMeshContainer->pEffects;
	delete [] pMeshContainer->pAdjacency;
	if(pMeshContainer->pSkinInfo) pMeshContainer->pSkinInfo->Release();
	delete [] pMeshContainer->boneOffsetMatrices;
	delete [] pMeshContainer->boneFrameMatrices;
	if(pMeshContainer->ppTextures) for(DWORD i=0; i<pMeshContainer->NumMaterials; ++i){ if(pMeshContainer->ppTextures[i]) pMeshContainer->ppTextures[i]->Release(); }
	delete [] pMeshContainer->ppTextures;
	delete    pMeshContainer;
	return D3D_OK;
}

template<typename T> T* GRAnimationMesh::AllocateHierarchy::AllocateArray(size_t count, const T *src){
	if(src==NULL) return NULL;
	T *result;
	result = new T[count];
	std::copy(src, src+count, result);
	return result;
}

LPDIRECT3DTEXTURE9* GRAnimationMesh::AllocateHierarchy::AllocateTextures(DWORD NumMaterials, CONST D3DXMATERIAL *pMaterials, LPDIRECT3DDEVICE9 device){
	if(NumMaterials==0) return NULL;
	LPDIRECT3DTEXTURE9 *result;
	result = new LPDIRECT3DTEXTURE9[NumMaterials];
	ZeroMemory(result, sizeof(LPDIRECT3DTEXTURE9)*NumMaterials);
	try{
		for(DWORD i=0; i<NumMaterials; i++){
			if(pMaterials[i].pTextureFilename != NULL){
				DWORD j;
				for(j=0; j<i; ++j){ if(pMaterials[j].pTextureFilename!=NULL && strcmp(pMaterials[j].pTextureFilename, pMaterials[i].pTextureFilename)==0) break; }
				if(j<i){
					result[i] = result[j];
					if(result[i]) result[i]->AddRef();
				}
				else{
					if(FAILED(D3DXCreateTextureFromFile(device, (directory + pMaterials[i].pTextureFilename).c_str(), &(result[i]))) ){
						DSTR << "Failed to load: " << (directory + pMaterials[i].pTextureFilename) << std::endl;
					}
				}
			}
		}
	}
	catch(...){
		for(DWORD i=0; i<NumMaterials; ++i) { if(result[i]) result[i]->Release(); }
		delete [] result;
		throw;
	}
	return result;
}

//----------------------------------------------------------------------------
//	GRAnimationMesh
IF_OBJECT_IMP(GRAnimationMesh, GRVisual);

GRAnimationMesh::GRAnimationMesh(const GRAnimationMeshDesc& desc):GRAnimationMeshDesc(desc){
	rootFrame = NULL;
	controller = NULL;
	loaded = false;
}

GRAnimationMesh::~GRAnimationMesh(){
	D3DXFrameDestroy(rootFrame, &AllocateHierarchy());
}

void GRAnimationMesh::SetMotion(const std::string& name){
	if(!loaded) if(!LoadMesh()) return;
	if(!rootFrame || !controller) return;
	
	CComPtr<ID3DXAnimationSet> aniSet;
	if(FAILED(controller->GetAnimationSetByName(name.c_str(), &aniSet))){
		DSTR << "Motion not found: " << name << std::endl;
		return;
	}
	controller->SetTrackAnimationSet(0, aniSet);
	controller->SetTrackEnable(0, TRUE);
	if(controller) controller->AdvanceTime(0, NULL);
}

void GRAnimationMesh::SetTime(double time){
	if(!loaded) if(!LoadMesh()) return;
	if(!rootFrame || !controller) return;
	
	controller->SetTrackPosition(0, time);
	if(controller) controller->AdvanceTime(0, NULL);
}

inline void PoseInvertZAxis(Posed& pose){
	pose.Ori().x *= -1;
	pose.Ori().y *= -1;
	pose.Pos().z *= -1;
}

Posed GRAnimationMesh::GetBoneKeyframePose(const std::string& name){
	if(!loaded) if(!LoadMesh()) return Posed::Unit();
	if(!rootFrame || !controller) return Posed::Unit();
	
	Posed result;
	assert(D3DXFrameFind(rootFrame, name.c_str()));
	result.FromAffine( *(Affinef*)& ((Frame*)D3DXFrameFind(rootFrame, name.c_str()))->TransformationMatrix );
	PoseInvertZAxis(result);	// DirectXÀ•WŒn‚©‚çSpringheadÀ•WŒn‚É•ÏŠ·
	return result;
}

void GRAnimationMesh::OverrideBoneOrientation(const std::string& name, const Quaterniond& orientation, double weight){
	if(!loaded) if(!LoadMesh()) return;
	if(!rootFrame || !controller) return;
	
	Frame* frame = (Frame*)D3DXFrameFind(rootFrame, name.c_str());
	assert(frame);
	frame->overridePose     = Posed::Unit(orientation);
	frame->overrideWeight   = weight;
	frame->overridePosition = false;
	PoseInvertZAxis(frame->overridePose);	// SpringheadÀ•WŒn‚©‚çDirectXÀ•WŒn‚É•ÏŠ·
}

void GRAnimationMesh::OverrideBonePose(const std::string& name, const Posed& pose, double weight){
	if(!loaded) if(!LoadMesh()) return;
	if(!rootFrame || !controller) return;
	
	Frame* frame = (Frame*)D3DXFrameFind(rootFrame, name.c_str());
	assert(frame);
	frame->overridePose     = pose;
	frame->overrideWeight   = weight;
	frame->overridePosition = true;
	PoseInvertZAxis(frame->overridePose);	// SpringheadÀ•WŒn‚©‚çDirectXÀ•WŒn‚É•ÏŠ·
}

void GRAnimationMesh::AddDrawSubsetListener(GRAnimationMeshDrawSubsetListenerFunc beforeFunc, GRAnimationMeshDrawSubsetListenerFunc afterFunc, void* ptr){
	DrawSubsetListener l;
	l.beforeFunc = beforeFunc;
	l.afterFunc  = afterFunc;
	l.ptr        = ptr;
	drawSubsetListeners.push_back(l);
}

void GRAnimationMesh::SetEffect(LPD3DXEFFECT effect, int matrixPaletteSize)
{
	if(!loaded) if(!LoadMesh()) return;
	assert(rootFrame);
	
	this->effect = effect;
	if(effect) CreateIndexedBlendedMeshes(rootFrame, matrixPaletteSize);
	else       InitFrame(rootFrame);
}

void GRAnimationMesh::Render(GRRenderIf* r){
	if(!loaded) if(!LoadMesh()) return;
	if(!rootFrame) return;
	
	LPDIRECT3DDEVICE9	d3ddevice = GRDeviceD3D::GetD3DDevice();
	D3DXMATRIX			world;
	D3DCULL				cull;

	d3ddevice->GetTransform(D3DTS_WORLD, &world);
	
	// ‚yŽ²‚ð”½“]‚·‚é‚Ì‚ÅƒJƒŠƒ“ƒO‚à‹t‚É‚·‚é
	d3ddevice->GetRenderState(D3DRS_CULLMODE, (DWORD*)&cull);
	d3ddevice->SetRenderState(D3DRS_CULLMODE, (cull==D3DCULL_NONE) ? D3DCULL_NONE : (cull==D3DCULL_CW) ? D3DCULL_CCW : D3DCULL_CW);

	if(effect){
		d3ddevice->SetTransform(D3DTS_WORLD, &(*D3DXMatrixScaling(&D3DXMATRIX(),1,1,-1) * world));
		UpdateFrame(rootFrame, *D3DXMatrixIdentity(&D3DXMATRIX()));
	}
	else{
		UpdateFrame(rootFrame, (*D3DXMatrixScaling(&D3DXMATRIX(),1,1,-1) * world));	// Šeƒ{[ƒ“‚ÌÀ•W•ÏŠ·iDirectXÀ•WŒnj -> ‚yÀ•W”½“] -> ƒ[ƒ‹ƒh•ÏŠ·iSpringheadÀ•WŒnj
	}

	DrawFrame(rootFrame);

	d3ddevice->SetTransform(D3DTS_WORLD, &world);
	d3ddevice->SetRenderState(D3DRS_CULLMODE, cull);	// ƒJƒŠƒ“ƒO‚ðŒ³‚É–ß‚·
}

void GRAnimationMesh::Rendered(GRRenderIf* r){
}

bool GRAnimationMesh::LoadMesh(){
	LPDIRECT3DDEVICE9 d3ddevice = GRDeviceD3D::GetD3DDevice();
	if(!d3ddevice) return false;

	LPD3DXFRAME rootFrameBase = NULL;
	if(FAILED(D3DXLoadMeshHierarchyFromX(filename.c_str(), D3DXMESH_MANAGED, d3ddevice, &AllocateHierarchy(filename), NULL, &rootFrameBase, &controller))){
		DSTR << "Failed to load: " << filename << std::endl;
		loaded = true;
		return true;
	}
	rootFrame = (Frame*)rootFrameBase;
	if(controller) { controller->ResetTime(); controller->AdvanceTime(0, NULL); }
	loaded = true;
	InitFrame(rootFrame);
	return true;
}

void GRAnimationMesh::InitFrame(Frame* frame){
	for(LPD3DXMESHCONTAINER meshContainer=frame->pMeshContainer; meshContainer!=NULL; meshContainer=meshContainer->pNextMeshContainer){
		if(meshContainer->pSkinInfo != NULL){
			CreateBlendedMesh((MeshContainer*)meshContainer);
			SetBoneMatrices((MeshContainer*)meshContainer);
		}
	}
	if(frame->pFrameSibling)    InitFrame((Frame*)frame->pFrameSibling);
	if(frame->pFrameFirstChild) InitFrame((Frame*)frame->pFrameFirstChild);
}

void GRAnimationMesh::CreateBlendedMesh(MeshContainer* meshContainer){
	DWORD* adjacency = new DWORD[meshContainer->MeshData.pMesh->GetNumFaces()*3];
	meshContainer->MeshData.pMesh->GenerateAdjacency(0, adjacency);
	meshContainer->pSkinInfo->ConvertToBlendedMesh(
		meshContainer->MeshData.pMesh, 0, adjacency, NULL, NULL, NULL,
		&meshContainer->maxFaceInfl, &meshContainer->numBoneCombinations,
		&meshContainer->boneCombinationTableBuffer, &meshContainer->blendedMesh
	);
	meshContainer->boneCombinationTable = (LPD3DXBONECOMBINATION)meshContainer->boneCombinationTableBuffer->GetBufferPointer();
	delete [] adjacency;
}

void GRAnimationMesh::SetBoneMatrices(MeshContainer* meshContainer){
	const DWORD n = meshContainer->pSkinInfo->GetNumBones();
	meshContainer->boneOffsetMatrices = new D3DXMATRIX*[n];
	meshContainer->boneFrameMatrices  = new D3DXMATRIX*[n];
	for(DWORD i=0; i<n; ++i){
		meshContainer->boneOffsetMatrices[i] = meshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
		meshContainer->boneFrameMatrices[i]  = &((Frame*)D3DXFrameFind(rootFrame, meshContainer->pSkinInfo->GetBoneName(i)))->CombinedTransformationMatrix;
	}
}

void GRAnimationMesh::CreateIndexedBlendedMeshes(Frame* frame, int matrixPaletteSize){
	for(LPD3DXMESHCONTAINER meshContainer=frame->pMeshContainer; meshContainer!=NULL; meshContainer=meshContainer->pNextMeshContainer){
		if(meshContainer->pSkinInfo != NULL){
			CreateIndexedBlendedMesh((MeshContainer*)meshContainer, matrixPaletteSize);
		}
	}
	if(frame->pFrameSibling)    CreateIndexedBlendedMeshes((Frame*)frame->pFrameSibling, matrixPaletteSize);
	if(frame->pFrameFirstChild) CreateIndexedBlendedMeshes((Frame*)frame->pFrameFirstChild, matrixPaletteSize);
}

void GRAnimationMesh::CreateIndexedBlendedMesh(MeshContainer* meshContainer, int matrixPaletteSize){
	DWORD* adjacency = new DWORD[meshContainer->MeshData.pMesh->GetNumFaces()*3];
	D3DCAPS9 caps;  GRDeviceD3D::GetD3DDevice()->GetDeviceCaps(&caps);

	meshContainer->MeshData.pMesh->GenerateAdjacency(0, adjacency);
	meshContainer->matrixPaletteSize = min((DWORD)matrixPaletteSize, min(meshContainer->pSkinInfo->GetNumBones(), (caps.MaxVertexBlendMatrixIndex+1)/2));
	meshContainer->blendedMesh.Release();
	meshContainer->boneCombinationTableBuffer.Release();
	meshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
		meshContainer->MeshData.pMesh, 0, meshContainer->matrixPaletteSize, adjacency, NULL, NULL, NULL,
		&meshContainer->maxVertexInfl, &meshContainer->numBoneCombinations,
		&meshContainer->boneCombinationTableBuffer, &meshContainer->blendedMesh
	);
	meshContainer->boneCombinationTable = (LPD3DXBONECOMBINATION)meshContainer->boneCombinationTableBuffer->GetBufferPointer();

	delete [] adjacency;
}

void GRAnimationMesh::UpdateFrame(Frame *frame, const D3DXMATRIX& parentMatrix){
	if(frame->overrideWeight<=0){
		frame->CombinedTransformationMatrix = frame->TransformationMatrix * parentMatrix;
	}
	else if(frame->overrideWeight>=1){
		if(frame->overridePosition){
			Affinef af;
			frame->overridePose.ToAffine(af);
			D3DXMatrixMultiply(&frame->CombinedTransformationMatrix, (D3DXMATRIX*)&af, &parentMatrix);
		}
		else{
			Affinef af = (*(Affinef*)&frame->TransformationMatrix);
			frame->overridePose.Ori().ToMatrix(af.Rot());
			D3DXMatrixMultiply(&frame->CombinedTransformationMatrix, (D3DXMATRIX*)&af, &parentMatrix);
		}
	}
	else{
		Affinef af = (*(Affinef*)&frame->TransformationMatrix);
		Quaterniond frameOri;  frameOri.FromMatrix(af.Rot());
		interpolate(frame->overrideWeight, frameOri, frame->overridePose.Ori()).ToMatrix(af.Rot());
		if(frame->overridePosition)  af.Pos() = frame->overridePose.Pos() * frame->overrideWeight + af.Pos() * (1-frame->overrideWeight);
		D3DXMatrixMultiply(&frame->CombinedTransformationMatrix, (D3DXMATRIX*)&af, &parentMatrix);
	}

	if(frame->pFrameSibling)    UpdateFrame((Frame*)frame->pFrameSibling,    parentMatrix);
	if(frame->pFrameFirstChild) UpdateFrame((Frame*)frame->pFrameFirstChild, frame->CombinedTransformationMatrix);
}

void GRAnimationMesh::DrawFrame(const Frame *frame){
	for(LPD3DXMESHCONTAINER meshContainer=frame->pMeshContainer; meshContainer!=NULL; meshContainer=meshContainer->pNextMeshContainer){
		if(meshContainer->pSkinInfo != NULL)  DrawSkinnedMeshContainer((MeshContainer*)meshContainer);
		else                                  DrawNormalMeshContainer((MeshContainer*)meshContainer, frame);
	}
	if(frame->pFrameSibling)    DrawFrame((Frame*)frame->pFrameSibling);
	if(frame->pFrameFirstChild) DrawFrame((Frame*)frame->pFrameFirstChild);
}

void GRAnimationMesh::DrawSkinnedMeshContainer(MeshContainer *meshContainer){
	LPDIRECT3DDEVICE9 d3ddevice = GRDeviceD3D::GetD3DDevice();

	if(effect){
		D3DXHANDLE paramWorld               = effect->GetParameterBySemantic(NULL, "WORLD");
		D3DXHANDLE paramMaxVertexInfl       = effect->GetParameterBySemantic(NULL, "MAXVERTEXINFL");
		D3DXHANDLE paramBoneMatrices        = effect->GetParameterBySemantic(NULL, "BONEMATRIXARRAY");
		D3DXHANDLE paramBoneQuaternionsReal = effect->GetParameterBySemantic(NULL, "BONEQUATERNIONARRAYREAL");
		D3DXHANDLE paramBoneQuaternionsDual = effect->GetParameterBySemantic(NULL, "BONEQUATERNIONARRAYDUAL");
		D3DXHANDLE paramDiffuseColor        = effect->GetParameterBySemantic(NULL, "DIFFUSECOLOR");

		D3DXMATRIX world;
		d3ddevice->GetTransform(D3DTS_WORLD, &world);
		effect->SetMatrix(paramWorld, &world);

		d3ddevice->SetFVF(meshContainer->blendedMesh->GetFVF());
		effect->SetInt(paramMaxVertexInfl, meshContainer->maxVertexInfl);

		for(DWORD subset=0; subset<meshContainer->numBoneCombinations; subset++){
			std::vector<D3DXMATRIX>     worldMatrices(meshContainer->matrixPaletteSize);
			std::vector<D3DXQUATERNION> boneQuaternionsReal(meshContainer->matrixPaletteSize);
			std::vector<D3DXQUATERNION> boneQuaternionsDual(meshContainer->matrixPaletteSize);

			assert(paramBoneMatrices || (paramBoneQuaternionsReal && paramBoneQuaternionsDual));

			for(DWORD i=0; i<meshContainer->matrixPaletteSize; i++){
				const DWORD b = meshContainer->boneCombinationTable[subset].BoneId[i];
				if(b==UINT_MAX) break;
				D3DXMatrixMultiply(&worldMatrices[i], meshContainer->boneOffsetMatrices[b], meshContainer->boneFrameMatrices[b]);
				if(paramBoneQuaternionsReal && paramBoneQuaternionsDual){
					D3DXQuaternionRotationMatrix(&boneQuaternionsReal[i], &worldMatrices[i]);
					const D3DXVECTOR3& t = *(D3DXVECTOR3*)&worldMatrices[i]._41;
					D3DXQuaternionMultiply(&boneQuaternionsDual[i], &boneQuaternionsReal[i], &D3DXQUATERNION(t.x/2, t.y/2, t.z/2, 0));
				}
			}

			if(paramBoneMatrices)        effect->SetMatrixArray(paramBoneMatrices, &worldMatrices[0], (UINT)worldMatrices.size());
			if(paramBoneQuaternionsReal) effect->SetVectorArray(paramBoneQuaternionsReal, (D3DXVECTOR4*)&boneQuaternionsReal[0], (UINT)boneQuaternionsReal.size());
			if(paramBoneQuaternionsDual) effect->SetVectorArray(paramBoneQuaternionsDual, (D3DXVECTOR4*)&boneQuaternionsDual[0], (UINT)boneQuaternionsDual.size());

			const D3DMATERIAL9& material = meshContainer->pMaterials[ meshContainer->boneCombinationTable[subset].AttribId ].MatD3D;
			effect->SetVector(paramDiffuseColor,  (D3DXVECTOR4*)&material.Diffuse);

			d3ddevice->SetTexture(0, meshContainer->ppTextures[ meshContainer->boneCombinationTable[subset].AttribId ]);

			for(size_t i=0; i<drawSubsetListeners.size(); ++i){ drawSubsetListeners[i].beforeFunc(meshContainer->boneCombinationTable[subset].AttribId, drawSubsetListeners[i].ptr); }
			effect->CommitChanges();
			meshContainer->blendedMesh->DrawSubset(subset);
			for(size_t i=0; i<drawSubsetListeners.size(); ++i){ drawSubsetListeners[i].afterFunc(meshContainer->boneCombinationTable[subset].AttribId, drawSubsetListeners[i].ptr); }
		}

		d3ddevice->SetTexture(0, NULL);
	}
	else{
		d3ddevice->SetFVF(meshContainer->blendedMesh->GetFVF());

		for(DWORD subset=0; subset<meshContainer->numBoneCombinations; subset++){
			DWORD i;
			for(i=0; i<meshContainer->maxFaceInfl; i++){
				const DWORD b = meshContainer->boneCombinationTable[subset].BoneId[i];
				if(b==UINT_MAX) break;
				d3ddevice->SetTransform(
					D3DTS_WORLDMATRIX(i),
					D3DXMatrixMultiply(&D3DXMATRIX(), meshContainer->boneOffsetMatrices[b], meshContainer->boneFrameMatrices[b]) );
			}
			d3ddevice->SetRenderState(D3DRS_VERTEXBLEND, i-1);
			d3ddevice->SetMaterial(&meshContainer->pMaterials[ meshContainer->boneCombinationTable[subset].AttribId ].MatD3D);
			d3ddevice->SetTexture(0, meshContainer->ppTextures[ meshContainer->boneCombinationTable[subset].AttribId ]);
			for(size_t i=0; i<drawSubsetListeners.size(); ++i){ drawSubsetListeners[i].beforeFunc(meshContainer->boneCombinationTable[subset].AttribId, drawSubsetListeners[i].ptr); }
			meshContainer->blendedMesh->DrawSubset(subset);
			for(size_t i=0; i<drawSubsetListeners.size(); ++i){ drawSubsetListeners[i].afterFunc(meshContainer->boneCombinationTable[subset].AttribId, drawSubsetListeners[i].ptr); }
		}

		d3ddevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		d3ddevice->SetTexture(0, NULL);
	}
}

void GRAnimationMesh::DrawNormalMeshContainer(MeshContainer *meshContainer, const Frame *frame){
	LPDIRECT3DDEVICE9 d3ddevice = GRDeviceD3D::GetD3DDevice();

	if(effect){
		D3DXHANDLE paramWorld        = effect->GetParameterBySemantic(NULL, "WORLD");
		D3DXHANDLE paramDiffuseColor = effect->GetParameterBySemantic(NULL, "DIFFUSECOLOR");
		
		D3DXMATRIX world;
		d3ddevice->GetTransform(D3DTS_WORLD, &world);
		effect->SetMatrix(paramWorld, &(frame->CombinedTransformationMatrix*world));

		d3ddevice->SetFVF(meshContainer->MeshData.pMesh->GetFVF());
		for(int i=0; i<(int)meshContainer->NumMaterials; i++){
			const D3DMATERIAL9& material = meshContainer->pMaterials[i].MatD3D;
			effect->SetFloatArray(paramDiffuseColor,  (float*)&material.Diffuse,  4);
			d3ddevice->SetTexture(0, meshContainer->ppTextures[i]);
			effect->CommitChanges();
			meshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
	else{
		d3ddevice->SetFVF(meshContainer->MeshData.pMesh->GetFVF());
		d3ddevice->SetTransform(D3DTS_WORLD, &frame->CombinedTransformationMatrix);
		for(int i=0; i<(int)meshContainer->NumMaterials; i++){
			d3ddevice->SetMaterial(&meshContainer->pMaterials[i].MatD3D);
			d3ddevice->SetTexture(0, meshContainer->ppTextures[i]);
			meshContainer->MeshData.pMesh->DrawSubset(i);
		}
		d3ddevice->SetTexture(0, NULL);
	}
}

}
