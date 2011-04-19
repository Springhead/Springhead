#include "ShoulderJointLimitApp.h"
#include <GL/glut.h>

void Display();
void Display2();
void Reshape(int w, int h);
void Reshape2(int w, int h);
void Timer(int id);
void Keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse2(int button, int state, int x, int y);
void motion(int x,int y);
void motion2(int x,int y);

void menu(int val){
	switch(val){
		case 1:
			Keyboard('o',0,0);
			break;
		case 2:
			Keyboard(';',0,0);
			break;
		case 3:
			Keyboard(':',0,0);
			break;
		case 4:
			Keyboard('Z',0,0);
			break;
	}
}
void sub_menu(int val){
	switch(val){
		case 0:
			Keyboard('2',0,0);
			break;
		case 2:
			Keyboard('3',0,0);
			break;
		case 1:
			Keyboard('0',0,0);
			break;
		case 3:
			Keyboard('1',0,0);
			break;
	}
}
void setmenu2(){
	int MENU_ID[2];
	/*** サブメニュー１ ***/
	MENU_ID[0] = glutCreateMenu(sub_menu);
	glutAddMenuEntry("leftShoulder", 0);
	glutAddMenuEntry("rightShoulder", 1);
	glutAddMenuEntry("leftWrist", 2);
	glutAddMenuEntry("rightWrist", 3);
	/*** ここから、メインメニューの並び ***/
	glutCreateMenu(menu);
	glutAddMenuEntry("Save", 1);
	glutAddMenuEntry("Load", 2);
	glutAddMenuEntry("Reset", 3);
	glutAddSubMenu("balljointNum", MENU_ID[0] );
	glutAddMenuEntry("ChangeView", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[]){
	app.Init(argc, argv);

	app.SetWindow1();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(Keyboard);	
	app.SetWindow2();
	glutDisplayFunc(Display2);
	glutReshapeFunc(Reshape2);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMouseFunc(mouse2);
	glutMotionFunc(motion2);
	glutKeyboardFunc(Keyboard);
	setmenu2();

	glutTimerFunc(20,Timer,0);


	app.grRender->SetDevice(app.grDevice);//Springhead2の描画機能にOpenGLの使用登録をする。
	// 照明の設定
	glutSetWindow(app.Win1);
	GRLightDesc light0, light1;
	light0.position = Vec4f(-10.0, 20.0, -20.0, 1.0);
	app.grRender->PushLight(light0);
	app.grRender->PushLight(light1);
	glutSetWindow(app.Win2);
	light0.position = Vec4f(-10.0, 20.0, -20.0, 1.0);
	app.grRender->PushLight(light0);
	app.grRender->PushLight(light1);

	glutMainLoop();


	return 0;
}

void Display(){
	app.Display();
}
void Display2(){
	app.Display2();
}

void Reshape(int w, int h){
	app.Reshape(w, h);
}
void Reshape2(int w, int h){
	app.Reshape2(w, h);
}

void Timer(int id){
	glutTimerFunc(20, Timer,id);
	app.Timer(id);
	glutSetWindow(app.Win1);
	glutPostRedisplay();
	glutSetWindow(app.Win2);
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	app.Keyboard(key,x,y);
}

void mouse(int button, int state, int x, int y){
	app.mouse(button,state,x,y);
}
void mouse2(int button, int state, int x, int y){
	app.mouse2(button,state,x,y);
}
void motion(int x, int y){
	app.motion(x,y);
}
void motion2(int x, int y){
	app.motion2(x,y);
}