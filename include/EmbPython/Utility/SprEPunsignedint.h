#ifndef SPREPUNSIGNEDINT_H
#define SPREPUNSIGNEDINT_H

#include <Python/Python.h>
#include "SprEPErrors.h"




/********************************EPunsignedint*************************************/
#define EPunsignedint_Check(ob) (((PyObject*)ob)->ob_type == &EPunsignedintType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	unsigned int *ptr;
} EPunsignedint;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義
PyObject* EPunsignedint_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPunsignedint* newEPunsignedint();
EPunsignedint* newEPunsignedint(unsigned int);
EPunsignedint* newEPunsignedint(unsigned int*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPunsignedintType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPunsignedint(void) ;

#endif