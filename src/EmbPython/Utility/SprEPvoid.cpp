
#include "..\..\..\include\EmbPython\Utility\SprEPvoid.h"



/////////////////////////////////////メソッド登録用
///voidのメソッドの定義
static PyMethodDef EPvoid_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPvoid_init(EPvoid* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPvoid_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPvoid *self;
	self = ( EPvoid*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPvoid_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPvoid_dealloc(EPvoid* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPvoidType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Test.void",             /*tp_name*/
	sizeof(EPvoid),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPvoid_dealloc,                        /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE , /*tp_flags*/
	"void",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPvoid_methods,             /* tp_methods */
	0,//EPvoid_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPvoid_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPvoid_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPvoid(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPvoidType ) < 0 ) return NULL ;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPvoidType);
	PyModule_AddObject(m,"void",(PyObject *)&EPvoidType);
	return m;
}

EPvoid* newEPvoid()
{
	return (EPvoid*)EPvoid_new(&EPvoidType,NULL,NULL);
}

EPvoid* newEPvoid (const void* ptr)
{
	EPvoid* ret = (EPvoid*)EPvoid_new(&EPvoidType,NULL,NULL);
	*ret->ptr = ptr;

	return ret;
}