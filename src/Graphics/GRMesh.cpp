#include "Graphics.h"
#include "GRMesh.h"

namespace Spr{;
IF_OBJECT_IMP(GRMesh, SceneObject);

GRMesh::GRMesh(GRMeshDesc& desc):GRMeshDesc(desc){
	list = 0;
	render = NULL;
}
void GRMesh::CreateList(GRRenderIf* r){
	if (list) render->ReleaseList(list);
	list = 0;
	render = r;
	if (texCoords.size() && normals.size()){
		std::vector<GRVertexElement::VFT2fN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].n = normals[i];
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fN3fP3f);
		list = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), vtx.size());
	}else if (normals.size()){
		std::vector<GRVertexElement::VFN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].n = normals[i];
		}
		render->SetVertexFormat(GRVertexElement::vfN3fP3f);
		list = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), vtx.size());
	}else if (texCoords.size()){
		std::vector<GRVertexElement::VFT2fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fP3f);
		list = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), vtx.size());
	}else{
		render->SetVertexFormat(GRVertexElement::vfP3f);
		list = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*positions.begin(), positions.size());
	}
}
void GRMesh::Render(GRRenderIf* r){
	if (r!=render || !list) CreateList(r);
	render->DrawList(list);
}
void GRMesh::Rendered(GRRenderIf* r){
}

}
