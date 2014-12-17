#include <HumanInterface/HISpaceNavigator.h>

#ifdef _WIN32
 #include <windows.h>
#endif

#include <boost/scoped_array.hpp>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

namespace Spr {;

std::vector<void*> HISpaceNavigator::deviceHandles;

bool HISpaceNavigator::Init(const void* desc) {
	mView = Affinef();
	mPers = Matrix4f(); for (int i=0; i<4; ++i) { mPers[i][i]=1.0f; }

	basePose = Posef();
	currPose = Posef();

	v  = Vec3d();
	aV = Vec3d();

	hWnd = ((HISpaceNavigatorDesc*)desc)->hWnd;

	if (hWnd) {
		UINT numDevices;

		// 接続されているSpaceNavigatorを確認してデバイスハンドルを控えておく
		GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));
		RAWINPUTDEVICELIST* deviceList = new RAWINPUTDEVICELIST[numDevices];
		if (GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST)) != numDevices) { }
		for (size_t i=0; i<numDevices; ++i) {
			if (deviceList[i].dwType == RIM_TYPEHID) {
				RID_DEVICE_INFO info;
				UINT cbSize = info.cbSize = sizeof(info);

				if (! GetRawInputDeviceInfo(deviceList[i].hDevice, RIDI_DEVICEINFO, &info, &cbSize)) { continue; }
				if (! (info.hid.dwVendorId == VENDOR_ID))                                            { continue; }
				if (! (PID_BEGIN <= info.hid.dwProductId && info.hid.dwProductId <= PID_END))        { continue; }

				// Space Navigatorをみつけた
				bool bAlreadyUsed = false;
				for (int j=0; j<(int)deviceHandles.size(); ++j) {
					if (deviceHandles[j] == deviceList[i].hDevice) { bAlreadyUsed = true; }
				}
				if (!bAlreadyUsed) {
					deviceHandles.push_back(deviceList[i].hDevice);
					hDevice = deviceList[i].hDevice;
					break;
				}
			}
		}
		delete[] deviceList;

		// Raw Inputを受け付けるよう登録
		RAWINPUTDEVICE sRawInputDevices[] = { {0x01, 0x08, 0x00, 0x00} };
		numDevices  = sizeof(sRawInputDevices) / sizeof(sRawInputDevices[0]);
		UINT cbSize = sizeof(sRawInputDevices[0]);
		for (size_t i=0; i<numDevices; ++i) {
			sRawInputDevices[i].hwndTarget = (  *((HWND*)hWnd)  );
			sRawInputDevices[i].dwFlags    = RIDEV_INPUTSINK;
		}
		::RegisterRawInputDevices(sRawInputDevices, numDevices, cbSize);

		return true;
	} else {
		return false;
	}
}

bool HISpaceNavigator::Calibration() {
	basePose = Posef();
	currPose = Posef();
	v  = Vec3d();
	aV = Vec3d();
	return true;
}

void HISpaceNavigator::Update(float dt) {
	currPose = basePose;

	double posScale = 0.1;
	double oriScale = 0.1;
	double velScale = 1.0;
	double aveScale = 1.0;

	v  = 0.99*v  + 0.01*velocity;
	aV = 0.99*aV + 0.01*angularVelocity;

	Vec3f dP = v  * posScale;
	Vec3f dQ = aV * oriScale;

	currPose.Pos() += dP;
	currPose.Ori()  = Quaterniond::Rot(dQ) * currPose.Ori();

	double rSecondShell = 0.7;
	if (dP.norm() > rSecondShell) {
		basePose.Pos() += ( dP.unit() * (dP.norm() - rSecondShell) ) * velScale * dt;
	}


	/*
	Vec3d dV = velocity * dt;
	Vec3d dQ = angularVelocity * dt;

	bool bCameraCoord  = false;
	bool bLimitInSight = true;
	float depthFar=50, depthNear=3;

	Affinef  mViewInv = mView.inv();
	Matrix4f mPersInv = mPers.inv();
	Quaterniond ori0, ori0_, ori1;
	Vec3d pos0, pos1, pos_limited, pos_limited_3d;

	// ポインタの新しい座標を計算する
	if (bCameraCoord) {
		Quaterniond q; q.FromMatrix(mView.Rot());
		ori0 = q.Inv();
		pos0 = currPose.Pos();
		pos1 = ori0*dV + pos0;

		ori0_= currPose.Ori();
		ori1 = Quaterniond::Rot(ori0*dQ) * ori0_;
	} else {
		pos0 = currPose.Pos();
		pos1 = dV + pos0;

		ori0_= currPose.Ori();
		ori1 = Quaterniond::Rot(dQ) * ori0_;
	}

	// ポインタの左右位置を視野内に制限する
	Vec4f pos_screen = mPers * Vec4f(pos1.x, pos1.y, pos1.z, 1);
	float w          = pos_screen[3];
	pos_screen *= (1.0 / w);

	pos_limited = Vec3f(pos_screen.x, pos_screen.y, pos_screen.z);
	if (bLimitInSight) {
		pos_limited.x    = min(max( -1.0,  pos_limited.x),  1.0);
		pos_limited.y    = min(max( -1.0,  pos_limited.y),  1.0);
	}
	Vec4f pos_limited_3d_ = mPersInv * Vec4f(pos_limited.x*w, pos_limited.y*w, pos_limited.z*w, w);
	pos_limited_3d = Vec3f(pos_limited_3d_.x, pos_limited_3d_.y, pos_limited_3d_.z);

	// ポインタの前後位置を一定範囲内に制限する
	pos_limited_3d = mView * pos_limited_3d;
	if (bLimitInSight) {
		pos_limited_3d.z = min(max( -depthFar,  pos_limited_3d.z),  -depthNear);
	}
	pos_limited_3d = mViewInv * pos_limited_3d;

	// ポインタの位置・姿勢に反映
	currPose.Pos() = pos_limited_3d;
	currPose.Ori() = ori1;
	*/
}

bool HISpaceNavigator::SetPose(Posef pose) {
	// currPose = pose;
	basePose = pose;
	currPose = Posef();
	return true;
}

Posef HISpaceNavigator::GetPose() {
	return currPose;
}

Vec3f HISpaceNavigator::GetPosition() {
	return currPose.Pos();
}

Quaternionf HISpaceNavigator::GetOrientation() {
	return currPose.Ori();
}

Affinef HISpaceNavigator::GetAffine() {
	Affinef aff;
	currPose.ToAffine(aff);
	return aff;
}

bool HISpaceNavigator::PreviewMessage(void *m) {
	MSG* msg = (MSG*)m;

	if (msg->message != WM_INPUT) return false;

	boost::scoped_array<BYTE> saRawInput;

	HRAWINPUT hRawInput = reinterpret_cast<HRAWINPUT>(msg->lParam);
	UINT dwSize=0;
	if (::GetRawInputData(hRawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER))!= 0) return false;

	saRawInput.reset(new BYTE[dwSize]);
	PRAWINPUT pRawInput = reinterpret_cast<PRAWINPUT>(saRawInput.get());

	if (!pRawInput) return false;
	if (::GetRawInputData(hRawInput, RID_INPUT, pRawInput, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) return false;
	if (pRawInput->header.dwType != RIM_TYPEHID) return false;

	RID_DEVICE_INFO sRidDeviceInfo;
	sRidDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
	dwSize = sizeof(RID_DEVICE_INFO);

	if (GetRawInputDeviceInfo(pRawInput->header.hDevice, RIDI_DEVICEINFO, &sRidDeviceInfo, &dwSize) != dwSize) return false;
	if (sRidDeviceInfo.hid.dwVendorId != VENDOR_ID) return false;

	if (hDevice==NULL) {
		bool bAlreadyUsed = false;
		for (int i=0; i<(int)deviceHandles.size(); ++i) {
			if (deviceHandles[i] == pRawInput->header.hDevice) { bAlreadyUsed = true; }
		}
		if (!bAlreadyUsed) {
			deviceHandles.push_back(pRawInput->header.hDevice);
			hDevice = pRawInput->header.hDevice;
		}
	}

	if (pRawInput->header.hDevice == hDevice) {
		// ----- ----- ----- ----- -----
		// Motion data comes in two packages
		// Orientation is a right handed coordinate system with Z down;
		// this is the standard HID orientation
		if (pRawInput->data.hid.bRawData[0] == 0x01) {
			// Translation vector
			short* pnData = reinterpret_cast<short*>(&pRawInput->data.hid.bRawData[1]);
			short X = pnData[0];
			short Y = pnData[1];
			short Z = pnData[2];
			velocity = Quaterniond::Rot(Rad(90), 'x') * (Vec3d(X,Y,Z)) * (maxVelocity / (double)(INPUT_ABS_MAX));
			return true;
		} else if (pRawInput->data.hid.bRawData[0] == 0x02) {
			// Directed rotation vector (NOT Euler)
			short* pnData = reinterpret_cast<short*>(&pRawInput->data.hid.bRawData[1]);
			short rX = pnData[0];
			short rY = pnData[1];
			short rZ = pnData[2];
			angularVelocity = (Quaterniond::Rot(Rad(90), 'x') * Vec3d(rX,rY,rZ)) * (maxAngularVelocity / (double)(INPUT_ABS_MAX));
			return true;
		} else if (pRawInput->data.hid.bRawData[0] == 0x03) {
			// State of the keys
			unsigned long dwKeystate = *reinterpret_cast<unsigned long *>(&pRawInput->data.hid.bRawData[1]);
			/// std::cout << "key : " << dwKeystate << std::endl;
			return true;
		}
	}

	return false;
}
}