/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef FW_FILELOADER_SAMPLE_H						// ヘッダファイルを作る時のおまじない
#define FW_FILELOADER_SAMPLE_H						// 同上

#include <Springhead.h>								// SpringheadのAPIを使うためのヘッダファイル
#include <Framework/SprFWApp.h>						// Frameworkクラスが宣言されているヘッダファイル

using namespace Spr;								// Springheadの名前空間

class FWFileLoaderSample : public FWApp{
public:
	std::string fileName;							///< ロードするファイル名
	FWFileLoaderSample();							///< コンストラクタ（このクラスのインスタンスが生成された時に呼ばれる関数）
	virtual void Init(int argc, char* argv[]);		///< アプリケーションの初期化
	virtual void Keyboard(int key, int x, int y);	///< キーボード関数

	void InitCameraView();							///< 初期カメラ位置の設定		
	void Reset();									///< リセット
}; 

#endif