/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHSkeleton.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{

void PHBone::SetLength(double length) {
	if (length < 1e-3) { length = 1e-3; }
	CDRoundConeIf* rc;
	rc = solid->GetShape(0)->Cast();
	if (!rc && proxySolid) {
		rc = proxySolid->GetShape(0)->Cast();
	}

	if (rc) {
		if (abs(rc->GetLength() - length) > 1e-3) {
			rc->SetLength(length);
		}
	}
}

void PHBone::SetDirection(Vec3d dir) {
	if (dir.norm() > 1e-3) {
		dir.unitize();
		Quaterniond lastOri = lastPose.Ori();
		Quaterniond currOri; currOri.RotationArc(Vec3d(0,0,1), dir);
		solid->SetOrientation(currOri);
		Vec3d w = (currOri * lastOri.Inv()).RotationHalf();
		solid->SetAngularVelocity(w / DCAST(PHSceneIf,GetScene())->GetTimeStep());
		lastPose.Ori() = currOri;
	}
}

void PHBone::SetPosition(Vec3d currPos) {
	Vec3d lastPos = lastPose.Pos();
	solid->SetFramePosition(currPos);
	if ((currPos - lastPos).norm() < 1.0) {
		solid->SetVelocity((currPos - lastPos) / DCAST(PHSceneIf,GetScene())->GetTimeStep());
	}
	lastPose.Pos() = currPos;

	if (proxySolid) {
		if (((currPos - lastPos).norm() > 1.0) ||
			((proxySolid->GetPose().Pos() - solid->GetPose().Pos()).norm() > 5.0) )
		{
			proxySolid->SetPose(solid->GetPose());
			proxySolid->SetVelocity(solid->GetVelocity());
			proxySolid->SetAngularVelocity(solid->GetAngularVelocity());
		}
	}
}

}
