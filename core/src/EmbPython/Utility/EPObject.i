// EPObjectに関するSwigマクロの定義

//ポインタではなく実体を受け取るコンストラクタを追加（デフォルトではポインタのみを受け取る
%define EXTEND_NEW(type)
%header%{
	PyObject* __EPDECL newEP##type(type $var1);
	PyObject* __EPDECL newEP##type();
%}

%wrapper%{
	PyObject* newEP##type(type $var1)
	{
		PyObject *ret = EP##type##_new(&EP##type##Type,NULL,NULL);
		EPObject_Ptr(ret) = new type($var1);
		((EPObject*)ret)->mm = EP_MM_PY;
		return ret;
	}
	
	PyObject* newEP##type()
	{
		PyObject *ret = EP##type##_new(&EP##type##Type,NULL,NULL);
		EPObject_Ptr(ret) = new type();
		((EPObject*)ret)->mm = EP_MM_PY;
		return ret;
	}
%}
%enddef

%define EXTEND_N_GETS_TO_LIST(CLS,NMETHOD,GETMETHOD,GETCLS)
%ignore CLS##::##GETMETHOD;
%extend CLS{
	PyObject* GETMETHOD(){
		int nobjs = $self->NMETHOD();
		PyObject* pyObjs = PyList_New(nobjs);
		PyObject* pyObj;
		GETCLS** objs = $self->GETMETHOD();

		for ( int i = 0 ; i < nobjs ; i ++){
			pyObj = newEP##GETCLS(objs[i]);
			EPObject_RuntimeDCast((EPObject*)pyObj,objs[i]->GetIfInfo());
			PyList_SET_ITEM(pyObjs,i,pyObj);
		}
		return pyObjs;
	}
}
%enddef

%define EXTEND_N_GET_TO_LIST(CLS,NMETHOD,GETMETHOD,GETCLS)
//%ignore CLS##::##GETMETHOD;
%extend CLS{
	PyObject* GETMETHOD##s(){
		int nobjs = $self->NMETHOD();
		PyObject* pyObjs = PyList_New(nobjs);
		PyObject* pyObj;
		GETCLS* obj;
		
		
		for ( int i = 0 ; i < nobjs ; i ++){
			obj = $self->GETMETHOD(i);
			pyObj = newEP##GETCLS( obj );
			EPObject_RuntimeDCast((EPObject*)pyObj,obj->GetIfInfo());
			PyList_SET_ITEM(pyObjs,i,pyObj);
		}
		return pyObjs;
	}
}
%enddef

