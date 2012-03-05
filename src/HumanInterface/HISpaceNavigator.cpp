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
	currPose = Posed();
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
				for (int i=0; i<deviceHandles.size(); ++i) {
					if (deviceHandles[i] == deviceList[i].hDevice) { bAlreadyUsed = true; }
				}
				if (!bAlreadyUsed) {
					deviceHandles.push_back(deviceList[i].hDevice);
					hDevice = deviceList[i].hDevice;
					break;
				}
			}
		}
		delete deviceList;

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
	currPose = Posef();
	return true;
}

void HISpaceNavigator::Update(float dt) {
	currPose.Pos() += (velocity * dt);
	currPose.Ori() = Quaterniond::Rot(angularVelocity * dt) * currPose.Ori();
}

bool HISpaceNavigator::SetPose(Posef pose) {
	currPose = pose;
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
		for (int i=0; i<deviceHandles.size(); ++i) {
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