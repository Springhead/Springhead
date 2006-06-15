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

#include "DRRealDevice.h"
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
#include <Springhead.h>
#include <Foundation/Object.h>
#include <Base/Affine.h>
#include <stdio.h>

#ifdef _WIN32

// winsock.hとwinsock2.hの仕様で
// winsock2.hが呼ばれその後にwinsock.hが呼ばれた場合、
// あとに呼ばれたwinsock.hは無効化されるが
// winsock.hが呼ばれた後にwinsock2.hが呼ばれた場合は
// 両方が実行され同名変数の二重定義のエラーが起こってしまう。
// 
// もちろんこの仕様はwinsock.hに一行書き足せば直るが、
// これを行うのはあまり良いことではないので敢えて行わない。
// 
// ここではWBWin.hでinclude<winsock2.h>を行っており、
// windows.hでinclude<winsock.h>を行っている。
// 前述したエラーを回避するために
// このファイルではinclude<WinBasis/WBWin.h>を行った後、
// include<windows.h>を行うことにする

 #include <WinBasis/WBWin.h>
 #include <WinBasis/WBPortIO.h>
 #include <WinBasis/WBWinDriver.h>
 #include <WinBasis/WBDllLoader.h>

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
#include "DREzUSB.h"
#include "DRIHCSh4.h"
//#include "DRNittaForce.h"
#include "DRRealDevice.h"
#include "DRRealDevicePool.h"
#include "DRTokin3D.h"
#include "DRUPP.h"
//#include "DRUsb20Sh4.h"
//#include "DRUsb20Simple.h"
//#include "DRUsbH8Simple.h"
#include "DRUsbH8SimpleDef.h"
#include "DVADBase.h"
#include "DVCounterBase.h"
#include "DVDABase.h"
#include "DVDeviceManager.h"
#include "DVForceBase.h"
#include "DVOrientationBase.h"
#include "DVPIOBase.h"
#include "DVVirtualDevice.h"
#include "DVVirtualDevicePool.h"
#include "Tkinput.h"
#include "Tkinputdef.h"
#include "Tuadapio.h"


