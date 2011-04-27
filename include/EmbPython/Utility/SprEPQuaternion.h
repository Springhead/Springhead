#ifndef SPREPQUATERNION_H
#define SPREPQUATERNION_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"
using namespace Spr;



/****************************************************Quaterniond***********************************************************************/
///マクロ
#define EPQuaterniond_Check(ob) (((PyObject*)ob)->ob_type == &EPQuaterniondType)
#define EPQuaterniond_ArgParseTuple( arg ) double wi,xi,yi,zi,pxi,pyi,pzi; PyArg_ParseTuple( arg , "dddd",&wi,&xi,&yi,&zi )
#define EPQuaterniond_BuildValue(self) Py_BuildValue("dddd",self->ptr->W(),self->ptr->X(),self->ptr->Y(),self->ptr->Z())

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Quaterniond *ptr;
} EPQuaterniond;


/////////////////////////////////////メソッド登録用
///Quaterniondのメソッドの定義



EPQuaterniond* newEPQuaterniond();
EPQuaterniond* newEPQuaterniond(Quaterniond);
EPQuaterniond* newEPQuaterniond(Quaterniond*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPQuaterniondType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPQuaterniond(void) ;

/****************************************************Quaternionf***********************************************************************/
///マクロ
#define EPQuaternionf_Check(ob) (((PyObject*)ob)->ob_type == &EPQuaternionfType)
#define EPQuaternionf_ArgParseTuple( arg ) float wi,xi,yi,zi,pxi,pyi,pzi; PyArg_ParseTuple( arg , "ffff",&wi,&xi,&yi,&zi )
#define EPQuaternionf_BuildValue(self) Py_BuildValue("ffff",self->ptr->W(),self->ptr->X(),self->ptr->Y(),self->ptr->Z())

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Quaternionf *ptr;
} EPQuaternionf;


/////////////////////////////////////メソッド登録用
///Quaternionfのメソッドの定義



EPQuaternionf* newEPQuaternionf();
EPQuaternionf* newEPQuaternionf(Quaternionf);
EPQuaternionf* newEPQuaternionf(Quaternionf*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPQuaternionfType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPQuaternionf(void) ;



PyMODINIT_FUNC initEPQuaternion(void) ;

#endif