#ifndef SPR_FW_GRAPHICSADAPTEE_H
#define SPR_FW_GRAPHICSADAPTEE_H

#include <Framework/SprFWWin.h>

namespace Spr{;

struct GRDeviceIf;

struct FWGraphicsAdapteeDesc{};

/** FWGraphicsAdaptee
	処理系に依存したウィンドウの作成・管理機能
	＊機能を考えるとFWWindowManagerの方が合う気がする tazz
 */

struct FWGraphicsAdapteeIf : ObjectIf{	
	SPR_IFDEF(FWGraphicsAdaptee);

	///	初期化を行う。最初にこれを呼ぶ必要がある。
	// ＊ char* argv[]という書式はswigが未対応の模様
	void Init(int argc = 0, char** argv = NULL);
	
	/// mainloopを呼び，タイマーをスタートする
	void StartMainLoop();

	/// Idle callbackの有効化/無効化
	void EnableIdleFunc(bool on = true);

	/// フルスクリーンモードへの切り替え．API名は暫定的にGLUTに準拠
	void EnterGameMode();
	void LeaveGameMode();

	/** ウィンドウ */
	///	ウィンドウを作成し、ウィンドウ IDを返す
	FWWinIf* CreateWin(const FWWinDesc& d);
	///	ウィンドウを破棄する
	void DestroyWin(FWWinIf* w);
	///	カレントウィンドウを設定する
	void SetCurrentWin(FWWinIf* w);
	///	カレントウィンドウを返す。
	FWWinIf* GetCurrentWin();
	///	カレントウィンドウを返す。
	int GetWinFromId();
	/// カレントウィンドウのノーマルプレーンを，再描画の必要に応じてマークする
	void PostRedisplay();
	/// Shift,Ctrl,Altのステートを返す
	int GetModifiers();

	/// グラフィクスデバイスの取得
	GRDeviceIf*	GetGRDevice();
};

}

#endif