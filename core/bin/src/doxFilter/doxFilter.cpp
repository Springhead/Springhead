#define NOMINMAX
#include <iostream>
#include <fstream>
#include <sstream>
#include <FileIO/FISpirit.h>
#include <boost/spirit/include/classic_core.hpp>
#include <Foundation/UTTypeDesc.h>
#include <mbstring.h>
#include <mbctype.h>

using namespace std;
using namespace boost::spirit::classic;
using namespace Spr;


std::string sjis2utf8(const char* in) {
	std::string str;
	// Convert SJIS -> UTF-16
	const int nSize = ::MultiByteToWideChar( CP_ACP, 0, in, -1, NULL, 0 );
 
	char* buffUtf16 = new char[ nSize * 2 + 2 ];
	::MultiByteToWideChar( CP_ACP, 0, in, -1, (LPWSTR)buffUtf16, nSize );
 
	// Convert UTF-16 -> UTF-8
	const int nSizeUtf8 = ::WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)buffUtf16, -1, NULL, 0, NULL, NULL );
 
	char* buffUtf8 = new char[ nSizeUtf8 * 2 ];
	ZeroMemory( buffUtf8, nSizeUtf8 * 2 );
	::WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)buffUtf8, nSizeUtf8, NULL, NULL );
 
	str.assign(buffUtf8, nSizeUtf8-1);

	delete buffUtf16;
	delete buffUtf8;
	return str;
}

struct Mb1_parser: public char_parser<Mb1_parser>{
	typedef Mb1_parser self_t;
    Mb1_parser(){}

	template <typename CharT>
    bool test(CharT ch_) const {
		return _mbbtype(ch_, 0) == _MBC_LEAD;
	}
};
Mb1_parser const mb1_p = Mb1_parser();


//	パースの状態変数
string lang, curLang;		//	出力すべきLANGと，現在のLANG設定
string cmdStr, idStr;		//	コマンドとID文字列
std::vector<int> contents;	//	目次出力位置
std::ostringstream out;		//	出力先文字列
struct SecInfo{
	int level;
	string id;
	SecInfo(int i, string s):level(i), id(s){}
};
std::vector<SecInfo> secs;

int secCount[4];
string chapStr;

void UnsetMode(const char* b, const char* e){
	curLang="";
}
void SetMode(const char* b, const char* e){
	curLang = string(b,e);
}

void Write(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	string str(b,e);
	out << str;
}
void WriteCh(const char c){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	out << c;
}
void SetCmd(const char* b, const char* e){
	cmdStr = string(b,e);
}
void SetId(const char* b, const char* e){
	idStr = string(b,e);
}
void WritePage(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	out << cmdStr << " " << idStr << " " << chapStr <<  sjis2utf8("　");
}
void WriteSec(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	secCount[0]++;
	for(int i=1; i<4; ++i) secCount[i]=0;
	secs.push_back(SecInfo(0, idStr));
	out << cmdStr << " " << idStr << " " << chapStr << secCount[0] << sjis2utf8("　");
}
void WriteSubSec(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	secCount[1]++;
	for(int i=2; i<4; ++i) secCount[i]=0;
	secs.push_back(SecInfo(1, idStr));
	out << cmdStr << " " << idStr << " " << chapStr << secCount[0] << "." << secCount[1] << sjis2utf8("　");
}
void WriteSubSubSec(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	secCount[2]++;
	for(int i=3; i<4; ++i) secCount[i]=0;
	secs.push_back(SecInfo(2, idStr));
	out << cmdStr << " " << idStr << " " << chapStr << secCount[0] << "." << secCount[1] << "." << secCount[2] << sjis2utf8("　");
}
void WriteParagraph(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	secCount[3]++;
	secs.push_back(SecInfo(3, idStr));
	out << cmdStr << " " << idStr << " " << chapStr << secCount[0] << "." << secCount[1] << "." << secCount[2] << "." << secCount[3] << sjis2utf8("　");
}
void WriteCmd(const char* b, const char* e){
	if (curLang.length() && curLang.compare(lang)!=0) return;
	if(cmdStr.compare("contents")==0){
		contents.push_back(out.str().length()-1);
	}else{
		out << string(b,e);
	}
}

int _cdecl main(int argc, const char* argv[]){
	if (argc !=3) return -1;
	lang = argv[1];
	ifstream file(argv[2]);
	const char* ptr = strrchr(argv[2], '/');
	if(!ptr) ptr = strrchr(argv[2], '\\');
	if(!ptr) ptr = argv[2];
	else ptr++;
	int chap = atoi(ptr);
	if (chap){
		ostringstream os;
		os << chap << ".";
		chapStr = os.str();
	}else{
		chapStr="";
	}
	ostringstream os;
	while(1){
		char ch = file.get();
		if (!file.good() || file.eof()) break;
		os.put(ch);
	}

	//	パーサの定義
	//	本文用パーサ
	rule<> src, code, bcmt, lcmt, ltext, btext, cmd, modeCmd, doxCmd, id, space;
	src		=	*(bcmt | lcmt | code[&Write]);
	code	=	~ch_p('/') |  ch_p('/') >> (~ch_p('/') & ~ch_p('*'));
	bcmt	=	str_p("/*")[&Write] >> *(cmd | btext[&Write]) >> str_p("*/")[&Write];
	lcmt	=	str_p("//")[&Write] >> *(cmd | ltext[&Write]) >> str_p("\n")[&Write];
	
	btext	=	mb1_p >> anychar_p | ~ch_p('*') | ch_p('*')>>~ch_p('/');
	ltext	=	mb1_p >> anychar_p | ~ch_p('\n');
		
	cmd		=	modeCmd | doxCmd;
	modeCmd	=	str_p("@!*")[&UnsetMode] | str_p("@!") >> (alnum_p>>alnum_p)[&SetMode];
	doxCmd	=	(ch_p('@') | ch_p('\\'))[&Write] >> (
					(str_p("page")[&SetCmd]			 >> space >> id[&SetId] >> space)[&WritePage] |
					(str_p("section")[&SetCmd]		 >> space >> id[&SetId] >> space)[&WriteSec] |
					(str_p("subsection")[&SetCmd]	 >> space >> id[&SetId] >> space)[&WriteSubSec] |
					(str_p("subsubsection")[&SetCmd] >> space >> id[&SetId] >> space)[&WriteSubSubSec] |
					(str_p("paragraph")[&SetCmd]	 >> space >> id[&SetId] >> space)[&WriteParagraph] |
					id[&SetCmd][&WriteCmd] |
					anychar_p[&WriteCh]
				);
	space	=	+blank_p;
	id		=	+(alnum_p | ch_p('_') | ch_p('-'));

	//	パース
	parse_info<> info = parse(os.str().c_str(), src);
	
	int curPos = 0;
	for(unsigned i=0; i<contents.size(); ++i){
		int cpos = contents[i];
		cout << out.str().substr(curPos, cpos-curPos);
		curPos = cpos+1;
		cout << "\\htmlonly\n";
		cout << sjis2utf8("<h2><a class=\"anchor\">◇ Contents ◇</a></h2>\n");
		cout << "\\endhtmlonly\n";
		for(unsigned s=0; s<secs.size(); ++s){
			for(int n=0; n<secs[s].level+1; ++n){
				cout << sjis2utf8("　　");
			}
			cout << "\\ref " << secs[s].id << " <br>\n";
		}
		cout << "<br><hr>";
	}
	cout << out.str().substr(curPos, out.str().length()-curPos);
	return 0;
}
