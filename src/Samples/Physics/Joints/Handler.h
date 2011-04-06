#ifndef JOINTS_HANDLER_H
#define JOINTS_HANDLER_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>

using namespace Spr;

/// シーンの構築とインタラクションの基本クラス
class Handler : public UTRefCount{
public:
	bool		bReady;
	PHSolidDesc sdFloor;					///< 床剛体のディスクリプタ
	PHSolidDesc sdBox;						///< 箱剛体のディスクリプタ
	CDShapeIf* shapeBox;					///< 箱形状
	CDShapeIf* shapeSphere;					///< 球形状

	PHSolidIf* soFloor;						///< 床剛体のインタフェース
	std::vector<PHSolidIf*> soBox;			///< 箱剛体のインタフェース
	std::vector<PHJointIf*> jntLink;		///< 関節のインタフェース
	std::vector<PHTreeNodeIf*> nodeTree;	///< ABA（関節座標シミュレーション）のためのツリーノード

	PHSdkIf* GetPHSdk(){ return FWApp::instance->GetSdk()->GetPHSdk(); }
	PHSceneIf* GetPHScene(){ return FWApp::instance->GetSdk()->GetScene()->GetPHScene(); }

public:
	Handler(){ bReady = false; }
	/// 共用のシーンセットアップ処理
	void BuildCommon(bool floor = true, bool rootBox = true, bool gravity = true){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		// 床
		if(floor){
			CDBoxDesc desc(Vec3d(50, 1, 50));
			soFloor = phScene->CreateSolid(sdFloor);
			soFloor->AddShape(phSdk->CreateShape(desc));
			soFloor->SetFramePosition(Vec3d(0,-2,0));
			soFloor->SetDynamical(false);			// 外力によって動かないようにする
		}

		//鎖の根になる箱
		if(rootBox){
			CDBoxDesc bd(Vec3d(2.0, 2.0, 2.0));
			shapeBox = phSdk->CreateShape(bd);
			shapeBox->SetName("shapeBox");
			PHSolidIf* s = phScene->CreateSolid(sdBox);
			soBox.push_back(s);
			Posed pose;
			s->AddShape(shapeBox);

			s->AddShape(shapeBox);
			pose.Pos() = Vec3d(2.1, 0.0, 0.0);
			s->SetShapePose(1, pose);

			s->AddShape(shapeBox);
			pose.Pos() = Vec3d(0.0, 2.1, 0.0);
			s->SetShapePose(2, pose);

			s->AddShape(shapeBox);
			pose.Pos() = Vec3d(0.0, 0.0, 2.1);
			s->SetShapePose(3, pose);
			
			//空中に固定する
			soBox.back()->SetFramePosition(Vec3d(0.0, 20.0, 0.0));
			soBox.back()->SetDynamical(false);	
			nodeTree.push_back(phScene->CreateRootNode(soBox.back(), PHRootNodeDesc()));
		}

		// 重力を設定
		if(gravity){
			phScene->SetGravity(Vec3d(0, -9.8, 0));
		}
	}

	/// クリア
	virtual void Clear(){
		soFloor = NULL;
		soBox.clear();
		jntLink.clear();
		nodeTree.clear();
		bReady = false;
	}
	virtual void Build(){ bReady = true; }
	virtual double GetSceneRadius(){ return 20.0; }	///< シーンの大体の大きさ（カメラ初期化用）
	virtual void OnTimer(){}
	
	/// 共通のキー入力処理
	virtual void OnKey(int key){
		if(key == 'x'){
			static bool bEnable = true;
			bEnable = !bEnable;
			for(int i = 0; i < (int)jntLink.size(); i++)
				jntLink[i]->Enable(bEnable);
		}
		/// 物体を投げる:
		/// 箱
		else if(key == 'z'){
			soBox.push_back(GetPHScene()->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(15.0, 15.0, 0.0));
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(1), 'z'));
			soBox.back()->SetVelocity(Vec3d(-5.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
		}
		else if(key == 'Z'){
			soBox.push_back(GetPHScene()->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(5.0, 13.0, 5.0));
			soBox.back()->SetVelocity(Vec3d(0.0, 0.0, -20.0));
			soBox.back()->SetMass(2.0);
		}
		/// カプセル
		else if(key == 'C'){
			CDCapsuleDesc cd;
			cd.radius = 1.0;
			cd.length = 4.0;
			CDShapeIf* shape = GetPHSdk()->CreateShape(cd);

			soBox.push_back(GetPHScene()->CreateSolid(sdBox));
			soBox.back()->AddShape(shape);
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(90), 'y'));
			soBox.back()->SetFramePosition(Vec3d(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-3.0, 0.0, 0.0));
			soBox.back()->SetAngularVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox.back()->SetMass(2.0);
		}
		/// 球
		else if(key == 'S'){
			CDSphereDesc sd;
			sd.radius = 1.0;
			CDShapeIf* shape = GetPHSdk()->CreateShape(sd);

			soBox.push_back(GetPHScene()->CreateSolid(sdBox));
			soBox.back()->AddShape(shape);
			soBox.back()->SetOrientation(Quaterniond::Rot(Rad(90), 'y'));
			soBox.back()->SetFramePosition(Vec3d(15.0, 15.0, 0.0));
			soBox.back()->SetVelocity(Vec3d(-3.0, 0.0, 0.0));
			soBox.back()->SetMass(2.0);
		}
		// dtを半分にする
		else if(key == '.'){
			double dt = GetPHScene()->GetTimeStep();
			dt /= 2.0;
			dt = std::max(0.001, dt);
			GetPHScene()->SetTimeStep(dt);
		}
		// dtを倍にする
		else if(key == ','){
			double dt = GetPHScene()->GetTimeStep();
			dt *= 2.0;
			GetPHScene()->SetTimeStep(dt);
		}
	}
};

#endif