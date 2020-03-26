
#include "../../include/EmbPython/SprEPInterpreter.h"

using namespace Spr;

//マルチスレッド用
#ifdef _WIN32
#include <process.h>
#endif

#ifdef _WIN32
#define NOMINMAX //std::maxなどを使うのに必要(windows.hと競合するから)
#include <windows.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define	Sleep sleep
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
#ifdef _WIN32
	if( EPInterpreter::instance != NULL)
		delete(EPInterpreter::instance);
#else
	// g++ complains "deleting object of polymorphic class type
	// ‘Spr::EPInterpreter’ which has non-virtual destructor
	// might cause undefined behaviour"
	// This results memory leak!
#endif
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
#ifdef _WIN32
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
#else
	const char* envp;
	envp = getenv("SPRPYTHONPATH");
	if (envp == NULL) {
		printf("Warning: "
			"Can not find environment variable of 'SPRPYTHONPATH' ."
			"Embeded python may cause serious error.\n");
	} else {
		newPath.append(envp);
		newPath.append(";");
	}
	envp = getenv("PYTHONPATH");
	newPath.append(envp);
	if (envp == NULL) {
		printf("Warning: "
			"Can not find environment variable of 'PYTHONPATH'"
			" and 'SPRPYTHONPATH'. " 
			"Embeded python will causes buffer over run error.\n");
		printf("Error: Python Lib not found\n");
		assert(0);
	}
	setenv((const char*)"PYTHONPATH", (const char*) newPath.c_str(), 1);
#endif


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
#ifdef _WIN32
		_beginthread(EPLoopLauncher, 0 , arg);
#else
		pthread_t thread;
		pthread_attr_t attr;
		void* param = NULL;

		pthread_attr_init(&attr);
		void* (*entry)(void*) = (void* (*)(void*)) arg;
		pthread_create(&thread, &attr, entry, param);
#endif
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
