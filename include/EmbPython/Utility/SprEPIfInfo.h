#ifndef SPREPIFINFO_H
#define SPREPIFINFO_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"

using namespace Spr;



/********************************EPIfInfo*************************************/
#define EPIfInfo_Check(ob) (((PyObject*)ob)->ob_type == &EPIfInfoType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	IfInfo *ptr;
} EPIfInfo;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPIfInfo_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPIfInfo* newEPIfInfo();
EPIfInfo* newEPIfInfo(IfInfo*);
//EPIfInfo* newEPIfInfo(IfInfo); 抽象クラスはインスタンス化できない

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPIfInfoType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPIfInfo(void) ;


#endif