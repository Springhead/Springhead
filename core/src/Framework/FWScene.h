/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FWSCENE_H
#define FWSCENE_H

#include <Framework/SprFWScene.h>
#include <Framework/SprFWObject.h>
#include <Framework/SprFWHapticPointer.h>
#include <Framework/SprFWOpHapticHandler.h>
#include <Framework/SprFWSkeletonSensor.h>
#include <Framework/FWSkeletonSensor.h>
#include <Physics/SprPHEngine.h>
#include <Foundation/Scene.h>
#include <HumanInterface/HIBase.h>
#include <Physics/PHSolid.h>
#include <Framework/FWOpHapticHandler.h>
#include <Physics/PHContactDetector.h>


namespace Spr{;

struct CDConvexMeshIf;
struct CDFaceIf;
struct PHSceneIf;
struct GRSceneIf;

class FWSdk;

///	FWSceneのファクトリ
class FWSceneFactory : public FactoryBase {
public:
	const IfInfo* GetIfInfo() const {
		return FWSceneIf::GetIfInfoStatic();
	}
	ObjectIf* Create(const void* desc, const ObjectIf*){
		return CreateFWScene();
	}
};

///	GraphicsとPhysicsのSceneをまとめたシーングラフ全体
class FWScene : public Scene, public FWSceneDesc {
public:
	struct Mat {
		Mat(int m=-1, float a=1):mat(m), alpha(a){}
		int mat;
		float alpha;
	};

	SPR_OBJECTDEF(FWScene);
	ACCESS_DESC(FWScene);

	typedef std::vector< UTRef<FWObjectIf> >		FWObjects;
	typedef std::vector< UTRef<HIHaptic> >			FWHumanInterfaces;
	typedef std::vector< UTRef<FWHapticPointerIf> > FWHapticPointers;
	
	FWSdk*				sdk;						///<	親SDKへの参照
	FWObjects			fwObjects;					///<	物理とグラフィックスのオブジェクトのリンク
	FWHapticPointers	fwHapticPointers;			///<	登録されている力覚ポインタへのリンク
	FWOpHapticHandler*   fwOpHapticHandler;			///<	OPに使う力覚ポインタへのハンドラー
	PHSceneIf*			phScene;					///<	物理シミュレーション用のシーン
	GRSceneIf*			grScene;					///<	グラフィックス用のシーン

	/// 描画制御フラグ
	std::map<ObjectIf*, bool>	renderObject;
	bool		renderPHScene;
	bool		renderGRScene;
	bool		renderSolid, renderWire;
	bool		renderAxisWorld, renderAxisSolid, renderAxisConst;
	bool		renderForceSolid, renderForceConst;
	bool		renderContact;
	bool		renderBBox;
	bool		renderGridX, renderGridY, renderGridZ;
	bool		renderLimit;
	bool		renderIK;
	bool		renderHaptic;
	bool		renderFEM;
	bool		renderSkeletonSensor;
	bool		renderOp;

	/// 描画スケール
	float		scaleAxisWorld, scaleAxisSolid, scaleAxisConst;
	float		scaleForce, scaleMoment;
	float		scaleIK;

	/// 描画マテリアル
	std::map<PHSolidIf*, Mat>	matSolid, matWire;
	Vec3i		matAxis;
	int			matForce, matMoment;
	int			matContact;
	int			matCoP[3];
	int			matBBoxLocal;
	int			matBBoxWorld;
	Vec3i		matGrid;
	int			matIK;
	
	/// 座標軸設定
	int			axisStyle;
	
	/// グリッド設定
	Vec3f		gridOffset;
	Vec3f		gridSize;
	Vec3i		gridSlice;

	//	hase TBW
	//	FWHumanInterfaces humanInterfaces;			///<	ユーザインタフェース．[0]がカメラ．HIForceDevice6Dのクラス名は変更する予定．
	enum HumanInterfacePurposeId{
		HI_CAMERACONTROLLER,
		HI_OBJECTMANIPULATOR0,
		HI_OBJECTMANIPULATOR1,
		HI_OBJECTMANIPULATOR2,
		HI_OBJECTMANIPULATOR3,
	};

public:

	/// コンストラクタ
	FWScene(const FWSceneDesc& d=FWSceneDesc());
	
	/** 基本機能
	 */
	FWSdkIf*		GetSdk();
	PHSceneIf*		GetPHScene(){ return phScene; }
	void			SetPHScene(PHSceneIf* s){ phScene = s; }
	GRSceneIf*		GetGRScene(){ return grScene; }
	void			SetGRScene(GRSceneIf* s){ grScene = s; }
	FWObjectIf*		CreateFWObject();
	int				NObject()const{ return (int)fwObjects.size(); }
	FWObjectIf*		GetObject(int i){ return fwObjects[i]; }
	FWObjectIf**	GetObjects(){ return (FWObjectIf**)&*fwObjects.begin(); }

	void		Sync();
	void		Step();

	/** 描画機能
	 */
	void	Draw			(GRRenderIf* render);
	void	Draw			(GRRenderIf* render, bool ph_or_gr);
	void	DrawPHScene		(GRRenderIf* render);
	void	DrawSolid		(GRRenderIf* render, PHSolidIf* solid, bool solid_or_wire);
	void    DrawBBox        (GRRenderIf* render, Posed pose, PHBBox* bbox);
	void	DrawShape		(GRRenderIf* render, CDShapeIf* shape, bool solid_or_wire);
	void	DrawConstraint	(GRRenderIf* render, PHConstraintIf* con);
	void	DrawContact		(GRRenderIf* render, PHContactPointIf* con);
	void	DrawContactSafe	(GRRenderIf* render, PHConstraintEngineIf* ce);
	void	DrawIK			(GRRenderIf* render, PHIKEngineIf* ikEngine);
	void	DrawLimit		(GRRenderIf* render, PHConstraintIf* con);
	void	DrawHaptic		(GRRenderIf* render, PHHapticEngineIf* hapticEngine);
	void	DrawFem			(GRRenderIf* render, PHFemEngineIf* femEngine);
	void	DrawOp			(GRRenderIf* render, PHOpEngineIf* opEngineif);
	void	DrawSkeletonSensor(GRRenderIf* render);
	
	/// 描画制御
	void	EnableRenderPHScene(bool enable = true);
	void	EnableRenderGRScene(bool enable = true);

	void	SetRenderMode		(bool solid, bool wire);
	void	EnableRender		(ObjectIf* obj, bool enable);
	void	SetSolidMaterial(int mat, PHSolidIf* solid);
	void	SetSolidMaterial(int mat, float alpha, PHSolidIf* solid);
	void	SetWireMaterial(int mat, PHSolidIf* solid);
	void	SetWireMaterial(int mat, float alpha, PHSolidIf* solid);

	void	EnableRenderAxis	(bool world, bool solid, bool con);
	void	SetAxisMaterial		(int matX, int matY, int matZ);
	void	SetAxisScale		(float scaleWorld, float scaleSolid, float scaleConst);
	void	SetAxisStyle		(int style);

	void	EnableRenderForce	(bool solid, bool con);
	void	SetForceMaterial	(int matf, int matm);
	void	SetForceScale		(float scalef, float scalem);

	void	EnableRenderContact	(bool enable);
	void	SetContactMaterial	(int mat);

	void    EnableRenderBBox    (bool enable);
	void    SetLocalBBoxMaterial(int mat);
	void    SetWorldBBoxMaterial(int mat);

	void	EnableRenderGrid	(bool x, bool y, bool z);
	void	SetGridOption		(char axis, float offset, float size, int slice);
	void	SetGridMaterial		(int matX, int matY, int matZ);

	void	EnableRenderIK		(bool enable);
	void	SetIKMaterial		(int mat);
	void	SetIKScale			(float scale);

	void	EnableRenderLimit	(bool enable);

	void	EnableRenderHaptic	(bool enable);

	void	EnableRenderFem		(bool enable);

	void	EnableRenderSkeletonSensor(bool enable);

	void	EnableRenderOp		(bool enable);

	/// 内部用
	void	DrawCoordinateAxis	(GRRenderIf* render, float scale, bool solid_or_wire);
	void	DrawMesh			(GRRenderIf* render, CDConvexMeshIf* mesh, bool solid_or_wire);
	void	DrawFaceSolid		(GRRenderIf* render, CDFaceIf* face, Vec3f * base);
	void	DrawFaceWire		(GRRenderIf* render, CDFaceIf* face, Vec3f * base);
	void	DrawForce			(GRRenderIf* render, const Vec3d& f, const Vec3d& t);
	void 	DrawContactEngine	(GRRenderIf* render, PHContactEngineIf* ce);
	bool	IsRenderEnabled		(ObjectIf* obj);
	Mat		GetSolidMaterial	(PHSolidIf* solid);
	Mat		GetWireMaterial		(PHSolidIf* solid);
	Mat		GetAutoMaterial		(int i);
	
	/** HumanInterface系関数
	 */
	HIForceDevice6D*	GetHumanInterface(size_t pos);
	void				AddHumanInterface(HIForceDevice6D* d);
	FWHapticPointerIf*	CreateHapticPointer();
	FWHapticPointerIf*	GetHapticPointer(int i);
	FWOpHapticHandlerIf* CreateOpHapticHandler();
	FWOpHapticHandlerIf* GetOpHapticHandler();
	int					NHapticPointers();
	void				UpdateHapticPointers();
	void				UpdateOpHapticHandler();

	/** SkeletonSensor系関数
	 */
	std::vector< UTRef<FWSkeletonSensor> >  skeletonSensors;
	FWSkeletonSensorIf* CreateSkeletonSensor(const FWSkeletonSensorDesc& desc) {
		FWSkeletonSensor* sensor = DBG_NEW FWSkeletonSensor();
		sensor->SetDesc(&desc);
		skeletonSensors.push_back(sensor);
		sensor->SetScene(this->Cast());
		return sensor->Cast();
	}
	void UpdateSkeletonSensors() {
		for (int i=0; i<(int)skeletonSensors.size(); ++i) {
			skeletonSensors[i]->Update();
		}
	}

	/** Scene関数の継承
	 */
	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual NamedObjectIf* FindObject(UTString name, UTString cls);
	
};


}

#endif
