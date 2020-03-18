#include "ClassInfo.h"
#include "SprpyInfoObjectUtility.h"
#include <sstream>



namespace SwigSprpy
{;

extern SprpyInfoObjectUtility Util;


ClassInfo::_ClassInfo():moduleinfo(NULL),constructors(NULL),classType(SPR_CLASSTYPE_NONE){}
ClassInfo::_ClassInfo(const _ClassInfo& obj )
{
	node = obj.node;
	moduleNameStr.assign(obj.moduleNameStr);
	moduleName = moduleNameStr.c_str();
	classNameStr.assign(obj.classNameStr);
	className = classNameStr.c_str();
	classNameExStr.assign(obj.classNameExStr);
	classNameEx = classNameExStr.c_str();
	pyobjectNameStr.assign( obj.pyobjectNameStr );
	pyobjectName = pyobjectNameStr.c_str();
	pysymbolNameStr.assign( obj.pysymbolNameStr);
	pysymbolName = pysymbolNameStr.c_str();
	pytypeobjectNameStr.assign(obj.pytypeobjectNameStr) ; pytypeobjectName = pytypeobjectNameStr.c_str();
			
	//methods.assign(obj.methods.begin(),obj.methods.end());
	//マップのコピー
	for(map<string,MethodInfoEx>::const_iterator it = obj.methods.begin(); it != obj.methods.end(); it++) {
		pair<map<string,MethodInfoEx>::iterator, bool> p = methods.insert(*it);
		if(!p.second)
			p.first->second = it->second;
	}
	//マップのコピー
	for(map<string,MethodInfoEx>::const_iterator it = obj.mathMethods.begin(); it != obj.mathMethods.end(); it++) {
		pair<map<string,MethodInfoEx>::iterator, bool> p = mathMethods.insert(*it);
		if(!p.second)
			p.first->second = it->second;
	}

	getsetTableElems.assign(obj.getsetTableElems.begin(),obj.getsetTableElems.end());

	methodTable.assign(obj.methodTable);
	mathMethodTable.assign(obj.mathMethodTable);
	getsetTable.assign(obj.getsetTable);
	methodTableName.assign(obj.methodTableName);
	mathMethodTableName.assign(obj.mathMethodTableName);
	getsetTableName.assign(obj.getsetTableName);
	getsetMethodCode.assign(obj.getsetMethodCode);
	typeobjectCode.assign(obj.typeobjectCode);

	constructors = new MethodInfoEx();
	if (obj.constructors)
		constructors->assign(*obj.constructors);
	bases.assign( obj.bases.begin(), obj.bases.end() );
	pyBaseTypes.assign( obj.pyBaseTypes.begin(), obj.pyBaseTypes.end() );
	isInherited = obj.isInherited;
	isStruct = obj.isStruct;
	isUTRef = obj.isUTRef;
	useDefaultConstructor = obj.useDefaultConstructor;
	classType = obj.classType;
	moduleinfo = obj.moduleinfo;
}
ClassInfo::~ClassInfo()
{
	if(this->constructors)
		delete this->constructors;
}


/////// Node*からMethodInfoExを作成、ClassInfoに追加する。
/////// function , constructorノードにおいて作成される
void ClassInfo::BuildMethodInfoEx(Node* node)
{
	MethodInfoEx *target = NULL;
	MethodInfoEx mex = MethodInfoEx::CreateMethodInfoEx(node,this->className);

	
	//SPR_IFDEFもコンストラクタとして扱われるので、回避
	if(mex.methodNameStr == "" || mex.methodNameStr == "SPR_IFDEF" || mex.methodNameStr == "SPR_DESCDEF" ||
		mex.methodNameStr == "SPR_VIFDEF")
		return ;
	
	//オペレータオーバーロードはとりあえず無視
	if(mex.methodNameStr.find("operator ") != -1) 
		return;


	//constructor 
	if( Checkattr(node,"nodeType","constructor") )
		target = this->constructors;

	//function
	else if (Checkattr(node,"nodeType","cdecl") && Checkattr(node,"kind","function"))
		target = &this->methods[mex.methodName];
	//other
	else
		return;


	
	//ClassInfoにMethodInfoExが無かったら代入
	if(target->methods.size() == 0 ) target->assign(mex);
					

	//ノードからMethodInfoを作ってMethodInfoExに入れる
	MethodInfo meth = MethodInfo::CreateMethodInfo(node,this->className);
	if(meth.node!=NULL)
		target->methods.push_back( meth );

	if(target->methods.empty()) *target = MethodInfoEx();

	

	//実装付きの場合 (%extendなど
	if (GetFlagAttr(node,"code"))
	{
		//実装付きで、かつ、.iファイルに定義されているもの（実装がどこにも無い場合）
		if ( Checkattr( parentNode(node) , "nodeType","extend") )
		{
			MethodInfo &mi = target->methods.back();
			mi.isExtend = true;
			mi.extendCode = GetString(Getattr(node,"code"));
		}
	}

}

void ClassInfo::UpdateMethodInfos()
	{
		//constructor 				
		//オーバーロードのフラグを元に、CreateMethodInfoで
		//埋められなかった項目をうめる
		if( !this->constructors->methods.empty())
			this->UpdateMethodInfo(*this->constructors);
		//コンストラクタが定義されていない場合は無理やり作る
		else
		{
			this->constructors->pyfuncNameCode = this->pyobjectNameStr + this->classNameStr;
			this->constructors->pyparamCode = string("PyObject* ") +Util.Vars.VARNAME_SELF+ ",PyObject* "+Util.Vars.VARNAME_ARG+ ",PyObject* kwds";
			this->constructors->methods.push_back(MethodInfo());
			if( this->useDefaultConstructor )
			{
				this->constructors->methods.back().bodyCode = string("EPObject_Ptr(")+Util.Vars.VARNAME_SELF+") = new "+ this->classNameStr +"();\n";
			}
			else
			{
				this->constructors->methods.back().bodyCode = string("EPObject_Ptr(")+Util.Vars.VARNAME_SELF+") = NULL;\n";
			}
			this->constructors->methods.back().returnCode = "return 0;\n";
		}
			
		//function
		for( map<string,MethodInfoEx>::iterator itex = this->methods.begin();
			itex != this->methods.end() ; itex++)
		{				
				//オーバーロードのフラグを元に、CreateMethodInfoで
				//埋められなかった項目をうめる
				this->UpdateMethodInfo((*itex).second);					
		}

	}

	//MethodInfoExはMethodInfoがpushされるまで引数などが決定できない
	//そのため一度ループですべてpushしてから、この関数により、引数などの決定をする
void ClassInfo::UpdateMethodInfo(MethodInfoEx &mex)
	{
		if (mex.methods.empty()) return;

		stringstream ss;
		

		mex.isConstructor = mex.methods[0].isConstructor;
		
		////
		//FUNCTION :=  
		//
		//PyObject* PY_FUNCNAME( PY_PARAMS )		<- MethInfoEx   MethInfo
		//{
		//  PUBLIC_BODY;				# PUBLIC_BODY := UTAutoLock lock(EPCriticalSection);
		//	if(CHECK){ BODY; RETURN; }	# CHECK := (
		//	if(CHECK){ BODY; RETURN; }	# BODY := { CALL_FUNC_AND_ASSIGN ret= NULL } or { CALL_FUNC_AND_ASSIGN  ret = WrawpValue(ret_tmp); } 
		//								# CALL_FUNC_AND_ASSIGN := { ret_tmp=NULL; /* C Code */ ret_tmp= .... ; } or 
		//														  { ret_tmp=NULL; ret_tmp = FUNCNAME(PARAM) } or
		//														  { FUNCNAME(PARAM) }
		//	if(CHECK){ BODY; RETURN; }	# RETURN { if ( ret ) return ret; else Py_Retern_None; }
		//	Retrun_PyArgError;
		//}


		// PUBLIC_BODY
		ss.str("");
		ss << "UTAutoLock LOCK(EPCriticalSection);\n";
		mex.publicBodyCode = ss.str();

		
		for (vector<MethodInfo>::iterator it = mex.methods.begin(); it != mex.methods.end() ; it++)
		{
			MethodInfo &mi = (*it);

			//paramNamesの登録
			ss.str("");
			for ( unsigned var_count = 0 ; var_count < mi.params.size() ; var_count++ )
			{
				//オーバーロードされているor引数が２こ以上orコンストラクタの場合はタプルで引数が渡されるから分解してから扱う
				//ただしMathMethodの場合は必ずPyObject*が渡されるので除外
				if( !mex.isMathMethod && (mex.isOverloaded || mi.params.size() > 1 || mi.isConstructor))
					ss << "(PyTuple_GetItem(" << Util.Vars.VARNAME_ARG << "," << var_count << "))" ;
				else
					ss << Util.Vars.VARNAME_ARG ;	

				mi.paramNames.push_back( ss.str() ); 
				ss.str("");
			}


			// PY_PARAM
			//pyparams,paramNamesの登録 (EP関数の引数
			//コンストラクタの場合３つ引数がいる
			if (mex.isConstructor)
			{
				mex.pyparams.clear(); mex.pyparamNames.clear();
			
				mex.pyparams.push_back("PyObject*");
				mex.pyparamNames.push_back(Util.Vars.VARNAME_SELF);
				
				mex.pyparams.push_back("PyObject*");			//initは必ず引数を３つ取る
				mex.pyparamNames.push_back(Util.Vars.VARNAME_ARG);
				mex.pyparams.push_back("PyObject*");
				mex.pyparamNames.push_back("kwds");
			}
			//コンストラクタじゃない場合
			else
			{
				mex.pyparams.clear(); mex.pyparamNames.clear();

				mex.pyparams.push_back("PyObject*");
				mex.pyparamNames.push_back(Util.Vars.VARNAME_SELF);

				//オーバーロード、もしくは引数がある場合
				if( mex.isOverloaded || ( !mex.methods.empty() && !mex.methods[0].params.empty() ) )
				{
					mex.pyparams.push_back("PyObject*");
					mex.pyparamNames.push_back(Util.Vars.VARNAME_ARG);
				}
			}

			ss.str("");
			for( unsigned i = 0 ; i < mex.pyparams.size() ; i++)
			{
				if(i != 0) ss << "," ;
				ss << mex.pyparams[i] << " " << mex.pyparamNames[i]; 
			}
			mex.pyparamCode.assign(ss.str());



			// CHECK (チェック関数登録		
			ss.str("");
			//オーバーロードされているor引数が２こ以上orコンストラクタの場合はタプルで引数が渡されるから分解してから扱う
			//ただしMathMethodの場合は必ずPyObject*が渡されるので除外
			if ( !mex.isMathMethod && ( mex.isOverloaded || mi.params.size() > 1 )) 
				ss << Util.Vars.VARNAME_ARG << " && PyTuple_Size(" << Util.Vars.VARNAME_ARG << ") == " << mi.params.size() << "&&" ;
			for ( unsigned i = 0 ; i < mi.params.size() ; i++)
			{
				if ( i != 0 ) ss << " && ";
				ss << Util.GetCheckfuncName( mi.params[i] , mi.paramNames[i] ) ;
			}
			if( mi.params.size() == 0 )
			{
				if ( mex.isOverloaded ) 
					ss << Util.Vars.VARNAME_ARG << " && PyTuple_Size(" << Util.Vars.VARNAME_ARG <<") == 0";
				else
					ss << "true" ;
			}
			mi.checkfuncCode.assign( ss.str() );

			// PARAM_DECL
			string PARAM_DECL;
			for( int i = 0 ; i < (int)mi.params.size(); i++){
				Variable param(&Util);
				param.pyName = string("py_param")+(char)('1'+i);
				param.pyCode = mi.paramNames[i];
				param.pyType = "PyObject *";
				param.cName = string("c_param")+ (char)('1'+i);
				param.cType = mi.params[i];

				param.UnWrapValue();
				PARAM_DECL += param.cCode + "\n";
			}

			// PARAM 
			ss.str("");
			for ( unsigned i = 0; i < mi.params.size() ; i++)
			{ 
				if( i != 0 ) ss << ",";
				ss << "c_param" << i+1;
			}
			mi.paramCode.assign( ss.str() );


			// CALL_FUNC_AND_ASSIGN
			//戻り値がある場合の、Cでの値を変数ret_tmpに入れるコード。ret_tmpはBODYでWrapValueが呼ばれ、変数retに代入され、Python側に渡される
			//関数のブリッジの場合は関数呼び出し文が、関数の実装(extend)の場合はコードが入る
			ss.str("");
			string CALL_FUNC_AND_ASSIGN;

			//関数の戻り値(C++)を格納する変数の宣言
			//戻り値ない場合はret_tmpを定義しない
			if (mi.returnTypeStr != "void" && !mi.isConstructor)
			{
				string returnTypeStr = mi.returnTypeStr;

				//戻り値で参照が返される場合はポインタで受け取る
				if (Util.IsRef(returnTypeStr))
					Util.StrReplaceall(returnTypeStr,"&","*");

				ss << returnTypeStr << " ret_tmp;\n";
			}

			//引数がある場合は引数格納変数の定義
			if ( !PARAM_DECL.empty() )
				ss << PARAM_DECL;


			if (mi.isExtend)
			{ 
				//vec __add__(vec var1){
				// return *$self + var1;
				//}
				//↓
				//vec EPvec__add__(PyObject* self,PyObject arg)
				//{
				//	vec ret_tmp;
				//	ret_tmp = *EPObject_Cast(self,vec) + *EPObject_Cast(arg,vec);
				//	return WrapValue( vec , ret_tmp );
				//}

				string src(mi.extendCode);
				
				Variable self(&Util);
				self.cName = "c_self";
				self.cType = mex.classNameStr+"*";		//$selfは自分の型のポインタ。だから*を追加して渡す
				self.pyName = string("py_") + Util.Vars.VARNAME_SELF;
				self.pyCode = Util.Vars.VARNAME_SELF;
				self.pyType = "PyObject *";
				self.UnWrapValue();

				//変数宣言&代入
				src.insert(1, self.cCode);

				//$selfの置換
				Util.StrReplaceall(src,"$self",self.cName);

				//var の置換
				if(mi.params.size() > 9) assert(0);	//引数10個以上には未対応
				for(int i = 0 ; i < (int)mi.params.size() ; i++)
				{
					string param(mi.params[i]);
					Util.DelRef(param);//参照が渡された場合は、値渡しされたときと同様に
					
					Variable var(&Util);
					var.cName = string("c_var") + (char)('1'+i);
					var.cType = param;
					var.pyName = string("py_var") + (char)('1'+i);
					var.pyCode = mi.paramNames[i];
					var.pyType = "PyObject *";
					var.UnWrapValue();

					//変数宣言&代入
					src.insert(1, var.cCode);
					
					Util.StrReplaceall(src,string("$var") + (char)('1'+i),var.cName);
				}

				//return
				//戻り値ない場合はret_tmpを定義しない
				if (mi.returnTypeStr != "void")
					Util.StrReplaceall(src,"return","ret_tmp =");

				ss << src;
			}
			else if(mex.isConstructor)
			{
				ss << "EPObject_Ptr(" << mex.pyparamNames[0] << ") = new " << mi.methodNameStr << "(" << mi.paramCode << ");" ;
			}
			else
			{
				//戻り値ない場合はret_tmpを定義しない
				if (mi.returnTypeStr != "void" && !mi.isConstructor) 
					ss << "ret_tmp = ";

				//参照を返す関数はポインタとして受け取る
				//そのためアドレスを返すようにする
				if ( Util.IsRef(mi.returnTypeStr) )
					ss << "&(";
				{
					//static関数の場合はself->func()ではなくclass::func()を呼ぶ
					if ( mex.isStatic ){
						ss << mex.className << "::" << mi.methodName << "(" << mi.paramCode << ")" ;
					}
					//普通の関数の場合はself->func()を呼ぶ
					else{
						ss << "EPObject_Cast(" << mex.pyparamNames[0] << "," << mex.className << ")->" << mi.methodName << "(" << mi.paramCode << ")";
					}
				}
				if ( Util.IsRef(mi.returnTypeStr) )
					ss << ")";
				ss<<";\n";
			}
			CALL_FUNC_AND_ASSIGN = ss.str();


			// BODY (BODYの登録
			ss.str("");
			Variable ret(&Util);
			if ( mi.returnTypeStr == "void" || mi.isConstructor )
				ss << CALL_FUNC_AND_ASSIGN;
			else
			{
				ss << CALL_FUNC_AND_ASSIGN;	// ret_tmpに結果を代入するコード
					
				//BUILDVALUE
				//ret_tmpを元にPyObject* retを作る  
				ret.cName = "c_ret";
				ret.cType = mi.returnTypeStr;
				ret.cCode = ( (Util.CreateCElem(ret.cType).pr == "&") ? string("*") : string(" ")) + "ret_tmp";
				ret.pyName = "py_ret";
				ret.pyType = "PyObject*";
				ret.WrapValue();

				ss << ret.pyCode ;
			}
			mi.bodyCode = ss.str();


			// RETURN (return文の登録
			if(mex.isConstructor)
			{
				mi.returnCode = string("if(EPObject_Ptr(") + Util.Vars.VARNAME_SELF + ") != NULL) return 0;";
				mi.returnCode.append("else {PyErr_NoMemory(); return -1;}"); 
			}
			else
			{
				if ( mi.returnTypeStr == "void" )
					mi.returnCode = " Py_RETURN_NONE; ";
				else
				{
					mi.returnCode = string("if ( !") + ret.pyName + " ){ PyErr_BadInternalCall();\nreturn NULL;\n}";
					mi.returnCode += string("if ( ") + ret.pyName + " == Py_None ) Py_RETURN_NONE;";

					//char,int,PyObjectの場合そのままreturn
					if (Util.GetSprClassType(mi.returnType) == SPR_CLASSTYPE_NOTSPR )
						mi.returnCode += string("return ") + ret.pyName + ";\n";

					else
					{
						mi.returnCode += string("if ( !EPObject_Ptr(") + ret.pyName + ") ) Py_RETURN_NONE;\n";

					if ( Util.GetSprClassType(mi.returnType) ==   SPR_CLASSTYPE_IF ){
						mi.returnCode += string("EPObject_RuntimeDCast((EPObject*)") + ret.pyName + "," + ret.cName + "->GetIfInfo());\n";
						mi.returnCode += string("return ") + ret.pyName + ";";
					}
					else 
						mi.returnCode += string("return ") + ret.pyName + ";";
					}

				}
			}

		} 
	}
	 
void ClassInfo::BuildMemberInfo(Node* node)
{
	string varname( GetString(Getattr(node,"name")) );
#ifdef _WIN32
	string type( DecodeType(node) );
#else
	std::string _tmp = std::string();
	std::string& __tmp = _tmp;
	string type( DecodeType(node, __tmp) );
#endif

	//constなど、変更できない場合はgetのみ
	bool isImmutable = GetFlagAttr(node,"feature:immutable") != NULL;
	
	string getterCode;
	string getterName = this->pysymbolNameStr + "_get_"+ varname;
	string setterCode;
	string setterName = isImmutable ? "NULL" : this->pysymbolNameStr + "_set_"+ varname;
	string doc = "member ("+type+")"+varname+" of "+this->className;

	// tableの要素
	string tableElem = "{\""+varname+"\",(getter)"+getterName+",(setter)"+setterName+",\""+doc+"\",NULL},\n";
		

	Variable mem(&Util);
	mem.cName = "c_" + varname;
	mem.cType = type;	
	// PythonのクラスをCのクラスに戻し、そのメンバ変数をあらわす	
	mem.cCode = string("EPObject_Cast(") + Util.Vars.VARNAME_SELF + "," + this->className + ")->" + varname;
	mem.pyName = "py_" + varname;
	mem.pyType = "PyObject*";
	mem.WrapValue();// mem.pyCodeが使えるようになる

	// setのときに与えられるPythonから与えられる変数
	Variable arg(&Util);
	arg.cName = string("c_")  + Util.Vars.VARNAME_ARG;
	arg.cType = type;
	arg.pyName = string("py_") +Util.Vars.VARNAME_ARG;
	// PythonのクラスをCのクラスに戻し、そのメンバ変数をあらわす
	arg.pyCode = Util.Vars.VARNAME_ARG;
	arg.pyType = "PyObject *";
	arg.UnWrapValue();
	
	getterCode = "static PyObject* __PYDECL " + getterName + "(PyObject* "+Util.Vars.VARNAME_SELF+"){"+
		mem.pyCode+
		"return " + mem.pyName + ";"+
		"}";


#ifdef _WIN32
	setterCode = "static int __PYDECL " + setterName + "(PyObject* "+Util.Vars.VARNAME_SELF+", PyObject* "+Util.Vars.VARNAME_ARG+"){"+
		mem.cType + " &" + mem.cName + " = " + (mem.cType == "int" ? "(int)" : "") + mem.cCode +";\n" +	//enumをintとして認識させるためにキャスト
		arg.cCode + "\n" + 
		//mem.cCode+
		mem.cName + " = " + arg.cName +";\n"+
		"return 0;" +
		"}";
#else
	// g++ complains... "taking address of temporary"
	setterCode = "static int __PYDECL " + setterName + "(PyObject* "+Util.Vars.VARNAME_SELF+", PyObject* "+Util.Vars.VARNAME_ARG+"){"+
		mem.cType + " _tmp = " + (mem.cType == "int" ? "(int)" : "") + mem.cCode +";\n" + //enumをintとして認識させるためにキャスト
		mem.cType + " &" + mem.cName + " = _tmp;\n" +
		arg.cCode + "\n" + 
		//mem.cCode+
		mem.cName + " = " + arg.cName +";\n"+
		"return 0;" +
		"}";
#endif

	//getter,setterの定義（ソースコード）作成
	this->getsetMethodCode += getterCode;
	if(!isImmutable)
		this->getsetMethodCode += setterCode;

	this->getsetTableElems.push_back(tableElem);
}


void ClassInfo::UpdateClassInfo()
	{
		stringstream ss;

		
		/////////// __add__ などのオペレータオーバーライドのための構造体
		ss.str("");
		ss << "static PyNumberMethods " << this->mathMethodTableName << "={\n";
		{
			map<string,MethodInfoEx>::iterator it;

			/* __add__ */
			it = this->methods.find("__add__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __add__ */\n";
			/* __sub__ */
			it = this->methods.find("__sub__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __sub__ */\n";
			/* __mul__ */
			it = this->methods.find("__mul__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __mul__ */\n";
			/* __mod__ */
			it = this->methods.find("__mod__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __mod__ */\n";
			/* __divmod__ */
			it = this->methods.find("__divmod__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __divmod__ */\n";
			/* __pow__ */
			it = this->methods.find("__pow__");
			ss << "(ternaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __pow__ */\n";
			/* __neg__ */
			it = this->methods.find("__neg__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __neg__ */\n";
			/* __pos__ */
			it = this->methods.find("__pos__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __pos__ */\n";
			/* __abs__ */
			it = this->methods.find("__abs__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __abs__ */\n";
			/* __bool__ */
			it = this->methods.find("__bool__");
			ss << "(inquiry)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __bool__ */\n";
			/* __invert__ */
			it = this->methods.find("__invert__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __invert__ */\n";
			/* __lshift__ */
			it = this->methods.find("__lshift__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __lshift__ */\n";
			/* __rshift__ */
			it = this->methods.find("__rshift__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __rshift__ */\n";
			/* __and__ */
			it = this->methods.find("__and__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __and__ */\n";
			/* __xor__ */
			it = this->methods.find("__xor__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __xor__ */\n";
			/* __or__ */
			it = this->methods.find("__or__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __or__ */\n";
			/* __int__ */
			it = this->methods.find("__int__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __int__ */\n";
			/* __reserved__ */
			it = this->methods.find("__reserved__");
			ss << "(void *)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __reserved__ */\n";
			/* __float__ */
			it = this->methods.find("__float__");
			ss << "(unaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __float__ */\n";
			/* __iadd__ */
			it = this->methods.find("__iadd__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __iadd__ */\n";
			/* __isub__ */
			it = this->methods.find("__isub__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __isub__ */\n";
			/* __imul__ */
			it = this->methods.find("__imul__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __imul__ */\n";
			/* __imod__ */
			it = this->methods.find("__imod__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __imod__ */\n";
			/* __ipow__ */
			it = this->methods.find("__ipow__");
			ss << "(ternaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __ipow__ */\n";
			/* __ilshift__ */
			it = this->methods.find("__ilshift__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __ilshift__ */\n";
			/* __irshift__ */
			it = this->methods.find("__irshift__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __irshift__ */\n";
			/* __iand__ */
			it = this->methods.find("__iand__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __iand__ */\n";
			/* __ixor__ */
			it = this->methods.find("__ixor__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __ixor__ */\n";
			/* __ior__ */
			it = this->methods.find("__ior__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __ior__ */\n";
			/* __floordiv__ */
			it = this->methods.find("__floordiv__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __floordiv__ */\n";
			/* __div__ */
			it = this->methods.find("__div__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __div__ */\n";
			/* __ifloordiv__ */
			it = this->methods.find("__ifloordiv__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __ifloordiv__ */\n";
			/* __itruediv__ */
			it = this->methods.find("__itruediv__");
			ss << "(binaryfunc)" << ((it!=this->methods.end()) ? it->second.pymethodName : "NULL" ) << ",/* __itruediv__ */\n";		}
		ss << "};\n";
		this->mathMethodTable.assign(ss.str());


		//methodTableを作成
		ss.str("");
		ss << "static PyMethodDef " << this->methodTableName << "[] = {\n";
		for(map<string,MethodInfoEx>::iterator itmex = this->methods.begin() ; itmex != this->methods.end() ; itmex++)
		{
			MethodInfoEx mex = (*itmex).second;

			if(mex.isMathMethod) continue;

			if ( mex.isOverloaded ) 
				ss << "{\"" << mex.methodName << "\",(PyCFunction)" << this->pysymbolName << "_" << mex.methodName << ",METH_VARARGS"<<( mex.isStatic ?  " | METH_STATIC" : " " )<<",\"" << this->pysymbolName << "::" << mex.methodName << "\"},\n";
				
			else if (!mex.methods.empty())
			{
				MethodInfo mi = mex.methods[0];

				//NOARGS
				if( mi.params.size() == 0)
					ss << "{\"" << mex.methodName << "\",(PyCFunction)" << this->pysymbolName << "_" << mex.methodName << ",METH_NOARGS"<<( mex.isStatic ?  " | METH_STATIC" : " " )<<",\"" << this->pysymbolName << "::" << mex.methodName << "\"},\n";
				////O
				else if( mi.params.size() == 1)
					ss << "{\"" << mex.methodName << "\",(PyCFunction)" << this->pysymbolName << "_" << mex.methodName << ",METH_O"<<( mex.isStatic ?  " | METH_STATIC" : " " )<<",\""      << this->pysymbolName << "::" << mex.methodName << "\"},\n";
				///VARARGS
				else
					ss << "{\"" << mex.methodName << "\",(PyCFunction)" << this->pysymbolName << "_" << mex.methodName << ",METH_VARARGS"<<( mex.isStatic ?  " | METH_STATIC" : " " )<<",\""<< this->pysymbolName << "::" << mex.methodName << "\"},\n";

			}
		}
		ss << "{NULL}\n";//sentinel
		ss << "};\n";
		this->methodTable.assign(ss.str());

		//getsetTableを作成
		ss.str("");
		ss << "static PyGetSetDef "<< this->getsetTableName << "[] = {\n";
		{	
			for(vector<string>::iterator it = this->getsetTableElems.begin(); it != this->getsetTableElems.end() ; it++)
				ss << (*it);

			ss << "{NULL}\n";
		}
		ss << "};\n";
		this->getsetTable.assign(ss.str());


		//TypeObjectを作成
		ss.str("");
		ss << "PyTypeObject " << this->pytypeobjectName << " = {\n";
		{
			ss << "PyVarObject_HEAD_INIT(NULL,0)\n";
			ss << "\"" << this->moduleName << "." << Util.TrimEnd(this->className,"If") << "\",/*tp_name*/\n";
			ss << "sizeof(EPObject),/*tp_basicsize*/\n";
			ss << "0,/*tp_itemsize*/\n";
			ss << "(destructor)"<< this->pysymbolName <<"_dealloc,/*tp_dealloc*/\n";
			ss << "0,/*tp_print*/\n";
			ss << "0,/*tp_getattr*/\n";
			ss << "0,/*tp_setattr*/\n";
			ss << "0,/*tp_reserved*/\n";

			{
				map<string,MethodInfoEx>::iterator it;
				if ( (it = this->methods.find("__repr__")) != this->methods.end() )
					ss << "(reprfunc)"<<it->second.pyfuncNameCode<<",/*tp_repr*/\n";
				else
					ss << "0,/*tp_repr*/\n";
			}


			ss << "&"<< (this->mathMethodTable.empty() ? "0" : this->mathMethodTableName) << ",/*tp_as_number*/\n";
			ss << "0,/*tp_as_sequence*/\n";
			ss << "0,/*tp_as_mapping*/\n";
			ss << "0,/*tp_call*/\n";
			ss << "0,/*tp_hash*/\n";
			{
				map<string,MethodInfoEx>::iterator it;
				if ( (it = this->methods.find("__str__")) != this->methods.end() )
					ss << "(reprfunc)"<<it->second.pyfuncNameCode<<",/*tp_str*/\n";
				else
					ss << "(reprfunc)"<< this->pysymbolName <<"_str,/*tp_str*/\n";
			}
			
			ss << "0,/*tp_getattro*/\n";
			ss << "0,/*tp_setattro*/\n";
			ss << "0,/*tp_as_buffer*/\n";
			ss << "Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/\n";
			ss << "\""<< Util.TrimEnd(this->className,"If") <<"\",/*tp_doc*/\n";
			ss << "0,/*tp_traverse*/\n";
			ss << "0,/*tp_clear*/\n";
			ss << "0,/*tp_richcompare*/\n";
			ss << "0,/*tp_weaklistoffset*/\n";
			ss << "0,/*tp_iter*/\n";
			ss << "0,/*tp_iternext*/\n";
			ss << this->methodTableName <<",/*tp_methods*/\n";
			ss << "0,/*tp_members*/\n";
			ss << this->getsetTableName <<",/*tp_getset*/\n";
			if ( this->bases.size() > 0 )
			{
				//基底クラスは１つだけ　とりあえず
				//string tpbase;
				//for(int i=0; i<cls.bases.size( ++i){
				//tpbase.append(cls.bases[i]
				//}
				//"&%s,/*tp_base*/\n",GetPyTypeObjectName(tpbase.c_str()).c_str()
				ss << "&"<<this->pyBaseTypes[0]<<",/*tp_base*/\n";
			}
			else{
				//すべてのEPクラスはEPObjectを継承する
				ss << "&EPObjectType,\n";

				//ss << "0,/*tp_base*/\n";
			}
			ss << "0,/*tp_dict*/\n";
			ss << "0,/*tp_descr_get*/\n";
			ss << "0,/*tp_descr_set*/\n";
			ss << "0,/*tp_dictoffset*/\n";
			ss << "(initproc)"<<(this->constructors->methods.empty()?"0":this->constructors->pyfuncNameCode) << ",/*tp_init*/\n";
			ss << "0,/*tp_alloc*/\n";
			ss << "(newfunc)"<< this->pysymbolName <<"_new,/*tp_new*/\n";
		}
		ss << "};\n";
		this->typeobjectCode.assign(ss.str());
	}

//本来は.iファイルにextendで定義すべきだが、大量すぎて面倒な場合はここに書く
void ClassInfo::SetAdditionalMethods()
{
	//GetIfInfo
	if( this->classType == SPR_CLASSTYPE_IF)
	{
		string funcname;
		string pyfuncname;
		funcname = "GetIfInfoStatic";
		pyfuncname = this->pysymbolNameStr + "_" + funcname;
		MethodInfoEx mex;
		mex.classinfo = this;
		mex.classNameStr = this->classNameStr;
		mex.className = mex.classNameStr.c_str();
		mex.isConstructor = false;
		mex.isMathMethod = false;
		mex.isOverloaded = false;
		mex.isStatic = true;
		mex.methodNameStr = funcname;
		mex.methodName = mex.methodNameStr.c_str();
		mex.pymethodNameStr = pyfuncname;
		mex.pymethodName = mex.pymethodNameStr.c_str();
		mex.pyfuncNameCode = mex.pymethodNameStr;
		mex.pyparamCode = "";
		mex.pyparamNames = vector<string>();
		mex.pyparams = vector<string>();

		//mex.methods
		MethodInfo mi;
		mi.node = NULL;
		mi.methodNameStr = funcname;
		mi.methodName = mi.methodNameStr.c_str();
		mi.pymethodNameStr = pyfuncname;
		mi.pymethodName = mi.pymethodNameStr.c_str();
		mi.checkfuncCode = "CHECK";
		mi.bodyCode = "BODY";
		mi.returnCode = "RET";
		mi.paramCode = "PyObject* self";
		mi.isConstructor = mex.isConstructor;
		mi.returnTypeStr = "const IfInfo*";
		mi.returnType = mi.returnTypeStr.c_str();
		mi.pyreturnTypeStr = "PyObject*";
		mi.pyreturnType = mi.pyreturnTypeStr.c_str();
		mi.params = vector<string>();


		mex.methods.push_back(mi);

		this->methods.insert(pair<string,MethodInfoEx>(funcname,mex));
	}
}

ClassInfo ClassInfo::CreateClassInfo(Node* _node,const char* moduleName)
	{
		if(!_node) return ClassInfo();
		ClassInfo ci;
		ci.node = _node;
		ci.moduleNameStr = string(moduleName);
		ci.moduleName = ci.moduleNameStr.c_str();
		ci.classNameStr = GetString(Getattr(ci.node,"sym:name")); ci.className = ci.classNameStr.c_str();
		ci.pyobjectNameStr = Util.GetPyObjectName( ci.className ); ci.pyobjectName = ci.pyobjectNameStr.c_str();
		ci.pysymbolNameStr = Util.GetPySymbolName( ci.className ); ci.pysymbolName = ci.pysymbolNameStr.c_str();
		ci.pytypeobjectNameStr = Util.GetPyTypeObjectName(ci.className); ci.pytypeobjectName = ci.pytypeobjectNameStr.c_str();
		ci.isStruct = ( GetString(Getattr(ci.node,"kind")).compare("struct") == 0); 
		ci.useDefaultConstructor = GetFlagAttr(ci.node,"allocate:default_constructor") != NULL;
		ci.useDefaultConstructor = !GetFlagAttr(ci.node,"feature:not_newable");
		ci.classNameExStr.assign(string( ci.isStruct ? "struct " : "" )  + ci.classNameStr);
		ci.classNameEx = ci.classNameExStr.c_str();

		ci.methodTableName = ci.pysymbolNameStr + "_method_table";
		ci.mathMethodTableName = ci.pysymbolNameStr + "_math_method_table";
		ci.getsetTableName = ci.pysymbolNameStr + "_getset_table";

		ci.constructors = NULL; 

		DOH* blist = Getattr(_node,"baselist");
		if ( DohLen( blist ) > 0 )
		{
			ci.isInherited = true;

			for(int i=0; i<DohLen(blist); ++i){
				DOH* item = DohGetitem(blist, i);
				if (DohIsString(item)){
					ci.bases.push_back(GetString(item));
					ci.pyBaseTypes.push_back(Util.GetPyTypeObjectName(GetString(item)));
				}
			}
		}
		else ci.isInherited = false;

		ci.isUTRef = false;
		for ( vector<string>::iterator it = Util.Vars.UTRefList.begin() ; it != Util.Vars.UTRefList.end() ; it++)
		{
			int found =  ci.classNameStr.rfind( *it );
			if ( found != -1 && found == ci.classNameStr.length() - it->length() )
				ci.isUTRef = true;
		}

		
		ci.classType = Util.GetSprClassType(ci.className);
		return ci;
	}
}
