
#include "..\..\..\include\EmbPython\Utility\SprEPstring.h"



/////////////////////////////////////メソッド登録用
///stringのメソッドの定義
static PyMethodDef EPstring_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPstring_init(EPstring* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPstring_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPstring *self;
	self = ( EPstring*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPstring_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPstring_dealloc(EPstring* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPstringType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.string",             /*tp_name*/
	sizeof(EPstring),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPstring_dealloc,                        /*tp_dealloc*/
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
	"string",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPstring_methods,             /* tp_methods */
	0,//EPstring_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPstring_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPstring_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPstring()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPstringType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPstringType);
	PyModule_AddObject(m,"string",(PyObject *)&EPstringType);
	return m;
}

EPstring* newEPstring()
{
	return (EPstring*)EPstring_new(&EPstringType,NULL,NULL);
}

EPstring* newEPstring(std::string ptr)
{
	EPstring* ret = (EPstring*)EPstring_new(&EPstringType,NULL,NULL);
	*ret->ptr = ptr;

	return ret;
}
EPstring* newEPstring(std::string* ptr)
{
	EPstring* ret = (EPstring*)EPstring_new(&EPstringType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}