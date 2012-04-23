from Spr import *

import time
import datetime
import threading

INF = 3.4e+38
rad = 3.14/180.0;

st = ObjectStates().Create()

def Steps(n):
	d = datetime.datetime.today()
	print('%s:%s:%s.%s \n' % (d.hour, d.minute, d.second, d.microsecond))

	for i in range(0,n):
		phScene.Step()
		time.sleep(0.02)

	d = datetime.datetime.today()
	print('%s:%s:%s.%s \n' % (d.hour, d.minute, d.second, d.microsecond))

hiTrackball.SetPosition(Vec3f(0, 0, 50))

initPose = Posef()
initPose.setPos(Vec3f(-10,0,0))
spaceNavigator0.SetPose(initPose)

fwScene.EnableRenderLimit(True)

## -----

phScene = fwScene.GetPHScene()
phSdk   = phScene.GetSdk()

phScene.SetGravity(Vec3d(0,-9.8,0))

## ----- ----- ----- ----- -----

descSolid   = PHSolidDesc()

descCapsule = CDRoundConeDesc()
descCapsule.radius = Vec2f(0.3, 0.3)
descCapsule.length = 4

descSphere  = CDSphereDesc()
descSphere.radius  = 0.5

descBox = CDBoxDesc()
descBox.boxsize = Vec3d(1.0, 0.5, 1.0)

shapePose = Posed()
shapePose.setOri(Quaterniond.Rot(90*rad, "x"))

## ----- -----

# Wall
soW = phScene.CreateSolid(descSolid)
soW.SetDynamical(False)
soW.SetFramePosition(Vec3d(-3,2-0.3-0.25,0))
soW.AddShape(phSdk.CreateShape(CDBox.GetIfInfoStatic(), descBox))

# Base Link
so0 = phScene.CreateSolid(descSolid)
so0.SetDynamical(False)
so0.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so0.SetShapePose(0, shapePose)

# Link 1
so1 = phScene.CreateSolid(descSolid)
so1.SetFramePosition(Vec3d(0,4,0))
so1.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so1.SetShapePose(0, shapePose)

# Link 2
so2 = phScene.CreateSolid(descSolid)
so2.SetFramePosition(Vec3d(0,8,0))
so2.AddShape(phSdk.CreateShape(CDRoundCone.GetIfInfoStatic(), descCapsule))
so2.SetShapePose(0, shapePose)

# Pointer-base
so3 = phScene.CreateSolid(descSolid)
so3.SetDynamical(False)

# Pointer
so4 = phScene.CreateSolid(descSolid)
so4.SetFramePosition(Vec3d(-10,0,0))
so4.AddShape(phSdk.CreateShape(CDSphere.GetIfInfoStatic(), descSphere))

## ----- ----- ----- ----- -----

descSpring = PHSpringDesc()
descSpring.spring = Vec3d(1,1,1) * 200.0
descSpring.damper = Vec3d(1,1,1) *  10.0
descSpring.springOri = 200.0
descSpring.damperOri =  10.0

descJoint = PHHingeJointDesc()
descJoint.poseSocket = Posed(1,0,0,0, 0, 2,0)
descJoint.posePlug   = Posed(1,0,0,0, 0,-2,0)
descJoint.spring =     10.0
descJoint.damper =   1000.0
descJoint.fMax   =   1000.0

descLimit = PH1DJointLimitDesc()
descLimit.spring = 1000000.0
descLimit.damper =   10000.0

descIKA = PHIKHingeActuatorDesc()
descIKA.velocityGain = 10.0

descIKE = PHIKEndEffectorDesc()

## ----- -----

# Base <-> Link 1
jo1  = phScene.CreateJoint(so0, so1, PHHingeJoint.GetIfInfoStatic(), descJoint)
descLimit.range  = Vec2d(-90*rad, 90*rad)
jo1.CreateLimit(descLimit)
ika1 = phScene.CreateIKActuator(PHIKHingeActuator.GetIfInfoStatic(), descIKA)
ika1.AddChildObject(jo1)

# Link 1 <-> Link 2
jo2  = phScene.CreateJoint(so1, so2, PHHingeJoint.GetIfInfoStatic(), descJoint)
descLimit.range  = Vec2d(0*rad, 180*rad)
jo2.CreateLimit(descLimit)
ika2 = phScene.CreateIKActuator(PHIKHingeActuator.GetIfInfoStatic(), descIKA)
ika2.AddChildObject(jo2)

# Link2 = End Effector
ike1 = phScene.CreateIKEndEffector(descIKE)
ike1.AddChildObject(so2)
ika1.AddChildObject(ike1)
ika2.AddChildObject(ike1)
ike1.SetTargetPosition(Vec3d(-5,0,0))

# Pointer
jo3 = phScene.CreateJoint(so3, so4, PHSpring.GetIfInfoStatic(), descSpring)

## ----- ----- ----- ----- -----

st.SaveState(phScene)

phScene.SetContactMode(2)
phScene.SetContactMode(so0,so1, 0)
phScene.SetContactMode(so1,so2, 0)

st.SaveState(phScene)

## ----- ----- ----- ----- -----

constrEngine = phScene.GetConstraintEngine()

def start():
	st.LoadState(phScene)
	dt = phScene.GetTimeStep()
	phScene.GetIKEngine().Enable(True)
	target  = Vec3d()
	offset  = Vec3d()
	for i in range(0,1000):
		s = i*3/100 if i<=100 else 3
		target = Vec3d(0,8,0) + Vec3d(-s,-s*2,0)
		ike1.SetTargetPosition(target+offset)
		ike1.SetTargetLocalPosition(Vec3d(0,2,0))
		phScene.Step()
		curr   = so2.GetPose().transform(Vec3d(0,2,0))
		# print(target, "diff : ", (target - curr).norm())

		force = constrEngine.GetContactPoints().GetTotalForce(soW,so2)

		if (force.norm() > 0.0001):
			targetForce = Vec3d(0,-9.8,0)
			diffForce   = targetForce - force
			offset     += diffForce * 0.001
		else:
			offset      = Vec3d()

		print("force : ", force, "  offset : ", offset)

		time.sleep(0.01)

## ----- ----- ----- ----- -----

class SpaceNavigatorThread(threading.Thread):
	def __init__(self, soPointer):
		self.soPointer = soPointer
		threading.Thread.__init__(self)

	def run(self):
		while True:
			p = spaceNavigator0.GetPose().getPos()
			self.soPointer.SetFramePosition(Vec3d(p.x, p.y, 0))

spaceNav = SpaceNavigatorThread(so3)
spaceNav.start()

time.sleep(1)
start()

