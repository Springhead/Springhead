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
			fc->ErrorMessage(NULL, "FrameTransformMatrix appered outside of Frame.");
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
			mesh->positions = d.vertices;						// 頂点座標
			for (int f=0; f < d.nFaces; ++f){		
				if (d.faces[f].nFaceVertexIndices == 3) {		// 面を構成する頂点の番号
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
				} else if (d.faces[f].nFaceVertexIndices == 4) {				// 三角形に分割
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[3] );
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
#if 0
class FINodeHandlerXMeshMaterialList: public FINodeHandlerImp<MeshMaterialList>{
public:
	FINodeHandlerXMeshMaterialList():FINodeHandlerImp<Desc>("MeshMaterialList"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(GRMaterialIf::GetIfInfoStatic(), &GRMaterialDesc());
		GRMaterial* mat = DCAST(GRMaterial, fc->objects.Top());
		if (mat) {
			mat->

#endif

class FINodeHandlerXMeshNormals: public FINodeHandlerImp<MeshNormals>{
public:
	FINodeHandlerXMeshNormals():FINodeHandlerImp<Desc>("MeshNormals"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			for (int f=0; f < d.nFaceNormals; ++f){		
				if (d.faceNormals[f].nFaceVertexIndices == 3) {			
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[0] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[1] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[2] ] );
				} else if (d.faceNormals[f].nFaceVertexIndices == 4) {		// 三角形に分割
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[0] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[1] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[2] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[0] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[2] ] );
					mesh->normals.push_back( d.normals[ d.faceNormals[f].faceVertexIndices[3] ] );
				} else {
					fc->ErrorMessage(NULL, "MeshNormals::nFaceNormals error. ");
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
REGISTER_NODE_HANDLER(FINodeHandlerXMeshNormals);
}
}
