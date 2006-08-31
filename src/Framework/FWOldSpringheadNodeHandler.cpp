/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/WBPath.h>
#include <FileIO/FINodeHandler.h>
#include <FileIO/FIFileX.h>
#include <FileIO/FISdk.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRScene.h>
#include <Physics/PHSolid.h>
#include <Physics/PHJoint.h>
#include <Framework/FWOldSpringheadNodeHandler.h>
#include <Framework/FWObject.h>
#include <Framework/FWScene.h>
#include <Framework/FWSdk.h>
#include <Collision/CDConvexMesh.h>

#include <Framework/FWOldSpringheadNode.h>

namespace SprOldSpringhead{
using namespace Spr;

static FWScene* FindFWScene(FILoadContext* fc){
	FWScene* fs = NULL;
	for(int i=(int)fc->objects.size()-1; i>=0; --i){
		fs = DCAST(FWScene, fc->objects[i]);
		if (fs) break;
	}
	return fs;
}

static PHScene* FindPHScene(FILoadContext* fc){
	PHScene* ps = NULL;
	for(int i=(int)fc->objects.size()-1; i>=0; --i){
		ps = DCAST(PHScene, fc->objects[i]);
		if (ps) break;
		FWScene* fs = DCAST(FWScene, fc->objects[i]);
		if (fs) ps = DCAST(PHScene, fs->GetPHScene());
		if (ps) break;
	}
	return ps;
}
static GRScene* FindGRScene(FILoadContext* fc){
	GRScene* gs = NULL;
	for(int i=(int)fc->objects.size()-1; i>=0; --i){
		gs = DCAST(GRScene, fc->objects[i]);
		if (gs) break;
		FWScene* fs = DCAST(FWScene, fc->objects[i]);
		if (fs) gs = DCAST(GRScene, fs->GetGRScene());
		if (gs) break;
	}
	return gs;
}


class FWNodeHandlerXHeader: public FINodeHandlerImp<Header>{
public:
	FWNodeHandlerXHeader():FINodeHandlerImp<Desc>("Header"){}
	void Load(Desc& d, FILoadContext* fc){
	}
};
class FWNodeHandlerXFrame: public FINodeHandlerImp<Frame>{
public:	
	FWNodeHandlerXFrame():FINodeHandlerImp<Desc>("Frame"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(GRFrameIf::GetIfInfoStatic(), &GRFrameDesc());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};
class FWNodeHandlerXFrameTransformMatrix: public FINodeHandlerImp<FrameTransformMatrix>{
public:	
	FWNodeHandlerXFrameTransformMatrix():FINodeHandlerImp<Desc>("FrameTransformMatrix"){}
	void Load(Desc& d, FILoadContext* fc){
		GRFrame* fr = DCAST(GRFrame, fc->objects.Top());
		if (fr){
			fr->transform = d.matrix;
		}else{
			fc->ErrorMessage(NULL, NULL, "FrameTransformMatrix must be inside of Frame node.");
		}
	}
};
class FWNodeHandlerXLight8: public FINodeHandlerImp<Light8>{
public:
	class Adapter: public FILoadedTask{
	public:
		GRLight* light;
		bool AddChildObject(ObjectIf* o){
			GRFrame* fr = DCAST(GRFrame, o);
			if (fr){
				Affinef af = fr->GetTransform();
				light->position.sub_vector(PTM::TSubVectorDim<0,3>()) = af.Ez();
				return true;
			}
			return false;
		}
		void Execute(FILoadContext* ctx){}
	};
	FWNodeHandlerXLight8():FINodeHandlerImp<Desc>("Light8"){}
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
		//	左手系→右手系の変換
		grld.position.Z() *= -1;
		grld.spotDirection.Z() *= -1;
		fc->PushCreateNode(GRLightIf::GetIfInfoStatic(), &grld);
		Adapter* a = new Adapter;
		a->light = DCAST(GRLight, fc->objects.Top());
		fc->objects.Push(a->GetIf());
		fc->links.push_back(a);
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
		fc->objects.Pop();
	}
};
class FWNodeHandlerXMesh: public FINodeHandlerImp<Mesh>{
public:
	FWNodeHandlerXMesh():FINodeHandlerImp<Desc>("Mesh"){}
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
					fc->ErrorMessage(NULL, NULL, "Number of faces for mesh = 3 or 4.");
				}
			}
		}else{
			fc->ErrorMessage(NULL, NULL, "cannot create Mesh node.");
		}
	}
	void Loaded(Desc& d, FILoadContext* fc){
		fc->objects.Pop();
	}
};

class FWNodeHandlerXMeshMaterialList: public FINodeHandlerImp<MeshMaterialList>{
public:
	FWNodeHandlerXMeshMaterialList():FINodeHandlerImp<Desc>("MeshMaterialList"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->materialList = d.faceIndexes;		// マテリアル番号のリスト
		}else{
			fc->ErrorMessage(NULL, NULL, "MeshMaterialList must be inside of Mesh node.");
		}
	}
};

class FWXTextureTask: public FILoadedTask{
public:
	OBJECT_DEF_NOIF(FWXTextureTask);
	UTString filename;
	void Execute(FILoadContext* fc){}
};
OBJECT_IMP(FWXTextureTask, FILoadedTask);

class FWNodeHandlerXTextureFilename: public FINodeHandlerImp<TextureFilename>{
public:
	FWNodeHandlerXTextureFilename():FINodeHandlerImp<Desc>("TextureFilename"){}
	void Load(Desc& d, FILoadContext* fc){
		UTRef<FWXTextureTask> tex = new FWXTextureTask;
		tex->filename = d.filename;
		if (fc->datas.Top()->name.length()){
			GRScene* gs = FindGRScene(fc);
			tex->SetNameManager(gs);
			tex->SetName(fc->datas.Top()->name.c_str());
		}
		fc->objects.Top()->AddChildObject(tex->GetIf());
		fc->links.push_back(tex);
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FWNodeHandlerXMaterial: public FINodeHandlerImp<Material>{
public:
	class MaterialAdaptor: public FILoadedTask{
	public:
		GRMaterial* material;
		MaterialAdaptor(){}
		bool AddChildObject(ObjectIf* o){
			FWXTextureTask* tex = DCAST(FWXTextureTask, o);
			if (tex){	//	自分が作った materialにLink時にAddされるtexturefilenameを設定する。
				material->texname = tex->filename;
				return true;
			}
			return false;
		}
	};
	FWNodeHandlerXMaterial():FINodeHandlerImp<Desc>("Material"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMaterialDesc desc;
		desc.ambient = d.face;
		desc.diffuse = d.face;
		desc.specular = Vec4f(d.specular.x, d.specular.y, d.specular.z, 1.0);
		desc.emissive = Vec4f(d.emissive.x, d.emissive.y, d.emissive.z, 1.0);
		desc.power = d.power;
		fc->PushCreateNode(GRMaterialIf::GetIfInfoStatic(), &desc);
		MaterialAdaptor* mc = DBG_NEW MaterialAdaptor;
		mc->material = DCAST(GRMaterial, fc->objects.Top());
		fc->objects.Push(mc->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		MaterialAdaptor* mc = DCAST(MaterialAdaptor, fc->objects.Top());
		fc->links.push_back(mc);
		fc->objects.Pop();		// MaterialAdaptor
		fc->objects.Pop();		// GRMaterialDesc
	}
};


class FWNodeHandlerXMeshNormals: public FINodeHandlerImp<MeshNormals>{
public:
	FWNodeHandlerXMeshNormals():FINodeHandlerImp<Desc>("MeshNormals"){}
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
			fc->ErrorMessage(NULL, NULL, "MeshNormals must be inside of Mesh node.");
		}
	}
};

class FWNodeHandlerXMeshTextureCoords: public FINodeHandlerImp<MeshTextureCoords>{
public:
	FWNodeHandlerXMeshTextureCoords():FINodeHandlerImp<Desc>("MeshTextureCoords"){}
	void Load(Desc& d, FILoadContext* fc){
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		if (mesh){
			mesh->texCoords = d.textureCoords;
		}else{
			fc->ErrorMessage(NULL, NULL, "MeshTextureCoords must be inside of Mesh node.");
		}
	}
};

class FWNodeHandlerContactEngine: public FINodeHandlerImp<ContactEngine>{
public:	
	class Disabler: public FILoadedTask{
	public:
		PHSceneIf* phScene;
		Disabler():phScene(NULL){}
		void Execute(FILoadContext* fc){
			phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		}
	};
	class Adapter: public FILoadedTask{
	public:
		PHSceneIf* phScene;
		Adapter():phScene(NULL){}
		void AddFrameToSolid(PHSolid* solid, GRFrame* fr, Affinef af){
			af = af * fr->GetTransform();
			for(unsigned i=0; i<fr->NChildObject(); ++i){
				GRVisual* v = DCAST(GRVisual, fr->GetChildObject(i));
				GRFrame* f = DCAST(GRFrame, v);
				if (f) AddFrameToSolid(solid, f, af);
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
		virtual bool AddChildObject(ObjectIf* o){
			GRFrame* fr = DCAST(GRFrame, o);
			if (fr){	//	Solidに変換して追加
				PHSolidDesc sd;
				PHSolid* solid = DCAST(PHSolid, phScene->CreateSolid());
				UTString name("so");
				name.append(fr->GetName());
				solid->SetName(name.c_str());
				solid->SetDynamical(false);
				Posed pose;
				pose.FromAffine(fr->GetTransform());
				solid->SetPose(pose);
				AddFrameToSolid(solid, fr, fr->GetTransform().inv());
				phScene->SetContactMode(solid, PHSceneDesc::MODE_LCP);
				return true;
			}
			if (DCAST(PHSolid, o)){	//	solidなら何もしない。デフォルトONなので。
				//	受け取ったObjectを接触ONにする。
				phScene->SetContactMode(DCAST(PHSolid, o), PHSceneDesc::MODE_LCP);
				return true;
			}
			return false;
		}
		void Execute(FILoadContext* fc){}
	};
	FWNodeHandlerContactEngine():FINodeHandlerImp<Desc>("ContactEngine"){}
	void Load(Desc& d, FILoadContext* fc){
		PHScene* ps = FindPHScene(fc);
		Disabler* dis = DBG_NEW Disabler;
		dis->phScene = FindPHScene(fc);
		fc->links.push_back(dis);
		Adapter* task = DBG_NEW Adapter;
		task->phScene = ps;
		fc->objects.Push(task->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		Adapter* task = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(task);
		fc->objects.Pop();	//	task
	}
};

class FWNodeHandlerSolid: public FINodeHandlerImp<Solid>{
public:
	class Adapter: public FWNodeHandlerContactEngine::Adapter{
	public:
		PHSolid* solid;
		GRFrame* frame;
		FWScene* fwScene;
		Adapter():solid(NULL), fwScene(NULL){}
		virtual bool AddChildObject(ObjectIf* o){
			frame = DCAST(GRFrame, o);
			if (frame){	//	fr以下の全MeshをSolidに追加
				AddFrameToSolid(solid, frame, frame->GetTransform().inv());
				return true;
			}
			return false;
		}
		virtual void Execute(FILoadContext* ctx){
			FWObject* obj = DCAST(FWObject, fwScene->CreateObject(FWObjectIf::GetIfInfoStatic(), NULL));
			assert(frame);
			Posed pose;
			pose.FromAffine(frame->GetTransform());
			solid->SetPose(pose);
			obj->AddChildObject(frame->GetIf());
			obj->AddChildObject(solid->GetIf());
			fwScene->AddChildObject(obj->GetIf());
		}
	};

	FWNodeHandlerSolid():FINodeHandlerImp<Desc>("Solid"){}
	void Load(Desc& d, FILoadContext* fc){
		fc->PushCreateNode(PHSolidIf::GetIfInfoStatic(), &PHSolidDesc());
		PHSolid* solid = DCAST(PHSolid, fc->objects.Top());
		solid->center = d.center;
		solid->velocity = d.velocity;
		solid->angVelocity = d.angularVelocity;
		solid->inertia = d.inertia;
		solid->mass = d.mass;
		Adapter* task = DBG_NEW Adapter;
		task->solid = solid;
		task->fwScene = FindFWScene(fc);
		fc->objects.Push(task->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		Adapter* task = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(task);
		fc->objects.Pop();	//	task

		assert(DCAST(PHSolid, fc->objects.Top()));
		fc->objects.Pop();	//	solid
	}
};



class FWNodeHandlerCamera: public FINodeHandlerImp<Camera>{
public:	
	FWNodeHandlerCamera():FINodeHandlerImp<Desc>("Camera"){}
	void Load(Desc& d, FILoadContext* fc){
		d.view.ExZ() *= -1;
		d.view.EyZ() *= -1;
		d.view.EzX() *= -1;
		d.view.EzY() *= -1;
		d.view.PosZ() *= -1;

		GRCameraDesc cd;
		cd.back = d.back;
		cd.front = d.front;
		cd.center = Vec2f(d.offsetX, d.offsetY);
		cd.size = Vec2f(d.width, d.height);
		fc->PushCreateNode(GRCameraIf::GetIfInfoStatic(), &cd);
		GRCamera* cam = DCAST(GRCamera, fc->objects.Top());
		GRFrameDesc fd;
		fd.transform = d.view;
		GRFrameIf* frame = DCAST(GRFrameIf, cam->GetNameManager()->CreateObject(GRFrameIf::GetIfInfoStatic(), &fd));
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

class FWNodeHandlerGravityEngine: public FINodeHandlerImp<GravityEngine>{
public:	
	FWNodeHandlerGravityEngine():FINodeHandlerImp<Desc>("GravityEngine"){}
	int linkPos;
	void Load(Desc& d, FILoadContext* fc){
		PHScene* s = FindPHScene(fc);
		if (s) s->SetGravity(d.gravity);
		linkPos = (int)fc->links.size();
	}
	void Loaded(Desc& d, FILoadContext* fc){
		//	GravityEngineの子オブジェクトはとりあえず無視。本当はGravityのOn/Offに使う。
		fc->links.resize(linkPos);
	}
};


class FWNodeHandlerScene: public FINodeHandlerImp<Scene>{
public:	
	FWNodeHandlerScene():FINodeHandlerImp<Desc>("Scene"){}
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

class FWNodeHandlerSolidContainer: public FINodeHandlerImp<SolidContainer>{
public:	
	FWNodeHandlerSolidContainer():FINodeHandlerImp<Desc>("SolidContainer"){}
	void Load(Desc& d, FILoadContext* fc){
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

class FWNodeHandlerJointEngine: public FINodeHandlerImp<JointEngine>{
public:	
	class JointCreator: public FILoadedTask{
	public:
		PHScene* phScene;
		JointCreator* parent;
		PHJoint1DDesc desc;
		PHSolid* solid;
		UTString name;
		JointCreator(): parent(NULL), solid(NULL), phScene(NULL){}
		bool AddChildObject(ObjectIf* o){
			PHSolid* s = DCAST(PHSolid, o);
			if (s){
				solid = s;
				return true;
			}
			return false;
		}
		void Execute(FILoadContext* fc){
			const float tinyMass = 0.1f;
			if (!solid){
				PHSolidDesc sd;
				sd.mass *= tinyMass;
				sd.inertia *= tinyMass;
				solid = DCAST(PHSolid, phScene->CreateSolid(sd));
			}
			if (parent && !parent->solid){
				PHSolidDesc sd;
				sd.mass *= tinyMass;
				sd.inertia *= tinyMass;
				parent->solid = DCAST(PHSolid, phScene->CreateSolid(sd));
			}
			if (parent){
				PHJointIf* j= phScene->CreateJoint(solid, parent->solid, desc);
				j->SetName(name.c_str());
			}
		}
	};

	FWNodeHandlerJointEngine():FINodeHandlerImp<Desc>("JointEngine"){}
	void Load(Desc& d, FILoadContext* fc){
		JointCreator* j = DBG_NEW JointCreator;
		j->phScene = FindPHScene(fc);
		fc->objects.Push(j->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		JointCreator* j = DCAST(JointCreator, fc->objects.Top());
		fc->links.push_back(j);
		fc->objects.Pop();
	}
};

class FWNodeHandlerJoint: public FINodeHandlerImp<Joint>{
public:	
	typedef FWNodeHandlerJointEngine::JointCreator JointCreator;
	FWNodeHandlerJoint():FINodeHandlerImp<Desc>("Joint"){}
	void Load(Desc& d, FILoadContext* fc){
		JointCreator* j = DBG_NEW JointCreator;
		j->name = fc->datas.Top()->name;
		j->desc.type = d.nType ? PHJointDesc::SLIDERJOINT : PHJointDesc::HINGEJOINT;
		j->desc.posePlug.Pos() = d.crj;
		j->desc.posePlug.Ori().FromMatrix(d.cRj);
		j->desc.poseSocket.Pos() = d.prj;
		j->desc.poseSocket.Ori().FromMatrix(d.pRj);
		j->desc.spring = d.fPValue;
		j->desc.damper = d.fDValue;
		j->desc.origin = d.fInput;
		j->desc.lower = d.fMinPosition;
		j->desc.upper = d.fMaxPosition;
		
		j->parent = DCAST(JointCreator, fc->objects.Top());
		j->phScene = FindPHScene(fc);
		fc->objects.Push(j->GetIf());
	}
	void Loaded(Desc& d, FILoadContext* fc){
		JointCreator* j = DCAST(JointCreator, fc->objects.Top());
		fc->links.push_back(j);
		fc->objects.Pop();
	}
};

class FWNodeHandlerImport: public FINodeHandlerImp<Import>{
public:	
	FWNodeHandlerImport():FINodeHandlerImp<Desc>("Import"){}
	void Load(Desc& d, FILoadContext* fc){
		UTString filename = d.file;

		WBPath xFilePath;
		xFilePath.Path(fc->fileInfo.back()->name);
		xFilePath.Path(xFilePath.FullPath());
		WBPath imPath;
		imPath.Path(filename);
        UTString oldCwd = imPath.GetCwd();
		imPath.SetCwd(xFilePath.Drive() + xFilePath.Dir());
		UTString full = imPath.FullPath();		


		FISdk* sdk = fc->fileInfo.back()->file->GetSdk();		
		fc->fileInfo.Push();
		fc->fileInfo.Top() = new FILoadContext::FileInfo;
		fc->fileInfo.back()->Map(full);
		if (fc->IsGood()){
			UTRef<FIFileX> fileX = DCAST(FIFileX, sdk->CreateFileX());
			fileX->LoadImp(fc);
		}
		fc->fileInfo.Pop();
		fc->fileInfo.Top()->file->SetLoaderContext(fc);
	}
	void Loaded(Desc& d, FILoadContext* fc){
	}
};

}



namespace Spr{
using namespace SprOldSpringhead;
void SPR_CDECL FWRegisterOldSpringheadNode(){
	FINodeHandlers* handlers = FISdk::GetHandlers("OldSpringhead");
	handlers->insert(DBG_NEW FWNodeHandlerXHeader);
	handlers->insert(DBG_NEW FWNodeHandlerXFrame);
	handlers->insert(DBG_NEW FWNodeHandlerXFrameTransformMatrix);
	handlers->insert(DBG_NEW FWNodeHandlerXLight8);
	handlers->insert(DBG_NEW FWNodeHandlerXMesh);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshMaterialList);
	handlers->insert(DBG_NEW FWNodeHandlerXMaterial);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshNormals);
	handlers->insert(DBG_NEW FWNodeHandlerXTextureFilename);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshTextureCoords);
	handlers->insert(DBG_NEW FWNodeHandlerSolid);
	handlers->insert(DBG_NEW FWNodeHandlerScene);
	handlers->insert(DBG_NEW FWNodeHandlerCamera);
	handlers->insert(DBG_NEW FWNodeHandlerSolidContainer);
	handlers->insert(DBG_NEW FWNodeHandlerGravityEngine);
	handlers->insert(DBG_NEW FWNodeHandlerContactEngine);
	handlers->insert(DBG_NEW FWNodeHandlerJointEngine);
	handlers->insert(DBG_NEW FWNodeHandlerJoint);
	handlers->insert(DBG_NEW FWNodeHandlerImport);
}
}
