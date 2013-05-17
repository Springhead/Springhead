//#ifndef Spr_DEVICE_H
//#define Spr_DEVICE_H
/**	@page Device Device ライブラリ
@section aim 目的
	このパッケージは，デバイスドライバを抽象化するためのものです．
アプリケーションからデバイスドライバを直接呼び出すと，デバイスを変更したときに，
アプリケーションを修正しなければならなくなります．
そこで，例えばD/AコンバータならどのD/Aコンバータでも同じように呼び出せるように
抽象化したD/Aコンバータデバイス(バーチャルデバイス)を考え，アプリケーションは
バーチャルデバイスを使用するようにします．バーチャルデバイスは実際のデバイス
(リアルデバイス)を呼び出すように実装してあります．

@section usage 使い方
簡単なサンプルを示します．
@verbatim
	DVDeviceManager devMan;				//	デバイス管理クラス．
	devMan.RPool().Register(new DRAdapio(9));	//	リアルデバイスの登録
	devMan.Init();						//	デバイスの初期化，ここで
										//	バーチャルデバイスが生成される．
	std::cout << devMan;				//	状態の表示
	
	//	デバイス管理クラスからA/Dを借りてくる
	DVAdBase* ad;
	devMan.VPool().Rent(ad);
	std::cout << ad->Voltage();
@endverbatim
まずデバイスマネージャを作成し，実デバイスを登録し，初期化します．
するとバーチャルデバイスが生成されるので，アプリケーションは，
使いたいバーチャルデバイスをデバイスマネージャから借りてきます．

特定のデバイスの特定の機能を使いたい場合，
専用のデバイスマネージャオブジェクトを作成して，デバイスを登録，初期化して，
バーチャルデバイスを取得してください．

デバイスマネージャオブジェクトは複数作成しても問題ありません．
*/
/*
#include <sstream>

#include "DVDeviceManager.h"
#include "DVCounterBase.h"
#include "DVDaBase.h"
 #include "DVPioBase.h"

#include <HumanInterface/HIRealDevice.h>
#include <stdio.h>

#ifdef _WIN32
 #include <WinBasis/WBWin.h>
 #include <WinBasis/WBPortIO.h>
 #include <WinBasis/WBWinDriver.h>
 #include <WinBasis/WBDllLoader.h>

 #include "DRContecIsaCounter.h"
 #include "DRContecIsaDa.h"
 #include "DRNittaForce.h"
 #include "DRUsbH8Simple.h"
 #include "DRUsb20Simple.h"
 #include "DRUsb20Sh4.h"
#endif

#endif
*/

#include <sstream>
//#include <Springhead.h>
#include <Foundation/Object.h>
#include <Base/Affine.h>
#include <stdio.h>

#ifdef _WIN32

#include<winsock2.h>	//	windows.h の前にやらないと、 winsock1.hがincludeされる
#ifndef _WIN32_WINNT
 #define _WIN32_WINNT 0x0400
#endif
 #include <windows.h>

 #include "DRContecIsaCounter.h"
 #include "DRContecIsaDa.h"
 #include "DRNittaForce.h"
 #include "DRUsbH8Simple.h"
 #include "DRUsb20Simple.h"
 #include "DRUsb20Sh4.h"
#endif

#include "DRADAPIO.h"
//#include "DRContecIsaCounter.h"
//#include "DRContecIsaDa.h"
//#include "DRNittaForce.h"
#include "HIDevice.h"
#include "DRTokin3D.h"
#include "DRUPP.h"
//#include "DRUsb20Sh4.h"
//#include "DRUsb20Simple.h"
//#include "DRUsbH8Simple.h"
#include "DRUsbH8SimpleDef.h"
