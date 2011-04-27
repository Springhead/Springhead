
#include "..\..\..\include\EmbPython\Utility\SprEPchar.h"



/////////////////////////////////////メソッド登録用
///charのメソッドの定義
static PyMethodDef EPchar_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPchar_init(EPchar* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPchar_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPchar *self;
	self = ( EPchar*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPchar_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPchar_dealloc(EPchar* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPcharType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.char",             /*tp_name*/
	sizeof(EPchar),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPchar_dealloc,                        /*tp_dealloc*/
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
	"char",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPchar_methods,             /* tp_methods */
	0,//EPchar_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPchar_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPchar_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPchar()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPcharType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPcharType);
	PyModule_AddObject(m,"char",(PyObject *)&EPcharType);
	return m;
}

EPchar* newEPchar()
{
	return (EPchar*)EPchar_new(&EPcharType,NULL,NULL);
}

EPchar* newEPchar(const char* ptr)
{
	EPchar* ret = (EPchar*)EPchar_new(&EPcharType,NULL,NULL);
	ret->ptr = const_cast<char*>(ptr);

	return ret;
}