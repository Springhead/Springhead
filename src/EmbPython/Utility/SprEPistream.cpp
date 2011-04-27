
#include "..\..\..\include\EmbPython\Utility\SprEPistream.h"



/////////////////////////////////////メソッド登録用
///istreamのメソッドの定義
static PyMethodDef EPistream_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPistream_init(EPistream* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPistream_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPistream *self;
	self = ( EPistream*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPistream_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPistream_dealloc(EPistream* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPistreamType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.istream",             /*tp_name*/
	sizeof(EPistream),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPistream_dealloc,                        /*tp_dealloc*/
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
	"istream",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPistream_methods,             /* tp_methods */
	0,//EPistream_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPistream_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPistream_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPistream()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPistreamType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPistreamType);
	PyModule_AddObject(m,"istream",(PyObject *)&EPistreamType);
	return m;
}

EPistream* newEPistream()
{
	return (EPistream*)EPistream_new(&EPistreamType,NULL,NULL);
}

EPistream* newEPistream(std::istream* ptr)
{
	EPistream* ret = (EPistream*)EPistream_new(&EPistreamType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}