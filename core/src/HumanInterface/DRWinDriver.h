/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef DR_WINDRIVER_H
#define DR_WINDRIVER_H

#ifdef _WIN32

#include <Foundation/UTPath.h>
#include <base/BaseUtility.h>
#include <windows.h>
#include <winsvc.h>

namespace Spr {

///	Windowsのデバイスドライバを開くクラス
class SPR_DLL DRLoadWindowsDriver{
protected:
	UTPath path;
	std::string drvName;
	SC_HANDLE hSCMan;
	SC_HANDLE hService;
public:
	///
	DRLoadWindowsDriver();
	///
	~DRLoadWindowsDriver();

	///	デバイスドライバのパス名
	const char* Path(){ return path.Path().c_str(); }
	///	デバイスドライバのパス名の設定
	void Path(const char* p);
	///	デバイスドライバ名
	const char* Name(){ return drvName.c_str(); }
	///	ファイル名でデバイスドライバのパス名を設定
	void File(const char* f);
	///	デバイスドライバのインストール
	void Install();
	///	サービスの開始
	void Start();
	///	サービスの停止
	void Stop();
	///	デバイスドライバの削除
	void Remove();
	///	開いているかどうか
	bool IsOpen();
	///	開始しているかどうか
	bool IsStart();
protected:
	///	Windows の local service driver に接続する。
	void Init();
	///	Windows の local service driver との接続を解除する。
	void Cleanup();
	///	service を開く。
	void Open();
};

}	//	namespace Spr

#endif

#endif

