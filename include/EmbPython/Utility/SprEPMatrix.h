#ifndef SPREPMATRIX_H
#define SPREPMATRIX_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"

using namespace Spr;



/*********************************************Matrix3d**************************/
///マクロ
#define EPMatrix3d_Check(ob) (((PyObject*)ob)->ob_type == &EPMatrix3dType)

#define asMatrix3d(ob) (*(((EPMatrix3d*)ob)->ptr))

//debug

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Matrix3d *ptr;
} EPMatrix3d;


/////////////////////////////////////メソッド登録用
///Matrix3dのメソッドの定義

PyObject* EPMatrix3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPMatrix3d* newEPMatrix3d();
EPMatrix3d* newEPMatrix3d(Matrix3d);
EPMatrix3d* newEPMatrix3d(Matrix3d*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPMatrix3dType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPMatrix3d(void) ;



/*********************************************Matrix3f**************************/
///マクロ
#define EPMatrix3f_Check(ob) (((PyObject*)ob)->ob_type == &EPMatrix3fType)

#define asMatrix3f(ob) (*(((EPMatrix3f*)ob)->ptr))

//debug

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Matrix3f *ptr;
} EPMatrix3f;


/////////////////////////////////////メソッド登録用
///Matrix3fのメソッドの定義

PyObject* EPMatrix3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPMatrix3f* newEPMatrix3f();
EPMatrix3f* newEPMatrix3f(Matrix3f);
EPMatrix3f* newEPMatrix3f(Matrix3f*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPMatrix3fType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPMatrix3f(void) ;



PyMODINIT_FUNC initEPMatrix(void) ;

#endif