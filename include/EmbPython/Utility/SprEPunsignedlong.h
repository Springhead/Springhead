#ifndef SPREPUNSIGNEDLONG_H
#define SPREPUNSIGNEDLONG_H

#include <Python/Python.h>
#include "SprEPErrors.h"




/********************************EPunsignedlong*************************************/
#define EPunsignedlong_Check(ob) (((PyObject*)ob)->ob_type == &EPunsignedlongType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	unsigned long *ptr;
} EPunsignedlong;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義
PyObject* EPunsignedlong_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPunsignedlong* newEPunsignedlong();
EPunsignedlong* newEPunsignedlong(unsigned long);
EPunsignedlong* newEPunsignedlong(unsigned long*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPunsignedlongType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPunsignedlong(void) ;

#endif