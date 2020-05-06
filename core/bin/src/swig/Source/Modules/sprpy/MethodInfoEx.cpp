#include "MethodInfoEx.h"
#include "SprpyInfoObjectUtility.h"
#include <sstream>

namespace SwigSprpy
{;

extern SprpyInfoObjectUtility Util;

MethodInfoEx::_MethodInfoEx(): isOverloaded(false), isStatic(false), isMathMethod(false),
	isConstructor(false),className(NULL),methodName(NULL),pymethodName(NULL),
	classinfo(NULL)
{

}

MethodInfoEx::_MethodInfoEx(const _MethodInfoEx& obj)
{
	assign(obj);
}
void MethodInfoEx::assign(const _MethodInfoEx& obj)
{
	isOverloaded = obj.isOverloaded;
	isStatic = obj.isStatic;
	isConstructor = obj.isConstructor;
	isMathMethod = obj.isMathMethod;

	methodNameStr.assign(obj.methodNameStr);
	methodName = methodNameStr.c_str();

	pymethodNameStr.assign(obj.pymethodNameStr);
	pymethodName = pymethodNameStr.c_str();

	classNameStr = obj.classNameStr;
	className = classNameStr.c_str();
			
	//returnTypeStr.assign( obj.returnTypeStr);
	//returnType = returnTypeStr.c_str();
	//		
	//pyreturnTypeStr.assign( obj.pyreturnTypeStr );
	//pyreturnType = pyreturnTypeStr.c_str();

	methods.assign(obj.methods.begin(),obj.methods.end());

	pyparamCode.assign(obj.pyparamCode);
	pyfuncNameCode.assign(obj.pyfuncNameCode);
	publicBodyCode.assign(obj.publicBodyCode);

	pyparams.assign(obj.pyparams.begin(), obj.pyparams.end());
	pyparamNames.assign(obj.pyparamNames.begin(),obj.pyparamNames.end());

	classinfo = obj.classinfo;
}

MethodInfoEx MethodInfoEx::CreateMethodInfoEx(Node* _node ,const char* className)
{
	if (!_node) return MethodInfoEx();

	MethodInfoEx mex;
	Node* node = _node;

	mex.classNameStr.assign(className);
	mex.className = mex.classNameStr.c_str();

	mex.methodNameStr.assign(GetString(Getattr(node,"sym:name") )) ;
	if (mex.methodNameStr.empty())
		mex.methodNameStr.assign(GetString(Getattr(node,"name") )) ;
	Util.DelNamespace(mex.methodNameStr);

	mex.methodName = mex.methodNameStr.c_str(); 
	mex.pymethodNameStr = Util.GetPySymbolName( className ); 
	mex.pymethodNameStr.append("_"); 
	mex.pymethodNameStr.append(mex.methodName); 
	mex.pymethodName = mex.pymethodNameStr.c_str();

	mex.isStatic = (Checkattr(node,"storage","static")) ? true : false;

	mex.isOverloaded = (GetFlagAttr(node,"sym:overloaded")) ? true : false;

	if( mex.methodNameStr=="__add__"	||	mex.methodNameStr=="__sub__"	||
		mex.methodNameStr=="__mul__"	||	mex.methodNameStr=="__mod__"	||
		mex.methodNameStr=="__divmod__"	||	mex.methodNameStr=="__pow__"	||
		mex.methodNameStr=="__neg__"	||	mex.methodNameStr=="__pos__"	||
		mex.methodNameStr=="__abs__"	||	mex.methodNameStr=="__bool__"	||
		mex.methodNameStr=="__invert__"	||	mex.methodNameStr=="__lshift__"	||
		mex.methodNameStr=="__rshift__"	||	mex.methodNameStr=="__and__"	||
		mex.methodNameStr=="__xor__"	||	mex.methodNameStr=="__or__"		||
		mex.methodNameStr=="__int__"	||	mex.methodNameStr=="__reserved__"||
		mex.methodNameStr=="__float__"	||	mex.methodNameStr=="__iadd__"	||
		mex.methodNameStr=="__isub__"	||	mex.methodNameStr=="__imul__"	||
		mex.methodNameStr=="__imod__"	||	mex.methodNameStr=="__ipow__"	||
		mex.methodNameStr=="__ilshift__"||	mex.methodNameStr=="__irshift__"||
		mex.methodNameStr=="__iand__"	||	mex.methodNameStr=="__ixor__"	||
		mex.methodNameStr=="__ior__"	||	mex.methodNameStr=="__floordiv__"||
		mex.methodNameStr=="__div__"	||	mex.methodNameStr=="__ifloordiv__"||
		mex.methodNameStr=="__itruediv__"
	)
		mex.isMathMethod = true;

	mex.pyparamCode.assign("PY_PARAM");

	mex.pyfuncNameCode.assign(mex.pymethodNameStr);

	mex.publicBodyCode.assign("PUBLIC_BODY");

	return mex;
}
	

}
