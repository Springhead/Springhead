/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#include "GRMesh.h"

namespace Spr{;
IF_OBJECT_IMP(GRMesh, GRVisual);

GRMesh::GRMesh(const GRMeshDesc& desc):GRMeshDesc(desc){
	render = NULL;
}
void GRMesh::CreateList(GRRenderIf* r){
	for (unsigned int id=0; id<list.size(); ++id){
		if (list[id])	render->ReleaseList(list[id]);
	}
	render = r;
	if (texCoords.size() && normals.size() && colors.size()){
		std::vector<GRVertexElement::VFT2fC4fN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[originalFaces[i]].n = normals[faceNormals[i]];
			vtx[i].c = colors[i];
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fC4fN3fP3f);
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else if (texCoords.size() && normals.size()){
		std::vector<GRVertexElement::VFT2fN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[originalFaces[i]].n = normals[faceNormals[i]];
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fN3fP3f);
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else if (texCoords.size() && colors.size()){
		std::vector<GRVertexElement::VFT2fC4bP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].c = 
				((unsigned char)(colors[i].x*255) << 24) |
				((unsigned char)(colors[i].y*255) << 16) |
				((unsigned char)(colors[i].z*255) << 8) |
				((unsigned char)(colors[i].w*255));
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fC4bP3f);
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else if (normals.size() && colors.size()){
		std::vector<GRVertexElement::VFC4fN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[originalFaces[i]].n = normals[faceNormals[i]];
			vtx[i].c = colors[i];
		}
		render->SetVertexFormat(GRVertexElement::vfC4fN3fP3f);
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else if (normals.size()){
		std::vector<GRVertexElement::VFN3fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[originalFaces[i]].n = normals[faceNormals[i]];
		}		
		render->SetVertexFormat(GRVertexElement::vfN3fP3f);
		this->CreateSubList(&*vtx.begin());
	}else if (texCoords.size()){
		std::vector<GRVertexElement::VFT2fP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].t = texCoords[i];
		}
		render->SetVertexFormat(GRVertexElement::vfT2fP3f);
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else if (colors.size()){
		std::vector<GRVertexElement::VFC4bP3f> vtx;
		vtx.resize(positions.size());
		for(unsigned i=0; i<vtx.size(); ++i){
			vtx[i].p = positions[i];
			vtx[i].c = 
				((unsigned char)(colors[i].x*255) << 24) |
				((unsigned char)(colors[i].y*255) << 16) |
				((unsigned char)(colors[i].z*255) << 8) |
				((unsigned char)(colors[i].w*255));
		}
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), 
			&*vtx.begin(), faces.size());
		list.push_back(base);
	}else{
		render->SetVertexFormat(GRVertexElement::vfP3f);
		this->CreateSubList(&*positions.begin());
	}
}
void GRMesh::CreateSubList(void* vtx){
	// Xファイルからの materialList 指定がない場合
	if (!materialList.size()){		
		int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin(), vtx, faces.size());
		list.push_back(base);		
	}
	// Xファイルからの materialList 指定がある場合、material単位で sub mesh 分割する
	else{							
		unsigned int first	= 0;						// 境界の first frame
		unsigned int end	= first+1;					// 境界の end frame
		
		// (分割後の)三角形の面の数だけループ
		while (end < elementIndex.size()){		
			// Xファイルからの指定で、定義した面の数より、マテリアル設定された面の数が少ないかどうかのチェック
			if (materialList.size() > elementIndex[end]){
				// マテリアルインデックスが同じかどうか比較
				if (materialList[elementIndex[first]] == materialList[elementIndex[end]]){
					end++;							// 1個ずらして、またfirstと比較
					if (end >= elementIndex.size()){
						render->SetMaterial(material[materialList[elementIndex[first]]]);
						int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin()+first*3, vtx, (end-first)*3);
						list.push_back(base);
						break;
					}
				}else{
					render->SetMaterial(material[materialList[elementIndex[first]]]);
					int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin()+first*3, vtx, (end-first)*3);
					list.push_back(base);
					first = end;
					end = end+1;
						
					if (end >= elementIndex.size()){
						render->SetMaterial(material[materialList[elementIndex[first]]]);
						int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin()+first*3, vtx, (end-first)*3); 
						list.push_back(base);
						break;
					}
				}

			}else{
				/** Xファイルで MeshMaterialList を指定した場合、
					全ての定義した面に対して、たいていマテリアルを設定をしてあるが、
					定義した面の数より、マテリアル設定された面の数が少ない場合は、
					最後にマテリアル設定された面と同じマテリアルが、マテリアル設定されていない面に対して適用される。*/
				 
					// 最後に定義されているマテリアルを設定し、残りの頂点バッファに対してCreateIndexdListする。
					render->SetMaterial(material[materialList[elementIndex[first]]]);
					int base = render->CreateIndexedList(GRRender::TRIANGLES, &*faces.begin()+first*3, vtx, faces.size()-first*3);
					list.push_back(base);
					break;
					
			}
		}			
	}
}
void GRMesh::Render(GRRenderIf* r){
	if (r!=render || !list.size()) CreateList(r);
	for (unsigned int id=0; id<list.size(); ++id){
		render->DrawList(list[id]);
	}
}
void GRMesh::Rendered(GRRenderIf* r){
}

}
