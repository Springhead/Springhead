#include "Handler.h"

class ChainHandler : public Handler{
public:
	enum{
		ID_HINGE,
		ID_SLIDER,
		ID_BALL,
		ID_INC_POS,
		ID_DEC_POS,
		ID_INC_RANGE,
		ID_DEC_RANGE,
		ID_INC_SPRING,
		ID_DEC_SPRING,
		ID_INC_DAMPER,
		ID_DEC_DAMPER,
		ID_GEAR,
	};

	vector<PHSolidIf*>		links;
	vector<PHJointIf*>		joints;

	Vec3d				rootPos;			///< 鎖の固定点
	Vec3d				iniPos;				///< 箱が最初に置かれる場所

	int					posLevel;
	int					rangeLevel;
	int					springLevel;
	int					damperLevel;

	double				spring, damper;		///< バネ係数，ダンパ係数

public:
	ChainHandler(SampleApp* a):Handler(a){
		rootPos		= Vec3d(0.0, 15.0, 0.0);
		iniPos		= Vec3d(10, 10, 0);

		posLevel	= 0;
		rangeLevel	= 0;
		springLevel = 0;
		damperLevel = 0;

		app->AddAction(MENU_CHAIN, ID_HINGE, "create hinge");
		app->AddHotKey(MENU_CHAIN, ID_HINGE, 'j');	///< hだとhelpと重複する
		app->AddAction(MENU_CHAIN, ID_SLIDER, "create slider");
		app->AddHotKey(MENU_CHAIN, ID_SLIDER, 's');
		app->AddAction(MENU_CHAIN, ID_BALL, "create ball");
		app->AddHotKey(MENU_CHAIN, ID_BALL, 'b');
		app->AddAction(MENU_CHAIN, ID_GEAR, "create gear (not implemented)");
		app->AddHotKey(MENU_CHAIN, ID_GEAR, 'g');
	}
	~ChainHandler(){}

	void UpdateJoint(PHJointIf* jnt){
		PHHingeJointIf* hinge = DCAST(PHHingeJointIf, joints.back());
		if(hinge){
			PH1DJointLimitIf* limit = hinge->GetLimit()->Cast();
			hinge->SetTargetPosition(Rad(20.0) * (double)(posLevel - 5));
			limit->SetRange(Vec2d(-Rad(20.0) * rangeLevel, Rad(20.0) * rangeLevel));
			hinge->SetSpring(springLevel);
			hinge->SetDamper(damperLevel);
		}
		PHSliderJointIf* slider = DCAST(PHSliderJointIf, joints.back());
		if(slider){
			PH1DJointLimitIf* limit = slider->GetLimit()->Cast();
			slider->SetTargetPosition(0.1 * (double)(posLevel - 5));
			limit->SetRange(Vec2d(-0.1 * rangeLevel, 0.1 * rangeLevel));
			slider->SetSpring(springLevel);
			slider->SetDamper(damperLevel);
		}
		PHBallJointIf* ball = DCAST(PHBallJointIf, joints.back());
		if(ball){
			Quaterniond ori;
			switch(posLevel){
			case 0: ori = Quaterniond::Rot(Rad( 30), 'x'); break;
			case 1: ori = Quaterniond::Rot(Rad(-30), 'x'); break;
			case 2: ori = Quaterniond::Rot(Rad( 30), 'z'); break;
			case 3: ori = Quaterniond::Rot(Rad(-30), 'z'); break;
			case 4: ori = Quaterniond::Rot(Rad( 30), 'y'); break;
			case 5: ori = Quaterniond::Rot(Rad(-30), 'y'); break;
			case 6: ori = Quaterniond::Rot(Rad(120), 'x'); break;
			case 7: ori = Quaterniond::Rot(Rad(120), 'x') * Quaterniond::Rot(Rad(20), 'y');
			case 8: ori = Quaterniond::Rot(Rad(120), 'y'); break;
			case 9: ori = Quaterniond::Rot(Rad(120), 'y') * Quaterniond::Rot(Rad(20), 'z');
			default:;
			}
			ball->SetTargetPosition(ori);

			PHBallJointConeLimitIf* limit = ball->GetLimit()->Cast();
			limit->SetSwingRange(Vec2d(-Rad(20.0) * rangeLevel, Rad(20.0) * rangeLevel));
			limit->SetTwistRange(Vec2d(-Rad(20.0) * rangeLevel, Rad(20.0) * rangeLevel));

			ball->SetSpring(springLevel);
			ball->SetDamper(damperLevel);
		}
	}
	virtual void BuildScene(){
		soFloor = app->CreateFloor(true);

		PHSolidIf* so = GetPHScene()->CreateSolid();
		so->AddShape(app->shapeBox);
		so->SetFramePosition(rootPos);
		so->SetDynamical(false);
		links.push_back(so);
		nodeTree.push_back(GetPHScene()->CreateRootNode(so));
	}
	virtual void OnAction(int id){
		PHSceneIf* phScene = GetPHScene();

		PHSolidIf* so = NULL;
		if(id == ID_HINGE || id == ID_BALL || id == ID_SLIDER){
			so = phScene->CreateSolid();
			so->AddShape(app->shapeBox);
			so->SetFramePosition(iniPos);
		}
		if(id == ID_HINGE){
			PHHingeJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d( 1.1,  -1.1,  0);
			jdesc.posePlug.Pos() = Vec3d(-1.1, 1.1,  0);
			jdesc.damper = damper;
			joints.push_back(phScene->CreateJoint(links.back(), so, jdesc));
			PH1DJointLimitDesc ldesc;
			ldesc.range = Vec2d(Rad(20.0), Rad(30.0));
			DCAST(PH1DJointIf,joints.back())->CreateLimit(ldesc);
		}
		if(id == ID_SLIDER){
			PHSliderJointDesc jdesc;
			jdesc.poseSocket.Pos() = Vec3d(0, -1.1, 0);
			jdesc.poseSocket.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			jdesc.posePlug.Pos() = Vec3d(0,  1.1, 0);
			jdesc.posePlug.Ori() = Quaterniond::Rot(Rad(90.0), 'y');
			joints.push_back(phScene->CreateJoint(links.back(), so, jdesc));
			PH1DJointLimitDesc ldesc;
			DCAST(PH1DJointIf,joints.back())->CreateLimit(ldesc);
		}
		if(id == ID_BALL){
			PHBallJointDesc jdesc;
			//jdesc.limit[1].upper =  0.2;	// 最大スイング角
			//jdesc.limit[2].lower= -0.2;	// ツイスト角範囲
			//jdesc.limit[2].upper =  0.2;
			jdesc.poseSocket.Pos() = Vec3d(-1.01, -1.01, -1.01);
			jdesc.posePlug.Pos() = Vec3d(1.01, 1.01, 1.01);
			joints.push_back(phScene->CreateJoint(links.back(), so, jdesc));
			PHBallJointConeLimitDesc ldesc;
			ldesc.limitSwing    = Vec2d(-Rad(30.0), Rad(30.0));
			ldesc.limitTwist    = Vec2d(-Rad(30.0), Rad(30.0));
			ldesc.spring = 10000;
			ldesc.damper = 100;
			DCAST(PHBallJointIf,joints.back())->CreateLimit(ldesc);
		}
		if(id == ID_HINGE || id == ID_BALL || id == ID_SLIDER){
			links.push_back(so);
			nodeTree.push_back(GetPHScene()->CreateTreeNode(nodeTree.back(), so));
			UpdateJoint(joints.back());
		}

		if(id == ID_INC_POS){
			posLevel = std::min(10, posLevel+1);
			UpdateJoint(joints.back());
		}
		if(id == ID_DEC_POS){
			posLevel = std::max(0, posLevel-1);
			UpdateJoint(joints.back());
		}
		if(id == ID_INC_RANGE){
			rangeLevel = std::min(10, rangeLevel+1);
			UpdateJoint(joints.back());
		}
		if(id == ID_DEC_RANGE){
			rangeLevel = std::max(0, rangeLevel-1);
			UpdateJoint(joints.back());
		}
		if(id == ID_INC_SPRING){
			springLevel = std::min(10, springLevel+1);
			UpdateJoint(joints.back());
		}
		if(id == ID_DEC_SPRING){
			springLevel = std::max(0, springLevel-1);
			UpdateJoint(joints.back());
		}
		if(id == ID_INC_DAMPER){
			damperLevel = std::min(10, damperLevel+1);
			UpdateJoint(joints.back());
		}
		if(id == ID_DEC_DAMPER){
			damperLevel = std::max(0, damperLevel-1);
			UpdateJoint(joints.back());
		}
		if(id == ID_GEAR){
			// 以下はギアの作成コード
			/*if(jntLink.size() >= 2){
				size_t m = jntLink.size();
				PHGearDesc gdesc;
				gdesc.ratio = 1.0;
				phScene->CreateGear(DCAST(PH1DJointIf, jntLink[m-2]), DCAST(PH1DJointIf, jntLink[m-1]), gdesc);
			}*/
		}
	}
};
