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

		struct TrackingNode {
		public:
			TrackingNode* parent;
			PHBallJointIf* reactJoint;
			PHBallJointIf* calcJoint;
			PHSolidIf* reactRootSolid;
			PHSolidIf* calcRootSolid;
			bool		 isRoot;
			Quaterniond socketInput;
			Quaterniond plugInput;
			Quaterniond preLocalTargetRotation;
			Quaterniond jointTargetRotation;
			SpatialVector targetAcceleration;
			Vec3d localAngularVelocity;
			Vec3d localAngularVelocityDot;
			Vec3d globalAngularVelocity;
			Vec3d globalAngularVelocityDot;
			Vec3d globalAcceleration;
			Vec3d globalVelocity;
			Vec3d rootAccelerration;
			Vec3d rootPositionInput;
			Vec3d trackingForce;
			Vec3d trackingTorque;
		};
		struct ABATrackingInput {
		public:
			PHRootNodeIf* root;
			Vec3d trackingRootPosition;
			std::vector<Quaterniond> inputs;
		};
		struct ABATrackingNode {
		public:
			PHRootNodeIf* root;
			std::vector<TrackingNode> trackingNodes;
		};
		std::vector<ABATrackingInput> abaTrackingInputs;
		std::vector<ABATrackingNode> abaTrackingNodes;
		double maxForce = FLT_MAX;
		double maxRootVelocity = FLT_MAX;
		double maxAngularVelocity = FLT_MAX;
	public:
		int GetPriority() const { return SGBP_NONE; }
		void Step();

		virtual void Clear();
		virtual bool AddChildObject(ObjectIf* o);
		virtual bool DelChildObject(ObjectIf* o);
		void SetTrackingInputs(PHRootNodeIf* root, Vec3d rootPosition, std::vector<Quaterniond> inputs);
		void SetTrackingRootPosition(PHRootNodeIf* root, Vec3d input);
		void AddTrackingNode(PHRootNodeIf* root, PHBallJointIf* reactJoint, PHBallJointIf* calcJoint, PHSolidIf* reactRootSolid, PHSolidIf* calcRootSolid, bool isRoot);
		void DeleteTrackingNodes(PHRootNodeIf* root);
		SpatialVector GetTipAcceleration(PHRootNodeIf* root, int i);
		void TrackWithForce(PHRootNodeIf* root);
		void AddTrackingForce(PHBallJointNodeIf* calcNode, PHBallJointNodeIf* reactNode, double timeStep, Vec3d targetAngularAcceleration, SpatialVector parentTargetAcceleration, SpatialVector& targetAcceleration, Vec3d& force, Vec3d& torque);
		void SetMaxForce(double max) { maxForce = max; }
		void SetMaxRootVelocity(double max) { maxRootVelocity = max; }
		void SetMaxAngularVelocity(double max) { maxAngularVelocity = max; }
		ABATrackingNode* SearhABATrackingNode(PHRootNodeIf* root) {
			for (ABATrackingNode& a : abaTrackingNodes) {
				if (a.root == root) {
					return &a;
				}
			}
			return NULL;
		}
		ABATrackingInput* SearchABATrackingInput(PHRootNodeIf* root) {
			for (ABATrackingInput& a : abaTrackingInputs) {
				if (a.root == root) {
					return &a;
				}
			}
			return NULL;
		}
	};

}
