#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "UTPath.h"
#ifdef _MSC_VER	
# include <windows.h>
# include <io.h>
# include <direct.h>
#else
# include <string.h>
# ifdef __CYGWIN__
# include <sys/unistd.h>
# else
# include <unistd.h>
# endif
#endif

namespace Spr {;
void UTPath::Path(UTString p){
#ifdef _MSC_VER
	for(unsigned i=0; i<p.length(); ++i){
		if (p[i] == '/'){
			p[i] = '\\';
		}
	}
#endif
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
#ifdef _MSC_VER		
	char buf[1024];
	char buf2[1024];
	_splitpath(path.c_str(), NULL, NULL, buf, buf2);
	UTString file = buf;
	file += buf2;
	return file;
#else
	const char* pos = strrchr(path.c_str(), '/');
	if(pos == NULL)
	  return path;
	return UTString(++pos);
#endif
}
UTString UTPath::Main(){
#ifdef _MSC_VER			
	char buf[1024];
	_splitpath(path.c_str(), NULL, NULL, buf, NULL);
	UTString m = buf;
	return m;
#else
	UTString file = File();
	const char* pos = strrchr(file.c_str(), '.');
	if(pos == NULL)
	  return file;
	return UTString(file.c_str(), pos);
#endif
}
UTString UTPath::Ext(){
#ifdef _MSC_VER				
	char buf[1024];
	_splitpath(path.c_str(), NULL, NULL, NULL, buf);
	return UTString(buf);
#else
	const char* pos = strrchr(path.c_str(), '.');
	if(pos == NULL)
	  return UTString("");
	return UTString(pos);
#endif
}
UTString UTPath::Drive(){
	if (path.length() >=2 && path[0]=='\\' && path[1]=='\\'){
		size_t n = path.find('\\', 2);
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
  char delim;
#ifdef _MSC_VER
  delim = '\\';
#else
  delim = '/';
#endif
	if ((path.length() >=2 && path[0]==delim && path[1]==delim)
		|| (path.length() >=2 && path[1]==':')){
		size_t b = path.find(delim, 2);
		size_t e = path.find_last_of(delim);
		if (e == path.npos) return "";
		return path.substr(b, e-b+1);
	}else{
		size_t e = path.find_last_of(delim);
		if (e == path.npos) return "";
		return path.substr(0, e+1);
	}
}
UTString SPR_CDECL UTPath::GetCwd(){
	char buf[1024];
#ifdef _MSC_VER		
	GetCurrentDirectory(sizeof(buf), buf);
	UTString rv(buf);
	rv += '\\';
#else	
	getcwd(buf, sizeof(buf));
	UTString rv(buf);
	rv += '/';
#endif	
	return rv;
}
bool SPR_CDECL UTPath::SetCwd(UTString cwd){
#ifdef _MSC_VER		
	return SetCurrentDirectory(cwd.c_str())!=0;
#else		
	return (chdir(cwd.c_str()) == 0);
#endif		
}

void SPR_CDECL UTPath::CreateDir(UTString dirname){
#ifdef _MSC_VER
	CreateDirectory(dirname.c_str(), NULL);
#else
	mkdir(dirname.c_str(), 0777);
#endif
}

UTString UTPath::FullPath(){
	if (path.length() && path[0] == '\\'
		|| path.length()>=2 && path[1] == ':'){
		if (path.length()>=2 && path[0]=='\\' && path[1]!='\\'){
			//	ドライブ名が省略されているので補完．
			UTString cwd = GetCwd();
			size_t bsPos = cwd.find_first_of('\\', 2);
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
	if (path.length() && path[0] == '\\' || path.length()>=2 && path[1] == ':'){
		UTString fp = FullPath();
		//	もともと絶対パス
		UTString cwd = GetCwd();
		size_t i;
		for(i=0; i<cwd.length() && i<fp.length(); ++i){
			if (cwd[i] != fp[i]) break;
		}
 		i = cwd.find_last_of('\\', i)+1;
		if (i < 2){	//	共通部分なし
			return fp;
		}else{	//	共通部分があるので相対パス化
			int c=0;
			for(size_t p=i; p!=cwd.npos; c++){
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
