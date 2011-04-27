
#include "..\..\..\include\EmbPython\Utility\SprEPunsignedlong.h"



/////////////////////////////////////メソッド登録用
///unsignedlongのメソッドの定義
static PyMethodDef EPunsignedlong_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static long
EPunsignedlong_init(EPunsignedlong* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPunsignedlong_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPunsignedlong *self;
	self = ( EPunsignedlong*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPunsignedlong_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPunsignedlong_dealloc(EPunsignedlong* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPunsignedlongType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.unsignedlong",             /*tp_name*/
	sizeof(EPunsignedlong),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPunsignedlong_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_prlong*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"unsignedlong",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPunsignedlong_methods,             /* tp_methods */
	0,//EPunsignedlong_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPunsignedlong_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPunsignedlong_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPunsignedlong()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPunsignedlongType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPunsignedlongType);
	PyModule_AddObject(m,"unsignedlong",(PyObject *)&EPunsignedlongType);
	return m;
}

EPunsignedlong* newEPunsignedlong()
{
	return (EPunsignedlong*)EPunsignedlong_new(&EPunsignedlongType,NULL,NULL);
}

EPunsignedlong* newEPunsignedlong(unsigned long* ptr)
{
	EPunsignedlong* ret = (EPunsignedlong*)EPunsignedlong_new(&EPunsignedlongType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}

EPunsignedlong* newEPunsignedlong(unsigned long ptr)
{
	EPunsignedlong* ret = (EPunsignedlong*)EPunsignedlong_new(&EPunsignedlongType,NULL,NULL);
	*ret->ptr = ptr;

	return ret;
}