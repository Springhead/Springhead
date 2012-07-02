/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRMesh.h>
#include <Graphics/GRRender.h>

using namespace std;

namespace Spr{;

GRSkinWeight::GRSkinWeight(const GRSkinWeightDesc& desc):GRSkinWeightDesc(desc){
	frame = NULL;
}

bool GRSkinWeight::AddChildObject(ObjectIf* o){
	GRFrame* f = o->Cast();
	if(f){
		frame = f;
		return true;
	}
	return false;
}

size_t GRSkinWeight::NChildObject()const{
	return frame ? 1 : 0;
}

ObjectIf* GRSkinWeight::GetChildObject(size_t pos){
	if(pos == 0 && frame)
		return frame->Cast();
	return NULL;
}

//--------------------------------------------------------------------------------

GRMesh::GRMesh(const GRMeshDesc& desc):GRMeshDesc(desc){
	list			= 0;
	render			= NULL;
	vtxFormat		= NULL;
	stride			= -1;
	normalOffset	= -1;
	positionOffset	= -1;
	texOffset		= -1;
	tex3d			= false;

	AfterSetDesc();
}
GRMesh::~GRMesh(){
	if (list) 
		render->ReleaseList(list);
}

void GRMesh::DecomposeFaces(){
	// 面の3角形分割
	//orgFaceIds.clear();
	//orgFaces.clear();
	triFaces.clear();
	triML.clear();
	for(int i = 0; i < (int)faces.size(); ++i){
		// 3角形か4角形のみ想定
		if(!(faces[i].nVertices == 3 || faces[i].nVertices == 4)){
			DSTR << "number of vertices in a mesh must be 3 or 4." << endl;
			continue;
		}
		//orgFaceIds.push_back(i);
		triFaces.push_back( faces[i].indices[2] );
		triFaces.push_back( faces[i].indices[1] );
		triFaces.push_back( faces[i].indices[0] );
		if(!materialList.empty())
			triML.push_back(materialList[i]);
		//orgFaces.push_back( faces[i].indices[0] );
		//orgFaces.push_back( faces[i].indices[1] );
		//orgFaces.push_back( faces[i].indices[2] );

		if (faces[i].nVertices == 4){
			//orgFaceIds.push_back(i);
			// triFacesには、面が四角形なら三角形に分割したインデックスをpush
			triFaces.push_back( faces[i].indices[2] );
			triFaces.push_back( faces[i].indices[0] );
			triFaces.push_back( faces[i].indices[3] );
			if(!materialList.empty())
				triML.push_back(materialList[i]);

			// orgFaces には、4頂点目のインデックスをpush
			//orgFaces.push_back( faces[i].indices[3] );
		}
	}
}

/** 法線情報の自動生成
	面の法線の平均を頂点の法線とする
 */
void GRMesh::GenerateNormals(){
	normals.resize(vertices.size());
	fill(normals.begin(), normals.end(), Vec3f());

	// 頂点を共有する面の数
	std::vector<int> nFace(vertices.size(), 0);
	
	for(unsigned i = 0; i < triFaces.size(); i += 3){
		Vec3f n = (vertices[triFaces[i+1]] - vertices[triFaces[i]])
			    % (vertices[triFaces[i+2]] - vertices[triFaces[i]]);
		n.unitize();

		normals[triFaces[i  ]] += n;
		normals[triFaces[i+1]] += n;
		normals[triFaces[i+2]] += n;
		nFace[triFaces[i  ]] ++;
		nFace[triFaces[i+1]] ++;
		nFace[triFaces[i+2]] ++;
	}

	for(unsigned i = 0; i < normals.size(); ++i)
		normals[i] /= nFace[i];
}

void GRMesh::AfterSetDesc(){
	DecomposeFaces();
	if(normals.empty())
		GenerateNormals();
}

GRSkinWeightIf* GRMesh::CreateSkinWeight(const GRSkinWeightDesc& desc){
	GRSkinWeight* sw = DBG_NEW GRSkinWeight(desc);
	AddChildObject(sw->Cast());
	return sw->Cast();
}

void GRMesh::SwitchCoordinate(){
	for(int i = 0; i < (int)vertices.size(); ++i)
		vertices[i].z *= -1.0f;
	for(int i = 0; i < (int)normals.size(); ++i)
		normals[i].z *= -1.0f;

	for(int i = 0; i < (int)skinWeights.size(); ++i){
		skinWeights[i]->offset.ExZ()  *= -1.0f;
		skinWeights[i]->offset.EyZ()  *= -1.0f;
		skinWeights[i]->offset.EzX()  *= -1.0f;
		skinWeights[i]->offset.EzY()  *= -1.0f;
		skinWeights[i]->offset.PosZ() *= -1.0f;
	}
}

void GRMesh::MakeBuffer(){
	vtxs.clear();

	int nVtxs = max(max((int)vertices.size(), (int)normals.size()), max((int)colors.size(), (int)texCoords.size()));
	
	if (tex3d && texCoords.size() && normals.size()){
		stride		 = sizeof(GRVertexElement::VFT4fC4fN3fP4f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->p) - (float*)NULL;
		texOffset	 = (float*)(((GRVertexElement::VFT4fC4fN3fP4f*)NULL)->t) - (float*)NULL;
		
		//vtxs = DBG_NEW float[stride * nVtxs];
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT4fC4fN3fP4f* v = (GRVertexElement::VFT4fC4fN3fP4f*)&vtxs[0];
		
		/// 頂点座標
		for (int i = 0; i < (int)vertices.size(); ++i){
			v[i].p.x = vertices[i].x;
			v[i].p.y = vertices[i].y;
			v[i].p.z = vertices[i].z;
			v[i].p.w = 1;
		}

		/// 法線
		if(faceNormals.size())
			for(int i = 0; i < (int)faces.size(); ++i)
				for(int j = 0; j < (int)faces[i].nVertices; j++)
					v[faces[i].indices[j]].n = normals[faceNormals[i].indices[j]];
		else
			for(int i = 0; i < (int)normals.size(); ++i)
				v[i].n = normals[i];

		/// 頂点色
		for(int i = 0; i < (int)colors.size(); ++i)
			v[i].c = colors[i];

		/// テクスチャ座標
		for (int i = 0; i < (int)texCoords.size(); ++i){
			v[i].t.x = texCoords[i].x;
			v[i].t.y = texCoords[i].y;
			v[i].t.z = 0.0f;
			v[i].t.w = 1.0f;
		}
		vtxFormat = GRVertexElement::vfT4fC4fN3fP4f;
	}
	else if (texCoords.size() && normals.size() && colors.size()){
		stride		 = sizeof(GRVertexElement::VFT2fC4fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->p) - (float*)NULL;
		texOffset	 = (float*)(((GRVertexElement::VFT2fC4fN3fP3f*)NULL)->t) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT2fC4fN3fP3f* v = (GRVertexElement::VFT2fC4fN3fP3f*)&vtxs[0];
		
		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		if(faceNormals.size())
			for (int i = 0; i < (int)faces.size(); ++i)
				for(int j = 0; j < faces[i].nVertices; j++)
					v[faces[i].indices[j]].n = normals[faceNormals[i].indices[j]];
		else
			for (int i = 0; i < (int)normals.size(); ++i)
				v[i].n = normals[i];

		if (colors.size())
			for (int i = 0; i < (int)colors.size(); ++i)
				v[i].c = colors[i];
		else 
			for (int i = 0; i < (int)vertices.size(); ++i)
				v[i].c = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);

		for (int i = 0; i < (int)texCoords.size(); ++i)
			v[i].t = texCoords[i];

		vtxFormat = GRVertexElement::vfT2fC4fN3fP3f;
	}
	else if (texCoords.size() && normals.size()){
		stride		 = sizeof(GRVertexElement::VFT2fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->p) - (float*)NULL;
		texOffset	 = (float*)(((GRVertexElement::VFT2fN3fP3f*)NULL)->t) - (float*)NULL;		
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT2fN3fP3f* v = (GRVertexElement::VFT2fN3fP3f*)&vtxs[0];

		for(int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		if(faceNormals.size())
			for(int i = 0; i < (int)faces.size(); ++i)
				for(int j = 0; j < faces[i].nVertices; ++j)
					v[faces[i].indices[j]].n = normals[faceNormals[i].indices[j]];
		else
			for(int i = 0; i < (int)normals.size(); ++i)
				v[i].n = normals[i];

		for(int i = 0; i < (int)texCoords.size(); ++i)
			v[i].t = texCoords[i];

		vtxFormat = GRVertexElement::vfT2fN3fP3f;
	}
	else if (texCoords.size() && colors.size()){
		stride		 = sizeof(GRVertexElement::VFT2fC4bP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT2fC4bP3f*)NULL)->p) - (float*)NULL;
		texOffset	 = (float*)(((GRVertexElement::VFT2fC4bP3f*)NULL)->t) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT2fC4bP3f* v = (GRVertexElement::VFT2fC4bP3f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		for (int i = 0; i < (int)colors.size(); ++i)
			v[i].c = ((unsigned char)(colors[i].x*255)) |
					 ((unsigned char)(colors[i].y*255) << 8) |
					 ((unsigned char)(colors[i].z*255) << 16) |
					 ((unsigned char)(colors[i].w*255) << 24);

		for (int i = 0; i < (int)texCoords.size(); ++i)
			v[i].t = texCoords[i];

		vtxFormat = GRVertexElement::vfT2fC4bP3f;
	}
	else if (normals.size() && colors.size()){
		stride		 = sizeof(GRVertexElement::VFC4fN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFC4fN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFC4fN3fP3f*)NULL)->p) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFC4fN3fP3f* v = (GRVertexElement::VFC4fN3fP3f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		if(faceNormals.size())
			for(int i = 0; i < (int)faces.size(); ++i)
				for(int j = 0; j < faces[i].nVertices; ++j)
					v[faces[i].indices[j]].n = normals[faceNormals[i].indices[j]];
		else
			for(int i = 0; i < (int)normals.size(); ++i)
				v[i].n = normals[i];
		
		for(int i = 0; i < (int)colors.size(); ++i)
			v[i].c = colors[i];

		vtxFormat = GRVertexElement::vfC4fN3fP3f;
	}
	else if (normals.size()){
		stride		 = sizeof(GRVertexElement::VFN3fP3f)/sizeof(float);
		normalOffset = (float*)(((GRVertexElement::VFN3fP3f*)NULL)->n) - (float*)NULL;
		positionOffset = (float*)(((GRVertexElement::VFN3fP3f*)NULL)->p) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFN3fP3f* v = (GRVertexElement::VFN3fP3f*)&vtxs[0];

		for(int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		if(faceNormals.size())
			for(int i = 0; i < (int)faces.size(); ++i)
				for(int j = 0; j < faces[i].nVertices; ++j)
					v[faces[i].indices[j]].n = normals[faceNormals[i].indices[j]];
		else
			for(int i = 0; i < (int)normals.size(); ++i)
				v[i].n = normals[i];
		
		vtxFormat = GRVertexElement::vfN3fP3f;
	}
	else if (tex3d && texCoords.size()){
		stride		 = sizeof(GRVertexElement::VFT4fP4f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT4fP4f*)NULL)->p) - (float*)NULL;
		texOffset	 = (float*)(((GRVertexElement::VFT4fP4f*)NULL)->t) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT4fP4f* v = (GRVertexElement::VFT4fP4f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		for (int i=0; i < (int)texCoords.size(); ++i){
			v[i].t.x = texCoords[i].x;
			v[i].t.y = texCoords[i].y;
			v[i].t.z = 0.0f;
			v[i].t.w = 0.0f;
		}
		vtxFormat = GRVertexElement::vfT4fP4f;
	}
	else if (texCoords.size()){
		stride		 = sizeof(GRVertexElement::VFT2fP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFT2fP3f*)NULL)->p) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFT2fP3f* v = (GRVertexElement::VFT2fP3f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		for (int i = 0; i < (int)texCoords.size(); ++i)
			v[i].t = texCoords[i];

		vtxFormat = GRVertexElement::vfT2fP3f;
	}
	else if (colors.size()){
		stride		 = sizeof(GRVertexElement::VFC4bP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFC4bP3f*)NULL)->p) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFC4bP3f* v = (GRVertexElement::VFC4bP3f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		for (int i = 0; i < (int)colors.size(); ++i)
			v[i].c = ((unsigned char)(colors[i].x*255)) |
					 ((unsigned char)(colors[i].y*255) << 8) |
					 ((unsigned char)(colors[i].z*255) << 16) |
					 ((unsigned char)(colors[i].w*255) << 24);

		vtxFormat = GRVertexElement::vfC4bP3f;
	}
	else{
		stride		 = sizeof(GRVertexElement::VFP3f)/sizeof(float);
		positionOffset = (float*)(((GRVertexElement::VFP3f*)NULL)->p) - (float*)NULL;
		vtxs.resize(stride * nVtxs);
		GRVertexElement::VFC4bP3f* v = (GRVertexElement::VFC4bP3f*)&vtxs[0];

		for (int i = 0; i < (int)vertices.size(); ++i)
			v[i].p = vertices[i];

		vtxFormat = GRVertexElement::vfP3f;
	}

	blendedVtxs.clear();
	if (skinWeights.size()){
		blendedVtxs.resize(stride * nVtxs);
		copy(vtxs.begin(), vtxs.end(), blendedVtxs.begin());
	}
}

/// 同じマテリアルインデックスが続く場合は、それ毎に１つのディスプレイリストとして、登録する．
void GRMesh::DrawBuffer(void* vtx){
	if(faces.empty())
		return;

	// Mesh に material の指定がない場合
	// マテリアルリストはあるがマテリアルが無いモデルデータも考慮
	if (materialList.empty() || material.empty()){	
		render->DrawIndexed(GRRenderIf::TRIANGLES, &triFaces[0], vtx, triFaces.size());
	}
	// Xファイルからの materialList 指定がある場合、materialごとに描画
	else{
		int from = 0, to = 0;
		for(; to <= NTriangle(); ++to){
			if(to == NTriangle() || triML[from] != triML[to]){
				render->SetMaterial(material[triML[from]]->Cast());
				render->DrawIndexed(GRRenderIf::TRIANGLES, &triFaces[3*from], vtx, (to-from)*3);
				from = to;
			}
		}	
	}
}

void GRMesh::CreateList(GRRenderIf* r){
	if (list)
		render->ReleaseList(list);
	render = r;
	list = 0;
	MakeBuffer();
	list = render->StartList();
	render->SetVertexFormat(vtxFormat);
	DrawBuffer(&vtxs[0]);
	render->EndList();
}

void GRMesh::Render(GRRenderIf* r){
	bool usebuffer = false;
	if (!materialList.empty()) {
		for (unsigned i=0; i<material.size(); ++i) {
			if (!material[i]->texname.empty() && material[i]->texname[0]==':') {
				usebuffer = true;
				break;
			}
		}
	}

	if (skinWeights.size()){	//	SkinMeshは毎回描画する必要がある
		// if (r!=render || !list) CreateList(r);
		if (vtxs.empty())
			MakeBuffer();
		render = r;

		if (positionOffset >= 0){
			for(int v = 0; v < NVertex(); ++v)
				Pos(blendedVtxs, v).clear();

			for(unsigned i = 0; i < skinWeights.size(); ++i){
				Affinef af = skinWeights[i]->frame->GetWorldTransform() * skinWeights[i]->offset;
				for(unsigned j = 0; j < skinWeights[i]->indices.size(); ++j){
					int   v = skinWeights[i]->indices[j];
					float w = skinWeights[i]->weights[j];

					Pos(blendedVtxs, v) += w * (af * Pos(vtxs, v));
					if (normalOffset>=0)
						Normal(blendedVtxs, v) += w * (af.Rot() * Normal(vtxs, v));
	
				}
			}
		}

		render->SetVertexFormat(vtxFormat);
		DrawBuffer(&blendedVtxs[0]);
	}
	else if (tex3d){	//	3d textureも strq をいじりたいので、毎回描画
		if (r!=render) render = r;
		if (render){
			if (vtxs.empty()) MakeBuffer();
			render->SetVertexFormat(vtxFormat);
			DrawBuffer(&vtxs[0]);
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
			if (!skinWeights[i]->frame){
				skinWeights[i]->frame = f;
				return true;
			}
		}
	}
	GRSkinWeight* sw = o->Cast();
	if(sw){
		skinWeights.push_back(sw);
		return true;
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
	os << UTPadding(w+2) << "vertices: " << vertices.size() << std::endl;
	os << UTPadding(w+2) << "texCoords: " << texCoords.size() << std::endl;
	os << UTPadding(w+2) << "normals:   " << normals.size() << std::endl;
	os << UTPadding(w+2) << "faces:     " << faces.size() << std::endl;
	//os << UTPadding(w+2) << "origFaces: " << orgFaces.size() << std::endl;
	//os << UTPadding(w+2) << "origFaceId:" << orgFaceIds.size() << std::endl;

	os.width(w);
	PrintFooter(os);
}

}
