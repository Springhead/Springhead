#include <Springhead.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <fstream>
#include <HumanInterface/SprHIDRUsb.h>
using namespace PTM;//  行列クラスはPTM名前空間の中で宣言されている。
using namespace Spr;

class MyApp{
public:
	UTRef< FWSdkIf > fwSdk;
	UTRef< PHSdkIf > phSdk;
	UTRef< GRSdkIf > grSdk;
	UTRef< GRRenderIf >		 grRender;
	UTRef< PHSceneIf >       phScene;
	UTRef< FWSceneIf >		 fwScene;
	UTRef< PHSolidIf >		 solids[10];
	UTRef< PHSolidIf >		 ssolid[5];
	UTRef< PHSolidIf >		 Pointer[2];
	UTRef< PHSolidIf >		 Floor;

	UTRef< CDShapeIf > shapeBox;
	UTRef< CDShapeIf > shapeBox2;
	UTRef< CDShapeIf > shapeBox3;
	UTRef< CDShapeIf > shapePoint;
														
	//UTRef< PHBallJointIf >	 joint[8];
	UTRef< PHBallJointIf >	 balljoint;
	UTRef< PHBallJointIf >	 joints[5];
	UTRef< PHHingeJointIf >	 hjoints[5];

	//UTRef<HISdkIf> sdk;// = HISdkIf::CreateSdk();
	//UTRef<HISpidarGIf> spg;// = sdk->CreateHumanInterface(HISpidarGIf::GetIfInfoStatic())->Cast();
	//bool spidar;

	//UTRef< PHHingeJointIf >	 joint[4];
	UTRef< PHSpringIf >	 Spring;
	UTRef< PHSpringIf >	 spring[3];
	UTRef< PH1DJointIf >	 joint1;
	bool bLeftButton, bRightButton,bMiddleButton;
	// カメラ位置
	double CameraZoom;
	double CameraRotX;
	double CameraRotY;
	Vec3f cam;
	int Win1,Win2;

	bool Step;
	Vec3d Check;
	//クリックした位置
	int xlast, ylast;
	double point[2][2];				// 座標を記憶する配列 
	double lastPoint[2];
	int count[5];
	int count2;
	double points[100][6];
	Vec2d poleTwist;
	double Lpoint[20][5];
	bool Save;
	bool OneStep;
	bool Start;
	int St;
	std::fstream ConstData;
	Vec3d cal;


	Vec3d position;
	Vec3d pospos;
	bool abc;

	LARGE_INTEGER freq;
	LARGE_INTEGER begin;
	LARGE_INTEGER end;

	UTRef< GRDeviceGLIf > grDevice;

	int Count;
	int jointsNum;
	double width,hight;
	int pointnum;

	/////////////////////////////////////
	MyApp();
	void Init(int argc, char* argv[]);
	void SetWindow1();
	void SetWindow2();
	void Display();
	void Display2();
	void Reshape(int w, int h);
	void Reshape2(int w, int h);
	void Timer(int id);
	void Keyboard(unsigned char key,int x,int y);
	void mouse(int button, int state,int x,int y);
	void mouse2(int button, int state,int x,int y);
	void motion(int x, int y);
	void motion2(int x, int y);

	void CreateObj();

	bool loadData(char* filename);
	bool SaveData();
	void HermiteSpline(Vec2d para);
	void Constline();
	void GetSpline();
	bool twistview;
	double aaaaa;
	//文字出力用
	void DRAW_STRING(int x, int y, char *string, void *font = GLUT_BITMAP_TIMES_ROMAN_24);
	void DISPLAY_TEXT(int x, int y, char *string);
	int list;	//この変数はよくわからん
	int tn;
	bool twistFlag;
	bool LoadPos(char* filename);

	Vec2d pointpoint[100];
	int PAPA;
	void Pointpoint();
};

extern MyApp app;