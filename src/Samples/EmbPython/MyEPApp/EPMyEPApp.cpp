#include "../../../../include/Springhead.h"                 
#include "../../../../include/Python34/Python.h"    
#include "../../../../include/EmbPython/SprEPUtility.h"    
#include "../../../../include/EmbPython/SprEPBase.h"       
#include "../../../../include/EmbPython/SprEPFoundation.h" 
#include "MyEPApp.h"                            
#include "SprEPMyEPApp.h"                       
#pragma warning(disable:4244)                    
//*********** Decl Global variables ***********



//{*********EPSampleApp*******
int __PYDECL EPSampleAppObjectSampleApp( PyObject* self,PyObject* arg,PyObject* kwds )
{
	try
	{
		if(!arg) return 0;
		EPObject_Ptr(self) = new SampleApp();
		return 0;

	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}

static PyMethodDef EPSampleApp_method_table[] =
{
	{NULL}
};
static PyNumberMethods EPSampleApp_math_method_table=
{
	(binaryfunc)NULL,/* __add__ */
	(binaryfunc)NULL,/* __sub__ */
	(binaryfunc)NULL,/* __mul__ */
	(binaryfunc)NULL,/* __mod__ */
	(binaryfunc)NULL,/* __divmod__ */
	(ternaryfunc)NULL,/* __pow__ */
	(unaryfunc)NULL,/* __neg__ */
	(unaryfunc)NULL,/* __pos__ */
	(unaryfunc)NULL,/* __abs__ */
	(inquiry)NULL,/* __bool__ */
	(unaryfunc)NULL,/* __invert__ */
	(binaryfunc)NULL,/* __lshift__ */
	(binaryfunc)NULL,/* __rshift__ */
	(binaryfunc)NULL,/* __and__ */
	(binaryfunc)NULL,/* __xor__ */
	(binaryfunc)NULL,/* __or__ */
	(unaryfunc)NULL,/* __int__ */
	(void *)NULL,/* __reserved__ */
	(unaryfunc)NULL,/* __float__ */
	(binaryfunc)NULL,/* __iadd__ */
	(binaryfunc)NULL,/* __isub__ */
	(binaryfunc)NULL,/* __imul__ */
	(binaryfunc)NULL,/* __imod__ */
	(ternaryfunc)NULL,/* __ipow__ */
	(binaryfunc)NULL,/* __ilshift__ */
	(binaryfunc)NULL,/* __irshift__ */
	(binaryfunc)NULL,/* __iand__ */
	(binaryfunc)NULL,/* __ixor__ */
	(binaryfunc)NULL,/* __ior__ */
	(binaryfunc)NULL,/* __floordiv__ */
	(binaryfunc)NULL,/* __div__ */
	(binaryfunc)NULL,/* __ifloordiv__ */
	(binaryfunc)NULL,/* __itruediv__ */
};
static PyGetSetDef EPSampleApp_getset_table[] =
{
	{NULL}
};
void __PYDECL EPSampleApp_dealloc(PyObject* self)
{
#ifdef DEBUG_OUTPUT
	printf("SampleApp dealloc called (MemoryManager=");
	if( ((EPObject*)self)->mm == EP_MM_SPR ) printf("Springhead)\n");
	else if( ((EPObject*)self)->mm == EP_MM_PY ) printf("Python)\n");
#endif
	if ( ((EPObject*)self)->mm == EP_MM_PY ) delete EPObject_Ptr(self);
	self->ob_type->tp_free(self);
}
PyObject* __PYDECL EPSampleApp_str()
{
	return Py_BuildValue("s","This is EPSampleAppObject.");
}
PyObject* __PYDECL EPSampleApp_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	try
	{
		PyObject* self;
		self = type->tp_alloc(type,0);
		if ( self != NULL )
		{
			EPObject_Ptr(self) = NULL;
			((EPObject*)self)->mm = EP_MM_PY;
			return self;
		}
		return PyErr_NoMemory();
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyTypeObject EPSampleAppType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"MyEPApp.SampleApp",/*tp_name*/
	sizeof(EPObject),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPSampleApp_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	&EPSampleApp_math_method_table,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_call*/
	0,/*tp_hash*/
	(reprfunc)EPSampleApp_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"SampleApp",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPSampleApp_method_table,/*tp_methods*/
	0,/*tp_members*/
	EPSampleApp_getset_table,/*tp_getset*/
	&EPObjectType,
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPSampleAppObjectSampleApp,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPSampleApp_new,/*tp_new*/
};
void initEPSampleApp(PyObject *rootModule)
{
	if ( PyType_Ready( &EPSampleAppType ) < 0 ) return ;//PythonƒNƒ‰ƒX‚Ìì¬
	string package;
	if(rootModule) package = PyModule_GetName(rootModule);
	else // rootModule‚ª“n‚³‚ê‚½ê‡‚ÍEP_MODULE_NAME‚Í–³Ž‹‚³‚ê‚é
	{
#ifdef EP_MODULE_NAME
		package = EP_MODULE_NAME ".";
		rootModule = PyImport_AddModule( EP_MODULE_NAME );
#else
		package = "";
		rootModule = PyImport_AddModule("__main__");
#endif
	}
#ifdef EP_USE_SUBMODULE
	PyObject *subModule = PyImport_AddModule( (package+"MyEPApp").c_str() );
	Py_INCREF(subModule);
	PyModule_AddObject(rootModule,"MyEPApp",subModule);
#else
	PyObject *subModule = rootModule;
#endif
	Py_INCREF(&EPSampleAppType);
	PyModule_AddObject(subModule,"SampleApp",(PyObject*)&EPSampleAppType);
}
PyObject* newEPSampleApp(const SampleApp* org)
{
	try
	{
		if(org == NULL)
		{
			Py_RETURN_NONE;
		}
		PyObject *ret = EPSampleApp_new(&EPSampleAppType,NULL,NULL);
		EPObject_Ptr(ret) = org;
		((EPObject*)ret)->mm = EP_MM_SPR;
		return ret;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
void toEPSampleApp( EPObject* obj)
{
	obj->ob_base.ob_type = &EPSampleAppType;
}
//}SampleApp

//{*********EPHogeshi*******
int __PYDECL EPHogeshi_Hogeshi( PyObject* self,PyObject* arg,PyObject* kwds )
{
	try
	{
		UTAutoLock LOCK(EPCriticalSection);

		if(!arg) return 0;
		EPObject_Ptr(self) = new Hogeshi();
		if(EPObject_Ptr(self) != NULL) return 0;
		else
		{
			PyErr_NoMemory();
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
static PyObject* __PYDECL EPHogeshi_get_fuga(PyObject* self)
{
	float c_fuga = EPObject_Cast(self,Hogeshi)->fuga;
	PyObject* py_fuga = PyFloat_fromAny(c_fuga);
	return py_fuga;
}
static int __PYDECL EPHogeshi_set_fuga(PyObject* self, PyObject* arg)
{
	float &c_fuga = EPObject_Cast(self,Hogeshi)->fuga;
	PyObject * py_arg = arg;
	float c_arg = PyObject_asDouble(py_arg);

	c_fuga = c_arg;
	return 0;
}
static PyObject* __PYDECL EPHogeshi_get_guho(PyObject* self)
{
	double c_guho = EPObject_Cast(self,Hogeshi)->guho;
	PyObject* py_guho = PyFloat_fromAny(c_guho);
	return py_guho;
}
static int __PYDECL EPHogeshi_set_guho(PyObject* self, PyObject* arg)
{
	double &c_guho = EPObject_Cast(self,Hogeshi)->guho;
	PyObject * py_arg = arg;
	double c_arg = PyObject_asDouble(py_arg);

	c_guho = c_arg;
	return 0;
}
static PyObject* __PYDECL EPHogeshi_get_piyo(PyObject* self)
{
	int c_piyo = EPObject_Cast(self,Hogeshi)->piyo;
	PyObject* py_piyo = PyLong_fromAny(c_piyo);
	return py_piyo;
}
static int __PYDECL EPHogeshi_set_piyo(PyObject* self, PyObject* arg)
{
	int &c_piyo = (int)EPObject_Cast(self,Hogeshi)->piyo;
	PyObject * py_arg = arg;
	int c_arg = PyObject_asLong(py_arg);

	c_piyo = c_arg;
	return 0;
}
static PyObject* __PYDECL EPHogeshi_get_bOfu(PyObject* self)
{
	bool c_bOfu = EPObject_Cast(self,Hogeshi)->bOfu;
	PyObject* py_bOfu = PyBool_FromLong(c_bOfu? 1 : 0);
	return py_bOfu;
}
static int __PYDECL EPHogeshi_set_bOfu(PyObject* self, PyObject* arg)
{
	bool &c_bOfu = EPObject_Cast(self,Hogeshi)->bOfu;
	PyObject * py_arg = arg;
	bool c_arg = (Py_True == py_arg);

	c_bOfu = c_arg;
	return 0;
}
static PyMethodDef EPHogeshi_method_table[] =
{
	{NULL}
};
static PyNumberMethods EPHogeshi_math_method_table=
{
	(binaryfunc)NULL,/* __add__ */
	(binaryfunc)NULL,/* __sub__ */
	(binaryfunc)NULL,/* __mul__ */
	(binaryfunc)NULL,/* __mod__ */
	(binaryfunc)NULL,/* __divmod__ */
	(ternaryfunc)NULL,/* __pow__ */
	(unaryfunc)NULL,/* __neg__ */
	(unaryfunc)NULL,/* __pos__ */
	(unaryfunc)NULL,/* __abs__ */
	(inquiry)NULL,/* __bool__ */
	(unaryfunc)NULL,/* __invert__ */
	(binaryfunc)NULL,/* __lshift__ */
	(binaryfunc)NULL,/* __rshift__ */
	(binaryfunc)NULL,/* __and__ */
	(binaryfunc)NULL,/* __xor__ */
	(binaryfunc)NULL,/* __or__ */
	(unaryfunc)NULL,/* __int__ */
	(void *)NULL,/* __reserved__ */
	(unaryfunc)NULL,/* __float__ */
	(binaryfunc)NULL,/* __iadd__ */
	(binaryfunc)NULL,/* __isub__ */
	(binaryfunc)NULL,/* __imul__ */
	(binaryfunc)NULL,/* __imod__ */
	(ternaryfunc)NULL,/* __ipow__ */
	(binaryfunc)NULL,/* __ilshift__ */
	(binaryfunc)NULL,/* __irshift__ */
	(binaryfunc)NULL,/* __iand__ */
	(binaryfunc)NULL,/* __ixor__ */
	(binaryfunc)NULL,/* __ior__ */
	(binaryfunc)NULL,/* __floordiv__ */
	(binaryfunc)NULL,/* __div__ */
	(binaryfunc)NULL,/* __ifloordiv__ */
	(binaryfunc)NULL,/* __itruediv__ */
};
static PyGetSetDef EPHogeshi_getset_table[] =
{
	{"fuga",(getter)EPHogeshi_get_fuga,(setter)EPHogeshi_set_fuga,"member (float)fuga of Hogeshi",NULL},
	{"guho",(getter)EPHogeshi_get_guho,(setter)EPHogeshi_set_guho,"member (double)guho of Hogeshi",NULL},
	{"piyo",(getter)EPHogeshi_get_piyo,(setter)EPHogeshi_set_piyo,"member (int)piyo of Hogeshi",NULL},
	{"bOfu",(getter)EPHogeshi_get_bOfu,(setter)EPHogeshi_set_bOfu,"member (bool)bOfu of Hogeshi",NULL},
	{NULL}
};
void __PYDECL EPHogeshi_dealloc(PyObject* self)
{
#ifdef DEBUG_OUTPUT
	printf("Hogeshi dealloc called (MemoryManager=");
	if( ((EPObject*)self)->mm == EP_MM_SPR ) printf("Springhead)\n");
	else if( ((EPObject*)self)->mm == EP_MM_PY ) printf("Python)\n");
#endif
	if ( ((EPObject*)self)->mm == EP_MM_PY ) delete EPObject_Ptr(self);
	self->ob_type->tp_free(self);
}
PyObject* __PYDECL EPHogeshi_str()
{
	return Py_BuildValue("s","This is EPHogeshiObject.");
}
PyObject* __PYDECL EPHogeshi_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	try
	{
		PyObject* self;
		self = type->tp_alloc(type,0);
		if ( self != NULL )
		{
			EPObject_Ptr(self) = NULL;
			((EPObject*)self)->mm = EP_MM_PY;
			return self;
		}
		return PyErr_NoMemory();
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyTypeObject EPHogeshiType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"MyEPApp.Hogeshi",/*tp_name*/
	sizeof(EPObject),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPHogeshi_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	&EPHogeshi_math_method_table,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_call*/
	0,/*tp_hash*/
	(reprfunc)EPHogeshi_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Hogeshi",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPHogeshi_method_table,/*tp_methods*/
	0,/*tp_members*/
	EPHogeshi_getset_table,/*tp_getset*/
	&EPObjectType,
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPHogeshi_Hogeshi,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPHogeshi_new,/*tp_new*/
};
void initEPHogeshi(PyObject *rootModule)
{
	if ( PyType_Ready( &EPHogeshiType ) < 0 ) return ;//PythonƒNƒ‰ƒX‚Ìì¬
	string package;
	if(rootModule) package = PyModule_GetName(rootModule);
	else // rootModule‚ª“n‚³‚ê‚½ê‡‚ÍEP_MODULE_NAME‚Í–³Ž‹‚³‚ê‚é
	{
#ifdef EP_MODULE_NAME
		package = EP_MODULE_NAME ".";
		rootModule = PyImport_AddModule( EP_MODULE_NAME );
#else
		package = "";
		rootModule = PyImport_AddModule("__main__");
#endif
	}
#ifdef EP_USE_SUBMODULE
	PyObject *subModule = PyImport_AddModule( (package+"MyEPApp").c_str() );
	Py_INCREF(subModule);
	PyModule_AddObject(rootModule,"MyEPApp",subModule);
#else
	PyObject *subModule = rootModule;
#endif
	Py_INCREF(&EPHogeshiType);
	PyModule_AddObject(subModule,"Hogeshi",(PyObject*)&EPHogeshiType);
}
PyObject* newEPHogeshi(const struct Hogeshi* org)
{
	try
	{
		if(org == NULL)
		{
			Py_RETURN_NONE;
		}
		PyObject *ret = EPHogeshi_new(&EPHogeshiType,NULL,NULL);
		EPObject_Ptr(ret) = org;
		((EPObject*)ret)->mm = EP_MM_SPR;
		return ret;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
void toEPHogeshi( EPObject* obj)
{
	obj->ob_base.ob_type = &EPHogeshiType;
}
//}Hogeshi

//{*********EPMyEPApp*******
int __PYDECL EPMyEPAppObjectMyEPApp( PyObject* self,PyObject* arg,PyObject* kwds )
{
	try
	{
		if(!arg) return 0;
		EPObject_Ptr(self) = new MyEPApp();
		return 0;

	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyObject* __PYDECL EPMyEPApp_Drop( PyObject* self,PyObject* arg )
{
	try
	{
		UTAutoLock LOCK(EPCriticalSection);

		if( EPObject_Ptr(self) == NULL )
		{
			PyErr_SetString( PyErr_Spr_NullReference , "Null Reference in MyEPApp.Drop");
			return NULL;
		}
		if(arg && PyTuple_Size(arg) == 6&&(PyFloat_Check((PyTuple_GetItem(arg,0))) || PyLong_Check((PyTuple_GetItem(arg,0)))) && (PyFloat_Check((PyTuple_GetItem(arg,1))) || PyLong_Check((PyTuple_GetItem(arg,1)))) && (EPVec3d_Check((PyTuple_GetItem(arg,2))) || EPVec3f_Check((PyTuple_GetItem(arg,2)))) && (EPVec3d_Check((PyTuple_GetItem(arg,3))) || EPVec3f_Check((PyTuple_GetItem(arg,3)))) && (EPVec3d_Check((PyTuple_GetItem(arg,4))) || EPVec3f_Check((PyTuple_GetItem(arg,4)))) && EPQuaterniond_Check((PyTuple_GetItem(arg,5))))
		{
			PyObject * py_param1 = (PyTuple_GetItem(arg,0));
			int c_param1 = PyObject_asLong(py_param1);

			PyObject * py_param2 = (PyTuple_GetItem(arg,1));
			int c_param2 = PyObject_asLong(py_param2);

			PyObject * py_param3 = (PyTuple_GetItem(arg,2));
			Vec3d c_param3 = (*(&PyObject_asVec3d(py_param3)));

			PyObject * py_param4 = (PyTuple_GetItem(arg,3));
			Vec3d c_param4 = (*(&PyObject_asVec3d(py_param4)));

			PyObject * py_param5 = (PyTuple_GetItem(arg,4));
			Vec3d c_param5 = (*(&PyObject_asVec3d(py_param5)));

			PyObject * py_param6 = (PyTuple_GetItem(arg,5));
			Quaterniond c_param6 = (*EPObject_Cast(py_param6,Quaterniond));

			EPObject_Cast(self,MyEPApp)->Drop(c_param1,c_param2,c_param3,c_param4,c_param5,c_param6);
			Py_RETURN_NONE;
		}
		PyErr_BadArgument();
		return NULL;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyObject* __PYDECL EPMyEPApp_OnAction( PyObject* self,PyObject* arg )
{
	try
	{
		UTAutoLock LOCK(EPCriticalSection);

		if( EPObject_Ptr(self) == NULL )
		{
			PyErr_SetString( PyErr_Spr_NullReference , "Null Reference in MyEPApp.OnAction");
			return NULL;
		}
		if(arg && PyTuple_Size(arg) == 2&&(PyFloat_Check((PyTuple_GetItem(arg,0))) || PyLong_Check((PyTuple_GetItem(arg,0)))) && (PyFloat_Check((PyTuple_GetItem(arg,1))) || PyLong_Check((PyTuple_GetItem(arg,1)))))
		{
			PyObject * py_param1 = (PyTuple_GetItem(arg,0));
			int c_param1 = PyObject_asLong(py_param1);

			PyObject * py_param2 = (PyTuple_GetItem(arg,1));
			int c_param2 = PyObject_asLong(py_param2);

			EPObject_Cast(self,MyEPApp)->OnAction(c_param1,c_param2);
			Py_RETURN_NONE;
		}
		PyErr_BadArgument();
		return NULL;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyObject* __PYDECL EPMyEPApp_OnDraw( PyObject* self,PyObject* arg )
{
	try
	{
		UTAutoLock LOCK(EPCriticalSection);

		if( EPObject_Ptr(self) == NULL )
		{
			PyErr_SetString( PyErr_Spr_NullReference , "Null Reference in MyEPApp.OnDraw");
			return NULL;
		}
		if(EPGRRenderIf_Check(arg))
		{
			PyObject * py_param1 = arg;
			GRRenderIf * c_param1 = EPObject_Cast(py_param1,GRRenderIf);

			EPObject_Cast(self,MyEPApp)->OnDraw(c_param1);
			Py_RETURN_NONE;
		}
		PyErr_BadArgument();
		return NULL;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyObject* __PYDECL EPMyEPApp_OnStep( PyObject* self )
{
	try
	{
		UTAutoLock LOCK(EPCriticalSection);

		if( EPObject_Ptr(self) == NULL )
		{
			PyErr_SetString( PyErr_Spr_NullReference , "Null Reference in MyEPApp.OnStep");
			return NULL;
		}
		if(true)
		{
			EPObject_Cast(self,MyEPApp)->OnStep();
			Py_RETURN_NONE;
		}
		PyErr_BadArgument();
		return NULL;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
static PyObject* __PYDECL EPMyEPApp_get_vhoge(PyObject* self)
{
	std::vector< Hogeshi > c_vhoge = EPObject_Cast(self,MyEPApp)->vhoge;
	int size = c_vhoge.size();
	PyObject* py_vhoge = PyList_New(c_vhoge.size());
	for( int i = 0 ; i < size ; i++)
	{
		Hogeshi c_item = c_vhoge[i];
		PyObject* py_item = newEPHogeshi(c_item);
		PyList_SetItem(py_vhoge, i ,py_item);
	}
	return py_vhoge;
}
static int __PYDECL EPMyEPApp_set_vhoge(PyObject* self, PyObject* arg)
{
	std::vector< Hogeshi > &c_vhoge = EPObject_Cast(self,MyEPApp)->vhoge;
	PyObject * py_arg = arg;
	std::vector< Hogeshi > c_arg;
	if( PyList_Check(py_arg) )
	{
		int size = PyList_GET_SIZE(py_arg);
		for( int i = 0 ; i < size ; i++)
		{
			PyObject* py_item = PyList_GetItem(py_arg,i);
			Hogeshi c_item = (*EPObject_Cast(py_item,Hogeshi));
			c_arg.push_back(c_item);
		}
	}
	c_vhoge = c_arg;
	return 0;
}
static PyMethodDef EPMyEPApp_method_table[] =
{
	{"Drop",(PyCFunction)EPMyEPApp_Drop,METH_VARARGS ,"EPMyEPApp::Drop"},
	{"OnAction",(PyCFunction)EPMyEPApp_OnAction,METH_VARARGS ,"EPMyEPApp::OnAction"},
	{"OnDraw",(PyCFunction)EPMyEPApp_OnDraw,METH_O ,"EPMyEPApp::OnDraw"},
	{"OnStep",(PyCFunction)EPMyEPApp_OnStep,METH_NOARGS ,"EPMyEPApp::OnStep"},
	{NULL}
};
static PyNumberMethods EPMyEPApp_math_method_table=
{
	(binaryfunc)NULL,/* __add__ */
	(binaryfunc)NULL,/* __sub__ */
	(binaryfunc)NULL,/* __mul__ */
	(binaryfunc)NULL,/* __mod__ */
	(binaryfunc)NULL,/* __divmod__ */
	(ternaryfunc)NULL,/* __pow__ */
	(unaryfunc)NULL,/* __neg__ */
	(unaryfunc)NULL,/* __pos__ */
	(unaryfunc)NULL,/* __abs__ */
	(inquiry)NULL,/* __bool__ */
	(unaryfunc)NULL,/* __invert__ */
	(binaryfunc)NULL,/* __lshift__ */
	(binaryfunc)NULL,/* __rshift__ */
	(binaryfunc)NULL,/* __and__ */
	(binaryfunc)NULL,/* __xor__ */
	(binaryfunc)NULL,/* __or__ */
	(unaryfunc)NULL,/* __int__ */
	(void *)NULL,/* __reserved__ */
	(unaryfunc)NULL,/* __float__ */
	(binaryfunc)NULL,/* __iadd__ */
	(binaryfunc)NULL,/* __isub__ */
	(binaryfunc)NULL,/* __imul__ */
	(binaryfunc)NULL,/* __imod__ */
	(ternaryfunc)NULL,/* __ipow__ */
	(binaryfunc)NULL,/* __ilshift__ */
	(binaryfunc)NULL,/* __irshift__ */
	(binaryfunc)NULL,/* __iand__ */
	(binaryfunc)NULL,/* __ixor__ */
	(binaryfunc)NULL,/* __ior__ */
	(binaryfunc)NULL,/* __floordiv__ */
	(binaryfunc)NULL,/* __div__ */
	(binaryfunc)NULL,/* __ifloordiv__ */
	(binaryfunc)NULL,/* __itruediv__ */
};
static PyGetSetDef EPMyEPApp_getset_table[] =
{
	{"vhoge",(getter)EPMyEPApp_get_vhoge,(setter)EPMyEPApp_set_vhoge,"member (std::vector< Hogeshi >)vhoge of MyEPApp",NULL},
	{NULL}
};
void __PYDECL EPMyEPApp_dealloc(PyObject* self)
{
#ifdef DEBUG_OUTPUT
	printf("MyEPApp dealloc called (MemoryManager=");
	if( ((EPObject*)self)->mm == EP_MM_SPR ) printf("Springhead)\n");
	else if( ((EPObject*)self)->mm == EP_MM_PY ) printf("Python)\n");
#endif
	if ( ((EPObject*)self)->mm == EP_MM_PY ) delete EPObject_Ptr(self);
	self->ob_type->tp_free(self);
}
PyObject* __PYDECL EPMyEPApp_str()
{
	return Py_BuildValue("s","This is EPMyEPAppObject.");
}
PyObject* __PYDECL EPMyEPApp_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	try
	{
		PyObject* self;
		self = type->tp_alloc(type,0);
		if ( self != NULL )
		{
			EPObject_Ptr(self) = NULL;
			((EPObject*)self)->mm = EP_MM_PY;
			return self;
		}
		return PyErr_NoMemory();
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
PyTypeObject EPMyEPAppType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"MyEPApp.MyEPApp",/*tp_name*/
	sizeof(EPObject),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPMyEPApp_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	&EPMyEPApp_math_method_table,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_call*/
	0,/*tp_hash*/
	(reprfunc)EPMyEPApp_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"MyEPApp",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPMyEPApp_method_table,/*tp_methods*/
	0,/*tp_members*/
	EPMyEPApp_getset_table,/*tp_getset*/
	&EPSampleAppType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPMyEPAppObjectMyEPApp,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPMyEPApp_new,/*tp_new*/
};
void initEPMyEPApp(PyObject *rootModule)
{
	if ( PyType_Ready( &EPMyEPAppType ) < 0 ) return ;//PythonƒNƒ‰ƒX‚Ìì¬
	string package;
	if(rootModule) package = PyModule_GetName(rootModule);
	else // rootModule‚ª“n‚³‚ê‚½ê‡‚ÍEP_MODULE_NAME‚Í–³Ž‹‚³‚ê‚é
	{
#ifdef EP_MODULE_NAME
		package = EP_MODULE_NAME ".";
		rootModule = PyImport_AddModule( EP_MODULE_NAME );
#else
		package = "";
		rootModule = PyImport_AddModule("__main__");
#endif
	}
#ifdef EP_USE_SUBMODULE
	PyObject *subModule = PyImport_AddModule( (package+"MyEPApp").c_str() );
	Py_INCREF(subModule);
	PyModule_AddObject(rootModule,"MyEPApp",subModule);
#else
	PyObject *subModule = rootModule;
#endif
	Py_INCREF(&EPMyEPAppType);
	PyModule_AddObject(subModule,"MyEPApp",(PyObject*)&EPMyEPAppType);
}
PyObject* newEPMyEPApp(const MyEPApp* org)
{
	try
	{
		if(org == NULL)
		{
			Py_RETURN_NONE;
		}
		PyObject *ret = EPMyEPApp_new(&EPMyEPAppType,NULL,NULL);
		EPObject_Ptr(ret) = org;
		((EPObject*)ret)->mm = EP_MM_SPR;
		return ret;
	}
	catch (const std::exception& e)
	{
		PyErr_SetString(PyErr_Spr_OSException, const_cast<char *>(e.what()));
		return NULL;
	}
}
void toEPMyEPApp( EPObject* obj)
{
	obj->ob_base.ob_type = &EPMyEPAppType;
}
//}MyEPApp

PyObject* newEPHogeshi(Hogeshi var1)
{
	PyObject *ret = EPHogeshi_new(&EPHogeshiType,NULL,NULL);
	EPObject_Ptr(ret) = new Hogeshi(var1);
	((EPObject*)ret)->mm = EP_MM_PY;
	return ret;
}

PyObject* newEPHogeshi()
{
	PyObject *ret = EPHogeshi_new(&EPHogeshiType,NULL,NULL);
	EPObject_Ptr(ret) = new Hogeshi();
	((EPObject*)ret)->mm = EP_MM_PY;
	return ret;
}
/**************** for Module ******************/
void initMyEPApp(PyObject *rootModule)
{
	initEPSampleApp(rootModule);
	initEPHogeshi(rootModule);
	initEPMyEPApp(rootModule);
}
