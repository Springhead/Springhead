/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_HI_TRACKBALL_H
#define SPR_HI_TRACKBALL_H

#include <HumanInterface/SprHIDevice.h>
#include <HumanInterface/SprHIKeyMouse.h>

namespace Spr {;

/** @brief トラックボールインタフェース
	マウス操作により注視点からの向きと距離を操作することができる．
	主にカメラの操作に利用するが，オブジェクトの操作にも利用できるかも知れない．
	トラックボールとターンテーブルの2モードがあり，マウス移動量からカメラ回転へのマッピング方法が異なる．
	トラックボールではマウスのx,y移動量をそれぞれ横回転，縦回転（スクリーンに関して）にマッピングする．
	ターンテーブルではワールド座標系の経度，緯度の変化量にマッピングする．
	ズームおよび注視点移動に関しては共通．

	motion			default button state	function
	--------------------------------------------------------
	delta x			LBUTTON					(trackball) rotate w.r.t. screen y-axis
											(turntable) change longitude	
	delta y			LBUTTON					(trackball) rotate w.r.t. screen x-axis
											(turntable) change latitude
	delta x			LBUTTON + CTRL			(trackball) rotate w.r.t. screen z-axis
	delta y			RBUTTON					zoom in/out
	delta x			LBUTTON + ALT			move look-at point along screen x-axis
	delta y			LBUTTON + ALT			move look-at point along screen y-axis
 */

struct HITrackballIf;

struct HITrackballDesc{
	bool	trackball;	///< trueならトラックボール，falseならターンテーブル
	Vec3f	target;		///< 注視点

	float	longitude;	///< 経度[rad]
	float	latitude;	///< 緯度[rad]
	Vec2f	lonRange;	///< 経度範囲
	Vec2f	latRange;	///< 緯度範囲
	
	float	distance;	///< 距離
	Vec2f	distRange;	///< 距離範囲
	
	float	rotGain;	///< 回転ゲイン [rad/px]
	float	zoomGain;	///< ズームゲイン	 [ln/px]
	float	trnGain;	///< 平行移動ゲイン

	int		rotMask;
	int		zoomMask;
	int		trnMask;

	HITrackballDesc(){
		trackball	= false;		///< デフォルトでターンテーブル
		target		= Vec3f();

		longitude	= Radf(0.0f);
		latitude	= Radf(0.0f);
		lonRange	= Vec2f(Radf(-180.0f), Radf(180.0f));
		latRange	= Vec2f(Radf(- 80.0f), Radf( 80.0f));
		distance	= 1.0f;
		distRange	= Vec2f(0.1f, 100.0f);
		rotGain		= 0.01f;
		trnGain		= 0.001f;
		zoomGain	= 0.1f;

		rotMask		= DVButtonMask::LBUTTON;
		zoomMask	= DVButtonMask::RBUTTON;
		trnMask		= DVButtonMask::LBUTTON + DVButtonMask::ALT;
	}
};

/**	トラックボールのコールバッククラス
 **/
struct HITrackballCallback{
	/** @brief 向きの更新
		@param trackball	呼び出し元のトラックボール
	 */
	virtual void OnUpdatePose(HITrackballIf* trackball){}
};

struct GRCameraDesc;

struct HITrackballIf : HIPoseIf{
	SPR_VIFDEF(HITrackball);

	/// モード
	void	SetMode(bool mode);
	bool	GetMode();
	/// 注視点
	void	SetTarget(Vec3f t);
	Vec3f	GetTarget();
	/// 経度・緯度角
	void	SetAngle(float lon, float lat);
	void	GetAngle(float& lon, float& lat);
	/// 注視点からの距離
	void	SetDistance(float dist);
	float	GetDistance();
	/// 経度範囲
	void	SetLongitudeRange(float rmin, float rmax);
	void	GetLongitudeRange(float& rmin, float& rmax);
	/// 緯度範囲
	void	SetLatitudeRange(float rmin, float rmax);
	void	GetLatitudeRange(float& rmin, float& rmax);
	/// 距離範囲
	void	SetDistanceRange(float rmin, float rmax);
	void	GetDistanceRange(float& rmin, float& rmax);
	/// 回転操作ゲイン
	void	SetRotGain(float g);
	float	GetRotGain();
	/// ズーム操作ゲイン
	void	SetZoomGain(float g);
	float	GetZoomGain();
	/// 注視点移動ゲイン
	void	SetTrnGain(float g);
	float	GetTrnGain();

	/// カメラパラメータとオブジェクトのサイズから距離を設定
	void	Fit(const GRCameraDesc& cam, float radius);

	/// 指定された視点位置から経度，緯度，距離を逆算
	void	SetPosition(Vec3f pos);

	/// 指定されたカメラ姿勢から経度，緯度，注視点を逆算
	void	SetOrientation(Quaternionf ori);

	/// マウスによる操作のオン/オフ
	void	Enable(bool on = true);

	/** @brief	回転操作のボタンアサインの変更
		@param	mask	DVButtonMask値の組み合わせ
		maskに0を指定すると操作が無効化される
	 */
	void SetRotMask(int mask);
	/** @brief	ズーム操作のボタンアサインの変更
	 */
	void SetZoomMask(int mask);
	/** @brief	パン操作のボタンアサインの変更
	 */
	void SetTrnMask(int mask);
	/** @brief	リセット操作のボタンアサインの変更
	 */
	//void SetResetMask(int mask, bool single_or_double);

	/// トラックボールに関連付けられているマウスを取得する
	DVKeyMouseIf*	GetKeyMouse();
	/// トラックボールにマウスを関連付ける
	void			SetKeyMouse(DVKeyMouseIf* dv);
	/// コールバックを登録する
	void			SetCallback(HITrackballCallback* callback);
	///	Trackballをシーンのカメラに反映させる。
	void	UpdateView();
};

}	//	namespace Spr
#endif
