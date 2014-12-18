
#include "..\..\include\EmbPython\SprEPInterpreter.h"

using namespace Spr;

//マルチスレッド用
#include <process.h>

#ifdef _WIN32
#define NOMINMAX //std::maxなどを使うのに必要(windows.hと競合するから)
#include <windows.h>
#endif

EPInterpreter* EPInterpreter::instance = NULL;

EPInterpreter* EPInterpreter::Create()
{
	if( EPInterpreter::instance == NULL)
		EPInterpreter::instance = new EPInterpreter();
	
	return EPInterpreter::instance;
}

void EPInterpreter::Destroy()
{
	if( EPInterpreter::instance != NULL)
		delete(EPInterpreter::instance);
}

EPInterpreter::EPInterpreter()
{
	state = UNSTARTED;
}

EPInterpreter::~EPInterpreter()
{
	if (state == RUN) Stop();
}

// プログラム終了時にEPInterpreter::instanceをdeleteしてくれるひと
class EPInterpreterDestroyer {
public:
	~EPInterpreterDestroyer() {
		EPInterpreter::Destroy();
	}
};
static EPInterpreterDestroyer epid;

extern "C" { __declspec(dllexport) PyObject* PyInit__SprPy(void); }


void EPInterpreter::Initialize()
{
	//PYTHONPATHを変更する
	//SPRPYTHONPATHにSpringheadPythonで使うLIBのPATHを追加しておく
	std::string newPath;
	char buff[1024];
	if (!GetEnvironmentVariable("SPRPYTHONPATH",buff,1024) || !strlen(buff)){
		DSTR << "Warning: " << "Can not find environment variable of 'SPRPYTHONPATH'. " 
			<< "Embeded python may cause serious error." << std::endl;
	}
	else{
		newPath.append(buff);
		newPath.append(";");
	}
	GetEnvironmentVariable("PYTHONPATH", buff,1024);
	newPath.append(buff);
	if (newPath.length() <= 1){
		DSTR << "Warning: " << "Can not find environment variable of 'PYTHONPATH' and 'SPRPYTHONPATH'. " 
			<< "Embeded python will causes buffer over run error." << std::endl;
		printf("Error: Python Lib not found ");
		assert(0);
	}
	
	SetEnvironmentVariable("PYTHONPATH", newPath.c_str());
	// SetEnvironmentVariable("PYTHONHOME", newPath.c_str());


	//パイソン初期化
	Py_Initialize();
	

	////モジュール読み込み
	initUtility();
	initBase();
	initFoundation();
	initFileIO();
	initCollision();
	initPhysics();
	initGraphics();
	initCreature();
	initHumanInterface();
	initFramework();
}

void EPInterpreter::Run(void* arg)
{
	if( this->state == UNSTARTED || this->state == STOPED)
	{
		_beginthread(EPLoopLauncher, 0 , arg);
		this->state = RUN;
	}
}
void EPInterpreter::Finalize()
{
	if (state == RUN){
		fclose(stdin);	//	Pythonのインタラクティブループ(EPLoop)が、入力できなくなって、帰ってくるように、stdinを閉じてしまう。
		Stop();
	}
	Py_Finalize();	//	なぜか帰ってこない
}

void EPInterpreter::Stop()
{
	if(state == RUN){
		state = STOP_REQUEST;
		while (state != STOPED) Sleep(100);
	}
}

bool EPInterpreter::BindInt(int i)
{
	return false;
}

void EPInterpreter::EPLoopLauncher(void* arg)
{
	EPInterpreter::instance->EPLoopInit(arg);
	while(EPInterpreter::instance->state == RUN)
	{
		EPInterpreter::instance->EPLoop(arg);
	}
	EPInterpreter::instance->state = STOPED;
}
