#include <HumanInterface/HISpaceNavigator.h>

#ifdef _WIN32
 #include <windows.h>
#endif

#include <boost/scoped_array.hpp>

#ifdef USE_HDRSTOP
 #pragma hdrstop
#endif

#define LOGITECH_VENDOR_ID 0x46d

namespace Spr {;

bool HISpaceNavigator::Init(const void* desc) {
	// hWnd = (*(HWND*)(((HISpaceNavigatorDesc*)desc)->hWnd));
	hWnd = ((HISpaceNavigatorDesc*)desc)->hWnd;

	currPose = Posed();

	// Raw Input‚ðŽó‚¯•t‚¯‚é‚æ‚¤“o˜^
	RAWINPUTDEVICE sRawInputDevices[] = { {0x01, 0x08, 0x00, 0x00} };
	UINT uiNumDevices = sizeof(sRawInputDevices) / sizeof(sRawInputDevices[0]);
	UINT cbSize = sizeof(sRawInputDevices[0]);
	for (size_t i=0; i<uiNumDevices; ++i) {
		sRawInputDevices[i].hwndTarget = (  *((HWND*)hWnd)  );
	}
	::RegisterRawInputDevices(sRawInputDevices, uiNumDevices, cbSize);

	return true;
}

bool HISpaceNavigator::Calibration() {
	currPose = Posef();
	return true;
}

bool HISpaceNavigator::SetPose(Posef pose) {
	currPose = pose;
	return true;
}

Vec3f HISpaceNavigator::GetPosition() {
	return currPose.Pos();
}

Quaternionf HISpaceNavigator::GetOrientation() {
	return currPose.Ori();
}

Posef HISpaceNavigator::GetPose() {
	return currPose;
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
	if (sRidDeviceInfo.hid.dwVendorId != LOGITECH_VENDOR_ID) return false;

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
		Translate(Vec3d(X,Y,Z));
		return true;
	} else if (pRawInput->data.hid.bRawData[0] == 0x02) {
		// Directed rotation vector (NOT Euler)
		short* pnData = reinterpret_cast<short*>(&pRawInput->data.hid.bRawData[1]);
		short rX = pnData[0];
		short rY = pnData[1];
		short rZ = pnData[2];
		Rotate(Vec3d(rX,rY,rZ));
		return true;
	} else if (pRawInput->data.hid.bRawData[0] == 0x03) {
		// State of the keys
		unsigned long dwKeystate = *reinterpret_cast<unsigned long *>(&pRawInput->data.hid.bRawData[1]);
		/// std::cout << "key : " << dwKeystate << std::endl;
		return true;
	}

	return false;
}

void HISpaceNavigator::Translate(Vec3f trn) {
	dTrn = Quaterniond::Rot(Rad(90), 'x') * ((Vec3d)trn * 0.001);
	currPose.Pos() += dTrn;
}

void HISpaceNavigator::Rotate(Vec3f rot) {
	dRot = (Quaterniond::Rot(Rad(90), 'x') * (Vec3d)rot) * 0.0001;
	currPose.Ori() = Quaterniond::Rot(dRot) * currPose.Ori();
}

}