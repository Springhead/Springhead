
#include "..\..\..\include\EmbPython\Utility\SprEPunsignedint.h"



/////////////////////////////////////メソッド登録用
///unsignedintのメソッドの定義
static PyMethodDef EPunsignedint_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPunsignedint_init(EPunsignedint* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPunsignedint_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPunsignedint *self;
	self = ( EPunsignedint*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPunsignedint_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPunsignedint_dealloc(EPunsignedint* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPunsignedintType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.unsignedint",             /*tp_name*/
	sizeof(EPunsignedint),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPunsignedint_dealloc,                        /*tp_dealloc*/
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
	"unsignedint",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPunsignedint_methods,             /* tp_methods */
	0,//EPunsignedint_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPunsignedint_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPunsignedint_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPunsignedint()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPunsignedintType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPunsignedintType);
	PyModule_AddObject(m,"unsignedint",(PyObject *)&EPunsignedintType);
	return m;
}

EPunsignedint* newEPunsignedint()
{
	return (EPunsignedint*)EPunsignedint_new(&EPunsignedintType,NULL,NULL);
}

EPunsignedint* newEPunsignedint(unsigned int* ptr)
{
	EPunsignedint* ret = (EPunsignedint*)EPunsignedint_new(&EPunsignedintType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}

EPunsignedint* newEPunsignedint(unsigned int ptr)
{
	EPunsignedint* ret = (EPunsignedint*)EPunsignedint_new(&EPunsignedintType,NULL,NULL);
	*ret->ptr = ptr;

	return ret;
}