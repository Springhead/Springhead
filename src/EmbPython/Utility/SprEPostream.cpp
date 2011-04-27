
#include "..\..\..\include\EmbPython\Utility\SprEPostream.h"



/////////////////////////////////////メソッド登録用
///ostreamのメソッドの定義
static PyMethodDef EPostream_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPostream_init(EPostream* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPostream_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPostream *self;
	self = ( EPostream*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPostream_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPostream_dealloc(EPostream* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPostreamType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.ostream",             /*tp_name*/
	sizeof(EPostream),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPostream_dealloc,                        /*tp_dealloc*/
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
	"ostream",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPostream_methods,             /* tp_methods */
	0,//EPostream_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPostream_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPostream_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPostream()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPostreamType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPostreamType);
	PyModule_AddObject(m,"ostream",(PyObject *)&EPostreamType);
	return m;
}

EPostream* newEPostream()
{
	return (EPostream*)EPostream_new(&EPostreamType,NULL,NULL);
}

EPostream* newEPostream(std::ostream* ptr)
{
	EPostream* ret = (EPostream*)EPostream_new(&EPostreamType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}