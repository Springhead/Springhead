#include "Handler.h"

class SpringHandler : public Handler{
public:
	enum{
		ID_EXPLICIT,
		ID_IMPLICIT,
		ID_ANALYTICAL,
		ID_SAVE,
	};

	// バネダンパの計算法
	struct Mode{
		enum{
			Implicit  ,
			Explicit  ,
			Analytical,
		};
	};

	struct LogRecord{
		double time;
		double target;
		double pos;
		double vel;
	};

	CDShapeIf*    shapeBase;
	CDShapeIf*    shapeObj;
	PHSolidIf*    soBase;
	PHSolidIf*    soObj;
	PH1DJointIf*  joint;

	Vec3d		rootPos;			///< 鎖の固定点
	Vec3d		iniPos;				///< 箱が最初に置かれる場所

	int         mode;
	int         count;
	double      period;             ///< 目標値を切り替える周期
	double		spring, damper;		///< バネ係数，ダンパ係数
	double      time;
	double      target;
	double      pos;
	double      vel;
	
	vector<LogRecord> log;

public:
	SpringHandler(SampleApp* a):Handler(a){
		rootPos	= Vec3d(0.0, 5.0, 0.0);

		app->AddAction(MENU_SPRING, ID_EXPLICIT  , "switch to explicit method");
		app->AddHotKey(MENU_SPRING, ID_EXPLICIT  , 'e');
		app->AddAction(MENU_SPRING, ID_IMPLICIT  , "switch to implicit method");
		app->AddHotKey(MENU_SPRING, ID_IMPLICIT  , 'i');
		app->AddAction(MENU_SPRING, ID_ANALYTICAL, "switch to analytical method");
		app->AddHotKey(MENU_SPRING, ID_ANALYTICAL, 'a');
		app->AddAction(MENU_SPRING, ID_SAVE      , "save log");
		app->AddHotKey(MENU_SPRING, ID_SAVE      , 's');
	}
	~SpringHandler(){}

	void SwitchMode(int m){
		mode = m;
		if(mode == Mode::Implicit){
			joint->Enable(true);
			joint->SetSpring(spring);
			joint->SetDamper(damper);
			joint->SetOffsetForce(0.0);
		}
		if(mode == Mode::Explicit){
			joint->Enable(true);
			joint->SetSpring(0.0);
			joint->SetDamper(0.0);
		}
		if(mode == Mode::Analytical){
			joint->Enable(false);
		}

		log.clear();
	}

	void SaveLog(){
		ofstream file("log.csv");
		file << "time, target, pos, vel" << endl;
		for(int i = 0; i < (int)log.size(); i++)
			file << log[i].time << ", " << log[i].target << ", " << log[i].pos << ", " << log[i].vel << endl;
	}

	virtual void BuildScene(){
		soFloor = app->CreateFloor(true);

		CDBoxDesc bd;
		bd.boxsize = Vec3d(2.0, 2.0, 2.0);
		shapeBase = GetPHSdk()->CreateShape(bd);
		bd.boxsize = Vec3d(2.0, 6.0, 2.0);
		shapeObj  = GetPHSdk()->CreateShape(bd);

		soBase = GetPHScene()->CreateSolid();
		soBase->AddShape(shapeBase);
		soBase->SetFramePosition(rootPos);
		soBase->SetDynamical(false);

		soObj = GetPHScene()->CreateSolid();
		soObj->AddShape(shapeObj);
		soObj->SetFramePosition(rootPos);
		
		PHHingeJointDesc jd;
		joint = GetPHScene()->CreateJoint(soBase, soObj, jd)->Cast();

		/* explicitでM = 1, dt = 0.05だと
		   K = 4/(h^2) = 1600, D = 2/h = 40 が限界
		   振動系としては減衰比zeta = D/(2*sqrt(M*K))でzeta < 1で減衰振動，zeta > 1で過減衰
		   M = 1なら D^2 = 4K が臨界
		 */
		spring = 300.0;
		damper = 33.0;
		
		GetPHScene()->CreateTreeNodes(soBase);
		GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);
		//nodeTree.push_back(GetPHScene()->CreateRootNode(so));

		count  = 0;
		time   = 0.0;
		period = 3.0;
		target = Rad(90.0);
		pos    = 0.0;
		vel    = 0.0;
		
		SwitchMode(Mode::Implicit);
		//SwitchMode(Mode::Explicit);
		//SwitchMode(Mode::Analytical);
	}

	virtual void OnAction(int id){
		if(id == ID_EXPLICIT)
			SwitchMode(Mode::Explicit);
		if(id == ID_IMPLICIT)
			SwitchMode(Mode::Implicit);
		if(id == ID_ANALYTICAL)
			SwitchMode(Mode::Analytical);
		if(id == ID_SAVE)
			SaveLog();
	}

	virtual void OnStep(){
		if(mode == Mode::Implicit || mode == Mode::Explicit){
			pos = joint->GetPosition();
			vel = joint->GetPosition();
		}
		LogRecord l;
		l.time   = time;
		l.target = target;
		l.pos    = pos;
		l.vel    = vel;
		log.push_back(l);

		time += GetPHScene()->GetTimeStep();
		if(time > period * (count+1)){
			target += Rad(90.0);
			if(target > Rad(180.0))
				target -= Rad(360.0);
			count++;
		}

		if(mode == Mode::Implicit){
			joint->SetTargetPosition(target);
		}
		if(mode == Mode::Explicit){
			joint->SetOffsetForce(spring * (target - joint->GetPosition()) - damper * joint->GetVelocity());
		}
		if(mode == Mode::Analytical){
			double eps   = 1.0e-10;
			double dt    = GetPHScene()->GetTimeStep();
			double mass  = 1.0;
			double zeta  = damper / (2.0 * sqrt(mass * spring));
			double omega = sqrt(spring / mass);

			// 減衰振動
			if(zeta < 1.0){
				double C, alpha;

				if(std::abs(pos - target) < eps){
					alpha = M_PI / 2.0;
					C     = vel / (omega * sqrt(1 - zeta*zeta));
				}
				else{
					double sigma = vel / ((pos - target) * omega);
					alpha = atan((sigma + zeta) / sqrt(1.0 - zeta*zeta));
					C     = (pos - target) / cos(alpha);
				}
				pos = target + C * exp(-zeta * omega * dt) * cos(omega * sqrt(1.0 - zeta*zeta) * dt - alpha);
				vel = -1.0 * omega * C * exp(-zeta * omega * dt) * (
					 zeta                  * cos(omega * sqrt(1.0 - zeta*zeta) * dt - alpha) +
					 sqrt(1.0 - zeta*zeta) * sin(omega * sqrt(1.0 - zeta*zeta) * dt - alpha)
					 );				
			}
			// 過減衰
			if(zeta > 1.0){
				double a = -omega * zeta;
				double b = omega * sqrt(zeta*zeta - 1.0);
				double p = (pos - target);
				double v = vel;
				pos = target + exp(a*dt) * ( p * cosh(b*dt) + ((v - a*p)/b)           * sinh(b*dt) );
				vel =          exp(a*dt) * (     cosh(b*dt) + ((a*v - (a*a+b*b)*p)/b) * sinh(b*dt) );
			}

			soObj->SetOrientation(Quaterniond::Rot(pos, 'z'));
		}
	}
};
