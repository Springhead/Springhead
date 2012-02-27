from Spr import *

import time
import datetime
import threading

INF = 3.4e+38
rad = 3.14/180.0;

hiTrackball.SetPosition(Vec3f(30, 0, 0))

initPose = Posef()
initPose.setPos(Vec3f(-10,0,0))
spaceNavigator.SetPose(initPose)

fwScene.EnableRenderLimit(True)

## -----

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

## ----- ----- ----- ----- -----

class SpaceNavigator(threading.Thread):
	def __init__(self, phScene):
		# Pointer-base
		self.soB = phScene.CreateSolid(descSolid)
		self.soB.SetDynamical(False)

		# Pointer
		descSphere  = CDSphereDesc()
		descSphere.radius  = 0.5
		self.soP = phScene.CreateSolid(descSolid)
		self.soP.SetFramePosition(Vec3d(-10,0,0))
		self.soP.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), descSphere))

		# Spring
		descSpring = PHSpringDesc()
		descSpring.spring = Vec3d(1,1,1) * 200.0
		descSpring.damper = Vec3d(1,1,1) *  10.0
		descSpring.springOri = 200.0
		descSpring.damperOri =  10.0
		jo = phScene.CreateJoint(self.soB, self.soP, PHSpring.GetIfInfoStatic(), descSpring)
		threading.Thread.__init__(self)

	def run(self):
		while True:
			p = spaceNavigator.GetPose().getPos()
			self.soB.SetFramePosition(Vec3d(p.x, p.y, p.z))

## ----- ----- ----- ----- -----

class Leg:
	def __init__(self, soBody, socketPose):
		self.length = 1.5
		self.socket = Posed()
		self.socket.setPos(socketPose.getPos())
		self.socket.setOri(socketPose.getOri())

		self.soBody  = soBody
		self.phScene = soBody.GetScene()
		self.phSdk   = self.phScene.GetSdk()

		descSolid   = PHSolidDesc()
		descSolid.mass = 0.1

		descCapsule = CDRoundConeDesc()
		descCapsule.length = self.length

		shapePose = Posed()
		shapePose.setOri(Quaterniond.Rot(90*rad, "x"))

		descCapsule.radius = Vec2f(0.2, 0.15)
		self.soUpperLeg = self.phScene.CreateSolid(descSolid)
		self.soUpperLeg.AddShape(self.phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
		self.soUpperLeg.SetShapePose(0, shapePose)
		self.soUpperLeg.SetFramePosition((soBody.GetPose() * socketPose).transform(Vec3d(0,0,0)) + Vec3d(0,-self.length/2,0))
		
		descCapsule.radius = Vec2f(0.15, 0.05)
		self.soLowerLeg = self.phScene.CreateSolid(descSolid)
		self.soLowerLeg.AddShape(self.phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
		self.soLowerLeg.SetShapePose(0, shapePose)
		self.soLowerLeg.SetFramePosition((soBody.GetPose() * socketPose).transform(Vec3d(0,0,0)) + Vec3d(0,-3*self.length/2,0))
		descIKE = PHIKEndEffectorDesc()
		descIKE.targetLocalPosition = Vec3d(0,-self.length/2,0)
		self.ikFoot = self.phScene.CreateIKEndEffector(PHIKEndEffector.GetIfInfoStatic(), descIKE)
		self.ikFoot.AddChildObject(self.soLowerLeg)

		descJoint = PHBallJointDesc()
		descJoint.poseSocket = socketPose
		descJoint.posePlug   = Posed(1,0,0,0, 0,self.length/2,0)
		descJoint.spring =       0.0
		descJoint.damper =   10000.0
		descJoint.fMax   =    1000.0
		descIKA = PHIKBallActuatorDesc()
		descIKA.velocityGain = 10.0

		self.joShoulder = self.phScene.CreateJoint(self.soBody, self.soUpperLeg, PHBallJoint.GetIfInfoStatic(), descJoint)
		self.ikShoulder = self.phScene.CreateIKActuator(PHIKBallActuator.GetIfInfoStatic(), descIKA)
		self.ikShoulder.AddChildObject(self.joShoulder)
		self.ikShoulder.AddChildObject(self.ikFoot)

		descJoint.poseSocket = Posed(1,0,0,0, 0,-self.length/2,0)
		self.joKnee = self.phScene.CreateJoint(self.soUpperLeg, self.soLowerLeg, PHBallJoint.GetIfInfoStatic(), descJoint)
		self.ikKnee = self.phScene.CreateIKActuator(PHIKBallActuator.GetIfInfoStatic(), descIKA)
		self.ikKnee.AddChildObject(self.joKnee)
		self.ikKnee.AddChildObject(self.ikFoot)

		self.phScene.SetContactMode(self.soBody,     self.soUpperLeg, 0)
		self.phScene.SetContactMode(self.soUpperLeg, self.soLowerLeg, 0)
		
		# --- API
		self.targetPos   = Vec3d()
		self.offset      = Vec3d()
		self.targetForce = Vec3d()
		self.soFloor     = None
		# ---

	def Step(self):
		constrEngine = phScene.GetConstraintEngine()
		dt = phScene.GetTimeStep()
		currForce = constrEngine.GetContactPoints().GetTotalForce(self.soFloor,self.soLowerLeg)
		if (currForce.norm() > 0.0001):
			diffForce    = self.targetForce - currForce
			self.offset += diffForce * 0.001
		else:
			self.offset  = Vec3d()

		self.ikFoot.SetTargetPosition((self.soBody.GetPose() * self.socket).transform((self.targetPos+self.offset) - Vec3d(0,2*self.length,0)))
		
		
		

## ----- ----- ----- ----- -----

descSolid   = PHSolidDesc()

descBox = CDBoxDesc()

descBox.boxsize = Vec3d(4.0, 0.5, 2.5)
soBody = phScene.CreateSolid(descSolid)
soBody.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
soBody.SetFramePosition(Vec3d(0,3,0))
soBody.SetDynamical(False)

descBox.boxsize = Vec3d(30.0, 5.0, 30.0)
soFloor = phScene.CreateSolid(descSolid)
soFloor.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))
soFloor.SetFramePosition(Vec3d(0,-2.5+1,0))
soFloor.SetDynamical(False)

## ----- ----- ----- ----- -----

pose = Posed()

pose.setPos( Vec3d( 2.00, 0.00, 1.25) );leg1 = Leg(soBody, pose); leg1.soFloor = soFloor
pose.setPos( Vec3d(-2.00, 0.00, 1.25) );leg2 = Leg(soBody, pose); leg2.soFloor = soFloor
pose.setPos( Vec3d( 2.00, 0.00,-1.25) );leg3 = Leg(soBody, pose); leg3.soFloor = soFloor
pose.setPos( Vec3d(-2.00, 0.00,-1.25) );leg4 = Leg(soBody, pose); leg4.soFloor = soFloor

phScene.GetIKEngine().Enable(True)

## ----- ----- ----- ----- -----

spaceNav = SpaceNavigator(phScene)
spaceNav.start()

time.sleep(1)

for i in range(0,30):
	leg1.targetPos = Vec3d(0.1, 1.0,  0   )
	leg2.targetPos = Vec3d(0.1, 2.0, -1.25)
	leg3.targetPos = Vec3d(0.1, 2.0,  0   )
	leg4.targetPos = Vec3d(0.1, 2.0,  0   )
	leg1.Step()
	leg2.Step()
	leg3.Step()
	leg4.Step()
	phScene.Step()

t = 0
while True:
	leg1.targetForce = Vec3d(0,  -20.0,  0   )
	leg1.targetPos   = Vec3d(0.1,  1.0,  0   )
	leg1.Step()
	leg2.Step()
	leg3.Step()
	leg4.Step()
	phScene.Step()
	t += 1


