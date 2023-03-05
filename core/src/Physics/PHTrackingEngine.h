/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Physics/PHEngine.h>
#include <Physics/PHTreeNode.h>

namespace Spr {;

class PHTrackingEngine : public PHEngine {
	SPR_OBJECTDEF(PHTrackingEngine);
	std::vector<Quaterniond> trackingInputs;
	std::vector<TrackingNode> trackingNodes;
	Vec3d trackingRootPosition;
	public:
		int GetPriority() const { return SGBP_NONE; }
		void Step();

		virtual void Clear();
		virtual bool AddChildObject(ObjectIf* o);
		virtual bool DelChildObject(ObjectIf* o);
		void SetTrackingInputs(std::vector<Quaterniond> inputs); ///<’Ç]‚³‚¹‚éÛ‚Ì–Ú•W‚Æ‚È‚é“ü—ÍŠp“x‚ðÝ’è‚·‚é
		void SetTrackingRootPosition(Vec3d input);
		void AddTrackingNode(PHBallJointIf* reactJoint, PHBallJointIf* calcJoint, PHSolidIf* reactRootSolid, PHSolidIf* calcRootSolid, bool isRoot);
		SpatialVector GetTipAcceleration(int i);
		void TrackWithForce();
};

}
