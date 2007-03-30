#pragma hdrstop
#include "UTPath.h"
#ifdef _MSC_VER	
# include <windows.h>
# include <io.h>
# include <direct.h>
#endif

namespace Spr {;
void UTPath::Path(UTString p){
	for(unsigned i=0; i<p.length(); ++i){
		if (p[i] == '/'){
			p[i] = '\\';
		}
	}
	path = p;
}
bool UTPath::Search(UTString file){
#ifdef _MSC_VER	
	char* fn=NULL;
	char buf[1024];
	if (!SearchPath(NULL, file.c_str(), NULL, sizeof(buf), buf, &fn)){
		char sprPath[1024];
		if (!GetEnvironmentVariable("SPRINGHEAD", sprPath, sizeof(sprPath))) return false;
		strcat(sprPath, "\\bin");
		if (!SearchPath(sprPath, file.c_str(), NULL, sizeof(buf), buf, &fn)) return false;
	}
	path = buf;
#else
	
#endif
	return true;
}
UTString UTPath::File(){
	char buf[1024];
	char buf2[1024];
#ifdef _MSC_VER		
	_splitpath(path.c_str(), NULL, NULL, buf, buf2);
#else

#endif	
	UTString file = buf;
	file += buf2;
	return file;
}
UTString UTPath::Main(){
	char buf[1024];
#ifdef _MSC_VER			
	_splitpath(path.c_str(), NULL, NULL, buf, NULL);
#else

#endif		
	UTString m = buf;
	return m;
}
UTString UTPath::Ext(){
	char buf[1024];
#ifdef _MSC_VER				
	_splitpath(path.c_str(), NULL, NULL, NULL, buf);
#else

#endif			
	return UTString(buf);
}
UTString UTPath::Drive(){
	if (path.length() >=2 && path[0]=='\\' && path[1]=='\\'){
		int n = path.find('\\', 2);
		return path.substr(0, n-1);
	}else{
		char buf[1024];
#ifdef _MSC_VER			
		_splitpath(path.c_str(), buf, NULL, NULL, NULL);
#else

#endif			
		return UTString(buf);
	}
}
UTString UTPath::Dir(){
	if ((path.length() >=2 && path[0]=='\\' && path[1]=='\\')
		|| (path.length() >=2 && path[1]==':')){
		int b = path.find('\\', 2);
		int e = path.find_last_of('\\');
		if (e == (int)path.npos) return "";
		return path.substr(b, e-b+1);
	}else{
		int e = path.find_last_of('\\');
		if (e == (int)path.npos) return "";
		return path.substr(0, e+1);
	}
}
UTString UTPath::GetCwd(){
	char buf[1024];
#ifdef _MSC_VER		
	GetCurrentDirectory(sizeof(buf), buf);
#else	
	
#endif	
	UTString rv(buf);
	rv += "\\";
	return rv;
}
bool UTPath::SetCwd(UTString cwd){
#ifdef _MSC_VER		
	return SetCurrentDirectory(cwd.c_str())!=0;
#else		
	return false;
#endif		
}
UTString UTPath::FullPath(){
	if (path.length() && path[0] == '\\'
		|| path.length()>=2 && path[1] == ':'){
		if (path.length()>=2 && path[0]=='\\' && path[1]!='\\'){
			//	ドライブ名が省略されているので補完．
			UTString cwd = GetCwd();
			int bsPos = cwd.find_first_of('\\', 2);
			UTString rv = cwd.substr(0, bsPos-1);
			rv += path;
			return rv;
		}else{
			//	もともと絶対パス
			return Path();
		}
	}else{
		UTString fp = GetCwd();
		if (fp.end()[-1] != '\\') fp += "\\";
		fp += Path();
		return fp;
	}
}
UTString UTPath::RelPath(){
	if (path.length() && path[0] == '\\'
		|| path.length()>=2 && path[1] == ':'){
		UTString fp = FullPath();
		//	もともと絶対パス
		UTString cwd = GetCwd();
		unsigned int i;
		for(i=0; i<cwd.length() && i<fp.length(); ++i){
			if (cwd[i] != fp[i]) break;
		}
 		i = cwd.find_last_of('\\', i)+1;
		if (i < 2){	//	共通部分なし
			return fp;
		}else{	//	共通部分があるので相対パス化
			int c=0;
			for(int p=i; p!=(int)cwd.npos; c++){
				p = cwd.find('\\', p+1);
			}
			UTString rv;
			for(int j=1; j<c; j++){
				rv += "..\\";
			}
			rv += fp.substr(i);
			return rv;
		}
	}else{
		//	もともと相対パス
		return Path();
	}
}
UTString UTPath::FullDir(){
	UTString str = FullPath();
	size_t pos = str.find_last_of('\\');
	if (pos == UTString::npos){
		return str;
	}else{
		return str.substr(0, pos);
	}
}

}	//	namespace Spr
