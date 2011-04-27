#include "../../include\EmbPython\SprEPFoundation.h" 

//{*********EPObjectIf*******
PyObject* EPObjectIf_AddChildObject( EPObjectIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->AddChildObject((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_AddRef( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->AddRef());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_Cast( EPObjectIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(ObjectIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_Clear( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_ConstructState( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		self->ptr->ConstructState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_CreateObject( EPObjectIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPvoid_Check(((EPvoid*)PyTuple_GetItem(tuple,1))))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->CreateObject((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(((EPvoid*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_DelChildObject( EPObjectIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->DelChildObject((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_DelRef( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->DelRef());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_DestructState( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		self->ptr->DestructState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_DumpObjectR( EPObjectIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPostream_Check(((EPostream*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->DumpObjectR((*(((EPostream*)PyTuple_GetItem(tuple,0))->ptr)),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPostream_Check(((EPostream*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->DumpObjectR((*(((EPostream*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetChildObject( EPObjectIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetChildObject(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetChildObject(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetDesc( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->GetDesc((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetDescAddress( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPvoid* ret = (EPvoid*)newEPvoid();
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetDescSize( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->GetDescSize());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetObjectIf( EPObjectIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetObjectIf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetObjectIf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetState( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->GetState((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetStateAddress( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPvoid* ret = (EPvoid*)newEPvoid();
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_GetStateSize( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->GetStateSize());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_NChildObject( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->NChildObject());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_Print( EPObjectIf* self,EPostream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPostream_Check(var1))
	{
		self->ptr->Print((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_PrintShort( EPObjectIf* self,EPostream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPostream_Check(var1))
	{
		self->ptr->PrintShort((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_ReadState( EPObjectIf* self,EPstring* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPstring_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->ReadState((*(var1->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_ReadStateR( EPObjectIf* self,EPistream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPistream_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->ReadStateR((*(var1->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_RefCount( EPObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->RefCount());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_SetDesc( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		self->ptr->SetDesc((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_SetState( EPObjectIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		self->ptr->SetState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_WriteState( EPObjectIf* self,EPstring* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPstring_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->WriteState((*(var1->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIf_WriteStateR( EPObjectIf* self,EPostream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPostream_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->WriteStateR((*(var1->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPObjectIf_methods[] =
{
	{"AddChildObject",(PyCFunction)EPObjectIf_AddChildObject,METH_O,"EPObjectIf::AddChildObject"},
	{"AddRef",(PyCFunction)EPObjectIf_AddRef,METH_NOARGS,"EPObjectIf::AddRef"},
	{"Cast",(PyCFunction)EPObjectIf_Cast,METH_O,"EPObjectIf::Cast"},
	{"Clear",(PyCFunction)EPObjectIf_Clear,METH_NOARGS,"EPObjectIf::Clear"},
	{"ConstructState",(PyCFunction)EPObjectIf_ConstructState,METH_O,"EPObjectIf::ConstructState"},
	{"CreateObject",(PyCFunction)EPObjectIf_CreateObject,METH_VARARGS,"EPObjectIf::CreateObject"},
	{"DelChildObject",(PyCFunction)EPObjectIf_DelChildObject,METH_O,"EPObjectIf::DelChildObject"},
	{"DelRef",(PyCFunction)EPObjectIf_DelRef,METH_NOARGS,"EPObjectIf::DelRef"},
	{"DestructState",(PyCFunction)EPObjectIf_DestructState,METH_O,"EPObjectIf::DestructState"},
	{"DumpObjectR",(PyCFunction)EPObjectIf_DumpObjectR,METH_VARARGS,"EPObjectIf::DumpObjectR"},
	{"GetChildObject",(PyCFunction)EPObjectIf_GetChildObject,METH_VARARGS,"EPObjectIf::GetChildObject"},
	{"GetDesc",(PyCFunction)EPObjectIf_GetDesc,METH_O,"EPObjectIf::GetDesc"},
	{"GetDescAddress",(PyCFunction)EPObjectIf_GetDescAddress,METH_NOARGS,"EPObjectIf::GetDescAddress"},
	{"GetDescSize",(PyCFunction)EPObjectIf_GetDescSize,METH_NOARGS,"EPObjectIf::GetDescSize"},
	{"GetObjectIf",(PyCFunction)EPObjectIf_GetObjectIf,METH_VARARGS,"EPObjectIf::GetObjectIf"},
	{"GetState",(PyCFunction)EPObjectIf_GetState,METH_O,"EPObjectIf::GetState"},
	{"GetStateAddress",(PyCFunction)EPObjectIf_GetStateAddress,METH_NOARGS,"EPObjectIf::GetStateAddress"},
	{"GetStateSize",(PyCFunction)EPObjectIf_GetStateSize,METH_NOARGS,"EPObjectIf::GetStateSize"},
	{"NChildObject",(PyCFunction)EPObjectIf_NChildObject,METH_NOARGS,"EPObjectIf::NChildObject"},
	{"Print",(PyCFunction)EPObjectIf_Print,METH_O,"EPObjectIf::Print"},
	{"PrintShort",(PyCFunction)EPObjectIf_PrintShort,METH_O,"EPObjectIf::PrintShort"},
	{"ReadState",(PyCFunction)EPObjectIf_ReadState,METH_O,"EPObjectIf::ReadState"},
	{"ReadStateR",(PyCFunction)EPObjectIf_ReadStateR,METH_O,"EPObjectIf::ReadStateR"},
	{"RefCount",(PyCFunction)EPObjectIf_RefCount,METH_NOARGS,"EPObjectIf::RefCount"},
	{"SetDesc",(PyCFunction)EPObjectIf_SetDesc,METH_O,"EPObjectIf::SetDesc"},
	{"SetState",(PyCFunction)EPObjectIf_SetState,METH_O,"EPObjectIf::SetState"},
	{"WriteState",(PyCFunction)EPObjectIf_WriteState,METH_O,"EPObjectIf::WriteState"},
	{"WriteStateR",(PyCFunction)EPObjectIf_WriteStateR,METH_O,"EPObjectIf::WriteStateR"},
	{NULL}
};
void EPObjectIf_dealloc(PyObject* self)
{
}
PyObject* EPObjectIf_str()
{
	return Py_BuildValue("s","This is EPObjectIf.");
}
int EPObjectIf_init(EPObjectIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPObjectIf* EPObjectIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPObjectIf *self;
	self = (EPObjectIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPObjectIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPObjectIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPObjectIf",/*tp_name*/
	sizeof(EPObjectIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPObjectIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPObjectIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"ObjectIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPObjectIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPObjectIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPObjectIf_new,/*tp_new*/
};
void initEPObjectIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPObjectIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPObjectIfType);
	PyModule_AddObject(m,"ObjectIf",(PyObject *)&EPObjectIfType);//モジュールに追加
}
PyObject* newEPObjectIf(struct ObjectIf org)
{
	EPObjectIf *ret = EPObjectIf_new(&EPObjectIfType,NULL,NULL);
	ret->ptr = new ObjectIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPObjectIf(struct ObjectIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPObjectIf *ret = EPObjectIf_new(&EPObjectIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}ObjectIf

//{*********EPObjectIfs*******
PyObject* EPObjectIfs_Pop( EPObjectIfs* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Pop();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIfs_Push( EPObjectIfs* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		self->ptr->Push((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectIfs_Top( EPObjectIfs* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->Top());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPObjectIfs_methods[] =
{
	{"Pop",(PyCFunction)EPObjectIfs_Pop,METH_NOARGS,"EPObjectIfs::Pop"},
	{"Push",(PyCFunction)EPObjectIfs_Push,METH_O,"EPObjectIfs::Push"},
	{"Top",(PyCFunction)EPObjectIfs_Top,METH_NOARGS,"EPObjectIfs::Top"},
	{NULL}
};
void EPObjectIfs_dealloc(PyObject* self)
{
	delete ((EPObjectIfs*)self)->ptr;
}
PyObject* EPObjectIfs_str()
{
	return Py_BuildValue("s","This is EPObjectIfs.");
}
int EPObjectIfs_init(EPObjectIfs* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPObjectIfs* EPObjectIfs_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPObjectIfs *self;
	self = (EPObjectIfs*) type->tp_alloc(type,0);
	if ( self != NULL ) EPObjectIfs_init(self,args,kwds);
	return self;
}
PyTypeObject EPObjectIfsType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPObjectIfs",/*tp_name*/
	sizeof(EPObjectIfs),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPObjectIfs_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPObjectIfs_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"ObjectIfs",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPObjectIfs_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPObjectIfs_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPObjectIfs_new,/*tp_new*/
};
void initEPObjectIfs(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPObjectIfsType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPObjectIfsType);
	PyModule_AddObject(m,"ObjectIfs",(PyObject *)&EPObjectIfsType);//モジュールに追加
}
PyObject* newEPObjectIfs(struct ObjectIfs org)
{
	EPObjectIfs *ret = EPObjectIfs_new(&EPObjectIfsType,NULL,NULL);
	ret->ptr = new ObjectIfs();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPObjectIfs(struct ObjectIfs* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPObjectIfs *ret = EPObjectIfs_new(&EPObjectIfsType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}ObjectIfs

//{*********EPNamedObjectIf*******
PyObject* EPNamedObjectIf_Cast( EPNamedObjectIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPNamedObjectIf* ret = (EPNamedObjectIf*)newEPNamedObjectIf(NamedObjectIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPNamedObjectIf_GetName( EPNamedObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPchar* ret = (EPchar*)newEPchar(self->ptr->GetName());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPNamedObjectIf_GetNameManager( EPNamedObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPNameManagerIf* ret = (EPNameManagerIf*)newEPNameManagerIf(self->ptr->GetNameManager());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPNamedObjectIf_SetName( EPNamedObjectIf* self,EPchar* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPchar_Check(var1))
	{
		self->ptr->SetName((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPNamedObjectIf_methods[] =
{
	{"Cast",(PyCFunction)EPNamedObjectIf_Cast,METH_O,"EPNamedObjectIf::Cast"},
	{"GetName",(PyCFunction)EPNamedObjectIf_GetName,METH_NOARGS,"EPNamedObjectIf::GetName"},
	{"GetNameManager",(PyCFunction)EPNamedObjectIf_GetNameManager,METH_NOARGS,"EPNamedObjectIf::GetNameManager"},
	{"SetName",(PyCFunction)EPNamedObjectIf_SetName,METH_O,"EPNamedObjectIf::SetName"},
	{NULL}
};
void EPNamedObjectIf_dealloc(PyObject* self)
{
}
PyObject* EPNamedObjectIf_str()
{
	return Py_BuildValue("s","This is EPNamedObjectIf.");
}
int EPNamedObjectIf_init(EPNamedObjectIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPNamedObjectIf* EPNamedObjectIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPNamedObjectIf *self;
	self = (EPNamedObjectIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPNamedObjectIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPNamedObjectIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPNamedObjectIf",/*tp_name*/
	sizeof(EPNamedObjectIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPNamedObjectIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPNamedObjectIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"NamedObjectIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPNamedObjectIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPNamedObjectIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPNamedObjectIf_new,/*tp_new*/
};
void initEPNamedObjectIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPNamedObjectIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPNamedObjectIfType);
	PyModule_AddObject(m,"NamedObjectIf",(PyObject *)&EPNamedObjectIfType);//モジュールに追加
}
PyObject* newEPNamedObjectIf(struct NamedObjectIf org)
{
	EPNamedObjectIf *ret = EPNamedObjectIf_new(&EPNamedObjectIfType,NULL,NULL);
	ret->ptr = new NamedObjectIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPNamedObjectIf(struct NamedObjectIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPNamedObjectIf *ret = EPNamedObjectIf_new(&EPNamedObjectIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}NamedObjectIf

//{*********EPSceneObjectIf*******
PyObject* EPSceneObjectIf_Cast( EPSceneObjectIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPSceneObjectIf* ret = (EPSceneObjectIf*)newEPSceneObjectIf(SceneObjectIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPSceneObjectIf_CloneObject( EPSceneObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPSceneObjectIf* ret = (EPSceneObjectIf*)newEPSceneObjectIf(self->ptr->CloneObject());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPSceneObjectIf_GetScene( EPSceneObjectIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPSceneIf* ret = (EPSceneIf*)newEPSceneIf(self->ptr->GetScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPSceneObjectIf_methods[] =
{
	{"Cast",(PyCFunction)EPSceneObjectIf_Cast,METH_O,"EPSceneObjectIf::Cast"},
	{"CloneObject",(PyCFunction)EPSceneObjectIf_CloneObject,METH_NOARGS,"EPSceneObjectIf::CloneObject"},
	{"GetScene",(PyCFunction)EPSceneObjectIf_GetScene,METH_NOARGS,"EPSceneObjectIf::GetScene"},
	{NULL}
};
void EPSceneObjectIf_dealloc(PyObject* self)
{
}
PyObject* EPSceneObjectIf_str()
{
	return Py_BuildValue("s","This is EPSceneObjectIf.");
}
int EPSceneObjectIf_init(EPSceneObjectIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPSceneObjectIf* EPSceneObjectIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPSceneObjectIf *self;
	self = (EPSceneObjectIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPSceneObjectIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPSceneObjectIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPSceneObjectIf",/*tp_name*/
	sizeof(EPSceneObjectIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPSceneObjectIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPSceneObjectIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"SceneObjectIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPSceneObjectIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPNamedObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPSceneObjectIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPSceneObjectIf_new,/*tp_new*/
};
void initEPSceneObjectIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPSceneObjectIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPSceneObjectIfType);
	PyModule_AddObject(m,"SceneObjectIf",(PyObject *)&EPSceneObjectIfType);//モジュールに追加
}
PyObject* newEPSceneObjectIf(struct SceneObjectIf org)
{
	EPSceneObjectIf *ret = EPSceneObjectIf_new(&EPSceneObjectIfType,NULL,NULL);
	ret->ptr = new SceneObjectIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPSceneObjectIf(struct SceneObjectIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPSceneObjectIf *ret = EPSceneObjectIf_new(&EPSceneObjectIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}SceneObjectIf

//{*********EPObjectStatesIf*******
PyObject* EPObjectStatesIf_AllocateState( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		self->ptr->AllocateState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_CalcStateSize( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->CalcStateSize((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_Cast( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPObjectStatesIf* ret = (EPObjectStatesIf*)newEPObjectStatesIf(ObjectStatesIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_Create( EPObjectStatesIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPObjectStatesIf* ret = (EPObjectStatesIf*)newEPObjectStatesIf(ObjectStatesIf::Create());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_LoadState( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		self->ptr->LoadState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_ReleaseState( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		self->ptr->ReleaseState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPObjectStatesIf_SaveState( EPObjectStatesIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		self->ptr->SaveState((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPObjectStatesIf_methods[] =
{
	{"AllocateState",(PyCFunction)EPObjectStatesIf_AllocateState,METH_O,"EPObjectStatesIf::AllocateState"},
	{"CalcStateSize",(PyCFunction)EPObjectStatesIf_CalcStateSize,METH_O,"EPObjectStatesIf::CalcStateSize"},
	{"Cast",(PyCFunction)EPObjectStatesIf_Cast,METH_O,"EPObjectStatesIf::Cast"},
	{"Create",(PyCFunction)EPObjectStatesIf_Create,METH_NOARGS,"EPObjectStatesIf::Create"},
	{"LoadState",(PyCFunction)EPObjectStatesIf_LoadState,METH_O,"EPObjectStatesIf::LoadState"},
	{"ReleaseState",(PyCFunction)EPObjectStatesIf_ReleaseState,METH_O,"EPObjectStatesIf::ReleaseState"},
	{"SaveState",(PyCFunction)EPObjectStatesIf_SaveState,METH_O,"EPObjectStatesIf::SaveState"},
	{NULL}
};
void EPObjectStatesIf_dealloc(PyObject* self)
{
}
PyObject* EPObjectStatesIf_str()
{
	return Py_BuildValue("s","This is EPObjectStatesIf.");
}
int EPObjectStatesIf_init(EPObjectStatesIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPObjectStatesIf* EPObjectStatesIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPObjectStatesIf *self;
	self = (EPObjectStatesIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPObjectStatesIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPObjectStatesIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPObjectStatesIf",/*tp_name*/
	sizeof(EPObjectStatesIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPObjectStatesIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPObjectStatesIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"ObjectStatesIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPObjectStatesIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPObjectStatesIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPObjectStatesIf_new,/*tp_new*/
};
void initEPObjectStatesIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPObjectStatesIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPObjectStatesIfType);
	PyModule_AddObject(m,"ObjectStatesIf",(PyObject *)&EPObjectStatesIfType);//モジュールに追加
}
PyObject* newEPObjectStatesIf(struct ObjectStatesIf org)
{
	EPObjectStatesIf *ret = EPObjectStatesIf_new(&EPObjectStatesIfType,NULL,NULL);
	ret->ptr = new ObjectStatesIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPObjectStatesIf(struct ObjectStatesIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPObjectStatesIf *ret = EPObjectStatesIf_new(&EPObjectStatesIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}ObjectStatesIf

//{*********EPNameManagerIf*******
PyObject* EPNameManagerIf_Cast( EPNameManagerIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPNameManagerIf* ret = (EPNameManagerIf*)newEPNameManagerIf(NameManagerIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPNameManagerIf_FindObject( EPNameManagerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPUTString_Check(((EPUTString*)PyTuple_GetItem(tuple,0))) && EPUTString_Check(((EPUTString*)PyTuple_GetItem(tuple,1))))
	{
		EPNamedObjectIf* ret = (EPNamedObjectIf*)newEPNamedObjectIf(self->ptr->FindObject((*(((EPUTString*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPUTString*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPUTString_Check(((EPUTString*)PyTuple_GetItem(tuple,0))))
	{
		EPNamedObjectIf* ret = (EPNamedObjectIf*)newEPNamedObjectIf(self->ptr->FindObject((*(((EPUTString*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPNameManagerIf_methods[] =
{
	{"Cast",(PyCFunction)EPNameManagerIf_Cast,METH_O,"EPNameManagerIf::Cast"},
	{"FindObject",(PyCFunction)EPNameManagerIf_FindObject,METH_VARARGS,"EPNameManagerIf::FindObject"},
	{NULL}
};
void EPNameManagerIf_dealloc(PyObject* self)
{
}
PyObject* EPNameManagerIf_str()
{
	return Py_BuildValue("s","This is EPNameManagerIf.");
}
int EPNameManagerIf_init(EPNameManagerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPNameManagerIf* EPNameManagerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPNameManagerIf *self;
	self = (EPNameManagerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPNameManagerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPNameManagerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPNameManagerIf",/*tp_name*/
	sizeof(EPNameManagerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPNameManagerIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPNameManagerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"NameManagerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPNameManagerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPNamedObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPNameManagerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPNameManagerIf_new,/*tp_new*/
};
void initEPNameManagerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPNameManagerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPNameManagerIfType);
	PyModule_AddObject(m,"NameManagerIf",(PyObject *)&EPNameManagerIfType);//モジュールに追加
}
PyObject* newEPNameManagerIf(struct NameManagerIf org)
{
	EPNameManagerIf *ret = EPNameManagerIf_new(&EPNameManagerIfType,NULL,NULL);
	ret->ptr = new NameManagerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPNameManagerIf(struct NameManagerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPNameManagerIf *ret = EPNameManagerIf_new(&EPNameManagerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}NameManagerIf

//{*********EPSceneIf*******
PyObject* EPSceneIf_Cast( EPSceneIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPSceneIf* ret = (EPSceneIf*)newEPSceneIf(SceneIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPSceneIf_methods[] =
{
	{"Cast",(PyCFunction)EPSceneIf_Cast,METH_O,"EPSceneIf::Cast"},
	{NULL}
};
void EPSceneIf_dealloc(PyObject* self)
{
}
PyObject* EPSceneIf_str()
{
	return Py_BuildValue("s","This is EPSceneIf.");
}
int EPSceneIf_init(EPSceneIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPSceneIf* EPSceneIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPSceneIf *self;
	self = (EPSceneIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPSceneIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPSceneIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPSceneIf",/*tp_name*/
	sizeof(EPSceneIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPSceneIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPSceneIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"SceneIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPSceneIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPNameManagerIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPSceneIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPSceneIf_new,/*tp_new*/
};
void initEPSceneIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPSceneIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPSceneIfType);
	PyModule_AddObject(m,"SceneIf",(PyObject *)&EPSceneIfType);//モジュールに追加
}
PyObject* newEPSceneIf(struct SceneIf org)
{
	EPSceneIf *ret = EPSceneIf_new(&EPSceneIfType,NULL,NULL);
	ret->ptr = new SceneIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPSceneIf(struct SceneIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPSceneIf *ret = EPSceneIf_new(&EPSceneIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}SceneIf

//{*********EPSdkIf*******
PyObject* EPSdkIf_Cast( EPSdkIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		EPSdkIf* ret = (EPSdkIf*)newEPSdkIf(SdkIf::Cast((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPSdkIf_CreateSdk( EPSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPvoid_Check(((EPvoid*)PyTuple_GetItem(tuple,1))))
	{
		EPSdkIf* ret = (EPSdkIf*)newEPSdkIf(SdkIf::CreateSdk((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(((EPvoid*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPSdkIf_methods[] =
{
	{"Cast",(PyCFunction)EPSdkIf_Cast,METH_O,"EPSdkIf::Cast"},
	{"CreateSdk",(PyCFunction)EPSdkIf_CreateSdk,METH_VARARGS,"EPSdkIf::CreateSdk"},
	{NULL}
};
void EPSdkIf_dealloc(PyObject* self)
{
}
PyObject* EPSdkIf_str()
{
	return Py_BuildValue("s","This is EPSdkIf.");
}
int EPSdkIf_init(EPSdkIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPSdkIf* EPSdkIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPSdkIf *self;
	self = (EPSdkIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPSdkIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPSdkIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Foundation.EPSdkIf",/*tp_name*/
	sizeof(EPSdkIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPSdkIf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPSdkIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"SdkIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPSdkIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPNameManagerIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPSdkIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPSdkIf_new,/*tp_new*/
};
void initEPSdkIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPSdkIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Foundation");
	Py_INCREF(&EPSdkIfType);
	PyModule_AddObject(m,"SdkIf",(PyObject *)&EPSdkIfType);//モジュールに追加
}
PyObject* newEPSdkIf(struct SdkIf org)
{
	EPSdkIf *ret = EPSdkIf_new(&EPSdkIfType,NULL,NULL);
	ret->ptr = new SdkIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPSdkIf(struct SdkIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPSdkIf *ret = EPSdkIf_new(&EPSdkIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}SdkIf
/**************** for Module ******************/
void initFoundation(void)
{
	initEPObjectIf();
	initEPObjectIfs();
	initEPNamedObjectIf();
	initEPSceneObjectIf();
	initEPObjectStatesIf();
	initEPNameManagerIf();
	initEPSceneIf();
	initEPSdkIf();
}
