#include <Springhead.h>

#include <LeapSDK/Leap.h>
#pragma comment(lib, "Leap.lib")

using namespace Spr;

struct LeapBone {
	Spr::Vec3d position;
	Spr::Vec3d direction;
	double length;
};

struct LeapFinger {
	static const int BONE_NUM = 4;
	LeapBone bones[BONE_NUM];
};

struct LeapHand {
	enum HandKind{
		RIGHT_HAND,
		LEFT_HAND,
	} handKind;

	static const int FINGER_NUM = 5;
	Spr::Vec3d position;
	Spr::Vec3d direction;
	Spr::Quaterniond orientation;
	LeapFinger leapFingers[FINGER_NUM];
	int recFingersNum;
	float confidence;
	float grabStrength;
	inline int getRecFingersNum() { return recFingersNum; }

	bool isTracked;
	int originalLeapHandID;
	int bufID;

	int leapID;
};

inline Vec3d ToSpr(Leap::Vector lv) {
	return Vec3d(lv.x, lv.y, lv.z);
}

inline Quaterniond ToSpr(Leap::Matrix lm) {
	Matrix3d m;
	for (int i = 0; i<3; ++i) {
		m[0][i] = lm.xBasis[i]; m[1][i] = lm.yBasis[i]; m[2][i] = lm.zBasis[i];
	}
	Quaterniond q; q.FromMatrix(m);

	return q;
}

struct LeapFrame {
	std::vector<LeapHand> leapHands;
	int recHandsNum;
	int leapID;
};

extern "C" {
	__declspec(dllexport) void* __cdecl CreateLeapController(void) {
		return new Leap::Controller();
	}

	__declspec(dllexport) void __cdecl DeleteLeapController(void* leap_) {
		Leap::Controller* leap = (Leap::Controller*)leap_;
		if (leap) {
			delete leap;
		}
	}

	__declspec(dllexport) void __cdecl GetLeapFrame(void* leap_, int i, void* pFrame) {
		LeapFrame* frame = (LeapFrame*)(pFrame);

		Leap::Controller* leap = (Leap::Controller*)leap_;

		if (leap) {
			// Leapmotionからセンシング結果を取得
			Leap::Frame frame_ = leap->frame(i);

			// フレームデータの用意
			frame->leapHands.resize(frame_.hands().count());
			frame->recHandsNum = frame_.hands().count();

			std::cout << "Leap Hands : " << frame->recHandsNum << std::endl;

			for (int h = 0; h < frame_.hands().count(); ++h) {
				Leap::Hand hand = frame_.hands()[h];

				frame->leapHands[h].originalLeapHandID = hand.id();
				frame->leapHands[h].isTracked = (hand.timeVisible() > 1e-5 ? true : false);

				// 手全体の位置姿勢をセット
				frame->leapHands[h].position = ToSpr(hand.palmPosition());
				frame->leapHands[h].orientation = ToSpr(hand.basis());

				for (int f = 0; f < hand.fingers().count(); f++){
					for (int b = 0; b < frame->leapHands[h].leapFingers[f].BONE_NUM; b++) {
						Leap::Bone::Type boneType = static_cast<Leap::Bone::Type>(b);
						Leap::Bone bone = hand.fingers()[f].bone(boneType);

						frame->leapHands[h].leapFingers[f].bones[b].position = ToSpr(bone.center());
						frame->leapHands[h].leapFingers[f].bones[b].direction = ToSpr(bone.direction());
						frame->leapHands[h].leapFingers[f].bones[b].length = bone.length();
					}
				}
			}
		} else {
			std::cout << "Leap Controller Not Found" << std::endl;
		}
	}
}