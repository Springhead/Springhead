// CRSimpleTest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CRSimpleTestApp.h"
#include <GL/glut.h>
#define	ESC				27				// Esc key

using namespace Spr;
using namespace std;

CRSimpleTestApp* app;
int cycle_cnt;
const int CYCLE_LIMIT = 300;

void display(){
	app->Display();
}

void reshape(int w, int h){
	app->Reshape(w, h);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
	case ESC:
		exit(0);
		break;
	}
}

void idle(){
	app->Step();
	if (CYCLE_LIMIT < cycle_cnt++) exit(0);
	glutPostRedisplay();
}


int _tmain(int argc, _TCHAR* argv[]){

	cycle_cnt = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	int    window    = glutCreateWindow("CRSimpleTest");
	string filename  = "test.x";

	app = new CRSimpleTestApp(window, filename);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;

}
