#include "SprpyInfoObjectUtility.h"
#include "SprpyInfoObject.h"
#include <sstream>

namespace SwigSprpy
{
	vector<string> SprpyInfoObjectUtility::ParmtoStrings( DOH* parm )
	{
		string s = GetString(ParmList_protostr(parm));
		return ParmtoStrings(s.c_str());
	}

	vector<string> SprpyInfoObjectUtility::ParmtoStrings( const char* parm )
	{
		vector<string> ret;
		string s = string(parm);
		
		int index;
		if (s.length() != 0 ){

			//////切り分け
			// s = 'Vec3d,Vec2d,Vec3f'
			index = -1;
			while( -1 != (index = s.find(",")) && index+1 < (int)s.length() -1 )
			{
				ret.push_back( s.substr(0,index) );
				s.assign( s.substr(index+1) );
			}
			ret.push_back(s);
		}

		return ret;
	}

	// CElemのtypeにあたる文字列を受け取り、enumだった場合はEnum*を返す
	const Enum* SprpyInfoObjectUtility::GetEnum( string cls )
	{
		for(std::set<Enum>::iterator it = Vars.enuminfos.begin(); it != Vars.enuminfos.end(); it++)
		{
			if( it->name == cls || it->fullname == cls )
				return &*it;
		}

		return NULL;
	}

	string SprpyInfoObjectUtility::TrimEnd( string str , string key)
	{
		if( str.length() > key.length() && 
			str.substr( str.length() - key.length() ,key.length() ) == key)  
			str = str.substr(0,str.length() - key.length() );
		return str;
	}

	string SprpyInfoObjectUtility::ResolveTypedef(CElem elem)
	{
		if( elem.type == "GRHandler" )
		{
			return string("unsigned int");
		}
				
		if( elem.type == "IAMode" )
		{
			return string("Spr::FWInteractMode");
		}

		if( elem.type == "UTString")
		{
			return string("string");
		}
		return string();
	}
	 

#pragma region Variable
	Variable::Variable(SprpyInfoObjectUtility *util)
	{
		this->Util = util;
	}

	void Variable::WrapValue()
	{
		pyType = "PyObject*";
		////pyCode = this->Util->WrapValue( cType, cCode );
		
		CElem cls = Util->CreateCElem( cType );

		if ( cls.classkeyword == "enum")
			cls = Util->CreateCElem("int");
	
		int nPointer = Util->CountPointer(cls.pr);
		
		
		//リストを作るのではなく、APIを変える方向で
		//////ポインタの深さによる処理
		if (nPointer == 2 ) 
			pyCode = string("___assert___") + cls.type;

		//voidのばあい強制終了　 void*はOK
		if( cls.type == "void" && nPointer == 0 ) assert(0);

		//int doubleなどのポインタは未実装だから　強制終了
//		if( nPointer != 0 && Util->GetPySymbolName(cls.type).find(Util->Vars.classnamePrefix) == -1 )
			//assert(0);	//とりあえず動かす

		//特定の型の場合 int, double , などなど
		if( cls.type == "PyObject" )
		{
			pyCode = cName;
		}
		else if( cls.type == "int" || //unsignedも真
				cls.type == "short" ||  
				cls.type == "long"  || //unsignedも真
				(cls.type == "char" && !cls.usigned.empty())
				)
		{
			pyCode =  "PyLong_fromAny(" + cName +")";
		}
		else if( cls.type == "double" || cls.type == "float")	{
			pyCode =  "PyFloat_fromAny(" + cName +")";
		}
		else if( cls.type == "bool" )	{
			pyCode =  "PyBool_FromLong(" + cName + "? 1 : 0)";
		}
		else if( cls.type == "size_t")	{
			pyCode =  "Py_BuildValue(\"n\"," + cName +")";
		}
		else if(cls.type == "char")	{
			if (nPointer == 1)
				pyCode =  "Py_BuildValue(\"s\"," + cName +")";
			else if(nPointer == 0)
				pyCode =  "Py_BuildValue(\"c\"," + cName +")";
			else
				assert(0);
		}
		else if(cls.type == "string"){
			pyCode =  "Py_BuildValue(\"s\"," + cName +")";
		}
		else if( cls.type == "void" && nPointer > 0 ){
			pyCode =   "newEPObject(" + cName +")";
		}
		else if( cls.type == "vector" )	{
			Variable &list(*this);
			pyCode = "int size = " + list.cName + ".size();\n";
			pyCode+= list.pyType + " " + list.pyName + " = PyList_New("+list.cName+".size());\n";
			pyCode+= "for( int i = 0 ; i < size ; i++){\n"; 
			{
				Variable item(Util);
				item.cName = "c_item";
				item.cType = cls.child->storage +" "+ cls.child->classkeyword +" "+ cls.child->cv +" "+ cls.child->pr +" "+ cls.child->classkeyword +cls.child->fulltype;
				item.cCode = list.cName + "[i]";
				item.pyName = "py_item";
				item.pyType = "PyObject*";
				item.WrapValue();

				pyCode += item.pyCode;
				pyCode += "PyList_SetItem(" + list.pyName + ", i ," + item.pyName +");"; 
			}
			pyCode+= "}\n";
		}

		//特定の型じゃない場合 
		else{	
			pyCode =  "new" + Util->GetPySymbolName(cls.type) + "(" + cName +")";
		}



		//複数行の場合
		if( cls.type == "vector")
		{
			pyCode = cType + " " + cName + " = " + cCode + ";" + pyCode ;
		}
		//１行の場合
		else
		{
			//変数に代入する
			pyCode = pyType + " " +pyName + " = " + pyCode + ";";
			pyCode = cType + " " + cName + " = " + cCode + ";" + pyCode ;
		}
	}


	void Variable::UnWrapValue()
	{
		///////////cCode = this->Util->UnwrapValue( cType, pyCode);

		CElem cls = Util->CreateCElem( cType );
	
		int nPointer = Util->CountPointer(cls.pr);
				
		if ( cls.classkeyword == "enum" ){
			//intとして扱い、目的のenum型へキャストする
			cCode = "((" + cls.fulltype + ")PyLong_AsLong(" + pyName + "))";
		}
		//特定の型の場合 int, doubleなどの組み込み型やPyObject などなど
		else if( cls.type == "PyObject" )	{
			cCode = pyName  ;
		}
		else if( cls.type == "int" || //unsignedも真
				 cls.type == "short" || 
				 cls.type == "long" || //unsignedも真
				( cls.type == "char" && !cls.usigned.empty() ) //unsigned charだけ真
				)
		{
			cCode = "PyObject_asLong(" + pyName + ")"  ;
		}
		else if(  cls.type == "double" ||  cls.type == "float" )	{
			cCode = "PyObject_asDouble(" + pyName + ")"  ;
		}
		else if(  cls.type == "bool" )	{
			cCode = "(Py_True == " + pyName + ")"  ;
		}
		else if(  cls.type == "size_t")	{
			cCode = "PyLong_AsSsize_t(" + pyName + ")"  ;
		}
		else if(  cls.type == "char" )	{
			cCode = "ConvertStr conv_"+pyName+"("+pyName+");";
			if( nPointer == 0)
				cCode += cType + " " + cName + " = (*conv_"+pyName+".GetBuffer());" ;
			else if( nPointer == 1)
				cCode += cType + " " + cName + " = conv_"+pyName+".GetBuffer();" ;
			else 
				assert(0);
		}
		else if(  cls.type == "string" )	{
			cCode = "CONVERT_WCHARSTR_TO_STRING(" + pyName + ")"  ;
		}
		else if(  cls.type == "vector" )	{
			Variable &arg(*this);
			cCode = cType + " " + cName + ";";
			cCode+= string("if( PyList_Check(") + arg.pyName + ") ){";
			{
				cCode += "int size = PyList_GET_SIZE(" + arg.pyName + ");\n";
				cCode += "for( int i = 0 ; i < size ; i++){\n"; 
				{
					Variable item(Util);
					item.cName = "c_item";
					item.cType = cls.child->storage +" "+ cls.child->classkeyword +" "+ cls.child->cv +" "+ cls.child->pr +" "+ cls.child->classkeyword +cls.child->fulltype;
					item.pyName = "py_item";
					item.pyType = "PyObject*";
					item.pyCode = "PyList_GetItem("+arg.pyName+",i)";
					item.UnWrapValue();

					cCode += item.cCode;
					cCode += arg.cName + ".push_back(" + item.cName +");"; 
				}
				cCode+= "}\n";
			}
			cCode += "}";

		}
		//特定の型じゃない場合
		else
		{
			//暗黙のキャストが必要なSpringheadのクラスなど(Vec,Pose....
			if(  cls.type == "Vec3d")	{
				cCode = "(&PyObject_asVec3d(" + pyName + "))"  ;
			}
			else if(  cls.type == "Vec3f"){
				cCode = "(&PyObject_asVec3f(" + pyName + "))"  ;
			}
			else{
				cCode = "EPObject_Cast(" + pyName + "," + cls.fulltype + ")" ;
			}

			//ポインタの深さによる処理
			if( nPointer ==0 )	{
				cCode.insert(0,"(*");//（変換先が）実体,参照の場合はポインタを実体化
				cCode.append(")");
			}
			else if ( nPointer == 1)
				;		//ポインタが1つの場合はそのまま
			else
				cCode = "___assert___"+ cls.type ;
				//asset(0);
		}

		//複数行の場合
		if(  cls.type == "vector" ||
			(cls.type == "char" && cls.usigned.empty() )
			)
		{
			cCode = pyType + " " + pyName + " = " + pyCode + ";\n" + cCode;
		}
		//１行の場合
		else
		{
			cCode = cType + " " +cName + " = " + cCode + ";\n";
			cCode = pyType + " " + pyName + " = " + pyCode + ";\n" + cCode;
		}
	}


#pragma endregion
	
	string SprpyInfoObjectUtility::GetPySymbolName(string clsStr)
	{		
		CElem cls = CreateCElem( clsStr );
		if( cls.classkeyword == "enum")
			cls = CreateCElem("int");
	
		int nPointer = CountPointer(cls.pr);
		string symbol;

		
		if ( !cls.type.empty() )
		{
			//規定のキーワードの場合は規定の単語を返す double -> PyDoubleObject など
			if (  cls.type == "PyObject" ||  cls.type == "bool" )
			{
				symbol = "PyObject";
			}			
			else if (  cls.type == "double" ||  cls.type == "float")
			{
				symbol = "PyFloat";
			}
			else if ( 
				cls.type == "int" || //unsignedも真
				cls.type == "size_t" || 
				cls.type == "short" ||
				cls.type == "long" ||//unsignedも真
				(cls.type == "dchar" && !cls.usigned.empty()) //unsigned charのみ
				)
			{
				symbol = "PyLong";
			}
			else if ( cls.type == "char" ||
					  cls.type == "string" )
			{
				symbol = "PyUnicode";
			}
			else if ( cls.type == "void" && nPointer >0)
			{
				symbol = "EPObject";
			}
			else 
			{
				symbol = cls.type;
				if( nPointer <= 1)
				{
					symbol.insert(0,Vars.classnamePrefix);
				}
				//リストを作るのではなく、APIを変える方向で
				//else if( nPointer == 2 )
				//	{ name.insert(0,classnamePrefix); name.append("List");}
			}
		}
		return symbol;
	}
	
	string SprpyInfoObjectUtility::GetPyObjectName(string cls)
	{
		string name = GetPySymbolName(cls);
				
		if ( !name.empty() )
		{
			if ( name == "PyObject" )
			{
				//name = "PyObject";
			}			
			else
			{
				name.append("Object");
			}
		}
		return name;
	}

	int SprpyInfoObjectUtility::DelConst(string& cls)
	{
		bool isConst = false;
		int index ;
		
		while(true)
		{
			index = cls.find("const");
			if(index == 0)
			{
				if( cls.find(" ",index) == index + 5  || cls.find("*",index) == index + 5  || cls.find("&",index) == index + 5 )
					{ cls.erase( index, 5); isConst = true; }
			}
			else if ( index != -1 )
			{
				if( cls.find(" ",index) == index + 5  || cls.find("*",index) == index + 5  || cls.find("&",index) == index + 5  )
					{ cls.erase( index, 5); isConst = true; }
				else if( cls.find(" ") == index -1 )
					{ cls.erase( index, 5); isConst = true; }
			}
			else break;
		}

		//if ( cls.find("const ") != -1) { cls.erase( cls.find("const") , 5 ); isConst = true;} 
		//if ( cls.find("const") == 0)
		return isConst ? 1 : 0 ;
	}

	void SprpyInfoObjectUtility::DelRef(string& cls)
	{
		//////参照の場合　　無視
		while ( cls.find("&") != -1) cls.erase( cls.find("&") ,1 );
	}

	bool SprpyInfoObjectUtility::IsRef(string& cls)
	{
		return ( cls.find("&") != -1);
	}

	void SprpyInfoObjectUtility::DelSpace(string& cls)
	{
		/////スペース削除
		while ( cls.find(" ") != -1 ) cls.erase(cls.find(" ") , 1);
	}

	int SprpyInfoObjectUtility::DelPointer(string &cls)
	{
		int nPointer = 0;
		while ( cls.find("*") != -1) { cls.erase( cls.find("*") ,1 ); nPointer++; }
		return nPointer;
	}

	void SprpyInfoObjectUtility::DelNamespace(string& cls)
	{
		//namespace消す Spr::PHSolidなど
		while ( cls.find("::") != -1 )
			cls.assign( cls.erase(0, cls.find("::") + 2 ));
	}

	// constの装飾があった場合は、"q(const)"を消してtrueを返す
	// const出ない場合はfalse
	bool SprpyInfoObjectUtility::DelqConst(string& cls)
	{
		bool ret = false;
		while ( cls.find("q(const).") != -1 )
		{
			ret = true;
			cls.assign( cls.erase(cls.find("q(const).") , 9 ));
		}
		return ret;
	}

	bool SprpyInfoObjectUtility::IsSpacer( char c )
	{
		for( Chars::iterator it = this->Vars.spacer.begin(); it != this->Vars.spacer.end() ; it++)
		{
			if ( c == *it) return true;
		}
		return false;
	}

	CElem::CElem():child(NULL){}
	CElem::CElem(CElem& orig)
	{
		nspace = orig.nspace;
		storage = orig.storage;
		cv = orig.cv;
		classkeyword = orig.classkeyword;
		pr = orig.pr;
		type = orig.type;
		fulltype = orig.fulltype;
		usigned = orig.usigned;

		if ( orig.child ){
			child = new CElem(*orig.child);
		}
		else
		{
			child = NULL;
		}
		//printf("copy %p -> %p\n",&orig,this);
	}
	CElem::~CElem()
	{
		//printf("delete %p\n",this);
		if(this->child) delete this->child;
	}
	
	void SprpyInfoObjectUtility::CreateCElem(string cls,CElem** alloc)
	{
		*alloc = new CElem(CreateCElem(cls));
	}

	CElem SprpyInfoObjectUtility::CreateCElem(string cls)
	{
		//DelConst(cls);
		//DelRef(cls);
		//DelNamespace(cls);
		//DelSpace(cls);

		CElem elem = CElem();

		if(cls.find("UTRef") != -1)
			printf("");


		Strings list;
		//storage
		list.clear();
		list.push_back("extern");
		list.push_back("static");
		list.push_back("auto");
		list.push_back("register");
		list.push_back("typedef");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.storage = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}
			}
		}

		//childを取り出す
		{
			int s = cls.find("<");
			int e = cls.rfind(">");
			if(s != -1&& e!= -1&& s < e){
				string childCode = cls.substr(s+1,e-s+1 -2);
				CreateCElem(childCode,&elem.child); 
				string tmp = cls.substr(0,s);
				tmp += cls.substr(e+1);
				cls = tmp;
			}
		}

		// *,&,const*,const&を取り出す
		list.clear();
		list.push_back("const &");	//長い順番で検索しないとダメ
		list.push_back("const *");
		list.push_back("const&");
		list.push_back("const*");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.pr = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}

			}
		}
		list.clear();
		list.push_back("&");
		list.push_back("*");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( true )	//場所は関係なし
				{
					elem.pr += *it;			//**とかあるから追記
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}

			}
		}

		// cv装飾子(const,volatile)を取り出す
		list.clear();
		list.push_back("const");
		list.push_back("volatile");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.cv = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}

			}
		}
		//クラスキーワードを取り出す
		list.clear();
		list.push_back("struct");
		list.push_back("enum");
		list.push_back("union");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.classkeyword = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}

			}
		}

		//namespace
		list.clear();
		list.push_back("std");
		list.push_back("PTM");
		list.push_back("Spr");
		for ( Strings::iterator it = list.begin(); it != list.end(); it++)
		{
			int index = -1;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.nspace = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length()+2);//::も消す
					cls = tmp;
				}

			}
		}

		//unsigned
		{
			int index = -1;
			string key = "unsigned";
			string *it = &key;
			while (-1 != (index = cls.find(*it,index+1)) )
			{
				if( (index == 0							  ||(index >0						 && IsSpacer(cls[index-1]))) &&
					(index+it->length() == cls.length()   ||(index+it->length()<cls.length() && IsSpacer(cls[index+it->length()]))))
				{
					elem.nspace = *it;
					string tmp = cls.substr(0,index);
					tmp += cls.substr(index+it->length());
					cls = tmp;
				}

			}
		}

		//残りはfulltype
		DelSpace(cls);
		elem.fulltype = elem.usigned + cls;

		// classA::classB::type 
		//               ↑いらないところまで削除
		
		//namespace消す Spr::PHSolidなど
		while ( cls.find("::") != -1 )
			cls = cls.erase(0, cls.find("::") + 2 );
		elem.type = cls;

		string res = ResolveTypedef(elem);
		if( !res.empty() )
			return CreateCElem( res );

		const Enum* en = GetEnum(elem.type);
		if( en )
		{
			elem.classkeyword = "enum";
		}

		return elem;
	}

	// 戻り値: arrayではない場合 -1
	//		 : arrayで要素数がわかるとき　要素数
	//		 : arrayで要素数がわからないとき 0
	int SprpyInfoObjectUtility::DelArray(string &cls)
	{
		int beg = cls.find("[");
		int end = cls.find("]");
		if( beg == -1 ) return -1;
		if( beg+1 == end) 
		{
			cls.erase(beg,2);
			return 0;
		}

		string arraySize = cls.substr(beg+1,end-beg-1);

		cls.erase(beg, arraySize.length() + 2);

		return atoi( arraySize.c_str() );
	}

	int SprpyInfoObjectUtility::CountPointer(string &cls)
	{
		int nPointer = 0;
		int index = -1;
		while ((index = cls.find("*",index+1)) != -1) nPointer++;
		return nPointer;
	}



	/////型文字列から、Pythonの型チェック関数名を返す
	string SprpyInfoObjectUtility::GetCheckfuncName(string cls, string name)
	{
		cls.assign( GetPySymbolName(cls) );

		//特定の型の場合 int, double , などなど
		if (cls == "PyObject")
		{
			//PyObjectはチェックする関数がないので true
			return string("true");
		}
		else if( cls == "PyFloat" || cls == "PyLong") 
			return string("(PyFloat_Check(")+name+") || PyLong_Check("+name+"))";
		else if( cls == "EPVec3d" || cls == "EPVec3f")
			return string("(EPVec3d_Check(")+name+") || EPVec3f_Check("+name+"))";
		else if( cls == "EPVec4d" || cls == "EPVec4f")
			return string("(EPVec4d_Check(")+name+") || EPVec4f_Check("+name+"))";
			
		else 		
			return cls + "_Check(" + name + ")";
	}
		

	string SprpyInfoObjectUtility::GetPyTypeObjectName(string cls)
	{
		if ( cls != "" ) 
		{
			cls.insert(0,Vars.classnamePrefix);
			cls.append("Type");
		}
		return cls;
	}
	

	void SprpyInfoObjectUtility::StrReplaceall(string& str, string from , string to)
	{
		string::size_type index = str.find(from);

		while ( index != string::npos )
		{
			str.replace(index,from.length(),to);
			index = str.find(from,index+to.length());
		}
	}
	SprpyInfoObjectUtility::SprpyInfoObjectUtility(){};


	
	SprClassType SprpyInfoObjectUtility::GetSprClassType(string clsStr)
	{
		if(clsStr != ""){
			
			////////CreateCElem(clsStr);
			////////DelPointer(cls);
			CElem cls = CreateCElem( clsStr );
	
			int nPointer = CountPointer(cls.pr);
			string name = cls.type;

			//Springhead以外の型を分ける
			if (name == "int"      || //unsignedも真
				name == "PyObject" || 
				name == "short"    ||
				name == "long"     || //unsignedも真
				name == "float"    ||
				name == "double"   ||
				name == "bool"     || 
				name == "size_t"   ||
				name == "char"     || //unsignedも真
				name == "string" 
				)
				return SPR_CLASSTYPE_NOTSPR;

			string key("If");
			if( name.length() >= key.length() && name.substr( name.length() - key.length(),key.length()) == key )
				return SPR_CLASSTYPE_IF;
			
			key = "Desc";
			if( name.length() >= key.length() && name.substr( name.length() - key.length(),key.length()) == key )
				return SPR_CLASSTYPE_DESC;
			
			key = "State";
			if( name.length() >= key.length() && name.substr( name.length() - key.length(),key.length()) == key )
				return SPR_CLASSTYPE_STATE;
		}

		return SPR_CLASSTYPE_NONE;
	}
}//namespace