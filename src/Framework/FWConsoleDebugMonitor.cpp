#include <Framework/SprFWConsoleDebugMonitor.h>
#include <conio.h>

namespace Spr {

	FWConsoleDebugMonitor::FWConsoleDebugMonitor() : lineHistoryCur(0) {
	}

	bool FWConsoleDebugMonitor::ExecCommand(std::string cmd, std::string arg, std::vector<std::string> args) {
		return false;
	}

	void FWConsoleDebugMonitor::KeyCheck() {
		if (_kbhit()) {
			int key = _getch();
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
			while (word = strtok_s(NULL, " \t", &context)) {
				args.push_back(std::string(word));
			}
			delete buf;
			ExecCommand(cmd, arg, args);
			std::cout << ">";
			for (auto it = lineHistory.begin(); it != lineHistory.end(); ++it) {
				if (line.compare(*it) == 0) {
					goto SkipPushback;
				}
			}
			lineHistory.push_back(line);
			lineHistoryCur = lineHistory.size();
		SkipPushback:
			line.clear();
			rv = true;
		}
		else if (key == '\t') {
			std::string cmd;
			std::istringstream(line) >> cmd;
			std::vector<std::string> candidates = Candidates(0);
			bool bFirst = true;
			std::vector<std::string> matches;
			for (size_t i = 0; i < candidates.size(); ++i) {
				std::string nameCut = std::string(candidates[i].substr(0, cmd.length()));
				if (_stricmp(cmd.c_str(), nameCut.c_str()) == 0) {
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
				line = matches.begin()->substr(0, pos);
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
			key = _getch();
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
			case 'K':	//	LEFT
			case 'M':	//	RIHGT
			default:
				assert(rv == false);
				_ungetch(key);
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

}
