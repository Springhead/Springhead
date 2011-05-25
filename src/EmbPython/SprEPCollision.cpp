#include "../../include\EmbPython\SprEPCollision.h"

//{*********EPCDShapeIf*******
PyObject* EPCDShapeIf_CalcCenterOfMass( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->CalcCenterOfMass());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_CalcMomentOfInertia( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(self->ptr->CalcMomentOfInertia());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_CalcVolume( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->CalcVolume());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetDamperD( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetDamperD());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetDensity( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetDensity());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetDynamicFriction( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetDynamicFriction());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetElasticity( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetElasticity());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetMaterial( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPPHMaterial* ret = (EPPHMaterial*)newEPPHMaterial(self->ptr->GetMaterial());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetSpringK( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetSpringK());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetStaticFriction( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetStaticFriction());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetVibA( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetVibA());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetVibB( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetVibB());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetVibContact( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->GetVibContact()? 1 : 0);
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetVibT( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetVibT());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_GetVibW( EPCDShapeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetVibW());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetDensity( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetDensity(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetDynamicFriction( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetDynamicFriction(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetElasticity( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetElasticity(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetMaterial( EPCDShapeIf* self,EPPHMaterial* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPPHMaterial_Check(var1))
    {
        self->ptr->SetMaterial((*(var1->ptr)));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetSpringDamper( EPCDShapeIf* self,PyObject* tuple )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
    {
        self->ptr->SetSpringDamper(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetStaticFriction( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetStaticFriction(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibA( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetVibA(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibB( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetVibB(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibContact( EPCDShapeIf* self,PyObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyBool_Check(var1))
    {
        self->ptr->SetVibContact((Py_True == var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibT( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetVibT(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibW( EPCDShapeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetVibW(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDShapeIf_SetVibration( EPCDShapeIf* self,PyObject* tuple )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
    {
        self->ptr->SetVibration(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDShapeIf_methods[] =
{
    {"CalcCenterOfMass",(PyCFunction)EPCDShapeIf_CalcCenterOfMass,METH_NOARGS,"EPCDShapeIf::CalcCenterOfMass"},
    {"CalcMomentOfInertia",(PyCFunction)EPCDShapeIf_CalcMomentOfInertia,METH_NOARGS,"EPCDShapeIf::CalcMomentOfInertia"},
    {"CalcVolume",(PyCFunction)EPCDShapeIf_CalcVolume,METH_NOARGS,"EPCDShapeIf::CalcVolume"},
    {"GetDamperD",(PyCFunction)EPCDShapeIf_GetDamperD,METH_NOARGS,"EPCDShapeIf::GetDamperD"},
    {"GetDensity",(PyCFunction)EPCDShapeIf_GetDensity,METH_NOARGS,"EPCDShapeIf::GetDensity"},
    {"GetDynamicFriction",(PyCFunction)EPCDShapeIf_GetDynamicFriction,METH_NOARGS,"EPCDShapeIf::GetDynamicFriction"},
    {"GetElasticity",(PyCFunction)EPCDShapeIf_GetElasticity,METH_NOARGS,"EPCDShapeIf::GetElasticity"},
    {"GetMaterial",(PyCFunction)EPCDShapeIf_GetMaterial,METH_NOARGS,"EPCDShapeIf::GetMaterial"},
    {"GetSpringK",(PyCFunction)EPCDShapeIf_GetSpringK,METH_NOARGS,"EPCDShapeIf::GetSpringK"},
    {"GetStaticFriction",(PyCFunction)EPCDShapeIf_GetStaticFriction,METH_NOARGS,"EPCDShapeIf::GetStaticFriction"},
    {"GetVibA",(PyCFunction)EPCDShapeIf_GetVibA,METH_NOARGS,"EPCDShapeIf::GetVibA"},
    {"GetVibB",(PyCFunction)EPCDShapeIf_GetVibB,METH_NOARGS,"EPCDShapeIf::GetVibB"},
    {"GetVibContact",(PyCFunction)EPCDShapeIf_GetVibContact,METH_NOARGS,"EPCDShapeIf::GetVibContact"},
    {"GetVibT",(PyCFunction)EPCDShapeIf_GetVibT,METH_NOARGS,"EPCDShapeIf::GetVibT"},
    {"GetVibW",(PyCFunction)EPCDShapeIf_GetVibW,METH_NOARGS,"EPCDShapeIf::GetVibW"},
    {"SetDensity",(PyCFunction)EPCDShapeIf_SetDensity,METH_O,"EPCDShapeIf::SetDensity"},
    {"SetDynamicFriction",(PyCFunction)EPCDShapeIf_SetDynamicFriction,METH_O,"EPCDShapeIf::SetDynamicFriction"},
    {"SetElasticity",(PyCFunction)EPCDShapeIf_SetElasticity,METH_O,"EPCDShapeIf::SetElasticity"},
    {"SetMaterial",(PyCFunction)EPCDShapeIf_SetMaterial,METH_O,"EPCDShapeIf::SetMaterial"},
    {"SetSpringDamper",(PyCFunction)EPCDShapeIf_SetSpringDamper,METH_VARARGS,"EPCDShapeIf::SetSpringDamper"},
    {"SetStaticFriction",(PyCFunction)EPCDShapeIf_SetStaticFriction,METH_O,"EPCDShapeIf::SetStaticFriction"},
    {"SetVibA",(PyCFunction)EPCDShapeIf_SetVibA,METH_O,"EPCDShapeIf::SetVibA"},
    {"SetVibB",(PyCFunction)EPCDShapeIf_SetVibB,METH_O,"EPCDShapeIf::SetVibB"},
    {"SetVibContact",(PyCFunction)EPCDShapeIf_SetVibContact,METH_O,"EPCDShapeIf::SetVibContact"},
    {"SetVibT",(PyCFunction)EPCDShapeIf_SetVibT,METH_O,"EPCDShapeIf::SetVibT"},
    {"SetVibW",(PyCFunction)EPCDShapeIf_SetVibW,METH_O,"EPCDShapeIf::SetVibW"},
    {"SetVibration",(PyCFunction)EPCDShapeIf_SetVibration,METH_VARARGS,"EPCDShapeIf::SetVibration"},
    {NULL}
};
void EPCDShapeIf_dealloc(PyObject* self)
{
}
PyObject* EPCDShapeIf_str()
{
    return Py_BuildValue("s","This is EPCDShapeIf.");
}
int EPCDShapeIf_init(EPCDShapeIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDShapeIf* EPCDShapeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDShapeIf *self;
    self = (EPCDShapeIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDShapeIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDShapeIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDShapeIf",/*tp_name*/
    sizeof(EPCDShapeIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDShapeIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDShapeIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDShapeIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDShapeIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPNamedObjectIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDShapeIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDShapeIf_new,/*tp_new*/
};
void initEPCDShapeIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDShapeIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDShapeIfType);
    PyModule_AddObject(m,"CDShapeIf",(PyObject *)&EPCDShapeIfType);//モジュールに追加
}
PyObject* newEPCDShapeIf(struct CDShapeIf org)
{
    EPCDShapeIf *ret = EPCDShapeIf_new(&EPCDShapeIfType,NULL,NULL);
    ret->ptr = new CDShapeIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDShapeIf(struct CDShapeIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDShapeIf *ret = EPCDShapeIf_new(&EPCDShapeIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDShapeIf

//{*********EPCDShapePairIf*******
PyObject* EPCDShapePairIf_GetShape( EPCDShapePairIf* self,PyLongObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyLong_Check(var1))
    {
        EPCDShapeIf* ret = (EPCDShapeIf*)newEPCDShapeIf(self->ptr->GetShape(PyLong_AsLong((PyObject*)var1)));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDShapePairIf_methods[] =
{
    {"GetShape",(PyCFunction)EPCDShapePairIf_GetShape,METH_O,"EPCDShapePairIf::GetShape"},
    {NULL}
};
void EPCDShapePairIf_dealloc(PyObject* self)
{
}
PyObject* EPCDShapePairIf_str()
{
    return Py_BuildValue("s","This is EPCDShapePairIf.");
}
int EPCDShapePairIf_init(EPCDShapePairIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDShapePairIf* EPCDShapePairIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDShapePairIf *self;
    self = (EPCDShapePairIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDShapePairIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDShapePairIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDShapePairIf",/*tp_name*/
    sizeof(EPCDShapePairIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDShapePairIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDShapePairIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDShapePairIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDShapePairIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPObjectIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDShapePairIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDShapePairIf_new,/*tp_new*/
};
void initEPCDShapePairIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDShapePairIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDShapePairIfType);
    PyModule_AddObject(m,"CDShapePairIf",(PyObject *)&EPCDShapePairIfType);//モジュールに追加
}
PyObject* newEPCDShapePairIf(struct CDShapePairIf org)
{
    EPCDShapePairIf *ret = EPCDShapePairIf_new(&EPCDShapePairIfType,NULL,NULL);
    ret->ptr = new CDShapePairIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDShapePairIf(struct CDShapePairIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDShapePairIf *ret = EPCDShapePairIf_new(&EPCDShapePairIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDShapePairIf

//{*********EPCDConvexIf*******
PyObject* EPCDConvexIf_CurvatureRadius( EPCDConvexIf* self,EPVec3d* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPVec3d_Check(var1))
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->CurvatureRadius((*(var1->ptr))));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDConvexIf_Normal( EPCDConvexIf* self,EPVec3d* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPVec3d_Check(var1))
    {
        EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->Normal((*(var1->ptr))));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDConvexIf_methods[] =
{
    {"CurvatureRadius",(PyCFunction)EPCDConvexIf_CurvatureRadius,METH_O,"EPCDConvexIf::CurvatureRadius"},
    {"Normal",(PyCFunction)EPCDConvexIf_Normal,METH_O,"EPCDConvexIf::Normal"},
    {NULL}
};
void EPCDConvexIf_dealloc(PyObject* self)
{
}
PyObject* EPCDConvexIf_str()
{
    return Py_BuildValue("s","This is EPCDConvexIf.");
}
int EPCDConvexIf_init(EPCDConvexIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDConvexIf* EPCDConvexIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDConvexIf *self;
    self = (EPCDConvexIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDConvexIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDConvexIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDConvexIf",/*tp_name*/
    sizeof(EPCDConvexIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDConvexIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDConvexIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDConvexIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDConvexIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDConvexIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDConvexIf_new,/*tp_new*/
};
void initEPCDConvexIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDConvexIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDConvexIfType);
    PyModule_AddObject(m,"CDConvexIf",(PyObject *)&EPCDConvexIfType);//モジュールに追加
}
PyObject* newEPCDConvexIf(struct CDConvexIf org)
{
    EPCDConvexIf *ret = EPCDConvexIf_new(&EPCDConvexIfType,NULL,NULL);
    ret->ptr = new CDConvexIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDConvexIf(struct CDConvexIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDConvexIf *ret = EPCDConvexIf_new(&EPCDConvexIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDConvexIf

//{*********EPCDFaceIf*******
PyObject* EPCDFaceIf_GetIndices( EPCDFaceIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetIndices());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDFaceIf_NIndex( EPCDFaceIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NIndex());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDFaceIf_methods[] =
{
    {"GetIndices",(PyCFunction)EPCDFaceIf_GetIndices,METH_NOARGS,"EPCDFaceIf::GetIndices"},
    {"NIndex",(PyCFunction)EPCDFaceIf_NIndex,METH_NOARGS,"EPCDFaceIf::NIndex"},
    {NULL}
};
void EPCDFaceIf_dealloc(PyObject* self)
{
}
PyObject* EPCDFaceIf_str()
{
    return Py_BuildValue("s","This is EPCDFaceIf.");
}
int EPCDFaceIf_init(EPCDFaceIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDFaceIf* EPCDFaceIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDFaceIf *self;
    self = (EPCDFaceIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDFaceIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDFaceIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDFaceIf",/*tp_name*/
    sizeof(EPCDFaceIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDFaceIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDFaceIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDFaceIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDFaceIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPObjectIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDFaceIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDFaceIf_new,/*tp_new*/
};
void initEPCDFaceIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDFaceIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDFaceIfType);
    PyModule_AddObject(m,"CDFaceIf",(PyObject *)&EPCDFaceIfType);//モジュールに追加
}
PyObject* newEPCDFaceIf(struct CDFaceIf org)
{
    EPCDFaceIf *ret = EPCDFaceIf_new(&EPCDFaceIfType,NULL,NULL);
    ret->ptr = new CDFaceIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDFaceIf(struct CDFaceIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDFaceIf *ret = EPCDFaceIf_new(&EPCDFaceIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDFaceIf

//{*********EPCDQuadFaceIf*******
PyObject* EPCDQuadFaceIf_GetIndices( EPCDQuadFaceIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetIndices());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDQuadFaceIf_NIndex( EPCDQuadFaceIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NIndex());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDQuadFaceIf_methods[] =
{
    {"GetIndices",(PyCFunction)EPCDQuadFaceIf_GetIndices,METH_NOARGS,"EPCDQuadFaceIf::GetIndices"},
    {"NIndex",(PyCFunction)EPCDQuadFaceIf_NIndex,METH_NOARGS,"EPCDQuadFaceIf::NIndex"},
    {NULL}
};
void EPCDQuadFaceIf_dealloc(PyObject* self)
{
}
PyObject* EPCDQuadFaceIf_str()
{
    return Py_BuildValue("s","This is EPCDQuadFaceIf.");
}
int EPCDQuadFaceIf_init(EPCDQuadFaceIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDQuadFaceIf* EPCDQuadFaceIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDQuadFaceIf *self;
    self = (EPCDQuadFaceIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDQuadFaceIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDQuadFaceIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDQuadFaceIf",/*tp_name*/
    sizeof(EPCDQuadFaceIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDQuadFaceIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDQuadFaceIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDQuadFaceIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDQuadFaceIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPObjectIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDQuadFaceIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDQuadFaceIf_new,/*tp_new*/
};
void initEPCDQuadFaceIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDQuadFaceIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDQuadFaceIfType);
    PyModule_AddObject(m,"CDQuadFaceIf",(PyObject *)&EPCDQuadFaceIfType);//モジュールに追加
}
PyObject* newEPCDQuadFaceIf(struct CDQuadFaceIf org)
{
    EPCDQuadFaceIf *ret = EPCDQuadFaceIf_new(&EPCDQuadFaceIfType,NULL,NULL);
    ret->ptr = new CDQuadFaceIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDQuadFaceIf(struct CDQuadFaceIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDQuadFaceIf *ret = EPCDQuadFaceIf_new(&EPCDQuadFaceIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDQuadFaceIf

//{*********EPPHMaterial*******
static PyMethodDef EPPHMaterial_methods[] =
{
    {NULL}
};
void EPPHMaterial_dealloc(PyObject* self)
{
    delete ((EPPHMaterial*)self)->ptr;
}
PyObject* EPPHMaterial_str()
{
    return Py_BuildValue("s","This is EPPHMaterial.");
}
int EPPHMaterial_init(EPPHMaterial* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPPHMaterial* EPPHMaterial_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPPHMaterial *self;
    self = (EPPHMaterial*) type->tp_alloc(type,0);
    if ( self != NULL ) EPPHMaterial_init(self,args,kwds);
    return self;
}
PyTypeObject EPPHMaterialType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPPHMaterial",/*tp_name*/
    sizeof(EPPHMaterial),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPPHMaterial_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPPHMaterial_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "PHMaterial",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPPHMaterial_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    0,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPPHMaterial_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPPHMaterial_new,/*tp_new*/
};
void initEPPHMaterial(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPPHMaterialType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPPHMaterialType);
    PyModule_AddObject(m,"PHMaterial",(PyObject *)&EPPHMaterialType);//モジュールに追加
}
PyObject* newEPPHMaterial(struct PHMaterial org)
{
    EPPHMaterial *ret = EPPHMaterial_new(&EPPHMaterialType,NULL,NULL);
    ret->ptr = new PHMaterial();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPPHMaterial(struct PHMaterial* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPPHMaterial *ret = EPPHMaterial_new(&EPPHMaterialType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}PHMaterial

//{*********EPCDShapeDesc*******
static PyMethodDef EPCDShapeDesc_methods[] =
{
    {NULL}
};
void EPCDShapeDesc_dealloc(PyObject* self)
{
    delete ((EPCDShapeDesc*)self)->ptr;
}
PyObject* EPCDShapeDesc_str()
{
    return Py_BuildValue("s","This is EPCDShapeDesc.");
}
int EPCDShapeDesc_init(EPCDShapeDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDShapeDesc* EPCDShapeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDShapeDesc *self;
    self = (EPCDShapeDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDShapeDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDShapeDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDShapeDesc",/*tp_name*/
    sizeof(EPCDShapeDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDShapeDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDShapeDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDShapeDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDShapeDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    0,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDShapeDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDShapeDesc_new,/*tp_new*/
};
void initEPCDShapeDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDShapeDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDShapeDescType);
    PyModule_AddObject(m,"CDShapeDesc",(PyObject *)&EPCDShapeDescType);//モジュールに追加
}
PyObject* newEPCDShapeDesc(struct CDShapeDesc org)
{
    EPCDShapeDesc *ret = EPCDShapeDesc_new(&EPCDShapeDescType,NULL,NULL);
    ret->ptr = new CDShapeDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDShapeDesc(struct CDShapeDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDShapeDesc *ret = EPCDShapeDesc_new(&EPCDShapeDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDShapeDesc

//{*********EPCDConvexMeshIf*******
PyObject* EPCDConvexMeshIf_GetFace( EPCDConvexMeshIf* self,PyLongObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyLong_Check(var1))
    {
        EPCDFaceIf* ret = (EPCDFaceIf*)newEPCDFaceIf(self->ptr->GetFace(PyLong_AsSsize_t((PyObject*)var1)));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDConvexMeshIf_GetVertices( EPCDConvexMeshIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetVertices());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDConvexMeshIf_NFace( EPCDConvexMeshIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->NFace());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDConvexMeshIf_NVertex( EPCDConvexMeshIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->NVertex());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDConvexMeshIf_methods[] =
{
    {"GetFace",(PyCFunction)EPCDConvexMeshIf_GetFace,METH_O,"EPCDConvexMeshIf::GetFace"},
    {"GetVertices",(PyCFunction)EPCDConvexMeshIf_GetVertices,METH_NOARGS,"EPCDConvexMeshIf::GetVertices"},
    {"NFace",(PyCFunction)EPCDConvexMeshIf_NFace,METH_NOARGS,"EPCDConvexMeshIf::NFace"},
    {"NVertex",(PyCFunction)EPCDConvexMeshIf_NVertex,METH_NOARGS,"EPCDConvexMeshIf::NVertex"},
    {NULL}
};
void EPCDConvexMeshIf_dealloc(PyObject* self)
{
}
PyObject* EPCDConvexMeshIf_str()
{
    return Py_BuildValue("s","This is EPCDConvexMeshIf.");
}
int EPCDConvexMeshIf_init(EPCDConvexMeshIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDConvexMeshIf* EPCDConvexMeshIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDConvexMeshIf *self;
    self = (EPCDConvexMeshIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDConvexMeshIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDConvexMeshIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDConvexMeshIf",/*tp_name*/
    sizeof(EPCDConvexMeshIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDConvexMeshIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDConvexMeshIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDConvexMeshIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDConvexMeshIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDConvexMeshIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDConvexMeshIf_new,/*tp_new*/
};
void initEPCDConvexMeshIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDConvexMeshIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDConvexMeshIfType);
    PyModule_AddObject(m,"CDConvexMeshIf",(PyObject *)&EPCDConvexMeshIfType);//モジュールに追加
}
PyObject* newEPCDConvexMeshIf(struct CDConvexMeshIf org)
{
    EPCDConvexMeshIf *ret = EPCDConvexMeshIf_new(&EPCDConvexMeshIfType,NULL,NULL);
    ret->ptr = new CDConvexMeshIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDConvexMeshIf(struct CDConvexMeshIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDConvexMeshIf *ret = EPCDConvexMeshIf_new(&EPCDConvexMeshIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDConvexMeshIf

//{*********EPCDConvexMeshDesc*******
static PyMethodDef EPCDConvexMeshDesc_methods[] =
{
    {NULL}
};
void EPCDConvexMeshDesc_dealloc(PyObject* self)
{
    delete ((EPCDConvexMeshDesc*)self)->ptr;
}
PyObject* EPCDConvexMeshDesc_str()
{
    return Py_BuildValue("s","This is EPCDConvexMeshDesc.");
}
int EPCDConvexMeshDesc_init(EPCDConvexMeshDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDConvexMeshDesc* EPCDConvexMeshDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDConvexMeshDesc *self;
    self = (EPCDConvexMeshDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDConvexMeshDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDConvexMeshDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDConvexMeshDesc",/*tp_name*/
    sizeof(EPCDConvexMeshDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDConvexMeshDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDConvexMeshDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDConvexMeshDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDConvexMeshDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDConvexMeshDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDConvexMeshDesc_new,/*tp_new*/
};
void initEPCDConvexMeshDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDConvexMeshDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDConvexMeshDescType);
    PyModule_AddObject(m,"CDConvexMeshDesc",(PyObject *)&EPCDConvexMeshDescType);//モジュールに追加
}
PyObject* newEPCDConvexMeshDesc(struct CDConvexMeshDesc org)
{
    EPCDConvexMeshDesc *ret = EPCDConvexMeshDesc_new(&EPCDConvexMeshDescType,NULL,NULL);
    ret->ptr = new CDConvexMeshDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDConvexMeshDesc(struct CDConvexMeshDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDConvexMeshDesc *ret = EPCDConvexMeshDesc_new(&EPCDConvexMeshDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDConvexMeshDesc

//{*********EPCDConvexMeshInterpolateIf*******
static PyMethodDef EPCDConvexMeshInterpolateIf_methods[] =
{
    {NULL}
};
void EPCDConvexMeshInterpolateIf_dealloc(PyObject* self)
{
}
PyObject* EPCDConvexMeshInterpolateIf_str()
{
    return Py_BuildValue("s","This is EPCDConvexMeshInterpolateIf.");
}
int EPCDConvexMeshInterpolateIf_init(EPCDConvexMeshInterpolateIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDConvexMeshInterpolateIf* EPCDConvexMeshInterpolateIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDConvexMeshInterpolateIf *self;
    self = (EPCDConvexMeshInterpolateIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDConvexMeshInterpolateIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDConvexMeshInterpolateIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDConvexMeshInterpolateIf",/*tp_name*/
    sizeof(EPCDConvexMeshInterpolateIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDConvexMeshInterpolateIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDConvexMeshInterpolateIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDConvexMeshInterpolateIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDConvexMeshInterpolateIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexMeshIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDConvexMeshInterpolateIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDConvexMeshInterpolateIf_new,/*tp_new*/
};
void initEPCDConvexMeshInterpolateIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDConvexMeshInterpolateIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDConvexMeshInterpolateIfType);
    PyModule_AddObject(m,"CDConvexMeshInterpolateIf",(PyObject *)&EPCDConvexMeshInterpolateIfType);//モジュールに追加
}
PyObject* newEPCDConvexMeshInterpolateIf(struct CDConvexMeshInterpolateIf org)
{
    EPCDConvexMeshInterpolateIf *ret = EPCDConvexMeshInterpolateIf_new(&EPCDConvexMeshInterpolateIfType,NULL,NULL);
    ret->ptr = new CDConvexMeshInterpolateIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDConvexMeshInterpolateIf(struct CDConvexMeshInterpolateIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDConvexMeshInterpolateIf *ret = EPCDConvexMeshInterpolateIf_new(&EPCDConvexMeshInterpolateIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDConvexMeshInterpolateIf

//{*********EPCDConvexMeshInterpolateDesc*******
static PyMethodDef EPCDConvexMeshInterpolateDesc_methods[] =
{
    {NULL}
};
void EPCDConvexMeshInterpolateDesc_dealloc(PyObject* self)
{
    delete ((EPCDConvexMeshInterpolateDesc*)self)->ptr;
}
PyObject* EPCDConvexMeshInterpolateDesc_str()
{
    return Py_BuildValue("s","This is EPCDConvexMeshInterpolateDesc.");
}
int EPCDConvexMeshInterpolateDesc_init(EPCDConvexMeshInterpolateDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDConvexMeshInterpolateDesc* EPCDConvexMeshInterpolateDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDConvexMeshInterpolateDesc *self;
    self = (EPCDConvexMeshInterpolateDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDConvexMeshInterpolateDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDConvexMeshInterpolateDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDConvexMeshInterpolateDesc",/*tp_name*/
    sizeof(EPCDConvexMeshInterpolateDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDConvexMeshInterpolateDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDConvexMeshInterpolateDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDConvexMeshInterpolateDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDConvexMeshInterpolateDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexMeshDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDConvexMeshInterpolateDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDConvexMeshInterpolateDesc_new,/*tp_new*/
};
void initEPCDConvexMeshInterpolateDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDConvexMeshInterpolateDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDConvexMeshInterpolateDescType);
    PyModule_AddObject(m,"CDConvexMeshInterpolateDesc",(PyObject *)&EPCDConvexMeshInterpolateDescType);//モジュールに追加
}
PyObject* newEPCDConvexMeshInterpolateDesc(struct CDConvexMeshInterpolateDesc org)
{
    EPCDConvexMeshInterpolateDesc *ret = EPCDConvexMeshInterpolateDesc_new(&EPCDConvexMeshInterpolateDescType,NULL,NULL);
    ret->ptr = new CDConvexMeshInterpolateDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDConvexMeshInterpolateDesc(struct CDConvexMeshInterpolateDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDConvexMeshInterpolateDesc *ret = EPCDConvexMeshInterpolateDesc_new(&EPCDConvexMeshInterpolateDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDConvexMeshInterpolateDesc

//{*********EPCDSphereIf*******
PyObject* EPCDSphereIf_GetRadius( EPCDSphereIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetRadius());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDSphereIf_methods[] =
{
    {"GetRadius",(PyCFunction)EPCDSphereIf_GetRadius,METH_NOARGS,"EPCDSphereIf::GetRadius"},
    {NULL}
};
void EPCDSphereIf_dealloc(PyObject* self)
{
}
PyObject* EPCDSphereIf_str()
{
    return Py_BuildValue("s","This is EPCDSphereIf.");
}
int EPCDSphereIf_init(EPCDSphereIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDSphereIf* EPCDSphereIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDSphereIf *self;
    self = (EPCDSphereIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDSphereIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDSphereIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDSphereIf",/*tp_name*/
    sizeof(EPCDSphereIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDSphereIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDSphereIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDSphereIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDSphereIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDSphereIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDSphereIf_new,/*tp_new*/
};
void initEPCDSphereIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDSphereIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDSphereIfType);
    PyModule_AddObject(m,"CDSphereIf",(PyObject *)&EPCDSphereIfType);//モジュールに追加
}
PyObject* newEPCDSphereIf(struct CDSphereIf org)
{
    EPCDSphereIf *ret = EPCDSphereIf_new(&EPCDSphereIfType,NULL,NULL);
    ret->ptr = new CDSphereIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDSphereIf(struct CDSphereIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDSphereIf *ret = EPCDSphereIf_new(&EPCDSphereIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDSphereIf

//{*********EPCDSphereDesc*******
static PyMethodDef EPCDSphereDesc_methods[] =
{
    {NULL}
};
void EPCDSphereDesc_dealloc(PyObject* self)
{
    delete ((EPCDSphereDesc*)self)->ptr;
}
PyObject* EPCDSphereDesc_str()
{
    return Py_BuildValue("s","This is EPCDSphereDesc.");
}
int EPCDSphereDesc_init(EPCDSphereDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDSphereDesc* EPCDSphereDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDSphereDesc *self;
    self = (EPCDSphereDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDSphereDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDSphereDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDSphereDesc",/*tp_name*/
    sizeof(EPCDSphereDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDSphereDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDSphereDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDSphereDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDSphereDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDSphereDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDSphereDesc_new,/*tp_new*/
};
void initEPCDSphereDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDSphereDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDSphereDescType);
    PyModule_AddObject(m,"CDSphereDesc",(PyObject *)&EPCDSphereDescType);//モジュールに追加
}
PyObject* newEPCDSphereDesc(struct CDSphereDesc org)
{
    EPCDSphereDesc *ret = EPCDSphereDesc_new(&EPCDSphereDescType,NULL,NULL);
    ret->ptr = new CDSphereDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDSphereDesc(struct CDSphereDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDSphereDesc *ret = EPCDSphereDesc_new(&EPCDSphereDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDSphereDesc

//{*********EPCDCapsuleIf*******
PyObject* EPCDCapsuleIf_GetLength( EPCDCapsuleIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetLength());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDCapsuleIf_GetRadius( EPCDCapsuleIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetRadius());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDCapsuleIf_methods[] =
{
    {"GetLength",(PyCFunction)EPCDCapsuleIf_GetLength,METH_NOARGS,"EPCDCapsuleIf::GetLength"},
    {"GetRadius",(PyCFunction)EPCDCapsuleIf_GetRadius,METH_NOARGS,"EPCDCapsuleIf::GetRadius"},
    {NULL}
};
void EPCDCapsuleIf_dealloc(PyObject* self)
{
}
PyObject* EPCDCapsuleIf_str()
{
    return Py_BuildValue("s","This is EPCDCapsuleIf.");
}
int EPCDCapsuleIf_init(EPCDCapsuleIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDCapsuleIf* EPCDCapsuleIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDCapsuleIf *self;
    self = (EPCDCapsuleIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDCapsuleIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDCapsuleIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDCapsuleIf",/*tp_name*/
    sizeof(EPCDCapsuleIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDCapsuleIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDCapsuleIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDCapsuleIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDCapsuleIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDCapsuleIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDCapsuleIf_new,/*tp_new*/
};
void initEPCDCapsuleIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDCapsuleIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDCapsuleIfType);
    PyModule_AddObject(m,"CDCapsuleIf",(PyObject *)&EPCDCapsuleIfType);//モジュールに追加
}
PyObject* newEPCDCapsuleIf(struct CDCapsuleIf org)
{
    EPCDCapsuleIf *ret = EPCDCapsuleIf_new(&EPCDCapsuleIfType,NULL,NULL);
    ret->ptr = new CDCapsuleIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDCapsuleIf(struct CDCapsuleIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDCapsuleIf *ret = EPCDCapsuleIf_new(&EPCDCapsuleIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDCapsuleIf

//{*********EPCDCapsuleDesc*******
static PyMethodDef EPCDCapsuleDesc_methods[] =
{
    {NULL}
};
void EPCDCapsuleDesc_dealloc(PyObject* self)
{
    delete ((EPCDCapsuleDesc*)self)->ptr;
}
PyObject* EPCDCapsuleDesc_str()
{
    return Py_BuildValue("s","This is EPCDCapsuleDesc.");
}
int EPCDCapsuleDesc_init(EPCDCapsuleDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDCapsuleDesc* EPCDCapsuleDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDCapsuleDesc *self;
    self = (EPCDCapsuleDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDCapsuleDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDCapsuleDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDCapsuleDesc",/*tp_name*/
    sizeof(EPCDCapsuleDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDCapsuleDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDCapsuleDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDCapsuleDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDCapsuleDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDCapsuleDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDCapsuleDesc_new,/*tp_new*/
};
void initEPCDCapsuleDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDCapsuleDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDCapsuleDescType);
    PyModule_AddObject(m,"CDCapsuleDesc",(PyObject *)&EPCDCapsuleDescType);//モジュールに追加
}
PyObject* newEPCDCapsuleDesc(struct CDCapsuleDesc org)
{
    EPCDCapsuleDesc *ret = EPCDCapsuleDesc_new(&EPCDCapsuleDescType,NULL,NULL);
    ret->ptr = new CDCapsuleDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDCapsuleDesc(struct CDCapsuleDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDCapsuleDesc *ret = EPCDCapsuleDesc_new(&EPCDCapsuleDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDCapsuleDesc

//{*********EPCDRoundConeIf*******
PyObject* EPCDRoundConeIf_GetLength( EPCDRoundConeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetLength());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDRoundConeIf_GetRadius( EPCDRoundConeIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->GetRadius());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDRoundConeIf_SetLength( EPCDRoundConeIf* self,PyFloatObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyFloat_Check(var1))
    {
        self->ptr->SetLength(PyFloat_AS_DOUBLE((PyObject*)var1));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDRoundConeIf_SetRadius( EPCDRoundConeIf* self,EPVec2f* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPVec2f_Check(var1))
    {
        self->ptr->SetRadius((*(var1->ptr)));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDRoundConeIf_SetWidth( EPCDRoundConeIf* self,EPVec2f* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPVec2f_Check(var1))
    {
        self->ptr->SetWidth((*(var1->ptr)));
        Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDRoundConeIf_methods[] =
{
    {"GetLength",(PyCFunction)EPCDRoundConeIf_GetLength,METH_NOARGS,"EPCDRoundConeIf::GetLength"},
    {"GetRadius",(PyCFunction)EPCDRoundConeIf_GetRadius,METH_NOARGS,"EPCDRoundConeIf::GetRadius"},
    {"SetLength",(PyCFunction)EPCDRoundConeIf_SetLength,METH_O,"EPCDRoundConeIf::SetLength"},
    {"SetRadius",(PyCFunction)EPCDRoundConeIf_SetRadius,METH_O,"EPCDRoundConeIf::SetRadius"},
    {"SetWidth",(PyCFunction)EPCDRoundConeIf_SetWidth,METH_O,"EPCDRoundConeIf::SetWidth"},
    {NULL}
};
void EPCDRoundConeIf_dealloc(PyObject* self)
{
}
PyObject* EPCDRoundConeIf_str()
{
    return Py_BuildValue("s","This is EPCDRoundConeIf.");
}
int EPCDRoundConeIf_init(EPCDRoundConeIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDRoundConeIf* EPCDRoundConeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDRoundConeIf *self;
    self = (EPCDRoundConeIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDRoundConeIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDRoundConeIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDRoundConeIf",/*tp_name*/
    sizeof(EPCDRoundConeIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDRoundConeIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDRoundConeIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDRoundConeIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDRoundConeIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDRoundConeIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDRoundConeIf_new,/*tp_new*/
};
void initEPCDRoundConeIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDRoundConeIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDRoundConeIfType);
    PyModule_AddObject(m,"CDRoundConeIf",(PyObject *)&EPCDRoundConeIfType);//モジュールに追加
}
PyObject* newEPCDRoundConeIf(struct CDRoundConeIf org)
{
    EPCDRoundConeIf *ret = EPCDRoundConeIf_new(&EPCDRoundConeIfType,NULL,NULL);
    ret->ptr = new CDRoundConeIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDRoundConeIf(struct CDRoundConeIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDRoundConeIf *ret = EPCDRoundConeIf_new(&EPCDRoundConeIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDRoundConeIf

//{*********EPCDRoundConeDesc*******
static PyMethodDef EPCDRoundConeDesc_methods[] =
{
    {NULL}
};
void EPCDRoundConeDesc_dealloc(PyObject* self)
{
    delete ((EPCDRoundConeDesc*)self)->ptr;
}
PyObject* EPCDRoundConeDesc_str()
{
    return Py_BuildValue("s","This is EPCDRoundConeDesc.");
}
int EPCDRoundConeDesc_init(EPCDRoundConeDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDRoundConeDesc* EPCDRoundConeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDRoundConeDesc *self;
    self = (EPCDRoundConeDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDRoundConeDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDRoundConeDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDRoundConeDesc",/*tp_name*/
    sizeof(EPCDRoundConeDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDRoundConeDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDRoundConeDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDRoundConeDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDRoundConeDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDRoundConeDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDRoundConeDesc_new,/*tp_new*/
};
void initEPCDRoundConeDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDRoundConeDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDRoundConeDescType);
    PyModule_AddObject(m,"CDRoundConeDesc",(PyObject *)&EPCDRoundConeDescType);//モジュールに追加
}
PyObject* newEPCDRoundConeDesc(struct CDRoundConeDesc org)
{
    EPCDRoundConeDesc *ret = EPCDRoundConeDesc_new(&EPCDRoundConeDescType,NULL,NULL);
    ret->ptr = new CDRoundConeDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDRoundConeDesc(struct CDRoundConeDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDRoundConeDesc *ret = EPCDRoundConeDesc_new(&EPCDRoundConeDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDRoundConeDesc

//{*********EPCDBoxIf*******
PyObject* EPCDBoxIf_GetBoxSize( EPCDBoxIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetBoxSize());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDBoxIf_GetFace( EPCDBoxIf* self,PyLongObject* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(PyLong_Check(var1))
    {
        EPCDFaceIf* ret = (EPCDFaceIf*)newEPCDFaceIf(self->ptr->GetFace(PyLong_AsSsize_t((PyObject*)var1)));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDBoxIf_GetVertices( EPCDBoxIf* self )
{
    UTAutoLock lock(EPCriticalSection);
    if(true)
    {
        EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetVertices());
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
PyObject* EPCDBoxIf_SetBoxSize( EPCDBoxIf* self,EPVec3f* var1 )
{
    UTAutoLock lock(EPCriticalSection);
    if(EPVec3f_Check(var1))
    {
        EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->SetBoxSize((*(var1->ptr))));
        if ( ret ) return (PyObject*) ret;
        else Py_RETURN_NONE;
    }
    Return_ArgError;
}
static PyMethodDef EPCDBoxIf_methods[] =
{
    {"GetBoxSize",(PyCFunction)EPCDBoxIf_GetBoxSize,METH_NOARGS,"EPCDBoxIf::GetBoxSize"},
    {"GetFace",(PyCFunction)EPCDBoxIf_GetFace,METH_O,"EPCDBoxIf::GetFace"},
    {"GetVertices",(PyCFunction)EPCDBoxIf_GetVertices,METH_NOARGS,"EPCDBoxIf::GetVertices"},
    {"SetBoxSize",(PyCFunction)EPCDBoxIf_SetBoxSize,METH_O,"EPCDBoxIf::SetBoxSize"},
    {NULL}
};
void EPCDBoxIf_dealloc(PyObject* self)
{
}
PyObject* EPCDBoxIf_str()
{
    return Py_BuildValue("s","This is EPCDBoxIf.");
}
int EPCDBoxIf_init(EPCDBoxIf* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDBoxIf* EPCDBoxIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDBoxIf *self;
    self = (EPCDBoxIf*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDBoxIf_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDBoxIfType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDBoxIf",/*tp_name*/
    sizeof(EPCDBoxIf),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDBoxIf_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDBoxIf_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDBoxIf",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDBoxIf_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDConvexIfType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDBoxIf_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDBoxIf_new,/*tp_new*/
};
void initEPCDBoxIf(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDBoxIfType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDBoxIfType);
    PyModule_AddObject(m,"CDBoxIf",(PyObject *)&EPCDBoxIfType);//モジュールに追加
}
PyObject* newEPCDBoxIf(struct CDBoxIf org)
{
    EPCDBoxIf *ret = EPCDBoxIf_new(&EPCDBoxIfType,NULL,NULL);
    ret->ptr = new CDBoxIf();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDBoxIf(struct CDBoxIf* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDBoxIf *ret = EPCDBoxIf_new(&EPCDBoxIfType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDBoxIf

//{*********EPCDBoxDesc*******
static PyMethodDef EPCDBoxDesc_methods[] =
{
    {NULL}
};
void EPCDBoxDesc_dealloc(PyObject* self)
{
    delete ((EPCDBoxDesc*)self)->ptr;
}
PyObject* EPCDBoxDesc_str()
{
    return Py_BuildValue("s","This is EPCDBoxDesc.");
}
int EPCDBoxDesc_init(EPCDBoxDesc* self,PyObject *args, PyObject *kwds)
{
    return 0;
}
EPCDBoxDesc* EPCDBoxDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
    EPCDBoxDesc *self;
    self = (EPCDBoxDesc*) type->tp_alloc(type,0);
    if ( self != NULL ) EPCDBoxDesc_init(self,args,kwds);
    return self;
}
PyTypeObject EPCDBoxDescType =
{
    PyVarObject_HEAD_INIT(NULL,0)
    "Collision.EPCDBoxDesc",/*tp_name*/
    sizeof(EPCDBoxDesc),/*tp_basicsize*/
    0,/*tp_itemsize*/
    (destructor)EPCDBoxDesc_dealloc,/*tp_dealloc*/
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
    (reprfunc)EPCDBoxDesc_str,/*tp_str*/
    0,/*tp_getattro*/
    0,/*tp_setattro*/
    0,/*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "CDBoxDesc",/*tp_doc*/
    0,/*tp_traverse*/
    0,/*tp_clear*/
    0,/*tp_richcompare*/
    0,/*tp_weaklistoffset*/
    0,/*tp_iter*/
    0,/*tp_iternext*/
    EPCDBoxDesc_methods,/*tp_methods*/
    0,/*tp_members*/
    0,/*tp_getset*/
    &EPCDShapeDescType,/*tp_base*/
    0,/*tp_dict*/
    0,/*tp_descr_get*/
    0,/*tp_descr_set*/
    0,/*tp_dictoffset*/
    (initproc)EPCDBoxDesc_init,/*tp_init*/
    0,/*tp_alloc*/
    (newfunc)EPCDBoxDesc_new,/*tp_new*/
};
void initEPCDBoxDesc(void)
{
    PyObject* m;
    if ( PyType_Ready( &EPCDBoxDescType ) < 0 ) return ;//Pythonクラスの作成
    m = PyImport_AddModule("Collision");
    Py_INCREF(&EPCDBoxDescType);
    PyModule_AddObject(m,"CDBoxDesc",(PyObject *)&EPCDBoxDescType);//モジュールに追加
}
PyObject* newEPCDBoxDesc(struct CDBoxDesc org)
{
    EPCDBoxDesc *ret = EPCDBoxDesc_new(&EPCDBoxDescType,NULL,NULL);
    ret->ptr = new CDBoxDesc();
    *ret->ptr = org;
    return (PyObject*)ret;
}
PyObject* newEPCDBoxDesc(struct CDBoxDesc* org)
{
    if(org == NULL)
    {
        Return_NewNullError;
    }
    EPCDBoxDesc *ret = EPCDBoxDesc_new(&EPCDBoxDescType,NULL,NULL);
    ret->ptr = org;
    return (PyObject*)ret;
}

//}CDBoxDesc
/**************** for Module ******************/
void initCollision(void)
{
    initEPCDShapeIf();
    initEPCDShapePairIf();
    initEPCDConvexIf();
    initEPCDFaceIf();
    initEPCDQuadFaceIf();
    initEPPHMaterial();
    initEPCDShapeDesc();
    initEPCDConvexMeshIf();
    initEPCDConvexMeshDesc();
    initEPCDConvexMeshInterpolateIf();
    initEPCDConvexMeshInterpolateDesc();
    initEPCDSphereIf();
    initEPCDSphereDesc();
    initEPCDCapsuleIf();
    initEPCDCapsuleDesc();
    initEPCDRoundConeIf();
    initEPCDRoundConeDesc();
    initEPCDBoxIf();
    initEPCDBoxDesc();
}
