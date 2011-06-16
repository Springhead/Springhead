/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef HI_MOUSE6D_H
#define HI_MOUSE6D_H

#include <HumanInterface/HIBase.h>
#include <HumanInterface/HIDevice.h>

namespace Spr {;

class SPR_DLL HIMouse6D: public HIPose, public DVKeyMouseHandler{
public:
	SPR_OBJECTDEF(HIMouse6D);

	DVKeyMouseIf*	keyMouse;
	int				btnState;

public:
	DVKeyMouseIf*	GetKeyMouse(){ return keyMouse; }
	void			SetKeyMouse(DVKeyMouseIf* dv);

	/// 
	HIMouse6D(const HIMouse6DDesc& desc = HIMouse6DDesc()){}
	virtual ~HIMouse6D(){}

	/// マウスの初期化
	virtual bool Init(const void* desc);

	///	押されているボタンに応じてpos,oriを更新する。Shiftが押されているときはaxisを更新する。
	void Update(float dt);

	/// DVKeyMouseHandlerの関数
	virtual bool OnMouse(int button, int state, int x, int y);
	virtual bool OnDoubleClick(int button, int x, int y);
	virtual bool OnMouseMove(int state, int x, int y, int zdelta);
	virtual bool OnKey(int state, int key, int x, int y);

	/*
	///	キー処理
	virtual bool OnKeyDown(unsigned nChar);
	/// ドラッグ時の処理
	virtual void OnButtonMove(unsigned int state, int x, int y);
	/// ボタンを押したときの処理
	virtual void OnButtonDown(int x, int y);
	/// ボタンを離したときの処理
	virtual void OnButtonUp();
	///	ダブルクリック時の処理
	virtual void OnDblClick(unsigned fwKeys, int x, int y);
	/// 左ダブルクリック時の処理(ポインタの姿勢を水平に初期化)
	virtual void OnLButtonDClick();
	/// 右ダブルクリック時の処理(ポインタの位置を原点に初期化)
	virtual void OnRButtonDClick();
	/// Shift+左ダブルクリック時の処理(基準座標軸の位置・姿勢を初期化)
	virtual void OnShiftLButtonDClick();
	/// ホイールをまわしたときの処理(ポインタor基準座標軸をZ軸方向に水平移動)
	virtual void OnWheel(unsigned int state, short dz);
	*/

	///	位置の取得
	Vec3f GetPos(){ return pos; }
	///	力の取得
	Vec3f GetForce(){ return force; }
	///	トルクの取得
	Vec3f GetTorque(){ return torque; }
	///	角度の取得
	Quaternionf GetOri(){ return ori; }
	/// 基準座標軸の取得
	Affinef GetAxis(){ return axis; }

	/// スケールの設定
	void SetScale(float tp, float rp, float ta, float ra);

	/// 質量の設定
	void SetMass(float m){ mass = m; }
	/// 位置の設定
	void SetPos(Vec3f p){ pos = p; }
	/// 姿勢の設定
	void SetOri(Quaternionf o){ ori = o; }
	///	並進力・回転力の設定
	void SetForce(const Vec3f& f, const Vec3f& t){ force = f; torque = t; }
	void SetForce(const Vec3f& f){ force = f; }
	///	最低の力を設定する
	void SetMinForce(){ force = torque = Vec3f(); }	

	/// 基準座標軸の設定
	void SetAxis(Affinef a){ axis = a; }
	/// 初期化用座標軸の設定
	void SetAxisInit(Affinef ai){ axisInit = ai; }


	///	キャリブレーション(特に何もしない)
	bool Calib(){ return true; }

//-> 関口による変更 (2005/1/05)
	void SetViewSize(float inWidth, float inHeight);
//<-

protected:
	// マウスの座標(old:前ステップ，new:現ステップ)
	int oldX, oldY, newX, newY;
	// マウスの座標と仮想世界とのスケール調整用パラメータ
	float scaleTransPointer, scaleRotPointer, scaleTransAxis, scaleRotAxis;

	// ポインタに関する変数
	// 質量
	float mass;
	// ポインタにかかる並進力
	Vec3f force;
	// ポインタにかかる回転力
	Vec3f torque;
	// ポインタの位置。Update()で更新される
	Vec3f pos;
	// ポインタの姿勢。Update()で更新される
	Quaternionf ori;

	// 基準座標軸(ドラッグ操作時はこの座標軸にもどづいて動かされる)
	Affinef axis;
	// 初期化用座標軸
	Affinef axisInit;

//-> 関口による変更 (2005/1/05)
	float	viewCenterX, viewCenterY, trackBallRadius;

	Vec3f	GetTrackBallRotation(float inX, float inY);
//<-
};

}	//	namespace Spr
#endif
