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
#include <Graphics/GRRender.h>
#include <Physics/PHSolid.h>
#include <Framework/FWScene.h>
#include <Collision/CDConvexMesh.h>

namespace SprOldSpringhead{
using namespace Spr;

class FINodeHandlerXHeader: public FINodeHandlerImp<Header>{
public:
	FINodeHandlerXHeader():FINodeHandlerImp<Desc>("Header"){}
	void Load(Desc& d, FILoadContext* fc){
	}
};
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
class FINodeHandlerXLight8: public FINodeHandlerImp<Light8>{
public:
	FINodeHandlerXLight8():FINodeHandlerImp<Desc>("Light8"){}
	void Load(Desc& l8, FILoadContext* fc){
		GRLightDesc grld;
		grld.ambient = l8.ambient;
		grld.attenuation0 = l8.attenuation0;
		grld.attenuation1 = l8.attenuation1;
		grld.attenuation2 = l8.attenuation2;
		grld.diffuse = l8.diffuse;
		if (l8.type == Light8::XLIGHT_DIRECTIONAL){
			grld.position.sub_vector(PTM::TSubVectorDim<0,3>()) = l8.direction;
			grld.position.W() = 0;
		}else{
			grld.position.sub_vector(PTM::TSubVectorDim<0,3>()) = l8.position;
			grld.position.W() = 1;
		}
		grld.range = l8.range;
		grld.specular = l8.specular;
		grld.spotFalloff = l8.falloff;
		grld.spotInner = l8.spotInner;
		grld.spotCutoff = l8.spotCutoff;
		fc->PushCreateNode(GRLightIf::GetIfInfoStatic(), &grld);
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};
class FINodeHandlerXMesh: public FINodeHandlerImp<Mesh>{
public:
	FINodeHandlerXMesh():FINodeHandlerImp<Desc>("Mesh"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(GRMeshIf::GetIfInfoStatic(), &GRMeshDesc());	
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->positions = d.vertices;	// 頂点座標

			for (int f=0; f < d.nFaces; ++f){		
				if ((d.faces[f].nFaceVertexIndices == 3) || (d.faces[f].nFaceVertexIndices == 4)) {
					mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
					mesh->elementIndex.push_back(f);
					mesh->originalFaces.push_back( d.faces[f].faceVertexIndices[0] );
					mesh->originalFaces.push_back( d.faces[f].faceVertexIndices[1] );
					mesh->originalFaces.push_back( d.faces[f].faceVertexIndices[2] );

					if (d.faces[f].nFaceVertexIndices == 4){
						// facesには、面が四角形なら三角形に分割したインデックスをpush
						mesh->faces.push_back( d.faces[f].faceVertexIndices[0] );
						mesh->faces.push_back( d.faces[f].faceVertexIndices[2] );
						mesh->faces.push_back( d.faces[f].faceVertexIndices[3] );
						mesh->elementIndex.push_back(f);
						// originalFaces には、4頂点目のインデックスをpush
						mesh->originalFaces.push_back( d.faces[f].faceVertexIndices[3] );
					}
				}else{
					fc->ErrorMessage(NULL, "Number of faces for mesh = 3 or 4.");
				}
			}
		}else{
			fc->ErrorMessage(NULL, "cannot create Mesh node.");
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
			fc->ErrorMessage(NULL, "MeshMaterialList must be inside of Mesh node.");
		}
	}
};

class FINodeHandlerXMaterial: public FINodeHandlerImp<Material>{
public:
	FINodeHandlerXMaterial():FINodeHandlerImp<Desc>("Material"){}
	// TextureFilenameをマテリアルと関連付けるため、Materialもオブジェクトスタックに Push .
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			fc->PushCreateNode(GRMaterialIf::GetIfInfoStatic(), &GRMaterialDesc());	
			GRMaterial* mat = DCAST(GRMaterial, fc->objects.Top());		
			if(mat){											
				// "MeshTextureCoords"と関連づけるため、一度GRMaterialをスタックに積む。
				// 実体は GRMeshDesc で持ち、mesh のレンダリングで呼び出す。
				mat->ambient = mat->diffuse = d.face;		// スタックに積んであるmatへ値を代入
				mat->specular = Vec4f(d.specular.x, d.specular.y, d.specular.z, 1.0);
				mat->emissive = Vec4f(d.emissive.x, d.emissive.y, d.emissive.z, 1.0);
				mat->power = d.power;
				mesh->material.push_back(*mat);
			}else{
				fc->ErrorMessage(NULL, "cannot create Material node.");
			}
		}else{
			fc->ErrorMessage(NULL, "Material must be inside of Mesh node.  ");
		}
	}
	void Loaded(Desc& d, FILoadContext* fc){
		GRMaterial* mat = DCAST(GRMaterial, fc->objects.Top());
		if (mat){
			fc->objects.Pop();
			delete mat;
		}
	}
};

class FINodeHandlerXMeshNormals: public FINodeHandlerImp<MeshNormals>{
public:
	FINodeHandlerXMeshNormals():FINodeHandlerImp<Desc>("MeshNormals"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->normals = d.normals;														// 法線ベクトル
			for (int f=0; f < d.nFaceNormals; ++f){
				for (int i=0; i < d.faceNormals[f].nFaceVertexIndices; ++i){
					mesh->faceNormals.push_back( d.faceNormals[f].faceVertexIndices[i] );	// 法線インデックス
				}
			}
		}else{
			fc->ErrorMessage(NULL, "MeshNormals must be inside of Mesh node.");
		}
	}
};

class FINodeHandlerXTextureFilename: public FINodeHandlerImp<TextureFilename>{
public:
	FINodeHandlerXTextureFilename():FINodeHandlerImp<Desc>("TextureFilename"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMaterial* mat = DCAST(GRMaterial, fc->objects.Top());
		if (mat){
			fc->objects.Pop();
			GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
			if (mesh){
				mesh->material.back().texname = d.filename;
			}
		}else{
			fc->ErrorMessage(NULL, "TextureFilename must be inside of Material node.");
		}
	}
};

class FINodeHandlerXMeshTextureCoords: public FINodeHandlerImp<MeshTextureCoords>{
public:
	FINodeHandlerXMeshTextureCoords():FINodeHandlerImp<Desc>("MeshTextureCoords"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->texCoords = d.textureCoords;
		}else{
			fc->ErrorMessage(NULL, "MeshTextureCoords must be inside of Mesh node.");
		}
	}
};


class FINodeHandlerSolid: public FINodeHandlerImp<Solid>{
public:
	class SolidAdaptor: public FILoadContext::Task{
	public:
		PHSolid* solid;
		SolidAdaptor(PHSolid* s):solid(s){}
		virtual bool AddChildObject(ObjectIf* o){
			GRFrame* fr = DCAST(GRFrame, o);
			if (fr){	//	fr以下の全MeshをSolidに追加
				AddFrame(fr, Affinef());
				return true;
			}
			return false;
		}
		void AddFrame(GRFrame* fr, Affinef af){
			af = af * fr->GetTransform();
			for(unsigned i=0; i<fr->NChildObject(); ++i){
				GRVisual* v = DCAST(GRVisual, fr->GetChildObject(i));
				GRFrame* f = DCAST(GRFrame, v);
				if (f) AddFrame(f, af);
				GRMesh* m = DCAST(GRMesh, v);
				if (m){
					CDConvexMeshDesc desc;
					for(unsigned i=0; i<m->positions.size(); ++i){
						desc.vertices.push_back(m->positions[i]);
					}
					solid->CreateShape(desc);
					Posed pose;
					pose.FromAffine(af);
					solid->SetShapePose(solid->NShape()-1, pose);
				}
			}
		}
		virtual void Execute(FILoadContext* ctx){
			//	TODO: Frameworkへの登録
		}
	};

	FINodeHandlerSolid():FINodeHandlerImp<Desc>("Solid"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(PHSolidIf::GetIfInfoStatic(), &PHSolidDesc());
		PHSolid* solid = DCAST(PHSolid, fc->objects.Top());
		solid->center = d.center;
		solid->velocity = d.velocity;
		solid->angVelocity = d.angularVelocity;
		solid->inertia = d.inertia;
		solid->mass = d.mass;
		SolidAdaptor* task = DBG_NEW SolidAdaptor(solid);
		fc->objects.Push(task->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		SolidAdaptor* task = DCAST(SolidAdaptor, fc->objects.Top());
		fc->links.push_back(task);
		fc->objects.Pop();	//	task

		assert(DCAST(PHSolid, fc->objects.Top()));
		fc->objects.Pop();	//	solid
	}
};

class FINodeHandlerCamera: public FINodeHandlerImp<Camera>{
public:	
	FINodeHandlerCamera():FINodeHandlerImp<Desc>("Camera"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->objects.PrintShort(DSTR);
		GRCameraDesc cd;
		cd.back = d.back;
		cd.front = d.front;
		cd.center = Vec2f(d.offsetX, d.offsetY);
		cd.size = Vec2f(d.width, d.height);
		fc->PushCreateNode(GRCameraIf::GetIfInfoStatic(), &cd);
		GRCamera* cam = DCAST(GRCamera, fc->objects.Top());
		GRFrameDesc fd;
		fd.transform = d.view;
		GRFrameIf* frame = DCAST(GRFrameIf, cam->GetScene()->CreateObject(GRFrameIf::GetIfInfoStatic(), &fd));
		cam->AddChildObject(frame);
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};
class FINodeHandlerPhysicalMaterial: public FINodeHandlerImp<PhysicalMaterial>{
public:	
	FINodeHandlerPhysicalMaterial():FINodeHandlerImp<Desc>("PhysicalMaterial"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerGravityEngine: public FINodeHandlerImp<GravityEngine>{
public:	
	FINodeHandlerGravityEngine():FINodeHandlerImp<Desc>("GravityEngine"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerContactEngine: public FINodeHandlerImp<ContactEngine>{
public:	
	FINodeHandlerContactEngine():FINodeHandlerImp<Desc>("ContactEngine"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerScene: public FINodeHandlerImp<Scene>{
public:	
	FINodeHandlerScene():FINodeHandlerImp<Desc>("Scene"){}
	void Load(Desc& d, FILoadContext* fc){
		//	Frameworkを作る
		FWSceneDesc fwsd;
		fc->PushCreateNode(FWSceneIf::GetIfInfoStatic(), &fwsd);
		FWScene* fws = DCAST(FWScene, fc->objects.Top());
		
		//	PHSDKを作る。スタックからは消す。
		PHSdkDesc phsdkd;
		fc->PushCreateNode(PHSdkIf::GetIfInfoStatic(), &phsdkd);
		PHSdkIf* phSdk = DCAST(PHSdkIf, fc->objects.Top());
		fc->objects.Pop();
		//	GRSDKを作る。スタックからは消す。
		GRSdkDesc grsdkd;
		fc->PushCreateNode(GRSdkIf::GetIfInfoStatic(), &grsdkd);
		GRSdkIf* grSdk = DCAST(GRSdkIf, fc->objects.Top());
		fc->objects.Pop();

		//	GRSceneを作る。
		GRSceneIf* grScene = grSdk->CreateScene();
		//	PHSceneを作る。
		PHSceneDesc psd;
		PHSceneIf* phScene = phSdk->CreateScene(psd);


		//	Frameworkにシーンを登録
		fws->AddChildObject(phScene);
		fws->AddChildObject(grScene);
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};

class FINodeHandlerSolidContainer: public FINodeHandlerImp<SolidContainer>{
public:	
	FINodeHandlerSolidContainer():FINodeHandlerImp<Desc>("SolidContainer"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerJointEngine: public FINodeHandlerImp<JointEngine>{
public:	
	FINodeHandlerJointEngine():FINodeHandlerImp<Desc>("JointEngine"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerJoint: public FINodeHandlerImp<Joint>{
public:	
	FINodeHandlerJoint():FINodeHandlerImp<Desc>("Joint"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FINodeHandlerImport: public FINodeHandlerImp<Import>{
public:	
	FINodeHandlerImport():FINodeHandlerImp<Desc>("Import"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

}



namespace Spr{
using namespace SprOldSpringhead;
void RegisterOldSpringheadNodeHandlers(){
	REGISTER_NODE_HANDLER(FINodeHandlerXHeader);
	REGISTER_NODE_HANDLER(FINodeHandlerXFrame);
	REGISTER_NODE_HANDLER(FINodeHandlerXFrameTransformMatrix);
	REGISTER_NODE_HANDLER(FINodeHandlerXLight8);
	REGISTER_NODE_HANDLER(FINodeHandlerXMesh);
	REGISTER_NODE_HANDLER(FINodeHandlerXMeshMaterialList);
	REGISTER_NODE_HANDLER(FINodeHandlerXMaterial);
	REGISTER_NODE_HANDLER(FINodeHandlerXMeshNormals);
	REGISTER_NODE_HANDLER(FINodeHandlerXTextureFilename);
	REGISTER_NODE_HANDLER(FINodeHandlerXMeshTextureCoords);
	REGISTER_NODE_HANDLER(FINodeHandlerSolid);
	REGISTER_NODE_HANDLER(FINodeHandlerScene);
	REGISTER_NODE_HANDLER(FINodeHandlerCamera);
	REGISTER_NODE_HANDLER(FINodeHandlerSolidContainer);
	REGISTER_NODE_HANDLER(FINodeHandlerGravityEngine);
	REGISTER_NODE_HANDLER(FINodeHandlerContactEngine);
	REGISTER_NODE_HANDLER(FINodeHandlerJointEngine);
	REGISTER_NODE_HANDLER(FINodeHandlerJoint);
	REGISTER_NODE_HANDLER(FINodeHandlerImport);
}
}