/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "HumanInterface.h"
#pragma hdrstop
#include "HIMouse6D.h"

namespace Spr {;

bool HIMouse6D::Init(const void* desc){
	/*HISdkIf* sdk = GetSdk();
	HIVirtualDeviceIf* vd = sdk->RentVirtualDevice(DVKeyMouseIf::GetIfInfoStatic());
	keyMouse = DCAST(DVKeyMouseIf, vd);
	if (!keyMouse)
		return false;
	keyMouse->AddHandler(this);*/

/*	oldX = oldY = newX = newY = 0;
	btnState = NONE;

	// デフォルトの設定
	scaleTransPointer = 0.01f;
	scaleRotPointer = 0.008f;
	scaleTransAxis = 0.03f;
	scaleRotAxis = 0.008f;
	mass = 0.25f;
	pos = Vec3f();
	ori = Quaternionf();
	axis.Pos() = Vec3f(0, 0, -10);
    axis.LookAt(Vec3f(), Vec3f(0,1,0));
	axis = axis.inv();
	axisInit = axis;
	// マウスがアクティブかどうかの検出方法がよくわからないのでとりあえずtrueにしてある
	bGood = true;	
*/
//-> 関口による変更 (2005/1/05)
	SetViewSize(640, 480);
//<-

	return true;
}

void HIMouse6D::SetKeyMouse(DVKeyMouseIf* dv){
	keyMouse = dv;
	keyMouse->AddHandler(this);
}

void HIMouse6D::SetScale(float tp, float rp, float ta, float ra){
	scaleTransPointer = tp;
	scaleRotPointer = rp;
	scaleTransAxis = ta;
	scaleRotAxis = ra;
}

//-> 関口による変更 (2005/1/05)
void HIMouse6D::SetViewSize(float inWidth, float inHeight){
	//	VirtualTrackBallにはViewの大きさのパラメータが
	//	必須なため、それをセットするためのメンバ関数

	viewCenterX = inWidth / 2;
	viewCenterY = inHeight / 2;

	//	Viewの短軸側をVirtualTrackBallの半径とする
	float	d;
	if (viewCenterX < viewCenterY)
		d = viewCenterX;
	else
		d = viewCenterY;
	trackBallRadius = d * 1.0f;
}

Vec3f HIMouse6D::GetTrackBallRotation(float inX, float inY){
	//	クリックされたポイントをVirtualTrackBall内部の
	//	点にマッピングするためのメンバ関数

	Vec3f	v;
	float	d;

	//	TrackBallの中心が原点となるようにし、なおかつ
	//	半径が1となるように座標を正規化
	v.x = 1.0f * (inX - viewCenterX) / trackBallRadius;
	v.y = -1.0f * (inY - viewCenterY) / trackBallRadius;
	v.z = 0.0f;

	//	x, yが求まったのでzの決定
	d = v.x * v.x + v.y * v.y;
	if (d < 1.0)
	{
		v.z = (float)sqrt(1.0 - d);
	}
	else
	{
		d = sqrt(d);
		v.x = v.x / d;
		v.y = v.y / d;
		v.z = 0;
	}

	return v;
}
//<-

void HIMouse6D::Update(float dt){
	// force,torqueに基づいてpos,oriを更新
	// もっときちんと計算したほうがいいかもしれない
	pos += force * dt * dt / mass /2;
	torque	= ori.Inv() * torque;
	Vec3f drot;
	drot = torque * dt * dt / mass;
	float norm = drot.norm();
	if(norm > 1.0e-10){
		ori = Quaternionf::Rot(norm, drot) * ori;
		ori.unitize();
	}

	bool shift	 = !!(btnState & DVButtonMask::SHIFT);
	bool ctrl    = !!(btnState & DVButtonMask::CONTROL);
	bool lbutton = !!(btnState & DVButtonMask::LBUTTON);
	bool mbutton = !!(btnState & DVButtonMask::MBUTTON);
	bool rbutton = !!(btnState & DVButtonMask::RBUTTON);
	if(btnState != 0){
		// 前ステップのマウスの位置との差
		float dx, dy;
		dx = float(newX - oldX);
		dy = float(newY - oldY);

		// ドラッグ操作によって動かす方向の基準
		Affinef afAxis = axis.inv();		

		if (shift && lbutton){

//-> 関口による変更 (2005/1/05)

			//	Virtual Track Ballの実装
			//		透視変換の影響を無視した（つまり平行投影を仮定）
			//		微妙にいんちきなVirtual Track Ballであるが、
			//		ちゃんと動いているように感じるのでよしとする。
			//		（おそらく平行投影を仮定したことによる誤差は
			//		無視できるくらいであろう...）
			Vec3f	oldRot = GetTrackBallRotation((float)oldX, (float)oldY);
			Vec3f	newRot = GetTrackBallRotation((float)newX, (float)newY);
			Vec3f	rotAxis, rotDirection;
			float	rotVelocity;

			rotDirection = newRot - oldRot;
			rotVelocity = rotDirection.norm();

			if (rotVelocity > 0.001)
			{
				rotAxis = cross(newRot, oldRot);
				rotAxis = afAxis.Rot() * rotAxis;
				rotAxis.unitize();

				afAxis = Affinef::Rot(rotVelocity * 1.0f, rotAxis) * afAxis;
				axis = afAxis.inv();
			}

			//-> オリジナル
			// Shift + 左ドラッグのときは基準座標軸をx軸,y軸を中心にカメラを回転
			//afAxis = Affinef::Rot(dx*scaleRotAxis, afAxis.Ey()) * Affinef::Rot(dy*scaleRotAxis, afAxis.Ex()) * afAxis;
			//axis = afAxis.inv();
			//<- ここまで　（オリジナル）
//<-
		}
		else if (shift && mbutton){
			// Shift + 中ドラッグのときは基準座標軸をx-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx - afAxis.Ez() * dy) * scaleTransAxis;	
			axis = afAxis.inv();
		}
		else if (shift && lbutton && rbutton) {
			// Shift + 左右ドラッグのときは基準座標軸のx-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx - afAxis.Ez() * dy) * scaleTransAxis;
			axis = afAxis.inv();
		}
		else if (shift && rbutton && mbutton) {
			// Shift + 右中ドラッグのときは基準座標軸のy-z平面で平行移動
			afAxis.Pos() += (-afAxis.Ey() * dx - afAxis.Ez() * dy) * scaleTransAxis;
			axis = afAxis.inv();
		}
		else if (shift && rbutton){
			// Shift + 右ドラッグのときは基準座標軸をx-y平面で平行移動
			afAxis.Pos() += (-afAxis.Ex() * dx + afAxis.Ey() * dy) * scaleTransAxis;	
			axis = afAxis.inv();
		}
		else if (lbutton) {
			// 左ドラッグのときは基準座標軸のx軸,y軸を中心にポインタを回転
			ori = Quaternionf::Rot(dx*scaleRotPointer, -afAxis.Ey()) * Quaternionf::Rot(dy*scaleRotPointer, -afAxis.Ex()) * ori;
		}
		else if (mbutton) {
			// 中ドラッグのときは基準座標軸のx-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (ex * dx + ez * dy) * scaleTransPointer;
		}
		else if (lbutton && rbutton) {
			// 左右ドラッグのときは基準座標軸のx-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (ex * dx + ez * dy) * scaleTransPointer;
		}
		else if (rbutton && mbutton) {
			// 右中ドラッグのときは基準座標軸のy-z平面でポインタを平行移動
			Vec3f up(0,1,0);
			Vec3f ez = afAxis.Ez();
			ez = (ez - (ez*up)*ez).unit();
			pos += (up * dx + ez * dy) * scaleTransPointer;
		}
		else if (rbutton) {
			Vec3f up(0,1,0);
			Vec3f ex = afAxis.Ex();
			ex = (ex - (ex*up)*ex).unit();
			// 右ドラッグのときは基準座標軸のx-y平面でポインタを平行移動
			pos += (ex * dx - up * dy) * scaleTransPointer;
		}

		oldX = newX;
		oldY = newY;

		//DSTR << "(x, y, z) = (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	}
	HIPose::Update(dt);
}

/*#ifdef _WIN32
}	//	namespace Spr;
#define WIN32_LEAN_AND_MEAN        // Windows ヘッダーから使用されていない部分を除外します。
#define WINVER 0x0500            //    バージョン定義 Windows2000以上
#define _WIN32_WINNT WINVER
#include <Windows.h>
namespace Spr{;

bool HIMouse6D::PreviewMessage(void* m){
	MSG* msg = (MSG*)m;
	switch (msg->message){
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:{
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		OnButtonDown(x, y);
		SetCapture(msg->hwnd);
		}break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		OnButtonUp();
		break;
	case WM_MOUSEMOVE:{
		unsigned fwKeys = msg->wParam;  // key flags 
		int x = (short)LOWORD(msg->lParam);	// horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);	// vertical position of cursor 
		OnButtonMove(fwKeys, x, y);
		//DSTR << "mouse:" << x << " " << y << std::endl;
		}break;
	case WM_MOUSEWHEEL:{
		unsigned fwKeys = LOWORD(msg->wParam);    // key flags
		int zDelta = (short) HIWORD(msg->wParam);    // wheel rotation
		int xPos = (short) LOWORD(msg->lParam);    // horizontal position of pointer
		int yPos = (short) HIWORD(msg->lParam);    // vertical position of pointer
		OnWheel(fwKeys, zDelta);
		}break;
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:{
		unsigned fwKeys = msg->wParam;        // key flags 
		int x = (short)LOWORD(msg->lParam);  // horizontal position of cursor 
		int y = (short)HIWORD(msg->lParam);  // vertical position of cursor 
		OnDblClick(fwKeys, x, y);
		}break;
	case WM_KEYDOWN:{
		int nVirtKey = (int) msg->wParam;
		long lKeyData = msg->lParam;
		return OnKeyDown(nVirtKey);
		}break;
	default:
		return false;
	}
	return true;
}*/

bool HIMouse6D::OnKey(int state, int key, int x, int y){
	if((state & DVKeySt::PRESSED) == 0)
		return false;

	//	視点移動関係	
	Affinef afBody = GetAxis().inv();
	float df = 0.2f ; //移動変化量
	// カメラの平行移動
	if ( key == 'W' )		afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	else if ( key == 'Q' )	afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	else if ( key == 'A' )	afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	else if ( key == 'Z' )	afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	else if ( key == DVKeyCode::DOWN )	afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	else if ( key == DVKeyCode::UP )	afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	// カメラの回転
	else if ( key == DVKeyCode::LEFT )	afBody = Affinef::Rot(Radf(5), 'y') * afBody;
	else if ( key == DVKeyCode::RIGHT ) afBody = Affinef::Rot(Radf(-5), 'y') * afBody;
	else if ( key == 'S' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;
	else if ( key == 'X' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	else if ( key == 'C' ){	// ポインタとカメラをリセット
		afBody = axisInit.inv();
		afBody.Ex().unitize();
		afBody.Ey().unitize();
		afBody.Ez().unitize();
		pos = Vec3f();
		ori = Quaternionf();
	}
	else return false;	
	SetAxis(afBody.inv());
	return true;
}

bool HIMouse6D::OnMouse(int button, int state, int x, int y){
	if(state == DVButtonSt::DOWN){
		oldX = newX = x;
		oldY = newY = y;
	}	
	else{
		oldX = newX;
		oldY = newY;
	}
	return true;
}

bool HIMouse6D::OnMouseMove(int button, int x, int y, int dz){ 
	newX = x;
	newY = y;

	Affinef afAxis = axis.inv();		
	if (button & DVButtonMask::SHIFT){
		afAxis.Pos() += afAxis.Ez() * dz * scaleTransAxis;	
		axis = afAxis.inv();
	}
	else if (button == 0){
		pos += -afAxis.Ez() * dz * scaleTransPointer;
	}
	return true;
}

bool HIMouse6D::OnDoubleClick(int button, int x, int y){
	if (button & DVButtonMask::LBUTTON){
		if(button & DVButtonMask::SHIFT)
			 axis = axisInit;
		else ori = Quaternionf();
		return true;
	}
	else if (button & DVButtonMask::RBUTTON){
		pos = Vec3f();
		return true;
	}
	return false;
}

}	//	namespace Spr

