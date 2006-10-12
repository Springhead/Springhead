/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/UTLoadHandler.h>
#include <Foundation/UTLoadContext.h>
#include <Foundation/UTPath.h>
#include <FileIO/FISdk.h>
#include <FileIO/FIFileX.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
#include <Graphics/GRSphere.h>
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

// FWSceneの検索．
static FWScene* FindFWScene(UTLoadContext* fc){
	FWScene* fs = NULL;
	for(int i=(int)fc->objects.size()-1; i>=0; --i){
		fs = DCAST(FWScene, fc->objects[i]);
		if (fs) break;
	}
	return fs;
}

// PHSceneの検索．
static PHScene* FindPHScene(UTLoadContext* fc){
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

// GRSceneの検索．
static GRScene* FindGRScene(UTLoadContext* fc){
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

// Xファイル形式のバージョン情報．
class FWNodeHandlerXHeader: public UTLoadHandlerImp<Header>{
public:
	FWNodeHandlerXHeader():UTLoadHandlerImp<Desc>("Header"){}
	void Load(Desc& d, UTLoadContext* fc){
	}
};

// Springhead1のフレーム．
class FWNodeHandlerXFrame: public UTLoadHandlerImp<Frame>{
public:	
	FWNodeHandlerXFrame():UTLoadHandlerImp<Desc>("Frame"){}	
	void Load(Desc& d, UTLoadContext* fc){
		fc->PushCreateNode(GRFrameIf::GetIfInfoStatic(), &GRFrameDesc());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		fc->objects.Pop();
	}
};

// Springhead1のSimulatorタスク．
class FWSimulatorTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWSimulatorTask);
	double timeStep;	// 積分ステップ
	double decay;		// 剛体の速度の減衰率
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWSimulatorTask, UTLoadTask);

// Springhead1のSimulator．
class FWNodeHandlerSimulator: public UTLoadHandlerImp<Simulator>{
public:	
	FWNodeHandlerSimulator():UTLoadHandlerImp<Desc>("Simulator"){}
	void Load(Desc& d, UTLoadContext* fc){		
		UTRef<FWSimulatorTask> simtask = DBG_NEW FWSimulatorTask;
		simtask->timeStep = d.timeStep;
		simtask->decay    = d.decay;		
		PHScene* phScene = FindPHScene(fc);		
		if (phScene){		// phSceneを検索できた場合は、timeStepを設定. 
			fc->mapObj.insert(UTPairObject(simtask->GetIf(), phScene->GetIf()));
			phScene->SetTimeStep(d.timeStep);
		}else{				// Sceneロード時に、mapを検索し、timeStepを設定 			
			fc->mapObj.insert(UTPairObject(simtask->GetIf(), NULL));
		}
	}
};

// DirectXのFrameTransformMatrix．フレームの変換行列を表す．GRFrame::transformに対応．
class FWNodeHandlerXFrameTransformMatrix: public UTLoadHandlerImp<FrameTransformMatrix>{
public:	
	FWNodeHandlerXFrameTransformMatrix():UTLoadHandlerImp<Desc>("FrameTransformMatrix"){}
	void Load(Desc& d, UTLoadContext* fc){
		GRFrame* fr = DCAST(GRFrame, fc->objects.Top());
		if (fr){
			fr->transform = d.matrix;
		}else{
			fc->ErrorMessage(NULL, NULL, "FrameTransformMatrix must be inside of Frame node.");
		}
	}
};

// Springehead1の光源．GRLightに対応．
class FWNodeHandlerXLight8: public UTLoadHandlerImp<Light8>{
public:
	class Adapter: public UTLoadTask{
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
		void Execute(UTLoadContext* ctx){}
	};
	FWNodeHandlerXLight8():UTLoadHandlerImp<Desc>("Light8"){}
	void Load(Desc& l8, UTLoadContext* fc){
		GRLightDesc grld;
		grld.ambient = l8.ambient;
		grld.attenuation0 = l8.attenuation0;
		grld.attenuation1 = l8.attenuation1;
		grld.attenuation2 = l8.attenuation2;
		grld.diffuse = l8.diffuse;
		if (l8.type == Light8::XLIGHT_DIRECTIONAL){		// 平行光源
			grld.position.sub_vector(PTM::TSubVectorDim<0,3>()) = l8.direction;
			grld.position.W() = 0;
		}else{											// 点光源・スポットライト効果
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
		Adapter* a = DBG_NEW Adapter;
		a->light = DCAST(GRLight, fc->objects.Top());
		fc->objects.Push(a->GetIf());
		fc->links.push_back(a);
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		fc->objects.Pop();
		fc->objects.Pop();
	}
};

// DirectXのテクスチャファイル名タスク．
class FWXTextureTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWXTextureTask);
	UTString filename;					// テクスチャファイル名
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWXTextureTask, UTLoadTask);

// DirectXのテクスチャファイル名（Materialの内部タグ)．
class FWNodeHandlerXTextureFilename: public UTLoadHandlerImp<TextureFilename>{
public:
	FWNodeHandlerXTextureFilename():UTLoadHandlerImp<Desc>("TextureFilename"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTRef<FWXTextureTask> tex = DBG_NEW FWXTextureTask;
		tex->filename = d.filename;
		if (fc->datas.Top()->name.length()){
			GRScene* gs = FindGRScene(fc);
			tex->SetNameManager(gs);
			tex->SetName(fc->datas.Top()->name.c_str());
		}
		fc->objects.Top()->AddChildObject(tex->GetIf());
		fc->links.push_back(tex);
	}
	void Loaded(Desc& d, UTLoadContext* fc){
	}
};

// DirectXのマテリアル．GRMateiralに対応．
class FWNodeHandlerXMaterial: public UTLoadHandlerImp<Material>{
public:
	class Adapter: public UTLoadTask{
	public:
		GRMaterial* material;
		Adapter(){}
		bool AddChildObject(ObjectIf* o){
			FWXTextureTask* tex = DCAST(FWXTextureTask, o);
			if (tex){	//	自分が作った materialにLink時にAddされるtexturefilenameを設定する。
				material->texname = tex->filename;
				return true;
			}
			return false;
		}
	};
	FWNodeHandlerXMaterial():UTLoadHandlerImp<Desc>("Material"){}
	void Load(Desc& d, UTLoadContext* fc){
		GRMaterialDesc desc;
		desc.ambient = d.face;
		desc.diffuse = d.face;
		desc.specular = Vec4f(d.specular.x, d.specular.y, d.specular.z, 1.0);
		desc.emissive = Vec4f(d.emissive.x, d.emissive.y, d.emissive.z, 1.0);
		desc.power = d.power;
		fc->PushCreateNode(GRMaterialIf::GetIfInfoStatic(), &desc);
		Adapter* mc = DBG_NEW Adapter;
		mc->material = DCAST(GRMaterial, fc->objects.Top());
		fc->objects.Push(mc->GetIf());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		Adapter* mc = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(mc);
		fc->objects.Pop();		// Adapter
		fc->objects.Pop();		// GRMaterialDesc
	}
};	
	
// DirectXのマテリアルタスク．
class FWPHMaterialTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWPHMaterialTask);
	FWPHMaterialTask(): mu(0.2f), mu0(0.2f), e(0.2f){}
	float mu;		///<	動摩擦摩擦係数
	float mu0;		///<	静止摩擦係数
	float e;		///<	跳ね返り係数	
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWPHMaterialTask, UTLoadTask);

// DirectXのマテリアル．GRMateiralに対応．
class FWNodeHandlerPhysicalMaterial: public UTLoadHandlerImp<PhysicalMaterial>{
public:	
	FWNodeHandlerPhysicalMaterial():UTLoadHandlerImp<Desc>("PhysicalMaterial"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTRef<FWPHMaterialTask> phmtask = DBG_NEW FWPHMaterialTask;
		phmtask->mu = d.d;		// 最大静止摩擦係数
		phmtask->mu0 = d.s;		// 動摩擦係数
		fc->objects.Top()->AddChildObject(phmtask->GetIf());		// Mesh->AddChildObject();
		fc->links.push_back(phmtask);
	}
	void Loaded(Desc& d, UTLoadContext* fc){		
	}
};

// DirectXのMeshのマテリアルリストタスク．
class FWMeshMaterialListTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWMeshMaterialListTask);
	std::vector<int> materialList;
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWMeshMaterialListTask, UTLoadTask);

// DirectXのMeshのマテリアルリスト．
class FWNodeHandlerXMeshMaterialList: public UTLoadHandlerImp<MeshMaterialList>{
public:
	FWNodeHandlerXMeshMaterialList():UTLoadHandlerImp<Desc>("MeshMaterialList"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTRef<FWMeshMaterialListTask> mmltask = DBG_NEW FWMeshMaterialListTask;
		mmltask->materialList = d.faceIndexes;
		fc->objects.Top()->AddChildObject(mmltask->GetIf());
		fc->links.push_back(mmltask);
	}
};

// DirectXのMeshの法線ベクトルタスク．
class FWMeshNormalsTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWMeshNormalsTask);
	std::vector<Vec3f> normals;		// 法線
	std::vector<int> faceNormals;	// 法線インデックス
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWMeshNormalsTask, UTLoadTask);

// DirectXのMeshの法線ベクトル．
class FWNodeHandlerXMeshNormals: public UTLoadHandlerImp<MeshNormals>{
public:
	FWNodeHandlerXMeshNormals():UTLoadHandlerImp<Desc>("MeshNormals"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTRef<FWMeshNormalsTask> normtask = DBG_NEW FWMeshNormalsTask;
		normtask->normals = d.normals;
		for (int f=0; f < d.nFaceNormals; ++f){
			for (int i=0; i < d.faceNormals[f].nFaceVertexIndices; ++i){
				normtask->faceNormals.push_back( d.faceNormals[f].faceVertexIndices[i] );	// 法線インデックス
			}
		}
		fc->objects.Top()->AddChildObject(normtask->GetIf());
		fc->links.push_back(normtask);
	}
};

// DirectXのMeshのテクスチャ座標タスク．
class FWMeshTextureCoordsTask: public UTLoadTask{
public:
	OBJECT_DEF_NOIF(FWMeshTextureCoordsTask);
	std::vector<Vec2f> texCoords;	// テクスチャUV
	void Execute(UTLoadContext* fc){}
};
OBJECT_IMP(FWMeshTextureCoordsTask, UTLoadTask);

// DirectXのMeshのテクスチャ座標．
class FWNodeHandlerXMeshTextureCoords: public UTLoadHandlerImp<MeshTextureCoords>{
public:
	FWNodeHandlerXMeshTextureCoords():UTLoadHandlerImp<Desc>("MeshTextureCoords"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTRef<FWMeshTextureCoordsTask> textask = DBG_NEW FWMeshTextureCoordsTask;
		textask->texCoords = d.textureCoords;
		fc->objects.Top()->AddChildObject(textask->GetIf());
		fc->links.push_back(textask);
	}
};

// DirectXのMesh．GRMeshに対応．
class FWNodeHandlerXMesh: public UTLoadHandlerImp<Mesh>{
public:
	class Adapter: public UTLoadTask{
	public:
		GRMesh* mesh;
		UTLoadContext* fc;
		
		Adapter():mesh(NULL){}
		bool AddChildObject(ObjectIf* o){
			// Physical Material
			FWPHMaterialTask* phmtask = DCAST(FWPHMaterialTask, o);
			if (phmtask){
				// mapを検索し、PhysicalMaterialを登録．
				UTMapObject::iterator itr = fc->mapObj.find(mesh->GetIf());
				if (itr != fc->mapObj.end()){
					GRMesh* m = DCAST(GRMesh, itr->first);
					if (m){
						FWPHMaterialTask* maptask = DCAST(FWPHMaterialTask, itr->second);
						maptask->mu  = phmtask->mu;
						maptask->mu0 = phmtask->mu0; 					
					}
				}else{
					fc->ErrorMessage(NULL, NULL, "Key(mesh) is not found within map object.");
				}
				return true;
			}
			//Material
			GRMaterial* mat = DCAST(GRMaterial, o);
			if (mat){
				mesh->material.push_back(mat);
				return mesh->GetNameManager()->AddChildObject(mat->GetIf());
			}
			// Material List
			FWMeshMaterialListTask* mmltask = DCAST(FWMeshMaterialListTask, o);
			if (mmltask){
				mesh->materialList = mmltask->materialList;
				return true;
			}
			// Normal
			FWMeshNormalsTask* normtask = DCAST(FWMeshNormalsTask, o);
			if (normtask){
				mesh->normals = normtask->normals;
				mesh->faceNormals = normtask->faceNormals;
				return true;
			}
			// Texture Coords
			FWMeshTextureCoordsTask* textask = DCAST(FWMeshTextureCoordsTask, o);
			if (textask){
				mesh->texCoords = textask->texCoords;
				return true;
			}
			return false;
		}
	};
	FWNodeHandlerXMesh():UTLoadHandlerImp<Desc>("Mesh"){}
	void Load(Desc& d, UTLoadContext* fc){
		fc->PushCreateNode(GRMeshIf::GetIfInfoStatic(), &GRMeshDesc());	
		GRMesh* mesh = DCAST(GRMesh, fc->objects.Top());
		// map<mesh, phmtask> を作成.
		UTRef<FWPHMaterialTask> phmtask = DBG_NEW FWPHMaterialTask;
		fc->mapObj.insert(UTPairObject(mesh->GetIf(), phmtask->GetIf()));

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
			// スタックに Mesh::Adapterを積む
			Adapter* ad = DBG_NEW Adapter;
			ad->mesh = DCAST(GRMesh, fc->objects.Top());
			ad->fc = fc;
			fc->objects.Push(ad->GetIf());
		}else{
			fc->ErrorMessage(NULL, NULL, "cannot create Mesh node.");
		}
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		Adapter* ad = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(ad);
		fc->objects.Pop();
		assert(DCAST(GRMesh, fc->objects.Top()));
		fc->objects.Pop();
	}
};

// Spirnghead1のSphere
class FWNodeHandlerSphere: public UTLoadHandlerImp<Sphere>{
public:
	class Adapter: public UTLoadTask{
	public:
		GRSphere* sphere;
		UTLoadContext* fc;
		
		Adapter():sphere(NULL){}
		bool AddChildObject(ObjectIf* o){
			// PHysical Material
			FWPHMaterialTask* phmtask = DCAST(FWPHMaterialTask, o);
			if (phmtask){
				// mapを検索し、PhysicalMaterialを登録．
				UTMapObject::iterator itr = fc->mapObj.find(sphere->GetIf());
				if (itr != fc->mapObj.end()){
					GRSphere* s = DCAST(GRSphere, itr->first);
					if (s){
						FWPHMaterialTask* maptask = DCAST(FWPHMaterialTask, itr->second);
						maptask->mu  = phmtask->mu;
						maptask->mu0 = phmtask->mu0;
					}
				}else{
					fc->ErrorMessage(NULL, NULL, "Key(sphere) is not found within map object.");
				}
				return true;
			}
			// Material
			GRMaterial* mat = DCAST(GRMaterial, o);
			if (mat){
				sphere->material = mat;
				return sphere->GetNameManager()->AddChildObject(mat->GetIf());
			}
			return false;
		}
	};
	FWNodeHandlerSphere():UTLoadHandlerImp<Desc>("Sphere"){}
	void Load(Desc& d, UTLoadContext* fc){
		fc->PushCreateNode(GRSphereIf::GetIfInfoStatic(), &GRSphereDesc());
		GRSphere* sphere = DCAST(GRSphere, fc->objects.Top());
		sphere->radius = d.radius;
		sphere->slices = d.slices;
		sphere->stacks = d.stacks;
		// map<sphere, phmtask> を作成．
		UTRef<FWPHMaterialTask> phmtask = DBG_NEW FWPHMaterialTask;
		fc->mapObj.insert(UTPairObject(sphere->GetIf(), phmtask->GetIf()));
		// スタックにSphere::Adapterを積む．
		Adapter* ad = DBG_NEW Adapter;
		ad->sphere = DCAST(GRSphere, fc->objects.Top());
		ad->fc = fc;
		fc->objects.Push(ad->GetIf());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		Adapter* ad = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(ad);
		fc->objects.Pop();		
		assert(DCAST(GRSphere, fc->objects.Top()));
		fc->objects.Pop();
	}

};	

// Springhead1のContactEngine．
class FWNodeHandlerContactEngine: public UTLoadHandlerImp<ContactEngine>{
public:	
	class Disabler: public UTLoadTask{
	public:
		PHSceneIf* phScene;
		Disabler():phScene(NULL){}
		void Execute(UTLoadContext* fc){
			phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		}
	};
	class Adapter: public UTLoadTask{
	public:
		PHSceneIf* phScene;
		UTLoadContext* fc;
		Adapter():phScene(NULL){}
		void AddFrameToSolid(PHSolid* solid, GRFrame* fr, Affinef af){
			af = af * fr->GetTransform();
			for(unsigned i=0; i<fr->NChildObject(); ++i){
				GRVisual* v = DCAST(GRVisual, fr->GetChildObject(i));
				GRFrame* f = DCAST(GRFrame, v);
				if (f) AddFrameToSolid(solid, f, af);
				GRMesh* m = DCAST(GRMesh, v);
				if (m){
					CDConvexMeshDesc mdesc;
					for(unsigned i=0; i<m->positions.size(); ++i){
						mdesc.vertices.push_back(m->positions[i]);
					}

					// 動摩擦摩擦係数mu、静止摩擦係数mu0 
					UTMapObject::iterator itr = fc->mapObj.find(m->GetIf());
					if (itr != fc->mapObj.end()){
							FWPHMaterialTask* maptask = DCAST(FWPHMaterialTask, itr->second);	
							mdesc.material.mu = maptask->mu;
							mdesc.material.mu0 = maptask->mu0;
					}
					solid->CreateShape(mdesc);
					Posed pose;
					pose.FromAffine(af);
					solid->SetShapePose(solid->NShape()-1, pose);
				}

				GRSphere* s = DCAST(GRSphere, v);
				if (s){
					CDSphereDesc sdesc;
					sdesc.radius = s->radius;

					// 動摩擦摩擦係数mu、静止摩擦係数mu0 
					UTMapObject::iterator itr = fc->mapObj.find(s->GetIf());
					if (itr != fc->mapObj.end()){
							FWPHMaterialTask* maptask = DCAST(FWPHMaterialTask, itr->second);	
							sdesc.material.mu = maptask->mu;
							sdesc.material.mu0 = maptask->mu0;
					}
					solid->CreateShape(sdesc);
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
		void Execute(UTLoadContext* fc){}
	};
	FWNodeHandlerContactEngine():UTLoadHandlerImp<Desc>("ContactEngine"){}
	void Load(Desc& d, UTLoadContext* fc){
		PHScene* ps = FindPHScene(fc);
		Disabler* dis = DBG_NEW Disabler;
		dis->phScene = FindPHScene(fc);
		fc->links.push_back(dis);
		Adapter* task = DBG_NEW Adapter;
		task->phScene = ps;
		task->fc = fc;
		fc->objects.Push(task->GetIf());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		Adapter* task = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(task);
		fc->objects.Pop();	//	task
	}
};

// Springhead1のSolid．
class FWNodeHandlerSolid: public UTLoadHandlerImp<Solid>{
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
		virtual void Execute(UTLoadContext* ctx){
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

	FWNodeHandlerSolid():UTLoadHandlerImp<Desc>("Solid"){}
	void Load(Desc& d, UTLoadContext* fc){
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
		task->fc = fc;
		fc->objects.Push(task->GetIf());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		Adapter* task = DCAST(Adapter, fc->objects.Top());
		fc->links.push_back(task);
		fc->objects.Pop();	//	task

		assert(DCAST(PHSolid, fc->objects.Top()));
		fc->objects.Pop();	//	solid
	}
};

// Springhead1のCamera．
class FWNodeHandlerCamera: public UTLoadHandlerImp<Camera>{
public:	
	FWNodeHandlerCamera():UTLoadHandlerImp<Desc>("Camera"){}
	void Load(Desc& d, UTLoadContext* fc){
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
	void Loaded(Desc& d, UTLoadContext* fc){
		fc->objects.Pop();
	}
};

// Springhead1のGravityEngine．
class FWNodeHandlerGravityEngine: public UTLoadHandlerImp<GravityEngine>{
public:	
	FWNodeHandlerGravityEngine():UTLoadHandlerImp<Desc>("GravityEngine"){}
	int linkPos;
	void Load(Desc& d, UTLoadContext* fc){
		PHScene* s = FindPHScene(fc);
		if (s) s->SetGravity(d.gravity);
		linkPos = (int)fc->links.size();
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		//	GravityEngineの子オブジェクトはとりあえず無視。本当はGravityのOn/Offに使う。
		fc->links.resize(linkPos);
	}
};

// Springhead1のScene．
class FWNodeHandlerScene: public UTLoadHandlerImp<Scene>{
public:	
	FWNodeHandlerScene():UTLoadHandlerImp<Desc>("Scene"){}
	void Load(Desc& d, UTLoadContext* fc){
		//	Frameworkを作る．
		FWSceneDesc fwsd;
		fc->PushCreateNode(FWSceneIf::GetIfInfoStatic(), &fwsd);
		FWScene* fws = DCAST(FWScene, fc->objects.Top());
		
		//	PHSDKを作る．スタックからは消す．
		PHSdkDesc phsdkd;
		fc->PushCreateNode(PHSdkIf::GetIfInfoStatic(), &phsdkd);
		PHSdkIf* phSdk = DCAST(PHSdkIf, fc->objects.Top());
		fc->objects.Pop();
		//	GRSDKを作る．スタックからは消す．
		GRSdkDesc grsdkd;
		fc->PushCreateNode(GRSdkIf::GetIfInfoStatic(), &grsdkd);
		GRSdkIf* grSdk = DCAST(GRSdkIf, fc->objects.Top());
		fc->objects.Pop();

		//	GRSceneを作る．
		GRSceneIf* grScene = grSdk->CreateScene();
		//	PHSceneを作る．
		PHSceneDesc psd;
		PHSceneIf* phScene = phSdk->CreateScene(psd);

		//	Frameworkにシーンを登録．
		fws->AddChildObject(phScene);
		fws->AddChildObject(grScene);

		// timeStepを設定．
		UTMapObject::iterator itr;
		for (itr=fc->mapObj.begin(); itr!=fc->mapObj.end(); ++itr){
			FWSimulatorTask* simtask = DCAST(FWSimulatorTask, itr->first);
			if (simtask){
				phScene->SetTimeStep(simtask->timeStep);
				itr->second = phScene;
			}
		}				
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		fc->objects.Pop();
	}
};

// Springhead1のSolidContainer．
class FWNodeHandlerSolidContainer: public UTLoadHandlerImp<SolidContainer>{
public:	
	FWNodeHandlerSolidContainer():UTLoadHandlerImp<Desc>("SolidContainer"){}
	void Load(Desc& d, UTLoadContext* fc){
	}
	void Loaded(Desc& d, UTLoadContext* fc){
	}
};

// Springhead1のJointEngine．
class FWNodeHandlerJointEngine: public UTLoadHandlerImp<JointEngine>{
public:	
	class JointCreator: public UTLoadTask{
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
		void Execute(UTLoadContext* fc){
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

	FWNodeHandlerJointEngine():UTLoadHandlerImp<Desc>("JointEngine"){}
	void Load(Desc& d, UTLoadContext* fc){
		JointCreator* j = DBG_NEW JointCreator;
		j->phScene = FindPHScene(fc);
		fc->objects.Push(j->GetIf());
	}
	void Loaded(Desc& d, UTLoadContext* fc){
		JointCreator* j = DCAST(JointCreator, fc->objects.Top());
		fc->links.push_back(j);
		fc->objects.Pop();
	}
};

// Springhead1のJoint．
class FWNodeHandlerJoint: public UTLoadHandlerImp<Joint>{
public:	
	typedef FWNodeHandlerJointEngine::JointCreator JointCreator;
	FWNodeHandlerJoint():UTLoadHandlerImp<Desc>("Joint"){}
	void Load(Desc& d, UTLoadContext* fc){
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
	void Loaded(Desc& d, UTLoadContext* fc){
		JointCreator* j = DCAST(JointCreator, fc->objects.Top());
		fc->links.push_back(j);
		fc->objects.Pop();
	}
};

// Springhead1のImport．
class FWNodeHandlerImport: public UTLoadHandlerImp<Import>{
public:	
	FWNodeHandlerImport():UTLoadHandlerImp<Desc>("Import"){}
	void Load(Desc& d, UTLoadContext* fc){
		UTString filename = d.file;

		UTPath xFilePath;
		xFilePath.Path(fc->fileMaps.back()->name);
		xFilePath.Path(xFilePath.FullPath());
		UTPath imPath;
		imPath.Path(filename);
        UTString oldCwd = imPath.GetCwd();
		imPath.SetCwd(xFilePath.Drive() + xFilePath.Dir());
		UTString full = imPath.FullPath();		


		UTRef<FISdkIf> sdk = FISdk::CreateSdk();
		fc->PushFileMap(full);
		if (fc->IsGood()){
			UTRef<FIFileX> fileX = DCAST(FIFileX, sdk->CreateFileX());
			fileX->LoadImp((FILoadContext*)fc);
		}
		fc->fileMaps.Pop();
	}
	void Loaded(Desc& d, UTLoadContext* fc){
	}
};

}


// ハンドラ定義
namespace Spr{
using namespace SprOldSpringhead;
void SPR_CDECL FWRegisterOldSpringheadNode(){
	UTLoadHandlers* handlers = UTLoadHandlerDb::GetHandlers("OldSpringhead");
	handlers->insert(DBG_NEW FWNodeHandlerXHeader);
	handlers->insert(DBG_NEW FWNodeHandlerXFrame);
	handlers->insert(DBG_NEW FWNodeHandlerXFrameTransformMatrix);
	handlers->insert(DBG_NEW FWNodeHandlerXLight8);
	handlers->insert(DBG_NEW FWNodeHandlerXTextureFilename);
	handlers->insert(DBG_NEW FWNodeHandlerXMaterial);		
	handlers->insert(DBG_NEW FWNodeHandlerPhysicalMaterial);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshMaterialList);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshNormals);
	handlers->insert(DBG_NEW FWNodeHandlerXMeshTextureCoords);	
	handlers->insert(DBG_NEW FWNodeHandlerXMesh);
	handlers->insert(DBG_NEW FWNodeHandlerSphere);
	handlers->insert(DBG_NEW FWNodeHandlerSolid);
	handlers->insert(DBG_NEW FWNodeHandlerScene);
	handlers->insert(DBG_NEW FWNodeHandlerSimulator);
	handlers->insert(DBG_NEW FWNodeHandlerCamera);
	handlers->insert(DBG_NEW FWNodeHandlerSolidContainer);
	handlers->insert(DBG_NEW FWNodeHandlerGravityEngine);
	handlers->insert(DBG_NEW FWNodeHandlerContactEngine);
	handlers->insert(DBG_NEW FWNodeHandlerJointEngine);
	handlers->insert(DBG_NEW FWNodeHandlerJoint);
	handlers->insert(DBG_NEW FWNodeHandlerImport);
}
}
