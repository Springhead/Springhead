#ifndef SPREPAFFINE_H
#define SPREPAFFINE_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"
using namespace Spr;




/*********************************************Affinef**************************/
///マクロ
#define EPAffinef_Check(ob) (((PyObject*)ob)->ob_type == &EPAffinefType)

#define asAffinef(ob) (*(((EPAffinef*)ob)->ptr))

//debug

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Affinef *ptr;
} EPAffinef;


/////////////////////////////////////メソッド登録用
///Affinefのメソッドの定義

PyObject* EPAffinef_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPAffinef* newEPAffinef();
EPAffinef* newEPAffinef(Affinef);
EPAffinef* newEPAffinef(Affinef*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPAffinefType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPAffinef(void) ;


PyMODINIT_FUNC initEPAffine(void);
#endif