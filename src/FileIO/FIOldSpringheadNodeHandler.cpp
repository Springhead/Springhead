/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <FileIO/FINodeHandler.h>
#include <FileIO/FIOldSpringheadNode.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
namespace Spr{;
using namespace SprOldSpringehead;

class FINodeHandlerXFrame: public FINodeHandlerImp<Frame>{
public:	
	FINodeHandlerXFrame():FINodeHandlerImp<Desc>("Frame"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(GRFrameIf::GetIfInfoStatic(), &GRFrameDesc());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};
class FINodeHandlerXFrameTransformMatrix: public FINodeHandlerImp<FrameTransformMatrix>{
public:	
	FINodeHandlerXFrameTransformMatrix():FINodeHandlerImp<Desc>("FrameTransformMatrix"){}
	void Load(Desc& d, FILoadContext* fc){
		GRFrame* fr = DCAST(GRFrame, fc->objects.Top());
		if (fr){
			fr->transform = d.matrix;
		}else{
			fc->ErrorMessage(NULL, "FrameTransformMatrix must be inside of Frame node.");
		}
	}
};

class FINodeHandlerXMesh: public FINodeHandlerImp<Mesh>{
public:
	FINodeHandlerXMesh():FINodeHandlerImp<Desc>("Mesh"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(GRMeshIf::GetIfInfoStatic(), &GRMeshDesc());	
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->positions = d.vertices;							// 頂点座標
			for (int f=0; f < d.nFaces; ++f){		
				if (d.faces[f].nFaceVertexIndices == 3) {			// 面を構成する頂点の番号
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->elementIndex.push_back(f);
				} else if (d.faces[f].nFaceVertexIndices == 4) {	// 三角形に分割
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->elementIndex.push_back(f);				// 三角形分割前後の面同士の関連付け用
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[3] );
					mesh->elementIndex.push_back(f);
				} else {
					fc->ErrorMessage(NULL, "Mesh::nFaces error. ");
				}
			}
		}else{
			fc->ErrorMessage(NULL, "Mesh appered outside of Frame.");
		}
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};

class FINodeHandlerXMeshMaterialList: public FINodeHandlerImp<MeshMaterialList>{
public:
	FINodeHandlerXMeshMaterialList():FINodeHandlerImp<Desc>("MeshMaterialList"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->materialList = d.faceIndexes;		// マテリアル番号のリスト
		}else{
			fc->ErrorMessage(NULL, "MeshMaterialList appered outside of Mesh.");
		}
	}
};

class FINodeHandlerXMaterial: public FINodeHandlerImp<Material>{
public:
	FINodeHandlerXMaterial():FINodeHandlerImp<Desc>("Material"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			GRMaterialDesc mat;
			mat.ambient = mat.diffuse = d.face;
			mat.specular = Vec4f(mat.specular.x, mat.specular.y, mat.specular.z, 1.0);
			mat.emissive = Vec4f(mat.emissive.x, mat.emissive.y, mat.emissive.z, 1.0);
			mat.power = d.power;
			mesh->material.push_back(mat);
		}else{
			fc->ErrorMessage(NULL, "Material appered outside of Mesh. ");
		}
	}
};

class FINodeHandlerXMeshNormals: public FINodeHandlerImp<MeshNormals>{
public:
	FINodeHandlerXMeshNormals():FINodeHandlerImp<Desc>("MeshNormals"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			/** Xファイルから"MeshNormals"による法線インデックスを持ったデータの場合は、
			    MeshNormalsを、MeshNormalsのインデックスではなく、Meshの頂点座標用のインデックスでアクセスできるように
			    MeshNormalsの順番を入れ替えて格納する.																	*/
			for (int f=0; f < d.nFaceNormals; ++f){
				for (int i=0; i < d.faceNormals[f].nFaceVertexIndices; ++i){
					mesh->normals.push_back( d.normals[d.faceNormals[f].faceVertexIndices[i]] );
				}
			}
		}else{
			fc->ErrorMessage(NULL, "MeshNormals appered outside of Mesh.");
		}
	}
};

void RegisterOldSpringheadNodeHandlers(){
REGISTER_NODE_HANDLER(FINodeHandlerXFrame);
REGISTER_NODE_HANDLER(FINodeHandlerXFrameTransformMatrix);
REGISTER_NODE_HANDLER(FINodeHandlerXMesh);
REGISTER_NODE_HANDLER(FINodeHandlerXMeshMaterialList);
REGISTER_NODE_HANDLER(FINodeHandlerXMaterial);
REGISTER_NODE_HANDLER(FINodeHandlerXMeshNormals);
}
}
