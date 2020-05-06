/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRBlendMesh.h>
#include <Graphics/GRRender.h>

namespace Spr{;
GRBlendMesh::GRBlendMesh(const GRBlendMeshDesc& desc):GRBlendMeshDesc(desc){
	render = NULL;
}
GRBlendMesh::~GRBlendMesh(){
	for (unsigned int id=0; id<list.size(); ++id){
		if (list[id]) render->ReleaseList(list[id]);
	}
}
void GRBlendMesh::CreateList(GRRenderIf* r){
	assert(0);
/*
	for (unsigned int id=0; id<list.size(); ++id){
		if (list[id])	render->ReleaseList(list[id]);
	}
	list.clear();
	render = r;		
	render->InitShader();
	render->ClearBlendMatrix();
	for (unsigned i=0; i<blendMatrix.size(); ++i){ render->SetBlendMatrix(blendMatrix[i]); }
	unsigned int vtxsize = max(max(positions.size(), normals.size()), max(colors.size(), texCoords.size()));
	
	if (texCoords.size() && normals.size() && colors.size()){
		std::vector<GRVertexElement::VFT2fC4fN3fP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<originalFaces.size(); ++i){ vtx[originalFaces[i]].n = normals[faceNormals[i]]; }
		for (unsigned i=0; i<colors.size(); ++i){ vtx[i].c = colors[i];	}
		for (unsigned i=0; i<texCoords.size(); ++i){ vtx[i].t = texCoords[i]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }	
		render->SetVertexFormat(GRVertexElement::vfT2fC4fN3fP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shT2fC4fN3fP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fC4fN3fP3fB4f support." << std::endl;
			assert(0);
		}	
	}else if (texCoords.size() && normals.size()){
		std::vector<GRVertexElement::VFT2fN3fP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<originalFaces.size(); ++i){ vtx[originalFaces[i]].n = normals[faceNormals[i]]; }
		for (unsigned i=0; i<texCoords.size(); ++i){ vtx[i].t = texCoords[i]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfT2fN3fP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shT2fN3fP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fN3fP3fB4f support." << std::endl;
			assert(0);
		}		
	}else if (texCoords.size() && colors.size()){
		std::vector<GRVertexElement::VFT2fC4bP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<colors.size(); ++i){ 
			vtx[i].c = ((unsigned char)(colors[i].x*255) << 24) |
			 		   ((unsigned char)(colors[i].y*255) << 16) |
					   ((unsigned char)(colors[i].z*255) << 8) |
					   ((unsigned char)(colors[i].w*255));
		}
		for (unsigned i=0; i<texCoords.size(); ++i){ vtx[i].t = texCoords[i]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfT2fC4bP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shT2fC4bP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fN3fP3fB4f support." << std::endl;
			assert(0);
		}	
	}else if (normals.size() && colors.size()){
		std::vector<GRVertexElement::VFC4fN3fP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<originalFaces.size(); ++i){ vtx[originalFaces[i]].n = normals[faceNormals[i]]; }
		for (unsigned i=0; i<colors.size(); ++i){ vtx[i].c = colors[i];	}
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfC4fN3fP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shC4fN3fP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fN3fP3fB4f support." << std::endl;
			assert(0);
		}	
	}else if (normals.size()){
		std::vector<GRVertexElement::VFN3fP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<originalFaces.size(); ++i){ vtx[originalFaces[i]].n = normals[faceNormals[i]]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfN3fP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shN3fP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fN3fP3fB4f support." << std::endl;
			assert(0);
		}	
	}else if (texCoords.size()){
		std::vector<GRVertexElement::VFT2fP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<texCoords.size(); ++i){ vtx[i].t = texCoords[i]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfT2fP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shT2fP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){	// To Be Implemented.
			DSTR << "Failed to create shader.  No VFT2fN3fP3fB4f support." << std::endl;
			assert(0);
		}	
	}else if (colors.size()){
		std::vector<GRVertexElement::VFC4bP3fB4f> vtx;
		vtx.resize(vtxsize);
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<colors.size(); ++i){									// VFC4fP3fB4f にすれば？
			vtx[i].c = ((unsigned char)(colors[i].x*255) << 24) |
			 		   ((unsigned char)(colors[i].y*255) << 16) |
					   ((unsigned char)(colors[i].z*255) << 8) |
					   ((unsigned char)(colors[i].w*255));
		}
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfC4bP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shC4bP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){
			DSTR << "Failed to create shader. " << std::endl;
			assert(0);
		}
		GRShaderFormat::SFBlendLocation location;
		render->GetShaderLocation(shader, &location);
		int base = render->CreateShaderIndexedList(shader, &location, GRRenderIf::TRIANGLES, &*faces.begin(), &*vtx.begin(), faces.size());
		list.push_back(base);
	}else{
		std::vector<GRVertexElement::VFP3fB4f> vtx;
		for (unsigned i=0; i<positions.size(); ++i){ vtx[i].p = positions[i]; }
		for (unsigned i=0; i<blends.size(); ++i){ vtx[i].b = blends[i]; }
		render->SetVertexFormat(GRVertexElement::vfP3fB4f);
		render->SetShaderFormat(GRShaderFormat::shP3fB4f);
		GRHandler shader = render->CreateShader();
		if (!shader){
			DSTR << "Failed to create shader. " << std::endl;
			assert(0);
		}
		GRShaderFormat::SFBlendLocation location;
		render->GetShaderLocation(shader, &location);
		int base = render->CreateShaderIndexedList(shader, &location, GRRenderIf::TRIANGLES, &*faces.begin(), &*vtx.begin(), faces.size());
		list.push_back(base);		
	}
	*/
}

void GRBlendMesh::Render(GRRenderIf* r){
	if (r!=render || !list.size()) CreateList(r);
	for (unsigned int id=0; id<list.size(); ++id){
		render->DrawList(list[id]);
	}
}
void GRBlendMesh::Rendered(GRRenderIf* r){
}


}
