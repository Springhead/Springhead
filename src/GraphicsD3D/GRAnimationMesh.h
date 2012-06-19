/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRAnimationMesh_H
#define GRAnimationMesh_H

#include <SprGraphics.h>
#include <SprGraphicsD3D.h>
#include "../Graphics/GRFrame.h"
#include <d3dx9.h>
#include <atlcomcli.h>

namespace Spr{;

/**	@class	GRAnimationMesh
    @brief	キャラクタアニメーション用メッシュ */
class GRAnimationMesh: public GRVisual, public GRAnimationMeshIfInit, public GRAnimationMeshDesc{
	SPR_OBJECTDEF(GRAnimationMesh, GRVisual);
protected:
	struct Frame;
	struct MeshContainer;
	class AllocateHierarchy;
	struct DrawSubsetListener;
protected:
	Frame*								rootFrame;
	CComPtr<ID3DXAnimationController>	controller;
	bool								loaded;
	std::vector<DrawSubsetListener>		drawSubsetListeners;
	std::vector<Frame*>					frames;
	bool								directRenderMode;
	CComPtr<ID3DXEffect>	effect;
	D3DXHANDLE				fxpWorld;
	D3DXHANDLE				fxpMaxVertexInfl;
	D3DXHANDLE				fxpBoneMatrices;
	D3DXHANDLE				fxpBoneQuaternionsReal;
	D3DXHANDLE				fxpBoneQuaternionsDual;
	D3DXHANDLE				fxpDiffuseColor;
	D3DXHANDLE				fxpEmissiveColor;
	D3DXHANDLE				fxpTexture;
	D3DXHANDLE				fxpIsTextured;
public:
	GRAnimationMesh(const GRAnimationMeshDesc& desc=GRAnimationMeshDesc());
	~GRAnimationMesh();
	virtual void SetMotion(const std::string& name);
	virtual void SetTime(double time);
	virtual void SetBlend(const std::vector<GRAnimationMeshBlendData>& data);
	virtual Posed GetBoneKeyframePose(const std::string& name);
	virtual void OverrideBoneOrientation(const std::string& name, const Quaterniond& orientation, double weight);
	virtual void OverrideBonePose(const std::string& name, const Posed& pose, double weight, bool posRelative=false);
	virtual void SetAllBonesOverrideWeight(double weight);
	virtual void AddDrawSubsetListener(GRAnimationMeshDrawSubsetListenerFunc beforeFunc, GRAnimationMeshDrawSubsetListenerFunc afterFunc, void* ptr);
	virtual void SetEffect(LPD3DXEFFECT effect, int matrixPaletteSize);
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
	int     NFrames();
	Affinef GetFrameCombinedTransform(int index);
	void    SetFrameCombinedTransform(int index, const Affinef& transform);
	void    SetDirectRenderMode() { directRenderMode = true; }
protected:
	bool LoadMesh();
	void InitFrame(Frame* frame);
	void CreateBlendedMesh(MeshContainer* meshContainer);
	void SetBoneMatrices(MeshContainer* meshContainer);
	void CreateIndexedBlendedMeshes(Frame* frame, int matrixPaletteSize);
	void CreateIndexedBlendedMesh(MeshContainer* meshContainer, int matrixPaletteSize);
	void UpdateFrame(Frame *frame, const D3DXMATRIX& parentMatrix);
	void DrawFrame(const Frame *frame);
	void DrawSkinnedMeshContainer(MeshContainer *meshContainer);
	void DrawNormalMeshContainer(MeshContainer *meshContainer, const Frame *frame);
	void OverrideFrame(Frame* frame, double weight);
};

struct GRAnimationMesh::Frame : public D3DXFRAME
{
	D3DXMATRIX	CombinedTransformationMatrix;
	Posed		overridePose;
	double		overrideWeight;
	bool		overridePosition;
	bool		posRelative;
};

struct GRAnimationMesh::MeshContainer : public D3DXMESHCONTAINER
{
	CComPtr<ID3DXMesh>		blendedMesh;
	DWORD					maxFaceInfl;
	DWORD					numBoneCombinations;
	CComPtr<ID3DXBuffer>	boneCombinationTableBuffer;
	LPD3DXBONECOMBINATION	boneCombinationTable;
	D3DXMATRIX**			boneOffsetMatrices;
	D3DXMATRIX**			boneFrameMatrices;
	LPDIRECT3DTEXTURE9		*ppTextures;
	DWORD					maxVertexInfl;
	DWORD					matrixPaletteSize;
};

struct GRAnimationMesh::DrawSubsetListener{
	GRAnimationMeshDrawSubsetListenerFunc	beforeFunc;
	GRAnimationMeshDrawSubsetListenerFunc	afterFunc;
	void*									ptr;
};
}
#endif
