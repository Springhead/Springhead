
#include "..\..\..\include\EmbPython\Utility\SprEPIfInfo.h"



/////////////////////////////////////メソッド登録用
///IfInfoのメソッドの定義
static PyMethodDef EPIfInfo_methods[] =
{
	{NULL}
};

/////////////////////////////////////////メンバ変数登録用
/*未実装*/

//////////////////////////////////////////init
static int
EPIfInfo_init(EPIfInfo* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPIfInfo_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPIfInfo *self;
	self = ( EPIfInfo*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPIfInfo_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPIfInfo_dealloc(EPIfInfo* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPIfInfoType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.IfInfo",             /*tp_name*/
	sizeof(EPIfInfo),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPIfInfo_dealloc,                        /*tp_dealloc*/
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
	"IfInfo",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPIfInfo_methods,             /* tp_methods */
	0,//EPIfInfo_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPIfInfo_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPIfInfo_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPIfInfo()
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPIfInfoType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPIfInfoType);
	PyModule_AddObject(m,"IfInfo",(PyObject *)&EPIfInfoType);
	return m;
}

EPIfInfo* newEPIfInfo()
{
	return (EPIfInfo*)EPIfInfo_new(&EPIfInfoType,NULL,NULL);
}

EPIfInfo* newEPIfInfo(IfInfo* ptr)
{
	EPIfInfo* ret = (EPIfInfo*)EPIfInfo_new(&EPIfInfoType,NULL,NULL);
	ret->ptr = ptr;

	return ret;
}