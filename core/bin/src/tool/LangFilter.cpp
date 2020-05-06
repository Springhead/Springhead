#include <fstream>
#include <iostream>
#include <mbctype.h>
#include <mbstring.h>

using namespace std;
std::string mode;
char lang[3];
ifstream file;
bool bOut = true;

void check(){
	bOut = !lang[0] || mode.length()==0 || mode.compare(lang) == 0;
}
void out(char ch){
	if (file.good() && bOut){
		cout << ch;
	}
}

int main(int argc, char* argv[]){
	lang[0] = '\0';
	lang[2] = '\0';
	if (argc!=3) return -1;
	
	mode = argv[1];
	file.open(argv[2]);

	while(file.good()){
		if( _mbbtype(file.peek(), 0) == _MBC_LEAD ){
			out(file.get());
			out(file.get());
		}else if(file.peek() == '@'){
			file.get();
			if (file.peek() == '!'){
				file.get();
				if (file.peek() == '*'){
					file.get();
					lang[0]='\0';
					check();
				}else{
					lang[0]=file.get();
					lang[1]=file.get();
					check();
				}
			}else if (file.peek() == '@'){
				file.get();
				out('@');
				out('@');
			}else{
//				@‚Å”²‚¯‚éH
//				lang[0]='\0';
				out('@');
			}
		}else{
			out(file.get());
		}
	}
	return 0;
}
