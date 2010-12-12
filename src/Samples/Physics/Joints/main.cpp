/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/** \page pageJoints 関節機能のデモ
 Springhead2/src/Samples/Joints/main.cpp

\ref pagePhysics の関節機能のデモプログラム。

\section secTestPatternJoints テストパターン
- シーン0：鎖
- シーン1：チェビシェフリンク
- シーン2：バネダンパ入りジョイントとバネダンパ
- シーン3：ボールジョイント1つのテスト
- シーン4：パスジョイント
- シーン5：バネダンパ（安定なバネダンパの確認デモ） 


\section secSpecJoints 仕様
- 共通の操作：
 - シーン番号0, 1, ...をタイプすると対応するシーンに切り替わる（デフォルト：シーン0)
  - 'x'をタイプすると関節の有効/無効が切り替わる
  - 'z'で、右方向からボックスを飛ばし、衝突させる		
  - 'Z'で、手前方向からボックスを飛ばし、衝突させる
  - 'C'で、右方向からカプセルを飛ばし、衝突させる
  - 'S'で、右方向から球を飛ばし、衝突させる
  - 'P'で、シミュレーションを止める
- シーン0： 鎖
  - '0'で、ヒンジシーンの設定を行う（シーン切換え）
  - ' 'あるいは'b'でヒンジ用のboxを生成
		　' 'の場合は内部アルゴリズムはABAとなる
- シーン1：
  - '1'で、チェビシェフリンクを生成する（シーン切換え）
  - 'a'で、モータートルクを1.0に設定する
  - 's'で、モータートルクを0.0に設定する
  - 'd'で、モータートルクを-1.0に設定する
  - 'f'で、目標速度を90.0に設定する
  - 'g'で、目標速度を0.0に設定する
  - 'h'で、目標速度を-90.0に設定する
  - 'j'で、バネ原点(バネの力が0となる関節変位)を1.0に設定する
  - 'k'で、バネ原点(バネの力が0となる関節変位)を0.0に設定する
  - 'l'で、バネ原点(バネの力が0となる関節変位)を-1.0に設定する
- シーン4：
  - '4'で、パスジョイントシーンの設定を行う（シーン切換え）
  - 'a'で、重力方向を、(0.0, -9.8, 0.0)に設定する
  - 'd'で、重力方向を、(0.0,  9.8, 0.0)に設定する
  - 
- シーン5：
  - '5'で、バネ・ダンパシーンの設定を行う（シーン切換え）
  - 'a'で、剛体のフレーム原点を、位置(-20.0, 30.0, 0.0)に設定する
  - 's'で、剛体のフレーム原点を、位置(-10.0, 20.0, 0.0)に設定する
  - 'd'で、剛体のフレーム原点を、位置( -5.0, 10.0, 0.0)に設定する
  - 'f'で、剛体のフレーム原点を、位置(  0.0, 10.0, 0.0)に設定する
  - 'g'で、剛体のフレーム原点を、位置(  5.0, 10.0, 0.0)に設定する
  - 'h'で、剛体のフレーム原点を、位置( 10.0, 20.0, 0.0)に設定する
  - 'j'で、剛体のフレーム原点を、位置( 20.0, 30.0, 0.0)に設定する
  - '.'で、コールバック関数の呼び出し時間を1/2倍する
  - ','で、コールバック関数の呼び出し時間を2倍する
  - 'n'で、バネ原点を+0.01する
  - 'm'で、バネ原点を-0.01する
*/

#include <ctime>
#include <string>
#include <sstream>
#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <Framework/SprFWEditor.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
using namespace Spr;
using namespace std;

#define ESC		27

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


class ChainHandler : public Handler{
public:
	virtual void Build(){	
		BuildCommon();
		Handler::Build();
	}
	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();
		if(key == 'h'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10, 10, 0));
			PHHingeJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d( 1.1,  -1.1,  0);
			jdesc.posePlug.Pos() = Vec3d(-1.1, 1.1,  0);
			jdesc.lower = Rad(-30.0);
			jdesc.upper = Rad( 30.0);
			jdesc.damper = 2.0;
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
		}
		else if(key == 'b'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));
			PHBallJointDesc jdesc;
			//jdesc.limit[1].upper =  0.2;	// 最大スイング角
			//jdesc.limit[2].lower= -0.2;	// ツイスト角範囲
			//jdesc.limit[2].upper =  0.2;
			jdesc.poseSocket.Pos() = Vec3d(-1.01, -1.01, -1.01);
			jdesc.posePlug.Pos() = Vec3d(1.01, 1.01, 1.01);
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
			if(key == ' ')
				nodeTree.push_back(phScene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
			//phScene->SetContactMode(PHSceneDesc::MODE_NONE);
		}
		else if(key == 's'){
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));
			PHSliderJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d(0, -1.1, 0);
			jdesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			jdesc.posePlug.Pos() = Vec3d(0,  1.1, 0);
			jdesc.posePlug.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], jdesc));
			PHSliderJointIf* slider = DCAST(PHSliderJointIf, jntLink.back());
			slider->SetRange(-0.3, 0.3);
			//slider->SetSpring(1000.0);
			//slider->SetDamper(300);
			//slider->SetTargetPosition(0.0);
		}
		else if(key == 't'){
			// ツリーノードを作成し，ABAで計算するように指定
			size_t n = soBox.size();
			nodeTree.push_back(phScene->CreateTreeNode(nodeTree.back(), soBox[n-1], PHTreeNodeDesc()));
		}
		else if(key == 'g'){
			// 以下はギアの作成コード
			/*if(jntLink.size() >= 2){
				size_t m = jntLink.size();
				PHGearDesc gdesc;
				gdesc.ratio = 1.0;
				phScene->CreateGear(DCAST(PHJoint1DIf, jntLink[m-2]), DCAST(PHJoint1DIf, jntLink[m-1]), gdesc);
			}*/
		}
	}
};

class LinkHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon(true, false, true);
		
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		CDBoxDesc bd;
		soBox.resize(3);
		bd.boxsize = Vec3d(1.0, 2.0, 1.0);
		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(phSdk->CreateShape(bd));
		soBox[0]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));
		
		bd.boxsize = Vec3d(1.0, 5.0, 1.0);
		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(phSdk->CreateShape(bd));
		soBox[1]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));

		bd.boxsize = Vec3d(1.0, 10.0, 1.0);
		soBox[2] = phScene->CreateSolid(sdBox);
		soBox[2]->AddShape(phSdk->CreateShape(bd));
		soBox[2]->SetFramePosition(Vec3d(0.0, 20.0, 0.0));

		PHHingeJointDesc jd;
		jntLink.resize(4);
		jd.poseSocket.Pos() = Vec3d(0.0, 10.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -1.0, 0.0);
		jntLink[0] = phScene->CreateJoint(soFloor, soBox[0], jd);

		jd.poseSocket.Pos() = Vec3d(4.0, 10.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -2.5, 0.0);
		jntLink[1] = phScene->CreateJoint(soFloor, soBox[1], jd);

		jd.poseSocket.Pos() = Vec3d(0.0, 1.0, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, -5.0, 0.0);
		jntLink[2] = phScene->CreateJoint(soBox[0], soBox[2], jd);

		jd.poseSocket.Pos() = Vec3d(0.0, 2.5, 0.0);
		jd.posePlug.Pos() = Vec3d(0.0, 0.0, 0.0);
		jntLink[3] = phScene->CreateJoint(soBox[1], soBox[2], jd);

		// 以下を有効化するとABAが機能し、閉リンクを構成するための1関節のみLCPで解かれる
		nodeTree.push_back(phScene->CreateRootNode(soFloor, PHRootNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[0], soBox[0], PHTreeNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[1], soBox[2], PHTreeNodeDesc()));
		nodeTree.push_back(phScene->CreateTreeNode(nodeTree[0], soBox[1], PHTreeNodeDesc()));

		phScene->SetContactMode(&soBox[0], 3, PHSceneDesc::MODE_NONE);
		//phScene->SetContactMode(PHSceneDesc::MODE_NONE);

		Handler::Build();
	}
	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		const double K = 30.0;
		const double B = 10.0;
		PHHingeJointIf* hinge = DCAST(PHHingeJointIf, jntLink[0]);
		//PHPathJointIf* path = (jntLink.size() == 5 ? DCAST(PHPathJointIf, jntLink[4]) : NULL); 
		switch(key){
		case 'a': hinge->SetMotorTorque(1.0);	break;
		case 's': hinge->SetMotorTorque(0.0);	break;
		case 'd': hinge->SetMotorTorque(-1.0);	break;
		case 'f':
			hinge->SetTargetVelocity(Rad(180));
			//if(path)
			//	path->SetTargetVelocity(Rad(90.0));
			break;
		case 'g':
			hinge->SetTargetVelocity(Rad(0.0));
			//if(path)
			//	path->SetTargetVelocity(Rad(0.0));
			break;
		case 'h': hinge->SetTargetVelocity(Rad(-90.0));	break;
		case 'j':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(1.0);
			hinge->SetDamper(B);
			break;
		case 'k':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(0.0);
			hinge->SetDamper(B);
			break;
		case 'l':
			hinge->SetSpring(K);
			hinge->SetTargetPosition(-1.0);
			hinge->SetDamper(B);
			break;
		case 'c':{
			//チェビシェフリンク一周分の軌跡を記憶させてパスジョイントを作成
			//	2010.12.12 コメントを外してみたところ落ちる。未完？	by hase
			PHPathDesc descPath;
			descPath.bLoop = true;
			PHPathIf* trajectory = phScene->CreatePath(descPath);

			hinge->SetSpring(K);
			hinge->SetDamper(B);
			double theta = -Rad(180.0);
			hinge->SetTargetPosition(theta);
			for(int i = 0; i < 50; i++)
				phScene->Step();
			for(; theta < Rad(180.0); theta += Rad(2.0)){
				hinge->SetTargetPosition(theta);
				for(int i = 0; i < 5; i++)
					phScene->Step();
				Posed pose = soFloor->GetPose().Inv() * soBox[2]->GetPose();
				//pose.Ori() = Quaterniond();
				trajectory->AddPoint(theta, pose);
			}
		
			soBox.resize(4);
			soBox[3] = phScene->CreateSolid(sdBox);
			soBox[3]->AddShape(soBox[2]->GetShape(0));
			soBox[3]->SetFramePosition(Vec3d(10.0, 20.0, 0.0));

			PHPathJointDesc descJoint;
			descJoint.posePlug.Pos().x = 15.0;
			jntLink.resize(5);
			jntLink[4] = phScene->CreateJoint(soFloor, soBox[3], descJoint);
			PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[4]);
			joint->AddChildObject(trajectory);
			joint->SetPosition(0);
		
			}break;
		}
	}
};


/*
	PathJointのデモだが、誤差補正が上手く行っていないように見える。
	床にぶつかると発散してしまう。	2010.12.12 by hase	*/
class PathHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon();

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		PHPathDesc desc;
		PHPathIf* path = phScene->CreatePath(desc);
		double s;
		double radius = 5.0;
		double pitch = 4.0;
		Posed pose;
		for(s = 0.0; s < 4 * (2 * M_PI); s += Rad(1.0)){
			double stmp = s;
			while(stmp > M_PI) stmp -= 2 * M_PI;
			pose.Pos() = Vec3d(radius * cos(stmp), 5.0 + pitch * s / (2 * M_PI), radius * sin(stmp));
			pose.Ori().FromMatrix(Matrix3d::Rot(-stmp, 'y'));
			path->AddPoint(s, pose);
		}
		soBox[0]->SetDynamical(true);
		PHPathJointDesc descJoint;
		jntLink.push_back(phScene->CreateJoint(soFloor, soBox[0], descJoint));
		PHPathJointIf* joint = DCAST(PHPathJointIf, jntLink[0]);
		joint->AddChildObject(path);
		joint->SetPosition(2 * 2 * M_PI);
		PHTreeNodeIf* node = phScene->CreateRootNode(soFloor, PHRootNodeDesc());
		phScene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());

		Handler::Build();
	}
};

class ArmHandler : public Handler{
public:
	/** true	-> 外部で手先をPD制御
		false	-> バネ拘束でPD制御
	 */
	bool bExplicit;

	virtual void Build(){
		BuildCommon(true, false, true);

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();
		
		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);
		
		soBox.resize(6);

		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(shapeBox);

		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(shapeBox);

		soBox[2] = phScene->CreateSolid(sdBox);
		soBox[2]->AddShape(shapeBox);

		soBox[3] = phScene->CreateSolid(sdBox);
		soBox[3]->AddShape(shapeBox);

		soBox[4] = phScene->CreateSolid(sdBox);
		soBox[4]->AddShape(shapeBox);

		CDSphereDesc descSphere;
		descSphere.radius = 1.0;
		soBox[5] = phScene->CreateSolid(sdBox);
		soBox[5]->AddShape(phSdk->CreateShape(descSphere));

		jntLink.resize(6);
		PHHingeJointDesc descHinge;
		descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
		jntLink[0] = phScene->CreateJoint(soFloor, soBox[0], descHinge);

		descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
		jntLink[1] = phScene->CreateJoint(soBox[0], soBox[1], descHinge);
		jntLink[2] = phScene->CreateJoint(soBox[1], soBox[2], descHinge);
		jntLink[3] = phScene->CreateJoint(soBox[2], soBox[3], descHinge);
		jntLink[4] = phScene->CreateJoint(soBox[3], soBox[4], descHinge);

		//以下を有効化すると鎖がABAで計算されて関節のドリフトが防がれる．
		/*
		PHTreeNodeIf* node = phScene->CreateRootNode(soFloor, PHRootNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[0], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[1], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[2], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[3], PHTreeNodeDesc());
		node = phScene->CreateTreeNode(node, soBox[4], PHTreeNodeDesc());
		*/

		double K = 2000, D = 100;
		//double K = 100000, D = 10000;	
		DCAST(PHHingeJointIf, jntLink[0])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[0])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[1])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[1])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[2])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[2])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[3])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[3])->SetDamper(D);
		DCAST(PHHingeJointIf, jntLink[4])->SetSpring(K);
		DCAST(PHHingeJointIf, jntLink[4])->SetDamper(D);

		bExplicit = false;	
		if(!bExplicit){
			PHSpringDesc descSpring;
			descSpring.spring = 500 * Vec3d(1,1,1);
			descSpring.damper = 100 * Vec3d(1,1,1);
			jntLink[5] = phScene->CreateJoint(soBox[4], soBox[5], descSpring);
		}

		soBox[5]->SetFramePosition(Vec3d(10.0, 5.0, 1.0));
		soBox[5]->SetDynamical(false);
		
		phScene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る

		Handler::Build();
	}

	virtual void OnKey(int key){
		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		switch(key){
		/// 目標物体の位置変更
		case 'a': soBox[5]->SetFramePosition(Vec3d(-20.0, 30.0, 0.0)); break;
		case 's': soBox[5]->SetFramePosition(Vec3d(-10.0, 20.0, 0.0)); break;
		case 'd': soBox[5]->SetFramePosition(Vec3d( -5.0, 10.0, 0.0)); break;
		case 'f': soBox[5]->SetFramePosition(Vec3d(  0.0, 10.0, 0.0)); break;
		case 'g': soBox[5]->SetFramePosition(Vec3d(  5.0, 10.0, 0.0)); break;
		case 'h': soBox[5]->SetFramePosition(Vec3d( 10.0, 20.0, 0.0)); break;
		case 'j': soBox[5]->SetFramePosition(Vec3d( 20.0, 30.0, 0.0)); break;
		/*case ' ':{
			//	剛体追加
			soBox.push_back(phScene->CreateSolid(sdBox));
			soBox.back()->AddShape(shapeBox);
			soBox.back()->SetFramePosition(Vec3d(10.0, 10.0, 0.0));

			//	ジョイント作成
			PHHingeJointDesc descHinge;
			descHinge.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
			descHinge.posePlug.Pos() = Vec3d(0.0, -3.0, 0.0);
			size_t n = soBox.size();
			jntLink.push_back(phScene->CreateJoint(soBox[n-2], soBox[n-1], descHinge));
			DCAST(PHHingeJointIf, jntLink.back())->SetSpring(K6);
			DCAST(PHHingeJointIf, jntLink.back())->SetDamper(D6);

			phScene->SetContactMode(PHSceneDesc::MODE_NONE);	// 接触を切る
			}break;*/
		case 'n':
		//	goal += 0.01;
			for(unsigned i=0; i<jntLink.size(); ++i){
				PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
				//if (j) j->SetTargetPosition(goal);
			}
			break;
		case 'm':
			//goal -= 0.01;
			for(unsigned i=0; i<jntLink.size(); ++i){
				PHHingeJointIf* j = DCAST(PHHingeJointIf, jntLink[i]);
				//if (j) j->SetTargetPosition(goal);
			}
			break;
		}
	}

	virtual void OnTimer(){
		if(bExplicit){
			Vec3d dVel = Vec3d() - soBox[4]->GetVelocity();
			Vec3d dPos = soBox[5]->GetPose().Pos() - soBox[4]->GetPose().Pos();
			static float K = 500;
			static float B = 15;
			Vec3d force = K*dPos + B*dVel;
			soBox[4]->AddForce(force, soBox[4]->GetPose()*Vec3d(0,3,0));
		}
	}
};

/// PHBallJointのデモ
class BallJointHandler : public Handler{
public:
	virtual void Build(){
		BuildCommon();

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		//bd.boxsize = Vec3f(6.0, 2.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);

		soBox.resize(2);
		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(shapeBox);
		soBox[0]->SetDynamical(false);
		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(shapeBox);
		phScene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);
		phScene->SetGravity(Vec3d());

		jntLink.resize(1);
		PHBallJointDesc desc;
		desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(-90), 'x');
		desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(-90), 'x');
		/*
		desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		*/

		desc.spring			  = 1000;
		desc.damper			  = 50;
		desc.limitSwing[0]	  = Rad(  0); // swing lower
		desc.limitSwing[1]	  = Rad( 20); // swing upper
		desc.limitTwist[0]	  = Rad(-20); // twist lower
		desc.limitTwist[1]	  = Rad( 20); // twist upper
		jntLink[0] = phScene->CreateJoint(soBox[0], soBox[1], desc);
		
		Handler::Build();
	}

	virtual void OnKey(int key){
		PHSceneIf* phScene = GetPHScene();
		PHBallJointDesc ballDesc;
		PHBallJointIf* joint = DCAST(PHBallJointIf, phScene->GetJoint(0));
		
		joint->GetDesc(&ballDesc);
		
/*		if(key == 'a')		ballDesc.goal = Quaterniond::Rot(Rad( 30), 'x');
		else if(key == 's') ballDesc.goal = Quaterniond::Rot(Rad(-30), 'x');
		else if(key == 'd') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'z');
		else if(key == 'f') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'z');
		else if(key == 'w') ballDesc.goal = Quaterniond::Rot(Rad( 30), 'y');
		else if(key == 'i') ballDesc.goal = Quaterniond::Rot(Rad(120), 'x');
		else if(key == 'o') ballDesc.goal = Quaterniond::Rot(Rad(120), 'x') * Quaterniond::Rot(Rad(20), 'y');
*/		
		if(key == 'a') soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
		else if(key == 's') soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
		else if(key == 'd') soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
		else if(key == 'f') soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
		else if(key == 'g') soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		else if(key == 'h') soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		else if(key == 'w'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 'e'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, 2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		}
		else if(key == 'r'){
			soBox[1]->SetVelocity(Vec3d(0.0, 0.0, -2.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 't'){
			soBox[1]->SetVelocity(Vec3d(2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, 2.0, 0.0));
		}
		else if(key == 'y'){
			soBox[1]->SetVelocity(Vec3d(-2.0, 0.0, 0.0));
			soBox[1]->SetAngularVelocity(Vec3d(0.0, -2.0, 0.0));
		}
		else if(key == 'u')	soBox[1]->SetAngularVelocity(Vec3d(2.0, 0.0, 0.0));

		joint->SetDesc(&ballDesc);

	}
};

/// PHHingeJointのデモ
class HingeJointHandler : public Handler{
public:
	double curAngle;

	virtual void Build(){
		BuildCommon();

		PHSdkIf* phSdk = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		//bd.boxsize = Vec3f(6.0, 2.0, 2.0);
		shapeBox = phSdk->CreateShape(bd);
		
		soBox.resize(2);
		soBox[0] = phScene->CreateSolid(sdBox);
		soBox[0]->AddShape(shapeBox);
		soBox[0]->SetDynamical(false);
		soBox[1] = phScene->CreateSolid(sdBox);
		soBox[1]->AddShape(shapeBox);
		phScene->SetContactMode(soBox[0], soBox[1], PHSceneDesc::MODE_NONE);

		jntLink.resize(1);
		PHHingeJointDesc desc;
		desc.poseSocket.Pos() = Vec3d(0.0, 3.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(0.0, -3.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		/*
		desc.poseSocket.Pos() = Vec3d(3.0, 0.0, 0.0);
		desc.poseSocket.Ori() = Quaterniond::Rot(Rad(0), 'x');
		desc.posePlug.Pos()   = Vec3d(-3.0, 0.0, 0.0);
		desc.posePlug.Ori()   = Quaterniond::Rot(Rad(0), 'x');
		*/

		desc.spring			  = 1000;
		desc.damper			  = 10;
		desc.targetPosition	  = Rad(0);
		desc.lower			  = Rad(-60);
		desc.upper			  = Rad( 60);
		jntLink[0] = phScene->CreateJoint(soBox[0], soBox[1], desc);

		curAngle = 0.0;

		Handler::Build();
	}

	virtual void OnKey(int key){
		if(key == 'w')
			curAngle += 10.0;
		else if(key == 'i')
			curAngle -= 10.0;

		// バネ原点（目標角度）を更新
		PHHingeJointIf* hiJoint = DCAST(PHHingeJointIf, jntLink[0]);
		hiJoint->SetTargetPosition(Rad(curAngle));
		
		/*
		// デスクリプタで設定することも可能:
		PHHingeJointDesc hingeDesc;
		GetDesc(&hingeDesc);
		hingeDesc.origin = Rad(curAngle);
		hiJoint->SetDesc(&hingeDesc);
		*/
	}
};

class MyApp : public FWApp{
public:
	UTRef<ObjectStatesIf>		state;
	vector< UTRef<Handler> >	handlers;
	Handler*					activeHandler;
	FWEditorOverlay				editor;

	bool	bAutoStep;						// 自動ステップ

	void SwitchScene(int id){
		if(id < 0 || (int)handlers.size() <= id)
			return;

		GetSdk()->SwitchScene(GetSdk()->GetScene(id));
		editor.SetObject(GetSdk()->GetScene()->GetPHScene());
		activeHandler = handlers[id];
		if(!activeHandler->bReady)
			activeHandler->Build();
		cameraInfo.Fit(GetSdk()->GetRender()->GetCamera(), activeHandler->GetSceneRadius());
	}

	/**
	 brief 		キーボードコールバック関数 
	 param		<in/--> key　　 ASCIIコード
	 param 		<in/--> x　　　 キーが押された時のマウス座標
	 param 		<in/--> y　　　 キーが押された時のマウス座標
	 return 	なし
	 */
	virtual void Keyboard(int key, int x, int y){
		if(key == ESC || key == 'q')
			exit(0);
		if(key == ' ')
			bAutoStep = !bAutoStep;
		if(key == ';')
			Step();

		PHSceneIf* phScene = activeHandler->GetPHScene();
		if (key == 'W'){
			phScene->WriteState("state.bin");
		}
		else if (key == 'R'){
			phScene->ReadState("state.bin");
		}
		else if(key == ','){
			state->SaveState(phScene);
		}
		else if(key == '.'){
			state->LoadState(phScene);
		}
		else if(key == '/'){
			state->ReleaseState(phScene);
		}

		// FWEditorで処理されなかったキー入力のみ次に回す
		if(!editor.Key(key)){
			//シーン切り替え
			if(key <= 0xff && isdigit(key)){
				SwitchScene(key - '0');
			}
			else{
				activeHandler->OnKey(key);
				// FWEditorを更新
				editor.SetObject(GetSdk()->GetScene()->GetPHScene());
			}
		}
	}

	void Step(){
		GetSdk()->Step();
		activeHandler->OnTimer();
	}

	virtual void TimerFunc(int id){
		if(bAutoStep)
			Step();
		PostRedisplay();
	}	

	virtual void Init(int argc = 0, char* argv[] = NULL){
		SetGRAdaptee(TypeGLUT);
		GRInit(argc, argv);
		CreateSdk();
		
		FWWinDesc windowDesc;
		windowDesc.title = "Joints";
		CreateWin(windowDesc);
		InitWindow();

		int timerId = CreateTimer(FWTimer::GLUT);				// タイマーの生成

		cameraInfo.zoomRange[1] = 1000.0f;
		GetSdk()->SetDebugMode(true);

		GRRenderIf* render = GetSdk()->GetRender();
		GRLightDesc light;
		light.position = Vec4f(10.0, 20.0, 20.0, 1.0);
		light.diffuse = Vec4f(1,1,1,1) * 0.5f;
		light.specular = Vec4f(1,1,1,1) * 0.2f;
		light.ambient = Vec4f(1,1,1,1) * 0.0f;
		render->PushLight(light);

		// シーンの構築
		handlers.push_back(new ChainHandler());
		handlers.push_back(new LinkHandler());
		handlers.push_back(new ArmHandler());
		handlers.push_back(new BallJointHandler());
		handlers.push_back(new HingeJointHandler());
		handlers.push_back(new PathHandler());
		for(unsigned i = 0; i < handlers.size(); i++){
			GetSdk()->CreateScene();
			GetSdk()->GetScene()->GetPHScene()->SetTimeStep(0.05);
			GetSdk()->GetScene()->GetPHScene()->SetNumIteration(20);
		}

		state = ObjectStatesIf::Create();
		SwitchScene(0);
		bAutoStep = false;
	}

	virtual void Display(){
		// 描画モードの設定
		GRDebugRenderIf* render = GetCurrentWin()->render->Cast();

		render->SetRenderMode(true, false);
		render->EnableRenderWorldAxis();
		//render->EnableRenderAxis(false);
		render->EnableRenderForce();
		render->EnableRenderContact();

		GRCameraDesc cam = render->GetCamera();
		cam.front = 0.5f;
		render->SetCamera(cam);

		render->SetViewMatrix(cameraInfo.view.inv());

		// 描画の実行
		GetSdk()->Draw();

		// 情報のオーバレイ
		editor.Update();
		editor.Draw(render);
		
		glutSwapBuffers();
	}

} app;

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
}
