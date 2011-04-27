#include "..\..\..\include\EmbPython\Utility\SprEPUTString.h"



/////////////////////////////////////メソッド登録用
///UTStringのメソッドの定義
static PyMethodDef EPUTString_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPUTString_init(EPUTString* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPUTString_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPUTString *self;
	self = ( EPUTString*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPUTString_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPUTString_dealloc(EPUTString* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPUTStringType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.UTString",             /*tp_name*/
	sizeof(EPUTString),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPUTString_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
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
	"UTString",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPUTString_methods,             /* tp_methods */
	0,//EPUTString_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPUTString_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPUTString_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPUTString()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPUTStringType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPUTStringType);
	PyModule_AddObject(m,"UTString",(PyObject *)&EPUTStringType);
	return m;
}

EPUTString* newEPUTString()
{
	return (EPUTString*)EPUTString_new(&EPUTStringType,NULL,NULL);
}

EPUTString* newEPUTString(UTString ptr)
{
	EPUTString* ret = (EPUTString*)EPUTString_new(&EPUTStringType,NULL,NULL);
	*ret->ptr = ptr;

	return ret;
}

EPUTString* newEPUTString(UTString* ptr)
{
	EPUTString* ret = (EPUTString*)EPUTString_new(&EPUTStringType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}