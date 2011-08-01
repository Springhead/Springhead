// EPObjectに関するSwigマクロの定義

//ポインタではなく実体を受け取るコンストラクタを追加（デフォルトではポインタのみを受け取る
%define EXTEND_NEW(type)
%header%{
	PyObject* __EPDECL newEP##type##Object(type var1);
%}
%wrapper%{
	PyObject* newEP##type##Object(type var1)
	{
		PyObject *ret = EP##type##Object_new(&EP##type##Type,NULL,NULL);
		EPObject_Ptr(ret) = new type(var1);
		((EPObject*)ret)->mm = EP_MM_PY;
		return ret;
	}
%}
%enddef
