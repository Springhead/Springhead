#include <Framework/SprFWConsoleDebugMonitor.h>

#ifdef _MSC_VER
#include <conio.h>
#endif

#ifdef _MSC_VER
# define stricmp _stricmp
# define strnicmp _strnicmp
# define kbhit _kbhit
# define getch _getch
# define ungetch _ungetch
#else
# define strcpy_s(DST,LEN,SRC) strcpy(DST,SRC)
# define strtok_s(TOK,DEL,CON) strtok(TOK,DEL)
# define stricmp strcasecmp
# define strnicmp strncasecmp
int kbhit(){ return 0; }
int getch(){ return 0; }
void ungetch(int ch){}
#endif

namespace Spr {

	FWConsoleDebugMonitor::FWConsoleDebugMonitor() : lineHistoryCur(0) {
	}
	void FWConsoleDebugMonitor::KeyCheck() {
		if (kbhit()) {
			int key = getch();
			ProcessKey(key);
		}
	}
	bool FWConsoleDebugMonitor::ProcessKey(int key) {
		bool rv = false;
		if (key == '\r') {	//	コマンドの実行
			std::cout << std::endl;
			std::string cmd;
			std::istringstream(line) >> cmd;
			std::string arg = line.substr(cmd.length());
			std::vector<std::string> args;
			char* buf = new char[arg.length() + 1];
			strcpy_s(buf, arg.length() + 1, arg.c_str());
			char* context = NULL;
			char* word = strtok_s(buf, " \t", &context);
			if (word) args.push_back(std::string(word));
			while ((word = strtok_s(NULL, " \t", &context))) {
				args.push_back(std::string(word));
			}
			delete [] buf;
			ExecCommand(cmd, arg, args);
			std::cout << ">";
			if (line.length()) {
				for (auto it = lineHistory.begin(); it != lineHistory.end(); ++it) {
					if (line.compare(*it) == 0) {
						lineHistory.erase(it);
						break;
					}
				}
				lineHistory.push_back(line);
				lineHistoryCur = (int)lineHistory.size();
			}
			line.clear();
			rv = true;
		}
		else if (key == '\t') {
			size_t fs = line.find_last_of(" ");
			if (fs == std::string::npos) fs = 0;
			else fs += 1;
			std::string field = line.substr(fs);
			std::vector<std::string> candidates;
			Candidates(candidates, fs, field);
			bool bFirst = true;
			std::vector<std::string> matches;
			for (size_t i = 0; i < candidates.size(); ++i) {
				std::string nameCut = std::string(candidates[i].substr(0, field.length()));
				if (stricmp(field.c_str(), nameCut.c_str()) == 0) {
					if (matches.size() == 0) {
						std::cout << std::endl;
					}
					else {
						std::cout << " ";
					}
					matches.push_back(candidates[i]);
					std::cout << candidates[i];
				}
			}
			if (matches.size()) {
				std::cout << std::endl;
				size_t pos = 0;
				for (; pos < matches.begin()->length(); ++pos) {
					char ch = matches.begin()->at(pos);
					for (auto it = matches.begin(); it != matches.end(); ++it) {
						if (it->at(pos) != ch) {
							goto next;
						}
					}
				}
			next:
				line = line.substr(0, fs) + matches.begin()->substr(0, pos);
				std::cout << ">" << line;
			}
			rv = true;
		}
		else if (key == 0x08) {
			if (line.length() > 0) {
				line = line.substr(0, line.length() - 1);
				std::cout << (char)key << " " << (char)key;
			}
			rv = true;
		}
		else if (key == 224) {	//	方向キー
			key = getch();
			switch (key) {
			case 'H':	//	UP
				std::cout << "\r>";
				for (size_t i = 0; i < line.length(); ++i) std::cout << " ";
				if (lineHistory.size()) {
					lineHistoryCur--;
					if (lineHistoryCur >= (int)lineHistory.size()) {
						lineHistoryCur = (int)lineHistory.size() - 1;
					}
					else if (lineHistoryCur < 0) {
						lineHistoryCur = 0;
					}
					line = lineHistory[lineHistoryCur];
				}
				std::cout << "\r>" << line;
				rv = true;
				break;
			case 'P':	//	DOWN
				std::cout << "\r>";
				for (size_t i = 0; i < line.length(); ++i) std::cout << " ";
				if (lineHistory.size()) {
					lineHistoryCur++;
					if (lineHistoryCur > (int)lineHistory.size()) {
						lineHistoryCur = (int)lineHistory.size();
					}
					else if (lineHistoryCur < 0) {
						lineHistoryCur = 0;
					}
					if (lineHistoryCur < (int)lineHistory.size()) line = lineHistory[lineHistoryCur];
					else line = "";
				}
				std::cout << "\r>" << line;
				rv = true;
				break;
			case 'K':	//	LEFT
			case 'M':	//	RIHGT
			default:
				assert(rv == false);
				ungetch(key);
				break;
			}
		}
		else if (0 < key && key < 0x100 && isprint(key)) {
			line.append(1, key);
			std::cout << (char)key;
			rv = true;
		}
		return rv;
	}
	static std::string FindField(UTTypeDescIf*& td, void*& data, std::string field) {
		size_t cur = 0;
		size_t pp;
		while ((pp = field.find_first_of('.', cur)) != std::string::npos) {
			std::string name = field.substr(cur, pp - cur);
			bool bFound = false;
			for (int i = 0; i < td->NFields(); ++i) {
				if (strcmp(name.c_str(), td->GetFieldName(i)) == 0) {
					if (data) data = td->GetFieldAddress(i, data, 0);
					td = td->GetFieldType(i);
					cur = pp + 1;
					bFound = true;
					break;
				}
			}
			if (!bFound) {
				td = NULL;
				break;
			}
		}
		return field.substr(cur);
	}
	static std::string FindField(UTTypeDescIf*& td, std::string field) {
		void* n = NULL;
		return FindField(td, n, field);
	}
	void FWConsoleDebugMonitor::CandidatesForDesc(std::vector<std::string>& rv, UTTypeDescIf* td, std::string field) {
		std::string name = FindField(td, field);
		if (td) {
			for (int i = 0; i < td->NFields(); ++i) {
				if (strnicmp(name.c_str(), td->GetFieldName(i), name.length()) == 0) {
					std::string f;
					size_t pos = field.find_last_of('.');
					if (pos != std::string::npos) {
						f = field.substr(0, pos + 1);
					}
					f.append(td->GetFieldName(i));
					rv.push_back(f);
				}
			}
		}
	}
	FWConsoleDebugMonitor::ExecResults FWConsoleDebugMonitor::ExecCommandForDesc(UTTypeDescIf* td, void* data, std::string cmd, std::string arg, std::vector<std::string> args){
		std::string name;
		if (cmd.length()) {
			name = FindField(td, data, cmd);
		}
		if (td) {
			for (int i = 0; i < td->NFields(); ++i) {
				if (name.length() > 0 && strcmp(name.c_str(), td->GetFieldName(i)) == 0) {
					FWConsoleDebugMonitor::ExecResults rv = READ;
					if (args.size() > 1 && args[0].compare("=") == 0) arg = args[1];
					if (td->GetFieldType(i)->IsPrimitive()){
						if (arg.length()) {
							td->WriteFromString(arg, i, data, 0);
							rv = WRITE;
						}
						std::cout << cmd << " = " << td->ReadToString(i, data, 0) << std::endl;
					}
					else {
						data = td->GetFieldAddress(i, data, 0);
						td = td->GetFieldType(i);
						for(int i=0; i<td->NFields(); ++i){
							std::cout << cmd << "." << td->GetFieldName(i);
							if (td->GetFieldType(i)->IsPrimitive()) std::cout << " = " << td->ReadToString(i, data, 0);
							std::cout << std::endl;
						}
					}
					return rv;
				}
				else if (name.length() == 0){
					std::cout << td->GetFieldName(i);
					if (td->GetFieldType(i)->IsPrimitive()) std::cout << " = " << td->ReadToString(i, data, 0);
					std::cout << std::endl;
				}
			}
		}
		return NOTHING;
	}
}
