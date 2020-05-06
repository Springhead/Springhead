#include <iostream>
using namespace std;
int main(){
	char buf[10000];
	int indent = 0;
	while(!cin.eof()){
		cin.getline(buf, sizeof(buf));
		char* ptr = buf;
		while(*ptr){
			if (*ptr == '}') indent--;
			ptr++;
		}
		ptr = buf;
		while(*ptr == ' ' || *ptr == '\t') ++ptr;
		for(int i=0; i<indent; ++i) cout << "\t";
		if (*ptr == '{') cout << "\t";
		cout << ptr << std::endl;
		while(*ptr){
			if (*ptr == '{') indent++;
			ptr++;
		}
	}
}
