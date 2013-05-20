/** 旧glutの代わりにfreeglutを使う
	・特に理由が無い場合はfreeglutがおすすめ
	・ジョイスティックなどのfreeglutにしかない機能が使いたい場合は必要
	・OpenGL1.0より新しいバージョンが使用したい場合は必要
 */
#define USE_FREEGLUT

/** 使用するOpenGLのバージョン
	同時にUSE_FREEGLUTを定義することが必要
 */
#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 0

/** GLEWを使用
 */
#define USE_GLEW

/** Springhead2/closed のソースを使用するかどうか
	開発者以外は未定義としてください
 */
#include "UseClosedSrcOrNot.h" 
