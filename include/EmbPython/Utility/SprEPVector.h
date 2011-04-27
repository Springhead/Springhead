#ifndef SPREPVECTOR_H
#define SPREPVECTOR_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"
using namespace Spr;


/**********************************Vec3d***********************/
///マクロ
#define EPVec3d_Check(ob) (((PyObject*)ob)->ob_type == &EPVec3dType)

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Vec3d *ptr;
} EPVec3d;


/////////////////////////////////////メソッド登録用
///Vec3dのメソッドの定義

PyObject* EPVec3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPVec3d* newEPVec3d();
EPVec3d* newEPVec3d(Vec3d);
EPVec3d* newEPVec3d(Vec3d*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPVec3dType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPVec3d(void) ;

/**********************************Vec2d***********************/
///マクロ
#define EPVec2d_Check(ob) (((PyObject*)ob)->ob_type == &EPVec2dType)

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Vec2d *ptr;
} EPVec2d;


/////////////////////////////////////メソッド登録用
///Vec2dのメソッドの定義

PyObject* EPVec2d_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPVec2d* newEPVec2d();
EPVec2d* newEPVec2d(Vec2d);
EPVec2d* newEPVec2d(Vec2d*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPVec2dType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPVec2d(void) ;

/**********************************Vec3f***********************/
///マクロ
#define EPVec3f_Check(ob) (((PyObject*)ob)->ob_type == &EPVec3fType)

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Vec3f *ptr;
} EPVec3f;


/////////////////////////////////////メソッド登録用
///Vec3fのメソッドの定義

PyObject* EPVec3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPVec3f* newEPVec3f();
EPVec3f* newEPVec3f(Vec3f);
EPVec3f* newEPVec3f(Vec3f*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPVec3fType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPVec3f(void) ;

/**********************************Vec2f***********************/
///マクロ
#define EPVec2f_Check(ob) (((PyObject*)ob)->ob_type == &EPVec2fType)

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Vec2f *ptr;
} EPVec2f;


/////////////////////////////////////メソッド登録用
///Vec2fのメソッドの定義

PyObject* EPVec2f_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPVec2f* newEPVec2f();
EPVec2f* newEPVec2f(Vec2f);
EPVec2f* newEPVec2f(Vec2f*);


//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPVec2fType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPVec2f(void) ;

/**********************************Vec4f***********************/
///マクロ
#define EPVec4f_Check(ob) (((PyObject*)ob)->ob_type == &EPVec4fType)

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Vec4f *ptr;
} EPVec4f;


/////////////////////////////////////メソッド登録用
///Vec4fのメソッドの定義

PyObject* EPVec4f_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPVec4f* newEPVec4f();
EPVec4f* newEPVec4f(Vec4f);
EPVec4f* newEPVec4f(Vec4f*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPVec4fType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPVec4f(void) ;


PyMODINIT_FUNC initEPVector(void);

#endif