/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWCONSOLEDEBUGMONITOR_H
#define SPR_FWCONSOLEDEBUGMONITOR_H

#include <Framework/SprFWApp.h>

namespace Spr{;
/** \addtogroup gpFramework */
//@{

class FWConsoleDebugMonitor {
	std::string line;						//	コンソール入力の行
	std::vector<std::string> lineHistory;	//	入力履歴
	int lineHistoryCur;						//	現在の位置
public:
	FWConsoleDebugMonitor();
	///	Idelで呼び出すとコンソールキー入力をチェックする。ブロックしない。
	virtual void KeyCheck();
	///	キー入力を処理する
	virtual bool ProcessKey(int key);
	///	コマンド実行
	virtual void ExecCommand(std::string cmd, std::string arg, std::vector<std::string> args) {}
	///	コマンドの候補(i=0 コマンド, i=1.. arg1...)オーバライド用
	virtual void Candidates(std::vector<std::string>& rv, size_t fieldStart, std::string field) {}

	//-------------------------------------------------------------------------
	//	以下はコマンドのライブラリ
	void CandidatesForDesc(std::vector<std::string>& rv, UTTypeDescIf* td, std::string base);
	
	enum ExecResults {
		NOTHING,
		READ,
		WRITE
	};
	
	ExecResults ExecCommandForDesc(UTTypeDescIf* td, void* data, std::string cmd, std::string arg, std::vector<std::string> args);
};

//@}
}

#endif //SPR_FWCONSOLEDEBUGMONITOR_H
