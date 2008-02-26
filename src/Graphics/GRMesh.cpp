/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#include "GRMesh.h"
#include <gl/glut.h>


namespace Spr{;
IF_OBJECT_IMP(GRMesh, GRVisual);

GRMesh::GRMesh(const GRMeshDesc& desc):GRMeshDesc(desc){
	list			= 0;
	render			= NULL;
	vtxs			= NULL;
	nVtxs			= 0;
	vtxFormat		= NULL;
	blendedVtxs		= NULL;
	stride			= -1;
	normalOffset	= -1;
	positionOffset	= -1;
	texOffset		= -1;
	tex3d			= false;
}
GRMesh::~GRMesh(){
	if (list) render->ReleaseList(list);
	delete [] vtxs;
	delete [] blendedVtxs;
}
void GRMesh::MakeBuffer(){
	if (vtxs) delete vtxs;
	vtxs = NULL;
	nVtxs = max(max(positions.size(), normals.size()), max(colors.size(), texCoords.size()));
	if (tex3d && texCoords.size() && normals.size()){
		stride = sizeof(GRVertexElement::VFT4fC4fN3fP4f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->p) - (float*)NULL;
		texOffset = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->t) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i){
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->p.x = positions[i].x;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->p.y = positions[i].y;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->p.z = positions[i].z;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->p.w = 1;
		}
		if (faceNormals.size()){
			for (unsigned i=0; i<originalFaces.size(); ++i)
				((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + originalFaces[i]*stride))->n 
				= normals[faceNormals[i]];
		}else{
			for (unsigned i=0; i<normals.size(); ++i)
				((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->n = normals[i];
		}
		for (unsigned i=0; i<colors.size(); ++i)
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->c = colors[i];
		for (unsigned i=0; i<texCoords.size(); ++i){
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->t.x = texCoords[i].x;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->t.y = texCoords[i].y;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->t.z = 0;
			((GRVertexElement::VFT4fC4fN3fP4f*)(vtxs + i*stride))->t.w = 1;
		}
		vtxFormat = GRVertexElement::vfT4fC4fN3fP4f;
	}else if (texCoords.size() && normals.size() && colors.size()){
		stride = sizeof(GRVertexElement::VFT2fC4fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->p) - (float*)NULL;
		texOffset = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->t) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + i*stride))->p = positions[i];
		if (faceNormals.size()){
			for (unsigned i=0; i<originalFaces.size(); ++i)
				((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + originalFaces[i]*stride))->n 
				= normals[faceNormals[i]];
		}else{
			for (unsigned i=0; i<normals.size(); ++i)
				((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + i*stride))->n = normals[i];
		}
		if (colors.size())
			for (unsigned i=0; i<colors.size(); ++i)
				((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + i*stride))->c = colors[i];
		else 
			for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + i*stride))->c = Vec4f(1,1,1,1);
		for (unsigned i=0; i<texCoords.size(); ++i)
			((GRVertexElement::VFT2fC4fN3fP3f*)(vtxs + i*stride))->t = texCoords[i];
		vtxFormat = GRVertexElement::vfT2fC4fN3fP3f;
	}else if (texCoords.size() && normals.size()){
		stride = sizeof(GRVertexElement::VFT2fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->p) - (float*)NULL;
		texOffset = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->t) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT2fN3fP3f*)(vtxs + i*stride))->p = positions[i];
		if (faceNormals.size()){
			for (unsigned i=0; i<originalFaces.size(); ++i)
				((GRVertexElement::VFT2fN3fP3f*)(vtxs + originalFaces[i]*stride))->n 
				= normals[faceNormals[i]];
		}else{
			for (unsigned i=0; i<normals.size(); ++i)
				((GRVertexElement::VFT2fN3fP3f*)(vtxs + i*stride))->n = normals[i];
		}
		for (unsigned i=0; i<texCoords.size(); ++i)
			((GRVertexElement::VFT2fN3fP3f*)(vtxs + i*stride))->t = texCoords[i];
		vtxFormat = GRVertexElement::vfT2fN3fP3f;
	}else if (texCoords.size() && colors.size()){
		stride = sizeof(GRVertexElement::VFT2fC4bP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT2fC4bP3f*)NULL)->p) - (float*)NULL;
		texOffset = (float*)(((GRVertexElement::VFT2fC4bP3f*)NULL)->t) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT2fC4bP3f*)(vtxs + i*stride))->p = positions[i];
		for (unsigned i=0; i<colors.size(); ++i)
			((GRVertexElement::VFT2fC4bP3f*)(vtxs + i*stride))->c =
						((unsigned char)(colors[i].x*255)) |
						((unsigned char)(colors[i].y*255) << 8) |
						((unsigned char)(colors[i].z*255) << 16) |
						((unsigned char)(colors[i].w*255) << 24);
		for (unsigned i=0; i<texCoords.size(); ++i)
			((GRVertexElement::VFT2fC4bP3f*)(vtxs + i*stride))->t = texCoords[i];
		vtxFormat = GRVertexElement::vfT2fC4bP3f;
	}else if (normals.size() && colors.size()){
		stride = sizeof(GRVertexElement::VFC4fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFC4fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFC4fN3fP3f*)NULL)->p) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFC4fN3fP3f*)(vtxs + i*stride))->p = positions[i];
		if (faceNormals.size()){
			for (unsigned i=0; i<originalFaces.size(); ++i)
				((GRVertexElement::VFC4fN3fP3f*)(vtxs + originalFaces[i]*stride))->n 
				= normals[faceNormals[i]];
		}else{
			for (unsigned i=0; i<normals.size(); ++i)
				((GRVertexElement::VFC4fN3fP3f*)(vtxs + i*stride))->n = normals[i];
		}
		for (unsigned i=0; i<colors.size(); ++i)
			((GRVertexElement::VFC4fN3fP3f*)(vtxs + i*stride))->c = colors[i];
		vtxFormat = GRVertexElement::vfC4fN3fP3f;
	}else if (normals.size()){
		stride = sizeof(GRVertexElement::VFN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFN3fP3f*)NULL)->p) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFN3fP3f*)(vtxs + i*stride))->p = positions[i];
		if (faceNormals.size()){
			for (unsigned i=0; i<originalFaces.size(); ++i)
				((GRVertexElement::VFN3fP3f*)(vtxs + originalFaces[i]*stride))->n 
				= normals[faceNormals[i]];
		}else{
			for (unsigned i=0; i<normals.size(); ++i)
				((GRVertexElement::VFN3fP3f*)(vtxs + i*stride))->n = normals[i];
		}
		vtxFormat = GRVertexElement::vfN3fP3f;
	}else if (tex3d && texCoords.size()){
		stride = sizeof(GRVertexElement::VFT4fP4f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT4fP4f*)NULL)->p) - (float*)NULL;
		texOffset = (float*)(((GRVertexElement::VFT4fP4f*)NULL)->t) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT4fP4f*)(vtxs + i*stride))->p = positions[i];
		for (unsigned i=0; i<texCoords.size(); ++i){
			((GRVertexElement::VFT4fP4f*)(vtxs + i*stride))->t.x = texCoords[i].x;
			((GRVertexElement::VFT4fP4f*)(vtxs + i*stride))->t.y = texCoords[i].y;
			((GRVertexElement::VFT4fP4f*)(vtxs + i*stride))->t.z = 0;
			((GRVertexElement::VFT4fP4f*)(vtxs + i*stride))->t.w = 0;
		}
		vtxFormat = GRVertexElement::vfT4fP4f;
	}else if (texCoords.size()){
		stride = sizeof(GRVertexElement::VFT2fP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT2fP3f*)NULL)->p) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFT2fP3f*)(vtxs + i*stride))->p = positions[i];
		for (unsigned i=0; i<texCoords.size(); ++i)
			((GRVertexElement::VFT2fP3f*)(vtxs + i*stride))->t = texCoords[i];
		vtxFormat = GRVertexElement::vfT2fP3f;
	}else if (colors.size()){
		stride = sizeof(GRVertexElement::VFC4bP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFC4bP3f*)NULL)->p) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFC4bP3f*)(vtxs + i*stride))->p = positions[i];
		for (unsigned i=0; i<colors.size(); ++i)
			((GRVertexElement::VFC4bP3f*)(vtxs + i*stride))->c =
						((unsigned char)(colors[i].x*255)) |
						((unsigned char)(colors[i].y*255) << 8) |
						((unsigned char)(colors[i].z*255) << 16) |
						((unsigned char)(colors[i].w*255) << 24);
		vtxFormat = GRVertexElement::vfC4bP3f;
	}else{
		stride = sizeof(GRVertexElement::VFP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFP3f*)NULL)->p) - (float*)NULL;
		vtxs = DBG_NEW float[stride * nVtxs];
		for (unsigned i=0; i<positions.size(); ++i)
			((GRVertexElement::VFP3f*)(vtxs + i*stride))->p = positions[i];
		vtxFormat = GRVertexElement::vfP3f;
	}
	if (skinWeights.size()){
		blendedVtxs = DBG_NEW float[stride * nVtxs];
		memcpy(blendedVtxs, vtxs, stride*sizeof(float)*nVtxs);
	}
}
/// 同じマテリアルインデックスが続く場合は、それ毎に１つのディスプレイリストとして、登録する．
void GRMesh::DrawBuffer(void* vtx){
	if (materialList.empty()){	// Mesh に material の指定がない場合
		render->DrawIndexed(GRRenderIf::TRIANGLES, &*faces.begin(), vtx, faces.size());
	}else{	// Xファイルからの materialList 指定がある場合、materialごとに描画
		for(unsigned i=0; i < material.size(); ++i){
			render->SetMaterial(*material[i]);
			unsigned from=0, to=0;
			for(; to<originalFaceIds.size(); ++to){
				if (originalFaceIds[to] >= materialList.size()) continue;
				if (materialList[originalFaceIds[to]] == i) continue;
				if (from < to){
					render->DrawIndexed(GRRenderIf::TRIANGLES, 
						&*faces.begin() + 3*from, vtx, (to-from)*3);
				}
				from = to+1;
			}
			if (from < to){
				render->DrawIndexed(GRRenderIf::TRIANGLES, 
					&*faces.begin() + 3*from, vtx, (to-from)*3);
			}
		}	
	}
}

void GRMesh::CreateList(GRRenderIf* r){
	if (list) render->ReleaseList(list);
	render = r;
	list = 0;
	MakeBuffer();
	list = render->StartList();
	render->SetVertexFormat(vtxFormat);
	DrawBuffer(vtxs);
	render->EndList();
}

void GRMesh::Render(GRRenderIf* r){
	if (skinWeights.size()){	//	SkinMeshは毎回描画する必要がある
		if (r!=render || !list) CreateList(r);
		render = r;
		if (positionOffset>=0){
			for(int v=0; v<nVtxs; ++v){
				blendedVtxs[v*stride + positionOffset] = 0;
				blendedVtxs[v*stride + positionOffset+1] = 0;
				blendedVtxs[v*stride + positionOffset+2] = 0;
			}
			for(unsigned i=0; i<skinWeights.size(); ++i){
				Affinef af = skinWeights[i].frame->GetWorldTransform() * skinWeights[i].offset;
				for(unsigned j=0; j<skinWeights[i].indices.size(); ++j){
					int v = skinWeights[i].indices[j];
					float w = skinWeights[i].weights[j];
					*(Vec3f*)(blendedVtxs + v*stride + positionOffset) += 
						af * *(Vec3f*)(vtxs + v*stride + positionOffset) * w;
					if (normalOffset>=0){
						*(Vec3f*)(blendedVtxs + v*stride + normalOffset) += 
							af.Rot() * *(Vec3f*)(vtxs + v*stride + normalOffset) * w;
					}
				}
			}
		}

		render->SetVertexFormat(vtxFormat);
		DrawBuffer(blendedVtxs);
	}else if (tex3d){	//	3d textureも strq をいじりたいので、毎回描画
		if (r!=render) render = r;
		if (render){
			if (!vtxs) MakeBuffer();
			render->SetVertexFormat(vtxFormat);
			DrawBuffer(vtxs);
		}
	}else{
		if (r!=render || !list) CreateList(r);
		if (list) render->DrawList(list);
	}
}
void GRMesh::Rendered(GRRenderIf* r){
}
bool GRMesh::AddChildObject(ObjectIf* o){			
	GRMaterial* m = o->Cast();
	if (m){
		material.push_back(m);
		return GetNameManager()->AddChildObject(m->Cast());
	}
	GRFrame* f = o->Cast();
	if (f){
		for(unsigned i=0; i<skinWeights.size(); ++i){
			if (!skinWeights[i].frame){
				skinWeights[i].frame = f;
				return true;
			}
		}
	}
	return false;
}
size_t GRMesh::NChildObject() const {
	return material.size();
}
ObjectIf* GRMesh::GetChildObject(size_t pos){
	if (pos < material.size()) return material[pos]->Cast();
	return NULL;
}
void GRMesh::Print(std::ostream& os) const {
	PrintHeader(os, false);
	int w = os.width();
	os.width(0);
	os << UTPadding(w+2) << "positions: " << positions.size() << std::endl;
	os << UTPadding(w+2) << "texCoords: " << texCoords.size() << std::endl;
	os << UTPadding(w+2) << "normals:   " << normals.size() << std::endl;
	os << UTPadding(w+2) << "faces:     " << faces.size() << std::endl;
	os << UTPadding(w+2) << "origFaces: " << originalFaces.size() << std::endl;
	os << UTPadding(w+2) << "origFaceId:" << originalFaceIds.size() << std::endl;

	os.width(w);
	PrintFooter(os);
}

}
